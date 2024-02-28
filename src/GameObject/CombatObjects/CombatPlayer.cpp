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
#include <iostream>


Player::Player(double health, double dmg, Element element) : Mob(element, health, dmg) {

}

void Player::_drawHealth(float screenX, float screenY) {
    RenderHelper::getInstance()->text(std::to_string(this->health), screenX, screenY);
}

void Player::render() {
    this->_drawHealth(150, 150);
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
