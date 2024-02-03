#pragma once

#include "../utils.h"


struct Color;


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
	void rect(f32 transX = 0, f32 transY = 0, Color color = Color{ 1,1,1,1 }, f32 scaleX = 50, f32 scaleY = 50, f32 rotation = 0, f32 opacity = 1);
};