/* Start Header ************************************************************************/
/*!
\file CombatPlayer.cpp
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


#include "CombatPlayer.h"
#include "RenderHelper.h"
#include "MyMath.h"
#include <iostream>


Player::Player(double health, double dmg, Element element) : Mob(element, health, dmg) {
    RenderHelper::getInstance()->registerTexture("shield", "./Assets/Combat_UI/shield.png");

    AEVec2Set(&shield.pos, -AEGfxGetWindowWidth() / 2.f, -AEGfxGetWindowHeight() / 2.f * 2.f);
    AEVec2Set(&shield.size, AEGfxGetWindowWidth() / 2.f, (AEGfxGetWindowWidth() / 2.f) * 2.f);
}

void Player::_drawHealth(float screenX, float screenY) {
    RenderHelper::getInstance()->text(std::to_string(this->health), screenX, screenY);
}

void Player::update(double dt) {
    if (AEInputCheckCurr(AEVK_SPACE) && blockingState == PLAYER_BLOCKING_STATES::NOT_BLOCKING) {
        blockingState = PLAYER_BLOCKING_STATES::ON_ENTER;
    }
    else if (blockingState != PLAYER_BLOCKING_STATES::NOT_BLOCKING && !AEInputCheckCurr(AEVK_SPACE)) {
        blockingState = PLAYER_BLOCKING_STATES::ON_EXIT;
    }

    switch (blockingState) {
    case PLAYER_BLOCKING_STATES::NOT_BLOCKING:
        break;
    case PLAYER_BLOCKING_STATES::ON_ENTER:
        // translate the shield up
        break;
    case PLAYER_BLOCKING_STATES::ON_UPDATE:
        break;
    case PLAYER_BLOCKING_STATES::ON_EXIT:
        break;
    }
}

void Player::render() {
    this->_drawHealth(150, 150);

    switch (blockingState) {
    case PLAYER_BLOCKING_STATES::NOT_BLOCKING:
    case PLAYER_BLOCKING_STATES::ON_ENTER:
    case PLAYER_BLOCKING_STATES::ON_UPDATE:
    case PLAYER_BLOCKING_STATES::ON_EXIT:
        RenderHelper::getInstance()->texture("shield", shield.pos.x, shield.pos.y, shield.size.x, shield.size.y, 1, Color{ 0,0,0,0 }, Math::m_PI);
        break;
    }
}

double Player::attack(Mob& target, Element attackEl, double qtMultiplier) {
    DamageMultiplier dm = ElementProperties::getEffectiveDamage(attackEl, target.element);
    float multiplier = 1;
    std::cout << "attackEl enum: " << attackEl << "\n";
    std::cout << "targetEl enum: " << target.element << "\n";
    std::cout << "Damage multiplier enum: " << dm << "\n";
    switch (dm) {
    case Weak:
        multiplier = 0.5;
        break;
    case Strong:
        multiplier = 2;
        break;
    }

    double damage = this->dmg * multiplier * qtMultiplier;
    target.health -= damage;
    return damage;
}
