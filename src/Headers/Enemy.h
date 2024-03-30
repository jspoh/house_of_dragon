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
	float startingHealth{ 0 };
	//attack multipler holder

	Point attackPoint{ 0 };
	Point AttackEnd{ 0};
	float attacktime{ 0 };
	// a flag to help render the shaking
	bool attacked{0};
	bool attacking{ 0 };
	//shaking variables
	float shakeDuration = 1.f; // Duration of the shake effect in seconds
	float shakeAmplitude = 20.0f; // Maximum displacement during shake
	float shakeFrequency = 25.0f;

	float AttackedRenderX{ 0 };
	float AttackedRenderXprev{ 0 };
	const float healthRenderTimeMax = 0.75f;
	float healthRenderTime{ 0 };



public:
	bool isSelected = false;

	const int projectileTravelTimeMs;
	
	Enemy(Element element, float health, float dmg, std::string texturePath, std::string textureRef, float screenX = 100, float screenY = 100, float size = 50, int _projectileTravelTimeMs = 1000);
	~Enemy();

	void update([[maybe_unused]] double dt);
	void render();
	void enemyAttacked();
	void enemyAttacking(float timeleft);
	void EnemyAttackStop();
};


