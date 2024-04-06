/* Start Header ************************************************************************/
/*!
\file GameObjectManager.h
\author Soh Wei Jie, weijie.soh, 2301289
\par weijie.soh\@digipen.edu
\date 01 Apr 2024
\brief Game object manager
/*
Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#pragma once

#include "GameObject.h"

#include "GameObject_Misc_Enemy.h"
#include "GameObject_Projectiles.h"

#define PLAYERSCREENPOSX 0
#define PLAYERSCREENPOSY 100

class GameObject;

class GameObjectManager : public Singleton<GameObjectManager>
{
	friend Singleton<GameObjectManager>;
public:
	void update(double _dt);
	void render();
	void exit();

	void addEntity(GameObject* _newEntity);
	bool removeEntity(GameObject* _existingEntity);
	std::list<GameObject*> GetEntityList();
	
	GameObject* findObjectByReference(const std::string& _RefName);
	GameObject* findInactiveObjectByReference(const std::string& _RefName);
private:
	GameObjectManager();
	virtual ~GameObjectManager();

	std::list<GameObject*> m_entityList;
};
