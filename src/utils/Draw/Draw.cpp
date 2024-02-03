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

/*class draw*/
Draw* Draw::_instance = nullptr;

Draw::Draw() {
	// init reusable mesh
	AEGfxMeshStart();
	AEGfxTriAdd(
		-0.5, 0.5, 0xFF000000, 0, 0,  // top left
		-0.5, -0.5, 0xFF000000, 0, 0,  // bottom left
		0.5, -0.5, 0xFF000000, 0, 0  // bottom right
	);
	AEGfxTriAdd(
		-0.5, 0.5, 0xFF000000, 0, 0,  // top left
		0.5, 0.5, 0xFF000000, 0, 0,  // top right
		0.5, -0.5, 0xFF000000, 0, 0  // bottom right
	);
	_mesh = AEGfxMeshEnd();
}

Draw::~Draw() {
	AEGfxMeshFree(_mesh);
	delete _instance;
}

Draw* Draw::getInstance() {
	if (_instance == nullptr) {
		_instance = new Draw();
	}
	return _instance;
}


void Draw::rect(f32 transX, f32 transY, Color color, f32 scaleX, f32 scaleY, f32 rotation, f32 opacity) {
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
