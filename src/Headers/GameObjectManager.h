#pragma once
/*!************************************************************************
\file GameObjectManager.h
\author Soh Wei Jie (weijie.soh)
\par DP email: weijie.soh\@digipen.edu
\par Course: csd1451
\par House 0F Dragons
\date 25-1-2024
\brief This source file handles GameObjects.
**************************************************************************/
#ifndef GAMEOBJECT_MANAGER_H
#define GAMEOBJECT_MANAGER_H

#include "Vector3.h"
#include "GameObject.h"
#include "2DCollider.h"


#include "GameObject_AmeTest.h"
#include "GameObject_Misc_Enemy.h"
#include "GameObject_Projectiles.h"
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