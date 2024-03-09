#include "SceneStages.h"
#include <iostream>

SceneStages* SceneStages::sInstance = new SceneStages(SceneManager::GetInstance());

SceneStages::SceneStages() : m_LevelBuilder(nullptr)
{
}	

SceneStages::SceneStages(SceneManager* _sceneMgr) : m_LevelBuilder{nullptr}
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
		AEGfxSetCamPosition(static_cast<f32>((mouseX - AEGfxGetWindowWidth() / 2) / MOUSE_SENSITIVITY), 
			static_cast<f32>(-(mouseY - AEGfxGetWindowHeight()) / MOUSE_SENSITIVITY));
	}
}


void SceneStages::Render()
{
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
Add Lens Flare

Add Backdrop (by today)
Blocking (by today)
Cloud Scrolling (by today)
Commenting Code (by today)
Integrate combat system (by today)
Integrate camera panning with combat (NEED TO IMPROVE/OVERHAUL)
***/