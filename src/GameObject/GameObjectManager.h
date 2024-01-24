#pragma once
#ifndef GAMEOBJECT_MANAGER_H
#define GAMEOBJECT_MANAGER_H

#include "../Backend/SingletonTemplate.h"
#include "../Backend/Vector3.h"
#include <list>

class GameObject;

class GameObjectManager : public Singleton<GameObjectManager>
{
	friend Singleton<GameObjectManager>;
public:
	void Update(double _dt);
	void Render();
	void RenderUI();

	void AddEntity(GameObject* _newEntity);
	bool RemoveEntity(GameObject* _existingEntity);

private:
	GameObjectManager();
	virtual ~GameObjectManager();

	// Check for overlap
	bool CheckOverlap(Vector3 thisMinAABB, Vector3 thisMaxAABB, Vector3 thatMinAABB, Vector3 thatMaxAABB);
	// Check if this entity's bounding sphere collided with that entity's bounding sphere 
	bool CheckSphereCollision(GameObject* ThisEntity, GameObject* ThatEntity);
	// Check if this entity collided with another entity, but both must have collider
	bool CheckAABBCollision(GameObject* ThisEntity, GameObject* ThatEntity);
	// Check if any Collider is colliding with another Collider
	bool CheckForCollision(void);

	std::list<GameObject*> entityList;
};

#endif