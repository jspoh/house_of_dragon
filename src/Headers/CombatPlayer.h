/* Start Header ************************************************************************/
/*!
\file CombatPlayer.h
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

