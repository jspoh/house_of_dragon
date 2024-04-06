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
	std::string m_textureRef = "";
	// screen pos
	AEVec2 m_spos{ 0 };
	// world pos (rendering only!!!!)
	AEVec2 m_wpos{ 0 };
	float m_size = 50;
	AEVec2 m_healthpos{ 0 };
	float m_fullhealth = 0;
	//attack multipler holder

	AEVec2 m_attackPoint{ 0 };
	AEVec2 m_attackEnd{ 0};
	float m_attacktime{ 0 };
	// a flag to help render the shaking
	bool m_attacked{0};
	bool m_isAttacking{ 0 };
	//shaking variables
	float m_shakeDuration = 1.f; // Duration of the shake effect in seconds
	float m_shakeAmplitude = 20.0f; // Maximum displacement during shake
	float m_shakeFrequency = 25.0f;

	float m_attackedRenderX{ 0 };
	float m_attackedRenderXprev{ 0 };
	const float m_healthRenderTimeMax = 0.75f;
	float m_healthRenderTime{ 0 };
	std::string m_elementString;



public:
	bool m_isSelected = false;

	std::string getTextureRef() const;
	void elementstringinput(std::string el);
	static constexpr int m_PROJECTILE_TRAVEL_TIME_MS = 1000;
	
	Enemy(Element element, float m_health, float m_dmg, std::string texturePath, std::string textureRef, float screenX = 100, float screenY = 100, float size = 50);
	~Enemy();

	void update([[maybe_unused]] double dt);
	void render();
	void enemyAttacked();
	void enemyAttacking(float timeleft);
	void EnemyAttackStop();

	AEVec2 getWorldPos() const;

	AEVec2 getSize() const;
};


