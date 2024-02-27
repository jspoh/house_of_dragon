#include "Mob.h"

Mob::Mob(Element element, double health, double dmg) : health(health), dmg(dmg), element(element), maxHealth(health) {

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

    double damage = this->dmg * multiplier;
    target.health -= damage;
    return damage;
}

void Mob::reset() {
    this->health = maxHealth;
}


bool Mob::isDead() {
    return this->health <= 0;
}
