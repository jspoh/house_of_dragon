/* Start Header ************************************************************************/
/*!
\file GameObjectManager.cpp
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
void GameObjectManager::update(double _dt)
{
	// Update all entities
	std::list<GameObject*>::iterator it, end;
	end = m_entityList.end();
	for (it = m_entityList.begin(); it != end; ++it)
	{
		(*it)->update(_dt);
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
void GameObjectManager::render()
{
	// Render all entities
	for (std::list<GameObject*>::iterator it = m_entityList.begin();
		it != m_entityList.end();
		it++)
	{
		(*it)->render();
	}
}


// Add an entity to this GameObjectManager
void GameObjectManager::addEntity(GameObject* _newEntity)
{
	m_entityList.push_back(_newEntity);
	cout << "GameObject Add" << "\n";
}

// Remove an entity from this GameObjectManager
bool GameObjectManager::removeEntity(GameObject* _existingEntity)
{
	// Find the entity's iterator
	std::list<GameObject*>::iterator findIter = std::find(m_entityList.begin(), m_entityList.end(), _existingEntity);

	// Delete the entity if found
	if (findIter != m_entityList.end())
	{
		delete* findIter;
		findIter = m_entityList.erase(findIter);
		return true;
	}
	// Return false if not found
	return false;
}

std::list<GameObject*> GameObjectManager::GetEntityList()
{
	return m_entityList;
}

void GameObjectManager::exit()
{
	// Delete all scenes stored and empty the entire map
	for (std::list<GameObject*>::iterator it = m_entityList.begin(); 
		it != m_entityList.end(); 
		it++)
	{
		(*it)->exit();
		delete (*it);
	}
	m_entityList.clear();
}

GameObject* GameObjectManager::findInactiveObjectByReference(const std::string& _RefName)
{
	for (std::list<GameObject*>::iterator it = m_entityList.begin();
		it != m_entityList.end();
		it++)
	{
		if(_RefName == (*it)->m_RefName && !(*it)->m_Active)
			return *it;
	}

	return nullptr;
}

GameObject* GameObjectManager::findObjectByReference(const std::string& _RefName)
{
	for (std::list<GameObject*>::iterator it = m_entityList.begin();
		it != m_entityList.end();
		it++)
	{
		if (_RefName == (*it)->m_RefName)
			return *it;
	}

	return nullptr;
}