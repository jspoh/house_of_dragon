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

#include "Pch.h"
#include "Mob.h"

Mob::Mob(Element element, float health, float dmg) : m_health(health), m_dmg(dmg), element(element), m_maxHealth(health) {

}

double Mob::attack(Mob& target) {
    DamageMultiplier dm = ElementProperties::getEffectiveDamage(this->element, target.element);
    float elementMultiplier = 1;
    switch (dm) {
    case Weak:
        elementMultiplier = 0.5;
        break;
    case Strong:
        elementMultiplier = 2;
        break;
    }

    float damage = this->m_dmg * elementMultiplier;
    target.m_health -= damage;
    return damage;
}

double Mob::attack(Mob& target, float multiplier) {
    DamageMultiplier dm = ElementProperties::getEffectiveDamage(this->element, target.element);
    float elementMultiplier = 1;
    switch (dm) {
    case Weak:
        elementMultiplier = 0.5;
        break;
    case Strong:
        elementMultiplier = 2;
        break;
    }

    float damage = this->m_dmg * elementMultiplier * multiplier;
    target.m_health -= damage;
    return damage;
}

void Mob::reset() {
    this->m_health = m_maxHealth;
}


bool Mob::isDead() {
    return this->m_health <= 0;
}
