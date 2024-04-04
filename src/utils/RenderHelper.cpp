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

#include "Pch.h"
#include "RenderHelper.h"

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

}

void RenderHelper::load() {
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

	AEGfxMeshStart();
	AEGfxTriAdd(
		-0.5f, -0.5f, 0x00000000, 0.0f, 1.0f,  // bottom-left
		0.5f, -0.5f, 0x00000000, 1.0f, 1.0f,   // bottom-right
		-0.5f, 0.5f, 0x00000000, 0.0f, 0.0f	   // top-left
	);

	AEGfxTriAdd(
		0.5f, -0.5f, 0x00000000, 1.0f, 1.0f,   // bottom-right
		0.5f, 0.5f, 0x00000000, 1.0f, 0.0f,    // top-right
		-0.5f, 0.5f, 0x00000000, 0.0f, 0.0f    // top-left
	);
	_invisibleMesh = AEGfxMeshEnd();

	_meshRef["default"] = _defaultMesh;
	_meshRef["invis"] = _invisibleMesh;

	// font
	_font = AEGfxCreateFont("./Assets/Fonts/liberation-mono.ttf", _fontSize);

	// init static array used for int ids
	std::fill(_textureIdRefs.begin(), _textureIdRefs.end(), nullptr);
}

bool RenderHelper::registerMeshByRef(std::string reference, AEGfxVertexList* mesh) {
	try {
		_meshRef[reference] = mesh;
		return true;
	}
	catch (const std::exception& e) {
		std::cerr << "Failed to register mesh " << mesh << " with reference " << reference << " with error: " << e.what() << "\n";
		throw std::exception();
		return false;
	}
}

AEGfxVertexList* RenderHelper::getMeshByRef(std::string reference) {
	auto it = _meshRef.find(reference);
	if (it != _meshRef.end()) {
		return it->second;
	}
	std::cerr << "Mesh with ref " << reference << " does not exist!\n";
	throw std::exception();
}

bool RenderHelper::removeMeshByRef(std::string reference) {
	try {
		AEGfxMeshFree(_meshRef[reference]);
		_meshRef.erase(reference);
		return true;
	}
	catch (const std::exception& e) {
		std::cerr << "Failed to remove mesh with reference " << reference << " with error: " << e.what() << "\n";
		throw std::exception();
		return false;
	}
}

RenderHelper::~RenderHelper() {
	//AEGfxMeshFree(_defaultMesh);
	//AEGfxMeshFree(_invisibleMesh);
	
	for (std::pair<std::string, AEGfxTexture*> map : _textureRef) {
		cout << "Automatically removing texture with ref " << map.first << "\n";
		AEGfxTextureUnload(map.second);
		map.second = nullptr;
	}

	for (std::pair<std::string, AEGfxVertexList*> map : _meshRef) {
		cout << "Automatically removing mesh with ref " << map.first << "\n";
		AEGfxMeshFree(map.second);
		map.second = nullptr;
	}

	for (AEGfxTexture* pTex : _textureIdRefs) {
		if (pTex == nullptr) {
			continue;
		}
		cout << "Automatically removing texture with id \n";
		AEGfxTextureUnload(pTex);
		pTex = nullptr;
	}

	_textureRef.clear();
	_meshRef.clear();
}

