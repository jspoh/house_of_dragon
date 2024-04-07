/* Start Header ************************************************************************/
/*!
\file CombatManager.h
\author Poh Jing Seng, jingseng.poh, 2301363
\par jingseng.poh\@digipen.edu
\date 28 feb 2024
\brief combat turn manager for the game
/*
Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/


#pragma once

#include "Event.h"
#include "Elements.hpp"
#include "Pch.h"
#include "Enemy.h"

class CombatManager {
private:




public:

	enum TURN {
		NONE_TURN,
		PLAYER,
		ENEMY,
		NUM_TURNS
	};


	static CombatManager& getInstance();
	~CombatManager();

	TURN m_turn = TURN::PLAYER;
	EVENT_TYPES m_eventType;
	EVENT_RESULTS m_qtEventResult = EVENT_RESULTS::NONE_EVENT_RESULTS;  // used to track user quicktime event result
	Element m_attackElement = Element::NO_ELEMENT;  // used to track user attack element

	bool m_isPlayingEvent = false;
	bool m_playerFledLastFight = false;		// used to implement progression penalty if player flees from a fight

	Enemy* m_selectedEnemy = nullptr;

	static constexpr int m_MIN_ATTACK_INTERVAL_MS = 1000;
	static constexpr int m_MAX_ATTACK_INTERVAL_MS = 3000;
	static constexpr int m_INITIAL_ENEMY_ATTACK_TIME_MS = 5000;	// if enemy starts first, wait this long before m_isAttacking player
	int m_enemyNextTurnMs = 0;


	/**
	 * for level scene to .
	 */
	bool m_isInCombat = false;

	/**
	 * start combat manager turn based system.
	 *
	 */
	void start(TURN t = TURN::PLAYER);

	/**
	 * go to next turn
	 *
	 * end current player's turn and give turn to the next player (or enemy in our case).
	 *
	 */
	void next();

	/**
	 * combat is over, end turn system.
	 *
	 */
	void end();
};
