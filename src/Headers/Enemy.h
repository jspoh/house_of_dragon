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

class Enemy : public Mob {
private:
	std::string _textureRef = "";
	// screen pos
	Point _spos{ 0 };
	// world pos (rendering only!!!!)
	Point _wpos{ 0 };
	float _size = 50;
	Point healthpos{ 0 };
	float fullhealth = 0;
	//attack multipler holder
	float initialAttack{ 0 };
	int attackMultiplerTurn{ 0 };
	int attackMultiplerTurnStart{ 0 };



public:
	bool isSelected = false;
	
	Enemy();
	Enemy(Element element, float health, float dmg, std::string texturePath, std::string textureRef, float screenX = 100, float screenY = 100, float size = 50);
	~Enemy();

	void update([[maybe_unused]] double dt);
	void render();
};


