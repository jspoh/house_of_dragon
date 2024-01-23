#include "SceneStage1.h"

#include "../Backend/GameManager.h" //? Still thinking
#include "SceneManager.h"
#include <iostream> //Remove upon test finish

SceneStage1* SceneStage1::sInstance = new SceneStage1(SceneManager::GetInstance());

SceneStage1::SceneStage1()
{
}

SceneStage1::SceneStage1(SceneManager* _sceneMgr)
{
	_sceneMgr->AddScene("SceneStage1", this);
}

SceneStage1::~SceneStage1()
{
}

void SceneStage1::Load()
{
	//Still debating whether need this
}

void SceneStage1::Init()
{
	std::cout << "Loading Scene Stage1" << std::endl;
}

void SceneStage1::Update(double dt)
{
	std::cout << "Updating Scene Stage1" << std::endl;
}

void SceneStage1::Render()
{
	std::cout << "Rendering Scene Stage1" << std::endl;
}

void SceneStage1::Exit()
{
	std::cout << "Exiting Scene Stage1" << std::endl;
}