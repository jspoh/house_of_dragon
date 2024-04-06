/* Start Header ************************************************************************/
/*!
\file CombatScene.h
\author Poh Jing Seng, jingseng.poh, 2301363
\par jingseng.poh\@digipen.edu
\date 28 feb 2024
\brief renders and manages the combat scene
/*
Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/


#pragma once

#include "Pch.h"

#include "Scene.h"
#include "Elements.hpp"
#include "CombatManager.h"
#include "Enemy.h"


class CombatScene
{
public:
	CombatScene();
	~CombatScene();

	void load();
	void init(CombatManager::TURN startingTurn = CombatManager::TURN::PLAYER);
	void update(double dt);
	void render();
	void exit();
	void spawnEnemies(std::vector<std::string> enemyRefs);
	void cleanup();

	bool getWinFlag();

	static CombatScene& getInstance();
private:

	
};
struct enemiesGroup {
	std::vector<bool> activeEnemy;
	std::vector<AEVec2> coordinates;
	std::vector<std::string> names;
	std::vector<Enemy*> enemies;
};

