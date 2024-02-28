#pragma once

#include "Mob.h"


class Player : public Mob {
private:
	void _drawHealth(float screenX, float screenY);

public:
	Player(double health = 100, double dmg = 10, Element element = Element::NO_ELEMENT);

	/**
	 * .
	 *
	 * \param target
	 * \param attackEl element player attacked with
	 * \param qtMultiplier quicktime event multiplier
	 * \return damage dealt
	 */
	double attack(Mob& target, Element attackEl, double qtMultiplier);

	void render();
};

