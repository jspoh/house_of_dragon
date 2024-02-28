/* Start Header ************************************************************************/
/*!
\file Enemy.h
\author Poh Jing Seng, jingseng.poh, 2301363
\par jingseng.poh\@digipen.edu
\date 28 feb 2024
\brief handles enemy in combat
/*
Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/


#pragma once

#include "Mob.h"
#include <string>
#include "utils.h"
#include <iostream>

class Enemy : public Mob {
private:
	std::string _textureRef;
	// screen pos
	Point _spos;
	// world pos (rendering only!!!!)
	Point _wpos;
	float _size = 50;

public:
	Enemy();
	Enemy(Element element, double health, double dmg, std::string texturePath, std::string textureRef, float screenX = 100, float screenY = 100, float size = 50);
	~Enemy();

	void render();
};
