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

#include "Scene.h"
#include "Elements.hpp"
#include "utils.h"
#include "CombatManager.h"
#include "Enemy.h"

#include <vector>
//Testing SpriteAnimation
class SceneManager;
class CombatScene : public Scene
{
public:
	CombatScene();
	~CombatScene();

	virtual void Load();
	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

private:
	CombatScene(SceneManager* _sceneMgr); // This is used to register to SceneManager

	static CombatScene* sInstance; // The pointer to the object that gets registered
};


/*defining these functions here to chiong prototype, no time to make nicenice rn*/

//class Cat : public Enemy {
//public:
//	Cat(Element element = Element::Water, double health = 100, double dmg = 10, std::string texturePath = "", std::string textureRef = "", float screenX = 100, float screenY = 100, float size = 50);
//};

