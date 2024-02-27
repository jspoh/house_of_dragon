#include "CombatManager.h"

CombatManager* CombatManager::getInstance() {
    if (!_instance) {
        _instance = new CombatManager();
    }
    return _instance;
}

void CombatManager::next() {
    turn = static_cast<TURN>((turn + 1) % TURN::NUM_TURNS);
}
CombatManager* CombatManager::_instance = nullptr;