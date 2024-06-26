/* Start Header ************************************************************************/
/*!
\file CollisionChecker.h
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



#pragma once


class CollisionChecker {
private:
	/**
	 * returns if rects are close enough for collision possibility.
	 *
	 * \param screenX1
	 * \param screenY1
	 * \param w1
	 * \param h1
	 * \param screenX2
	 * \param screenY2
	 * \param w2
	 * \param h2
	 * \return
	 */
	static bool _rectDistance(float screenX1, float screenY1, float w1, float h1, float screenX2, float screenY2, float w2, float h2);

	/**
	 * returns if axis aligned rects are colliding.
	 *
	 * \param screenX1
	 * \param screenY1
	 * \param w1
	 * \param h1
	 * \param screenX2
	 * \param screenY2
	 * \param w2
	 * \param h2
	 * \return
	 */
	static bool _rectAABB(float screenX1, float screenY1, float w1, float h1, float screenX2, float screenY2, float w2, float h2);

public:
	static bool areRectsIntersecting(float screenX1, float screenY1, float w1, float h1, float screenX2, float screenY2, float w2, float h2);

	static bool isMouseInRect(float rectScreenX, float rectScreenY, float rectW, float rectH, float mouseX, float mouseY);

	static bool isMouseInCircle(float cX, float cY, float radius, float mouseX, float mouseY);

	static bool isRectTouchingScreenXBorder(float rectScreenX, float rectScreenY, float w, float h, float padding = 0);

	static bool isRectTouchingScreenYBorder(float rectScreenX, float rectScreenY, float w, float h, float padding = 0);

	static bool isRectTouchingScreenBorder(float rectScreenX, float rectScreenY, float w, float h);

	/**
	 * @brief 
	 * 
	 * @param cx1 circle1 screen x
	 * @param cy1 circle1 screen y
	 * @param cr1 circle1 radius
	 * @param cx2 circle2 screen x
	 * @param cy2 circle2 screen y
	 * @param cr2 circle2 radius
	 * @return true 
	 * @return false 
	 */
	static bool areCirclesIntersecting(float cx1, float cy1, float cr1, float cx2, float cy2, float cr2);

	static float getDistance(float x1, float y1, float x2, float y2);
};

