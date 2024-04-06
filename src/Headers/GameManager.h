/* Start Header ************************************************************************/
/*!
\file GameManager.h
\author Soh Wei Jie, weijie.soh, 2301289
\par weijie.soh\@digipen.edu
\date 28 feb 2024
\brief base game manager
/*
Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#pragma once

#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include <stdio.h>
#include <stdlib.h>
#include "Pch.h"
#include "SceneManager.h"
////#include "../GameObject/GameObjectManager.h"
#include "CombatManager.h"
#include "ParticleManager.h"

class GameManager
{
public:
	static GameManager& getInstance()
	{
		static GameManager game;
		return game;
	}
	void init();
	void run();
	void exit();
private:
	GameManager();
	~GameManager();
};
#endif
