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

};

