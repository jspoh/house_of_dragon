#include "Pch.h"
#include "GameObjectManager.h"

// Constructor
GameObjectManager::GameObjectManager()
{
}

// Destructor
GameObjectManager::~GameObjectManager()
{
}

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

	//Dont clean up during update 
	//// Clean up entities that are done
	//it = entityList.begin();
	//while (it != end)
	//{
	//	if (!(*it)->m_Active)
	//	{
	//		// Delete if done
	//		delete* it;
	//		it = entityList.erase(it);
	//	}
	//	else
	//	{
	//		// Move on otherwise
	//		++it;
	//	}
	//}
}

// Render all entities
void GameObjectManager::Render()
{
	// Render all entities
	for (std::list<GameObject*>::iterator it = entityList.begin();
		it != entityList.end();
		it++)
	{
		(*it)->Render();
	}
}


// Add an entity to this GameObjectManager
void GameObjectManager::AddEntity(GameObject* _newEntity)
{
	entityList.push_back(_newEntity);
	cout << "GameObject Add" << "\n";
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

std::list<GameObject*> GameObjectManager::GetEntityList()
{
	return entityList;
}

void GameObjectManager::Exit()
{
	// Delete all scenes stored and empty the entire map
	for (std::list<GameObject*>::iterator it = entityList.begin(); 
		it != entityList.end(); 
		it++)
	{
		(*it)->Exit();
		delete (*it);
	}
	entityList.clear();
}

GameObject* GameObjectManager::FindInactiveObjectByReference(const std::string& _RefName)
{
	for (std::list<GameObject*>::iterator it = entityList.begin();
		it != entityList.end();
		it++)
	{
		if(_RefName == (*it)->m_RefName && !(*it)->m_Active)
			return *it;
	}

	return nullptr;
}

GameObject* GameObjectManager::FindObjectByReference(const std::string& _RefName)
{
	for (std::list<GameObject*>::iterator it = entityList.begin();
		it != entityList.end();
		it++)
	{
		if (_RefName == (*it)->m_RefName)
			return *it;
	}

	return nullptr;
}