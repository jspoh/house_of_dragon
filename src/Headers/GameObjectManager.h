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

#include "SingletonTemplate.h"
#include "Vector3.h"
#include "GameObject.h"
#include "2DCollider.h"
#include <list>


#include "GameObject_AmeTest.h"
#include "GameObject_Misc_Enemy.h"

#include <iostream>
using namespace std; //Remvoe in future


class GameObject;

class GameObjectManager : public Singleton<GameObjectManager>
{
	friend Singleton<GameObjectManager>;
public:
	void Update(double _dt);
	void Render();
	//void RenderUI();
	void Exit();

	void AddEntity(GameObject* _newEntity);
	bool RemoveEntity(GameObject* _existingEntity);
	std::list<GameObject*> GetEntityList(); //Only use by SceneLevelBuilder - DONT TOUCH
	
	GameObject* FindObjectByReference(const std::string& _RefName);
private:
	GameObjectManager();
	virtual ~GameObjectManager();

	//// Check for overlap
	//bool CheckOverlap(Vector3 thisMinAABB, Vector3 thisMaxAABB, Vector3 thatMinAABB, Vector3 thatMaxAABB);
	//// Check if this entity's bounding sphere collided with that entity's bounding sphere 
	//bool CheckSphereCollision(GameObject* ThisEntity, GameObject* ThatEntity);
	//// Check if this entity collided with another entity, but both must have collider
	//bool CheckAABBCollision(GameObject* ThisEntity, GameObject* ThatEntity);
	//// Check if any Collider is colliding with another Collider
	//bool CheckForCollision(void);

	std::list<GameObject*> entityList;
};

#endif