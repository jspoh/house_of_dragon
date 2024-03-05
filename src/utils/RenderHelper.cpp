/* Start Header ************************************************************************/
/*!
\file RenderHelper.cpp
\author Poh Jing Seng, jingseng.poh, 2301363
\par jingseng.poh\@digipen.edu
\date 3 Feb 2024
\brief abstracts away drawing of shapes and textures
/*
Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/


#include "RenderHelper.h"
#include <iostream>

namespace {
	/**
	 * .
	 * 
	 * \param transX
	 * \param transY
	 * \param scaleX
	 * \param scaleY
	 * \param rotation
	 * \param matrix [out] output transform matrix
	 */
	void getTransformMatrix(f32 transX, f32 transY, f32 scaleX, f32 scaleY, f32 rotation, AEMtx33& matrix) {
		// create matrix
		AEMtx33 scale = { 0 };
		AEMtx33Scale(&scale, scaleX, scaleY);
		AEMtx33 rotate = { 0 };
		AEMtx33Rot(&rotate, rotation);

		AEMtx33 translate = { 0 };
		AEMtx33Trans(&translate, transX, transY);

		AEMtx33Concat(&matrix, &rotate, &scale);
		AEMtx33Concat(&matrix, &translate, &matrix);
	}
}

/*class draw*/
RenderHelper* RenderHelper::_instance = nullptr;

RenderHelper::RenderHelper() {
	// init reusable mesh
	AEGfxMeshStart();
	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFF000000, 0.0f, 1.0f,  // bottom-left
		0.5f, -0.5f, 0xFF000000, 1.0f, 1.0f,   // bottom-right
		-0.5f, 0.5f, 0xFF000000, 0.0f, 0.0f	   // top-left
	);

	AEGfxTriAdd(
		0.5f, -0.5f, 0xFF000000, 1.0f, 1.0f,   // bottom-right
		0.5f, 0.5f, 0xFF000000, 1.0f, 0.0f,    // top-right
		-0.5f, 0.5f, 0xFF000000, 0.0f, 0.0f    // top-left
	);
	_defaultMesh = AEGfxMeshEnd();

	// font
	_font = AEGfxCreateFont("./Assets/liberation-mono.ttf", _fontSize);
}

bool RenderHelper::registerMeshByRef(std::string reference, AEGfxVertexList* mesh) {
	try {
		_meshRef[reference] = mesh;
		return true;
	}
	catch (const std::exception& e) {
		std::cerr << "Failed to register mesh " << mesh << " with reference " << reference << " with error: " <<e.what() << "\n";
		return false;
	}
}

bool RenderHelper::removeMeshByRef(std::string reference) {
	try {
		AEGfxMeshFree(_meshRef[reference]);
		_meshRef.erase(reference);
		return true;
	}
	catch (const std::exception& e) {
		std::cerr << "Failed to remove mesh with reference " << reference << " with error: " << e.what() << "\n";
		return false;
	}
}

RenderHelper::~RenderHelper() {
	AEGfxMeshFree(_defaultMesh);
	
	for (const std::pair<std::string, AEGfxTexture*> map : _textureRef) {
		AEGfxTextureUnload(map.second);
	}

	for (const std::pair<std::string, AEGfxVertexList*> map : _meshRef) {
		AEGfxMeshFree(map.second);
	}

	_textureRef.clear();
	_meshRef.clear();
}

RenderHelper* RenderHelper::getInstance() {
	if (_instance == nullptr) {
		_instance = new RenderHelper();
	}
	return _instance;
}

void RenderHelper::setBackgroundColor(Color color) {
	AEGfxSetBackgroundColor(color.r, color.g, color.b);
}

void RenderHelper::rect(f32 transX, f32 transY, f32 scaleX, f32 scaleY, f32 rotation, Color color, f32 opacity) {
	AEMtx33 transform = { 0 };
	getTransformMatrix(transX, transY, scaleX, scaleY, rotation, transform);

	// prepare to draw
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetColorToAdd(color.r, color.g, color.b, color.a);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(opacity);
	AEGfxSetTransform(transform.m);
	AEGfxMeshDraw(_defaultMesh, AE_GFX_MDM_TRIANGLES);
}

void RenderHelper::rect(std::string meshRef, f32 transX, f32 transY, f32 scaleX, f32 scaleY, f32 rotation, Color color, f32 opacity) {
	// guard to ensure mesh exists
	if (_meshRef.find(meshRef) == _meshRef.end()) {
		std::cerr << "Mesh with reference " << meshRef << " does not exist\n";
		return;
	}
	
	AEGfxVertexList* pMesh = _meshRef[meshRef];
	
	AEMtx33 transform = { 0 };
	getTransformMatrix(transX, transY, scaleX, scaleY, rotation, transform);

	// prepare to draw
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetColorToAdd(color.r, color.g, color.b, color.a);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(opacity);
	AEGfxSetTransform(transform.m);
	AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
}

bool RenderHelper::registerTexture(std::string reference, std::string path) {
	std::cout << "Loading texture " << path << " with reference " << reference << "\n";
	AEGfxTexture* pTex = AEGfxTextureLoad(path.c_str());
	if (!pTex) {
		std::cerr << "Texture failed to load\n";
		return false;
	}
	_textureRef[reference] = pTex;
	std::cout << "Loaded texture at location " << pTex << "\n";
	return true;
}

AEGfxTexture* RenderHelper::getTextureByRef(std::string reference) {
	auto map = _textureRef.find(reference.c_str());
	if (map == _textureRef.end()) {  // does not exist
		return nullptr;
	}
	return map->second;
}

void RenderHelper::removeTextureByRef(std::string reference) {
	AEGfxTexture* pTex = getTextureByRef(reference);
	AEGfxTextureUnload(pTex);
	_textureRef.erase(reference);
}

void RenderHelper::texture(std::string textureRef, f32 transX, f32 transY, f32 scaleX, f32 scaleY, f32 opacity, Color color, f32 rotation) {
	// create matrix
	AEMtx33 scale = { 0 };
	AEMtx33Scale(&scale, scaleX, scaleY);
	AEMtx33 rotate = { 0 };
	AEMtx33Rot(&rotate, rotation);

	AEMtx33 translate = { 0 };
	AEMtx33Trans(&translate, transX, transY);

	AEMtx33 transform = { 0 };
	AEMtx33Concat(&transform, &rotate, &scale);
	AEMtx33Concat(&transform, &translate, &transform);

	// prep texture
	AEGfxTexture* pTex = getTextureByRef(textureRef);  // doesnt


	// prepare to draw
	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetColorToAdd(color.r, color.g, color.b, color.a);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(opacity);
	AEGfxSetTransform(transform.m);
	AEGfxTextureSet(pTex, 0, 0);
	AEGfxMeshDraw(_defaultMesh, AE_GFX_MDM_TRIANGLES);
}

void RenderHelper::text(std::string s, float screenX, float screenY, float r, float g, float b, float a) {
	AEGfxSetRenderMode(AE_GFX_RM_NONE);

	Point p = ston(screenX, screenY);

	f32 width, height;
	AEGfxGetPrintSize(_font, s.c_str(), 1.f, &width, &height);
	AEGfxPrint(_font, s.c_str(), p.x - width / 2, p.y - height / 2, 1, r, g, b, a);
}

void RenderHelper::setFontSize(int size) {
	throw std::exception("not working");
	_fontSize = size;
}


/*class DrawSpritesheet*/

