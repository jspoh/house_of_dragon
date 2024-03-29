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
namespace {
	enum TURN {
		NONE_TURN,
		PLAYER,
		ENEMY,
		NUM_TURNS
	};
}

class CombatManager {
private:




public:
	~CombatManager();

	static constexpr int PLAYER_BLOCKING_REACTION_ALLOWANCE_MS = 750;

	TURN turn = TURN::PLAYER;
	EVENT_RESULTS qtEventResult = EVENT_RESULTS::NONE_EVENT_RESULTS;  // used to track user quicktime event result
	double qtEventMul = 1;  // !TODO: for timer events where multiplier can be altered based on accuracy
	Element attackElement = Element::NO_ELEMENT;  // used to track user attack element

	bool isPlayingEvent = false;

	Enemy* selectedEnemy = nullptr;

	static constexpr int minAttackIntervalMs = 1000;
	static constexpr int maxAttackIntervalMs = 5000;
	int enemyNextTurnMs = 0;

	static CombatManager& getInstance();

	/**
	 * for level scene to .
	 */
	bool isInCombat = false;

	/**
	 * start combat manager turn based system.
	 *
	 */
	void start();

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
