#pragma once

#include "../Mob/Mob.h"
#include <string>
#include "../../../utils/utils.h"
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
