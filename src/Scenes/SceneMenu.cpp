#include "SceneMenu.h"

#include "../Backend/GameManager.h" //? Still thinking
#include "SceneManager.h"
#include <iostream> //Remove upon test finish

SceneMenu* SceneMenu::sInstance = new SceneMenu(SceneManager::GetInstance());

SceneMenu::SceneMenu()
{
}

SceneMenu::SceneMenu(SceneManager* _sceneMgr)
{
	_sceneMgr->AddScene("SceneMenu", this);
}

SceneMenu::~SceneMenu()
{
}

void SceneMenu::Load()
{
	//Still debating whether need this
}

void SceneMenu::Init()
{
	std::cout << "Loading Scene Menu" << std::endl;
}

void SceneMenu::Update(double dt)
{
	std::cout << "Updating Scene Menu" << std::endl;
}

void SceneMenu::Render()
{
	std::cout << "Rendering Scene Menu" << std::endl;
}

void SceneMenu::Exit()
{
	std::cout << "Exiting Scene Menu" << std::endl;
}