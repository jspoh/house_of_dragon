/* Start Header ************************************************************************/
/*!
\file SceneManager.cpp
\author Soh Wei Jie, weijie.soh, 2301289
\par weijie.soh\@digipen.edu
\date 01 Apr 2024
\brief manages changing of scenes
/*
Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/


#include "Pch.h"
#include "SceneManager.h"
#include "Scene.h"
#include <stdexcept>

SceneManager::SceneManager() : activeScene(nullptr), nextScene(nullptr)
{

}

SceneManager::~SceneManager()
{
}

void SceneManager::update(double _dt)
{
	// Check for change of scene
	if (nextScene != activeScene || restart)
	{
		if (activeScene)
		{
			// Scene is valid, need to call appropriate function to exit
			activeScene->exit();
		}
		
		activeScene = nextScene;
		//Change this for restart function
		activeScene->load();
		activeScene->init();

		if (restart) {
			restart = !restart;
		}
	}

	if (activeScene) {
		activeScene->update(_dt);
	}
}

void SceneManager::render()
{
	if (activeScene) {
		activeScene->render();
	}
}

void SceneManager::exit()
{
	// Delete all scenes stored and empty the entire map
	activeScene->exit();
	activeScene = nullptr;
	std::map<std::string, Scene*>::iterator it, end;
	end = sceneMap.end();
	for (it = sceneMap.begin(); it != end; ++it)
	{
		delete it->second;
	}
	sceneMap.clear();
}

void SceneManager::AddScene(const std::string& _name, Scene* _scene)
{
	if (CheckSceneExist(_name))
	{
		// Scene Exist, unable to proceed
		//throw std::exception("Duplicate scene name provided");
	}

	if (_scene == nullptr)
	{
		throw std::invalid_argument("Invalid scene pointer");
	}

	// Nothing wrong, add the scene to our map
	sceneMap[_name] = _scene;
}

void SceneManager::RemoveScene(const std::string& _name)
{
	// Does nothing if it does not exist
	if (!CheckSceneExist(_name))
		return;

	Scene* target = sceneMap[_name];
	if (target == activeScene || target == nextScene)
	{
		//throw std::exception("Unable to remove active/next scene");
	}

	// Delete and remove from our map
	delete target;
	sceneMap.erase(_name);
}

void SceneManager::SetActiveScene(const std::string& _name)
{
	if (!CheckSceneExist(_name))
	{
		// Scene does not exist
		//throw std::exception("Scene does not exist");
	}

	// Scene exist, set the next scene pointer to that scene
	nextScene = sceneMap[_name];
}

bool SceneManager::CheckSceneExist(const std::string& _name)
{
	return sceneMap.count(_name) != 0;
}

//SceneManager& SceneManager::GetCurrScene(void)
//{
//	static Scene scene;
//	return scene;
//}

void SceneManager::restartScene() {
	restart = true;
}

