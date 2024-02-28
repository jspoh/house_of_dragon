#pragma once

#include "Event.h"
#include "Elements.hpp"
#include "Pch.h"

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

    static CombatManager* getInstance();

    void next();
};
