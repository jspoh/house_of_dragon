#pragma once


#include "../../Extern/AlphaEngine/include/AEEngine.h"
//#include "../Backend/SingletonTemplate.h"


/**
 * all values to be between 0 and 1 (both inclusive).
 */
struct Color {
	f32 r, g, b, a;
};

struct Point {
	float x, y;
};

struct Controls {
	int UP, DOWN, LEFT, RIGHT;
};


/**
 * converts screen coordinates to world coordinates
 * 
 * \return 
 */
Point stow(float x, float y);

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


class Draw /*: Singleton<Draw>*/ {
private:
	static Draw* _instance;
	AEGfxVertexList* _mesh;

	Draw();
	~Draw();

protected:

public:
	static Draw* getInstance();

	//Draw* getInstance();

	/**
	 * .
	 * 
	 * \param color
	 * \param scaleX
	 * \param scaleY
	 * \param rotation anticlockwise rotation of `rotation` degrees
	 * \param transX
	 * \param transY
	 * \param opacity [0.0, 1.0]
	 */
	void rect(f32 transX=0, f32 transY=0, Color color=Color{1,1,1,1}, f32 scaleX=50, f32 scaleY=50, f32 rotation=0, f32 opacity=1);
};

