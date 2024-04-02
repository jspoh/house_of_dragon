/* Start Header ************************************************************************/
/*!
\file CombatPlayer.h
\author Poh Jing Seng, jingseng.poh, 2301363
\author Soh Wei Jie, weijie.soh, 
\par jingseng.poh\@digipen.edu
\par weijie.soh\@digipen.edu
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

	Shield shield;

	void _drawHealth(float screenX, float screenY);


	//attack multipler holder
	float initialAttack{ 0 };
	int attackMultiplerTurn{ 0 };
	int attackMultiplerTurnStart{ 0 };

	void _updateBlockingHands();

	void updateHands(float t_dt);

	// discarded in favour of hands
	void _updateShield(double dt);
	void _renderShield();

	float AttackedRenderX{ 0 };
	float AttackedRenderXprev{ 0 };
	float startingHealth{ 0 };
	bool attacked{ 0 };
	float healthRenderTime{ 0 };
	const float healthRenderTimeMax = 0.75f;




public:
	enum class HandAnimationType {
		None,
		Punch,
		Block,
		Ready,
	};

	static constexpr int shieldUpTransitionTimeMs = 100;
	static constexpr int shieldDownTransitionTimeMs = 300;
	static constexpr int shieldUpTimeMs = 1000;
	static constexpr int timeBeforeNextBlockMs = 1000;

	/**
	 * used to trigger hand animations.
	 * 
	 * \param t
	 */
	void setHandStateAnimationType(HandAnimationType t);


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
	void renderHealth(double x, double y);
	void render();
	void _renderHands();
	void playerAttacked();

	void attackMultipler(int turn);

private:
	HandAnimationType HandStateAnimationType = HandAnimationType::None; //Can use enum
};

