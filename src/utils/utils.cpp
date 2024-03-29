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


#include "Pch.h"
#include "utils.h"

int mouseX, mouseY;
AEVec2 camOffset;


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

Point wtos(float x, float y) {
	x = x + AEGfxGetWindowWidth() / 2.f;
	y = -y + AEGfxGetWindowHeight() / 2.f;

	return Point{ x, y };
}


float precisionRound(float x, int precision) {
	int mult = static_cast<int>(pow(10, precision));
	int num = static_cast<int>(x * mult + 0.5f);
	return num / static_cast<f32>(mult);
}

float degToRad(float degrees) {
	return degrees * Math::m_PI / 180.f;
}

float lerp(float start, float end, float t) {
	return start + t * (end - start);
}


std::vector<std::string> split(const std::string& str, const char sep) {
    std::vector<std::string> out;
    std::string curr = "";

    for (const char c : str) {
        if (c == sep && curr.length() > 0) {
            out.push_back(curr);
            curr.clear();
            continue;
        }

        if (c != sep) {
            curr += c;
        }
    }
    if (curr.length() > 0) {
        out.push_back(curr);
    }

    return out;
}
