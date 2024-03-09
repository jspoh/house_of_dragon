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
        PLAYER,
        ENEMY,
        NUM_TURNS
    };
}

class CombatManager {
private:
    static CombatManager* _instance;




public:
    ~CombatManager();

    TURN turn = TURN::PLAYER;
    EVENT_RESULTS qtEventResult = EVENT_RESULTS::NONE_EVENT_RESULTS;  // used to track user quicktime event result
    double qtEventMul = 1;  // !TODO: for timer events where multiplier can be altered based on accuracy
    Element attackElement = Element::NO_ELEMENT;  // used to track user attack element

    bool isPlayingEvent = false;

    Enemy* selectedEnemy = nullptr;

    static CombatManager* getInstance();

    void next();

    void setSelectedEnemy(Enemy* enemy); // set the selected enemy for the damage intake
};