RenderHelper* RenderHelper::getInstance() {
	if (_instance == nullptr) {
		_instance = new RenderHelper;
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
	cout << "Loading texture " << path << " with reference " << reference << "\n";

	if (_textureRef.find(reference) != _textureRef.end()) {
		// it is intended behaviour to catch all textures that has already been loaded to mitigate the issue of 
		// load being called on state reload. we do not want to free assets that has already been loaded to save
		// resources and load times
		
		//std::cerr << "Texture ref " << reference << " already in use!\n";
		//throw std::exception();
		return false;
	}

	AEGfxTexture* pTex = AEGfxTextureLoad(path.c_str());
	if (!pTex) {
		std::cerr << "Texture with reference " << reference << " and path " << path << " failed to load\n";
		throw std::exception();
		return false;
	}
	_textureRef[reference] = pTex;
	cout << "Loaded texture with string reference " << reference << " at location " << pTex << "\n";
	return true;
}

bool RenderHelper::registerTexture(int reference, std::string path) {
	if (reference >= MAX_TEXTURE_IDS) {
		std::cerr << "Texture reference id(" << reference << ") cannot be greater than array size!\n";
		throw std::exception();
		return false;
	}
	if (_textureIdRefs[reference] != nullptr) {
		// it is intended behaviour to catch all textures that has already been loaded to mitigate the issue of 
		// load being called on state reload. we do not want to free assets that has already been loaded to save
		// resources and load times

		std::cerr << "int reference " << reference << " already used\n";
		//throw std::exception();
		return false;
	}

	AEGfxTexture* pTex = AEGfxTextureLoad(path.c_str());
	if (!pTex) {
		std::cerr << "Texture with reference " << reference << " and path " << path << " failed to load\n";
		throw std::exception();
		return false;
	}
	_textureIdRefs[reference] = pTex;
	cout << "Loaded texture with int reference " << reference << " at location " << pTex << "\n";
	return true;
}

AEGfxTexture* RenderHelper::getTextureByRef(std::string reference) {
	auto map = _textureRef.find(reference.c_str());
	if (map == _textureRef.end() || map->second == nullptr) {  // does not exist
		std::cerr << "Reference " << reference << " does not exist!\n";
		throw std::exception();
		return nullptr;
	}
	return map->second;
}

AEGfxTexture* RenderHelper::getTextureByRef(int reference) {
	if (_textureIdRefs[reference] == nullptr) {
		std::cerr << "Reference " << reference << " has not been set!\n";
	}
	return _textureIdRefs[reference];
}

void RenderHelper::removeTextureByRef(std::string reference) {
	auto map = _textureRef.find(reference.c_str());
	if (map == _textureRef.end()) {  // does not exist
		std::cerr << "Reference " << reference << " does not exist!\n";
		return;
	}

	AEGfxTexture* pTex = getTextureByRef(reference);
	AEGfxTextureUnload(pTex);
	_textureRef.erase(reference);

	cout << "Manually removed texture with ref " << reference << "\n";
}

void RenderHelper::removeTextureByRef(int reference) {
	AEGfxTexture* pTex = getTextureByRef(reference);
	if (pTex == nullptr) {
		std::cerr << "texture with int reference " << reference << " was never initialized!\n";
		return;
	}
	AEGfxTextureUnload(pTex);
	_textureIdRefs[reference] = nullptr;

	cout << "Manually removed texture with ref " << reference << "\n";
}

void RenderHelper::texture(std::string textureRef, f32 transX, f32 transY, f32 scaleX, f32 scaleY, f32 opacity, Color addColor, f32 rotation, Color defaultColor) {
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
	AEGfxSetColorToMultiply(defaultColor.r, defaultColor.g, defaultColor.b, defaultColor.a);
	AEGfxSetColorToAdd(addColor.r, addColor.g, addColor.b, addColor.a);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(opacity);
	AEGfxSetTransform(transform.m);
	AEGfxTextureSet(pTex, 0, 0);
	AEGfxMeshDraw(_defaultMesh, AE_GFX_MDM_TRIANGLES);
}

void RenderHelper::texture(int textureRef, f32 transX, f32 transY, f32 scaleX, f32 scaleY, f32 opacity, Color addColor, f32 rotation, Color defaultColor) {
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

	if (pTex == nullptr) {
		std::cerr << "Texture with reference " << textureRef << " wasnt initialized!\n";
		return;
	}


	// prepare to draw
	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetColorToMultiply(defaultColor.r, defaultColor.g, defaultColor.b, defaultColor.a);
	AEGfxSetColorToAdd(addColor.r, addColor.g, addColor.b, addColor.a);
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
	AEGfxGetPrintSize(_font, s.c_str(), 1, &width, &height);
	AEGfxPrint(_font, s.c_str(), p.x - width / 2, p.y - height / 2, 1, r, g, b, a);
}

int RenderHelper::getFontSize() {
	return _fontSize;
}

AEGfxVertexList* RenderHelper::GetdefaultMesh()
{
	return _defaultMesh;
}

AEGfxTexture* RenderHelper::GetTexture(int textureRef)
{
	AEGfxTexture* pTex = getTextureByRef(textureRef);  // doesnt

	if (pTex == nullptr) {
		std::cerr << "Texture with reference " << textureRef << " wasnt initialized!\n";
		return nullptr;
	}
	return pTex;
}
/*class DrawSpritesheet*/

