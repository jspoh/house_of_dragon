/* Start Header ************************************************************************/
/*!
\file CombatPlayer.h
\author Poh Jing Seng, jingseng.poh, 2301363
\par jingseng.poh\@digipen.edu
\date 28 feb 2024
\brief handles player in combat
/*
Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/


#pragma once

#include "Mob.h"
#include "Pch.h"

enum class PLAYER_BLOCKING_STATES {
	NOT_BLOCKING,
	ON_ENTER,
	ON_UPDATE,
	ON_EXIT,
	ON_COOLDOWN
};

struct Shield {
	AEVec2 pos;
	AEVec2 size;
};


class Player : public Mob {
private:
	AEVec2 shieldInitialPos{ 0,0 };		// set in constructor
	AEVec2 shieldBlockingPos{ 0, 0 };
	AEVec2 shieldInitialToShieldBlocking_vector;		// vector from shield initial pos to final blocking pos
	float transitionUpSpeed;
	float transitionDownSpeed;
	int elapsedTimeMs = 0;
	// !TODO: jspoh json difficulty settinng
	static constexpr float snapThreshold = 20.f;		// distance before shield snaps

	AEVec2 camOffset{ 0,0 };

	Shield shield;

	void _drawHealth(float screenX, float screenY);


	//attack multipler holder
	float initialAttack{ 0 };
	int attackMultiplerTurn{ 0 };
	int attackMultiplerTurnStart{ 0 };


public:
	static constexpr int shieldUpTransitionTimeMs = 100;
	static constexpr int shieldDownTransitionTimeMs = 300;
	static constexpr int shieldUpTimeMs = 1000;
	static constexpr int timeBeforeNextBlockMs = 1000;
	Player(float health = 100, float dmg = 10, Element element = Element::NO_ELEMENT);
	~Player();

	static constexpr int BLOCKING_TIMEOUT_MS = 2000;
	static constexpr int BLOCKING_DURATION = 2000;

	PLAYER_BLOCKING_STATES blockingState = PLAYER_BLOCKING_STATES::NOT_BLOCKING;

	/**
	 * .
	 *
	 * \param target
	 * \param attackEl element player attacked with
	 * \param qtMultiplier quicktime event multiplier
	 * \return damage dealt
	 */
	void healthGain(float healthIncrease);

	float attack(Mob& target, Element attackEl, float qtMultiplier);

	void update(double dt);

	void render();

	void attackMultipler(int turn);
};

