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
#ifndef GAMEOBJECT_MANAGER_H
#define GAMEOBJECT_MANAGER_H

#include "Vector3.h"
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
	void Update(double _dt);
	void Render();
	void Exit();

	void AddEntity(GameObject* _newEntity);
	bool RemoveEntity(GameObject* _existingEntity);
	std::list<GameObject*> GetEntityList();
	
	GameObject* FindObjectByReference(const std::string& _RefName);
	GameObject* FindInactiveObjectByReference(const std::string& _RefName);
private:
	GameObjectManager();
	virtual ~GameObjectManager();

	std::list<GameObject*> entityList;
};

#endif