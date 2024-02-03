/*****************************************************************//**
 * \file   utils.cpp
 * \brief  common utility functions
 * 
 * \author jspoh
 * \date   February 2024
 *********************************************************************/


#include "utils.h"
#include <vector>
#include "../Backend/Pch.h"


/*utility functions*/

/* screen coordinates to world coordinates */
Point stow(float x, float y) {
	float wX = x - (AEGfxGetWindowWidth() / 2);
	float wY = (AEGfxGetWindowHeight() / 2) - y;  // Corrected this line
	return Point{ wX, wY };
}


/*class collision checker*/
bool CollisionChecker::_rectDistance(float screenX1, float screenY1, float w1, float h1, float screenX2, float screenY2, float w2, float h2) {
	// radius rect1 squared
	double r1sq = w1 * w1 + h1 * h1;
	// radius rect2 squared
	double r2sq = w2 * w2 + h2 * h2;

	double dx = screenX2 - screenX1;
	double dy = screenY2 - screenY1;
	// distance between rect1 and rect2 squared
	double dsq = dx * dx + dy * dy;

	if (dsq <= r1sq + r2sq) {
		return true;
	}
	return false;
}

bool CollisionChecker::_rectAABB(float screenX1, float screenY1, float w1, float h1, float screenX2, float screenY2, float w2, float h2) {
	/*x axis intersects*/
	if (screenX1 + (w1 / 2) >= screenX2 - (w2 / 2) && screenX1 - (w1 / 2) <= screenX2 + (w2 / 2)) {
		/*y axis intersects*/
		if (screenY1 + (h1 / 2) >= screenY2 - (h2 / 2) && screenY1 - (h1 / 2) <= screenY2 + (h2 / 2)) {
			return true;
		}
	}
	return false;
}

bool CollisionChecker::areRectsIntersecting(float screenX1, float screenY1, float w1, float h1, float screenX2, float screenY2, float w2, float h2) {
	if (_rectDistance(screenX1, screenY1, w1, h1, screenX2, screenY2, w2, h2)) {
		return _rectAABB(screenX1, screenY1, w1, h1, screenX2, screenY2, w2, h2);
	}
	return false;
}


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
