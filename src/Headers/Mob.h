#pragma once

#include "Elements.hpp"


class Mob {
public:
	double maxHealth = 100;
	double health = maxHealth;
	double dmg;
	Element element;

	Mob();
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
