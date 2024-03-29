/* Start Header ************************************************************************/
/*!
\file CollisionChecker.cpp
\author Poh Jing Seng, jingseng.poh, 2301363
\par jingseng.poh\@digipen.edu
\date 3 Feb 2024
\brief abstracts away collision checking
/*
Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#include "Pch.h"
#include "CollisionChecker.h"


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

bool CollisionChecker::isMouseInRect(float rectScreenX, float rectScreenY, float rectW, float rectH, float mouseX, float mouseY) {
	return _rectAABB(rectScreenX, rectScreenY, rectW, rectH, mouseX, mouseY, 1, 1);
}

bool CollisionChecker::isMouseInCircle(float cX, float cY, float radius, float mouseX, float mouseY) {
	float distanceSq = (mouseX - cX) * (mouseX - cX) + (mouseY - cY) * (mouseY - cY);
	return distanceSq <= radius * radius;
}

bool CollisionChecker::areRectsIntersecting(float screenX1, float screenY1, float w1, float h1, float screenX2, float screenY2, float w2, float h2) {
	if (_rectDistance(screenX1, screenY1, w1, h1, screenX2, screenY2, w2, h2)) {
		return _rectAABB(screenX1, screenY1, w1, h1, screenX2, screenY2, w2, h2);
	}
	return false;
}

bool CollisionChecker::isRectTouchingScreenXBorder([[maybe_unused]] float rectScreenX, [[maybe_unused]] float rectScreenY, float w, [[maybe_unused]] float h) {
	if (rectScreenX - w / 2 <= 0) {
		return true;		// left
	}
	if (rectScreenX + w / 2 >= AEGfxGetWindowWidth()) {
		return true;		// right
	}
	return false;
}

bool CollisionChecker::isRectTouchingScreenYBorder([[maybe_unused]] float rectScreenX, float rectScreenY, [[maybe_unused]] float w, float h) {
	if (rectScreenY - h / 2 <= 0) {
		return true;		// top
	}
	if (rectScreenY + h / 2 >= AEGfxGetWindowHeight()) {
		return true;		// bottom
	}
	return false;
}


bool CollisionChecker::isRectTouchingScreenBorder(float rectScreenX, float rectScreenY, float w, float h) {
	if (isRectTouchingScreenXBorder(rectScreenX, rectScreenY, w, h) || isRectTouchingScreenYBorder(rectScreenX, rectScreenY, w, h)) {
		return true;
	}
	return false;
}

bool CollisionChecker::areCirclesIntersecting(float cx1, float cy1, float cr1, float cx2, float cy2, float cr2) {
	float distanceSq = (cx1 - cx2) * (cx1 - cx2) + (cy1 - cy2) * (cy1 - cy2);
	return (cr1 + cr2) * (cr1 + cr2) >= distanceSq;
}
