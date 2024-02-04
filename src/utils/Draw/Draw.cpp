/* Start Header ************************************************************************/
/*!
\file Draw.cpp
\author Poh Jing Seng, jingseng.poh, 2301363
\par jingseng.poh\@digipen.edu
\date 3 Feb 2024
\brief abstracts away drawing of shapes
/*
Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/


#include "Draw.h"
#include <iostream>

/*class draw*/
Draw* Draw::_instance = nullptr;

Draw::Draw() {
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
	_mesh = AEGfxMeshEnd();
}

Draw::~Draw() {
	AEGfxMeshFree(_mesh);
	delete _instance;
	
	for (std::pair<std::string, AEGfxTexture*> map : _textureRef) {
		AEGfxTextureUnload(map.second);
	}
	_textureRef.clear();
}

Draw* Draw::getInstance() {
	if (_instance == nullptr) {
		_instance = new Draw();
	}
	return _instance;
}

void Draw::background(Color color) {
	AEGfxSetBackgroundColor(color.r, color.g, color.b);
}

void Draw::rect(f32 transX, f32 transY, f32 scaleX, f32 scaleY, f32 rotation, Color color, f32 opacity) {
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


	// prepare to draw
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetColorToAdd(color.r, color.g, color.b, color.a);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(opacity);
	AEGfxSetTransform(transform.m);
	AEGfxMeshDraw(_mesh, AE_GFX_MDM_TRIANGLES);
}

bool Draw::registerTexture(std::string reference, std::string path) {
	AEGfxTexture* pTex = AEGfxTextureLoad(path.c_str());
	if (!pTex) {
		std::cerr << "Texture failed to load\n";
		exit(1);
		return false;
	}
	_textureRef[reference] = pTex;
	std::cout << "Loaded texture at location " << pTex << "\n";
	return true;
}

AEGfxTexture* Draw::getTextureByRef(std::string reference) {
	auto map = _textureRef.find(reference.c_str());
	if (map == _textureRef.end()) {  // does not exist
		return nullptr;
	}
	return map->second;
}

void Draw::removeTextureByRef(std::string reference) {
	AEGfxTexture* pTex = getTextureByRef(reference);
	AEGfxTextureUnload(pTex);
	_textureRef.erase(reference);
}

void Draw::texture(std::string textureRef, f32 transX, f32 transY, f32 scaleX, f32 scaleY, f32 opacity, Color color, f32 rotation) {
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
	AEGfxSetTransparency(1);
	AEGfxSetTransform(transform.m);
	AEGfxTextureSet(pTex, 0, 0);
	AEGfxMeshDraw(_mesh, AE_GFX_MDM_TRIANGLES);
}


/*class DrawSpritesheet*/

