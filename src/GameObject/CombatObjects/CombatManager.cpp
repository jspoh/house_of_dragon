/* Start Header ************************************************************************/
/*!
\file CombatManager.cpp
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


#include "Pch.h"
#include "CombatManager.h"

CombatManager::~CombatManager() {
    std::cout << "Destroying CombatManager\n";
}

CombatManager& CombatManager::getInstance() {
    static CombatManager instance;
    return instance;
}

void CombatManager::start(TURN t) {
    turn = t;
    isInCombat = true;
}

void CombatManager::next() {
    //turn = static_cast<TURN>((turn + 1) % TURN::NUM_TURNS);
    turn = turn == TURN::PLAYER ? TURN::ENEMY : TURN::PLAYER;

    enemyNextTurnMs = rand() % (maxAttackIntervalMs - minAttackIntervalMs) + minAttackIntervalMs;
}


void CombatManager::end() {
    turn = TURN::NONE_TURN;
    isInCombat = false;
}
