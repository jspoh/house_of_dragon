/* Start Header ************************************************************************/
/*!
\file Mob.h
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


#pragma once

#include "Elements.hpp"


class Mob {
public:
	const float m_maxHealth;
	float m_health;
	float m_dmg;
	Element element;

	Mob();
	Mob(Element element, float health, float dmg);

	bool isDead();

	/**
	 * .
	 *
	 * \param target
	 * \param attackEl
	 * \return amount of damage dealt
	 */
	virtual double attack(Mob& target);

	/**
	 * .
	 * 
	 * @param target
	 * @param multiplier custom multiplier (can be used for stuff like blocking or anything special)
	 */
	virtual double attack(Mob& target, float multiplier);

	virtual void reset();
};
