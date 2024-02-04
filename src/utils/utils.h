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


/*for ease of inclusion*/
#include "./Draw/Draw.h"
#include "./CollisionChecker/CollisionChecker.h"
