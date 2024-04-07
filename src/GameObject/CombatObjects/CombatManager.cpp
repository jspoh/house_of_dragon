/* Start Header ************************************************************************/
/*!
\file CombatManager.cpp
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


#include "Pch.h"
#include "CombatManager.h"

CombatManager::~CombatManager() {
    cout << "Destroying CombatManager\n";
}

CombatManager& CombatManager::getInstance() {
    static CombatManager instance;
    return instance;
}

void CombatManager::start(TURN t) {
    m_turn = t;
    m_isInCombat = true;
    m_enemyNextTurnMs = m_INITIAL_ENEMY_ATTACK_TIME_MS;
}

void CombatManager::next() {
    //m_turn = static_cast<TURN>((m_turn + 1) % TURN::NUM_TURNS);
    m_turn = m_turn == TURN::PLAYER ? TURN::ENEMY : TURN::PLAYER;

    m_enemyNextTurnMs = rand() % (m_MAX_ATTACK_INTERVAL_MS - m_MIN_ATTACK_INTERVAL_MS) + m_MIN_ATTACK_INTERVAL_MS;
}


void CombatManager::end() {
    m_turn = TURN::NONE_TURN;
    m_isInCombat = false;
    m_selectedEnemy = nullptr;
}
