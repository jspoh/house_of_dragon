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
#include "Pch.h"

enum class PLAYER_BLOCKING_STATES {
	NOT_BLOCKING,
	ON_ENTER,
	ON_UPDATE,
	ON_EXIT
};

struct Shield {
	AEVec2 pos;
	AEVec2 size;
};


class Player : public Mob {
private:
	AEVec2 shieldInitialPos;
	const AEVec2 shieldBlockingPos { 0, 0 };

	Shield shield;

	void _drawHealth(float screenX, float screenY);

public:
	Player(double health = 100, double dmg = 10, Element element = Element::NO_ELEMENT);

	static constexpr int BLOCKING_TIMEOUT_MS = 2000;
	static constexpr int BLOCKING_DURATION = 2000;

	PLAYER_BLOCKING_STATES blockingState = PLAYER_BLOCKING_STATES::NOT_BLOCKING;

	/**
	 * .
	 *
	 * \param target
	 * \param attackEl element player attacked with
	 * \param qtMultiplier quicktime event multiplier
	 * \return damage dealt
	 */
	double attack(Mob& target, Element attackEl, double qtMultiplier);

	void update(double dt);

	void render();
};

