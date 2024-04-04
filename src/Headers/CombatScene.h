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

	void Load();
	void Init(CombatManager::TURN startingTurn = CombatManager::TURN::PLAYER);
	void Update(double dt);
	void Render();
	void Exit();
	void spawnEnemies(std::vector<std::string> enemyRefs);
	void cleanup();

	static CombatScene& getInstance();
private:

	
};
struct enemiesGroup {
	std::vector<bool> activeEnemy;
	std::vector<AEVec2> coordinates;
	std::vector<std::string> names;
	std::vector<Enemy*> enemies;
};
// !TODO: kuek
/**
 * @brief 
 * 1. read from json file the attributes and create Enemy objects
 * 2. 
 * @param enemyRefs 
 */
 // void spawnEnemies(std::vector<std::string> enemyRefs);


/*defining these functions here to chiong prototype, no time to make nicenice rn*/

//class Cat : public Enemy {
//public:
//	Cat(Element element = Element::Water, double health = 100, double dmg = 10, std::string texturePath = "", std::string textureRef = "", float screenX = 100, float screenY = 100, float size = 50);
//};


