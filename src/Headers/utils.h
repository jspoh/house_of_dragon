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
#include <vector>
//#include "../Backend/SingletonTemplate.h"

const int AEVK_OFFSET = 'a' - AEVK_A;

// why doesnt it work using std::array w size 26??
const std::vector<std::pair<int, const char>> keyMappings {
	{ AEVK_A, 'a' },
	{ AEVK_B, 'b' },
	{ AEVK_C, 'c' },
	{ AEVK_D, 'd' },
	{ AEVK_E, 'e' },
	{ AEVK_F, 'f' },
	{ AEVK_G, 'g' },
	{ AEVK_H, 'h' },
	{ AEVK_I, 'i' },
	{ AEVK_J, 'j' },
	{ AEVK_K, 'k' },
	{ AEVK_L, 'l' },
	{ AEVK_M, 'm' },
	{ AEVK_N, 'n' },
	{ AEVK_O, 'o' },
	{ AEVK_P, 'p' },
	{ AEVK_Q, 'q' },
	{ AEVK_R, 'r' },
	{ AEVK_S, 's' },
	{ AEVK_T, 't' },
	{ AEVK_U, 'u' },
	{ AEVK_V, 'v' },
	{ AEVK_W, 'w' },
	{ AEVK_X, 'x' },
	{ AEVK_Y, 'y' },
	{ AEVK_Z, 'z' }
};



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
 * world to screen coordinates.
 */
Point wtos(float x, float y);

/**
 * rounds a float to a set precision.
 */
float precisionRound(float x, int precision);

float degToRad(float degrees);

// mathematical technique used to find a value between two endpoints based on a linear progression.
float lerp(float start, float end, float t);

/*for ease of inclusion*/
#include "RenderHelper.h"
#include "CollisionChecker.h"
