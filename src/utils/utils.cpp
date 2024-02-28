/* Start Header ************************************************************************/
/*!
\file utils.cpp
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


#include "utils.h"
#include <vector>
#include "Pch.h"


/*utility functions*/

/* screen coordinates to world coordinates */
Point stow(float x, float y) {
	float wX = x - (AEGfxGetWindowWidth() / 2);
	float wY = (AEGfxGetWindowHeight() / 2) - y;  // Corrected this line
	return Point{ wX, wY };
}

Point ston(float x, float y) {
	float nX = (2.0f * x / AEGfxGetWindowWidth()) - 1.0f;
	float nY = 1.0f - (2.0f * y / AEGfxGetWindowHeight());

	return Point{ nX, nY };
}

float precisionRound(float x, int precision) {
	int mult = static_cast<int>(pow(10, precision));
	int num = static_cast<int>(x * mult + 0.5f);
	return num / static_cast<float>(mult);
}

namespace {
	void _toastHelper(std::string& msg) {
		//RenderHelper::getInstance()->text(msg);

	}
}

void toast(std::string msg) {

}
