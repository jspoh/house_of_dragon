/* Start Header ************************************************************************/
/*!
\file Mob.cpp
\author Poh Jing Seng, jingseng.poh, 2301363
\par jingseng.poh\@digipen.edu
\date 28 feb 2024
\brief abstract base class for player and enemy classes to inherit from
/*
Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/


#include "Mob.h"

Mob::Mob(Element element, float health, float dmg) : health(health), dmg(dmg), element(element), maxHealth(health) {

}

double Mob::attack(Mob& target) {
    DamageMultiplier dm = ElementProperties::getEffectiveDamage(this->element, target.element);
    float multiplier = 1;
    switch (dm) {
    case Weak:
        multiplier = 0.5;
        break;
    case Strong:
        multiplier = 2;
        break;
    }

    float damage = this->dmg * multiplier;
    target.health -= damage;
    return damage;
}

void Mob::reset() {
    this->health = maxHealth;
}


bool Mob::isDead() {
    return this->health <= 0;
}
