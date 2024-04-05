/* Start Header ************************************************************************/
/*!
\file CombatManager.h
\author Poh Jing Seng, jingseng.poh, 2301363
\par jingseng.poh\@digipen.edu
\date 28 feb 2024
\brief combat manager for the game
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

	TURN turn = TURN::PLAYER;
	EVENT_TYPES eventType;
	EVENT_RESULTS qtEventResult = EVENT_RESULTS::NONE_EVENT_RESULTS;  // used to track user quicktime event result
	Element attackElement = Element::NO_ELEMENT;  // used to track user attack element

	bool isPlayingEvent = false;
	bool playerFledLastFight = false;		// used to implement progression penalty if player flees from a fight

	Enemy* selectedEnemy = nullptr;

	static constexpr int minAttackIntervalMs = 1000;
	static constexpr int maxAttackIntervalMs = 3000;
	static constexpr int initialEnemyAttackTimeMs = 5000;	// if enemy starts first, wait this long before attacking player
	int enemyNextTurnMs = 0;


	/**
	 * for level scene to .
	 */
	bool isInCombat = false;

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
