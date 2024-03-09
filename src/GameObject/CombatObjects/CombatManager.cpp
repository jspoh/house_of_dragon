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



#include "CombatManager.h"

CombatManager* CombatManager::_instance = nullptr;

CombatManager::~CombatManager() {
    std::cout << "Destroying CombatManager\n";
}

CombatManager* CombatManager::getInstance() {
    if (!_instance) {
        _instance = new CombatManager();
    }
    return _instance;
}

void CombatManager::next() {
    turn = static_cast<TURN>((turn + 1) % TURN::NUM_TURNS);
}

void setSelectedEnemy(Enemy* enemy) {
    return;
}