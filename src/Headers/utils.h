/* Start Header ************************************************************************/
/*!
\file utils.h
\author Poh Jing Seng, jingseng.poh, 2301363
\par jingseng.poh\@digipen.edu
\date 3 Feb 2024
\brief common utility functions
/*
Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/


#pragma once


#include "AEEngine.h"
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

/* screen coordinates to normalized coordinates */
Point ston(float x, float y);

/**
 * rounds a float to a set precision.
 */
float precisionRound(float x, int precision);

// mathematical technique used to find a value between two endpoints based on a linear progression.
float lerp(float start, float end, float t);

/*for ease of inclusion*/
#include "RenderHelper.h"
#include "CollisionChecker.h"
