#include "../GameObject/GameObjectManager.h"
#include "../GameObject/GameObject.h"
#include "../Backend/2DCollider.h"

#include <iostream>
using namespace std;

// Update all entities
void GameObjectManager::Update(double _dt)
{
	// Update all entities
	std::list<GameObject*>::iterator it, end;
	end = entityList.end();
	for (it = entityList.begin(); it != end; ++it)
	{
		(*it)->Update(_dt);
	}

	// Clean up entities that are done
	it = entityList.begin();
	while (it != end)
	{
		if (!(*it)->m_Active)
		{
			// Delete if done
			delete* it;
			it = entityList.erase(it);
		}
		else
		{
			// Move on otherwise
			++it;
		}
	}
}

// Render all entities
void GameObjectManager::Render()
{
	// Render all entities
	std::list<GameObject*>::iterator it, end;
	end = entityList.end();
	for (it = entityList.begin(); it != end; ++it)
	{
		(*it)->Render();
	}
}

// Render the UI entities
void GameObjectManager::RenderUI()
{
	//// Render all entities UI
	//std::list<GameObject*>::iterator it, end;
	//end = entityList.end();
	//for (it = entityList.begin(); it != end; ++it)
	//{
	//	(*it)->RenderUI();
	//}
}

// Add an entity to this GameObjectManager
void GameObjectManager::AddEntity(GameObject* _newEntity)
{
	entityList.push_back(_newEntity);
	cout << "GameObject Add" << endl;
}

// Remove an entity from this GameObjectManager
bool GameObjectManager::RemoveEntity(GameObject* _existingEntity)
{
	// Find the entity's iterator
	std::list<GameObject*>::iterator findIter = std::find(entityList.begin(), entityList.end(), _existingEntity);

	// Delete the entity if found
	if (findIter != entityList.end())
	{
		delete* findIter;
		findIter = entityList.erase(findIter);
		return true;
	}
	// Return false if not found
	return false;
}

// Constructor
GameObjectManager::GameObjectManager()
{
}

// Destructor
GameObjectManager::~GameObjectManager()
{
	std::cout << "HI MEMORY DEBUGGING" << std::endl;
}

// Check for overlap
bool GameObjectManager::CheckOverlap(Vector3 thisMinAABB, Vector3 thisMaxAABB, Vector3 thatMinAABB, Vector3 thatMaxAABB)
{
	return false;
}

// Check if this entity's bounding sphere collided with that entity's bounding sphere 
bool GameObjectManager::CheckSphereCollision(GameObject* ThisEntity, GameObject* ThatEntity)
{
	//Ignore Warnings
	return false;
}

// Check if this entity collided with another entity, but both must have collider
bool GameObjectManager::CheckAABBCollision(GameObject* ThisEntity, GameObject* ThatEntity)
{
	//Ignore Warnings
	return false;
}

// Check if any Collider is colliding with another Collider
bool GameObjectManager::CheckForCollision(void)
{
	return false;
}

GameObject* GameObjectManager::FindObjectByReference(const std::string& _RefName)
{
	std::list<GameObject*>::iterator it, end;
	end = entityList.end();
	for (it = entityList.begin(); it != end; ++it)
	{
		if(_RefName == (*it)->m_RefName)
			return *it;
	}

	return nullptr;
}