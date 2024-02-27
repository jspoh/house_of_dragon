#pragma once

#include "../../../GameObject/CombatObjects/Elements/Elements.hpp"


class Mob {
public:
	double health;
	double maxHealth;
	double dmg;
	Element element;

	Mob(Element element, double health, double dmg);

	bool isDead();

	/**
	 * .
	 *
	 * \param target
	 * \param attackEl
	 * \return amount of damage dealt
	 */
	virtual double attack(Mob& target);

	virtual void reset();
};
