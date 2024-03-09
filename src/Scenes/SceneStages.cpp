#include "SceneStages.h"
#include <iostream>

SceneStages* SceneStages::sInstance = new SceneStages(SceneManager::GetInstance());

//double m_FloorSpeedTimer = 0.5;

SceneStages::SceneStages() : m_LevelBuilder(nullptr)
{
}	

SceneStages::SceneStages(SceneManager* _sceneMgr)
{
	_sceneMgr->AddScene("SceneStages", this);
}

SceneStages::~SceneStages()
{
}

void SceneStages::Load()
{
}

void SceneStages::Init()
{
	std::cout << "Loading Scene Stage1" << std::endl;

	m_LevelBuilder = new SceneLevelBuilder();
}

void SceneStages::Update(double dt)
{	
	static bool start = true;

	if (start)
	{
		m_LevelBuilder->Update(dt);
		int mouseX, mouseY;
		AEInputGetCursorPosition(&mouseX, &mouseY);
		AEGfxSetCamPosition((mouseX - AEGfxGetWindowWidth()/2)/MOUSE_SENSITIVITY, -(mouseY - AEGfxGetWindowHeight()) / MOUSE_SENSITIVITY);
	}
}


void SceneStages::Render()
{
	//std::cout << Database::getInstance()->data["levels"][0]["enemySpawnWeight"]["cat"] << "\n";

	m_LevelBuilder->Render();
}

void SceneStages::Exit()
{
	std::cout << "Exiting Scene SplashScreen" << std::endl;

	delete m_LevelBuilder;
	m_LevelBuilder = nullptr;
}

/**********************************
TODO:
FIX LAG AAAAAAAAAAAAAAAAAAAAHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH
(SIMI LANJIAO NO LAG ON OTHER PEOPLE COM)
Fix Floor
Adjust SceneObject Placement
Blocking
Cloud Scrolling
Probability Spawning based on Json (DOING NOW)
Add Backdrop 
Add Lens Flare
***/