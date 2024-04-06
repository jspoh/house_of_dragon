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
	AEVec2 m_shieldInitialPos{ 0,0 };		// set in constructor
	AEVec2 m_shieldBlockingPos{ 0, 0 };
	AEVec2 m_shieldInitialToShieldBlockingVector;		// vector from m_shield initial pos to final blocking pos
	float m_transitionUpSpeed;
	float m_transitionDownSpeed;
	int m_elapsedTimeMs = 0;
	static constexpr float m_snapThreshold = 20.f;		// distance before m_shield snaps

	Shield m_shield;

	void _drawHealth(float screenX, float screenY);

	void _updateBlockingHands();

	void updateHands(float t_dt);

	// discarded in favour of hands
	void _updateShield(double dt);
	void _renderShield();

	float m_healthRenderTime{ 0 };
	const float m_healthRenderTimeMax = 0.75f;

	float m_renderX{ 0 };
	float m_renderXprev{ 0 };
	bool m_hasAttacked{ 0 };
	bool m_healthIncrease{ 0 };

	int m_playerLevel;

	// for every level increment, increase dmg by percentage
	static constexpr float m_LEVEL_DAMAGE_INC_PERCENTAGE = 0.1f;

	// dmgMul is used when player consumes an item that grants a damage boost
	static constexpr float m_DEFAULT_DMG_MUL = 1;
	float m_dmgMul = m_DEFAULT_DMG_MUL;

public:
	enum class HandAnimationType {
		None,
		Punch,
		Block,
		Ready,
	};

	static constexpr int m_shieldUpTransitionTimeMs = 100;
	static constexpr int m_shieldDownTransitionTimeMs = 300;
	static constexpr int m_shieldUpTimeMs = 1000;
	static constexpr int m_timeBeforeNextBlockMs = 1000;

	/**
	 * used to trigger hand animations.
	 * 
	 * \param t
	 */
	void setHandStateAnimationType(HandAnimationType t);


	Player(float m_health = 100, float m_dmg = 10, Element element = Element::NO_ELEMENT);
	~Player();

	static constexpr int m_BLOCKING_TIMEOUT_MS = 2000;
	static constexpr int m_BLOCKING_DURATION = 2000;

	PLAYER_BLOCKING_STATES m_blockingState = PLAYER_BLOCKING_STATES::NOT_BLOCKING;

	/**
	 * .
	 *
	 * \param target
	 * \param attackEl element player m_hasAttacked with
	 * \param qtMultiplier quicktime event multiplier
	 * \return damage dealt
	 */
	void healthGain(float healthIncrease);

	/**
	 * set next attack's damage multiplier.
	 * to be used when player consumes an item
	 * 
	 * damage multiplier will not stick through different
	 * combats!
	 * 
	 * \param mul
	 */
	void setNextAttackDmgMul(float mul);

	float attack(Mob& target, Element attackEl, float qtMultiplier);

	void update(double dt);
	void renderHealth(double x, double y);
	void render();
	void _renderHands();
	void playerAttacked();

	std::unordered_map<std::string, int> m_inventory;

	int getLevel() const;

	void resetHealth();

	void giveXpForEnemyKilled(int enemiesKilled = 1);

private:
	HandAnimationType m_HandStateAnimationType = HandAnimationType::None; //Can use enum
};

