/* Start Header ************************************************************************/
/*!
\file GameManager.cpp
\author Soh Wei Jie, weijie.soh, 2301289
\par weijie.soh\@digipen.edu
\date 28 feb 2024
\brief base game manager
/*
Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/


#include "Pch.h"
#include "GameManager.h"
#include "MyMath.h"
#include "CombatScene.h"
#include "CombatPlayer.h"

extern std::unique_ptr<Player> player;

////Define an error callback
//static void error_callback(int error, const char* description)
//{
//	fputs(description, stderr);
//	_fgetchar();
//}

GameManager::GameManager()
{
	//loadAllTextures();
}

GameManager::~GameManager()
{
}

void GameManager::Init()
{
	// seed PRNG
	Math::InitRNG();

	//Load Textures

	//Init Scene Manager
	SceneManager::GetInstance()->SetActiveScene("SceneSplashScreen");

	//Init Sound Manager
	//SoundManager::GetInstance()->Init();

	initGlobals();
	RenderHelper::getInstance()->load();
	// cant load opengl textures on a different thread (technically can but no access to opengl, only alpha engine)
	//std::thread t1(loadAllTextures);
	//t1.detach();
}

void GameManager::Run()
{
	////Update Current Scene
	//scene->Update(m_timer.getElapsedTime());
	////Render Current Scene
	////m_timer.startTimer();    // Start timer to calculate how long it takes to render this frame
	//while (!glfwWindowShouldClose(m_window) && !IsKeyPressed(VK_ESCAPE))
	//{
	//	//UpdateInput();
	//	GetMouseUpdate();
	//	
	//	scene->Render();
	//	//Swap buffers
	//	glfwSwapBuffers(m_window);
	//	//Get and organize events, like keyboard and mouse input, window resizing, etc...
	//	glfwPollEvents();
	//	m_timer.waitUntil(frameTime);       // Frame rate limiter. Limits each frame to a specified time in ms.  
	//	PostInputUpdate();
	//} //Check if the ESC key had been pressed or if the window had been closed
	//scene->Exit();
	//delete scene;

	//float sfx, music;
	//SoundManager::GetInstance()->getVolume(sfx, music);
	//cout << sfx << " | " << music << "\n";

	//cout << SoundManager::GetInstance()->getSfxVolume() << ", " << SoundManager::GetInstance()->getMusicVolume() << "\n";

	if (AEInputCheckTriggered(AEVK_LBUTTON)) {
		SoundPlayer::GlobalAudio::getInstance().playSfxClick();
	}

	updateGlobals();

	SceneManager::GetInstance()->Update((f32)AEFrameRateControllerGetFrameTime());
	//GameObjectManager::GetInstance()->Update(AEGetTime(NULL));

	//Render
	SceneManager::GetInstance()->Render();
	//GameObjectManager::GetInstance()->Render();
}

void GameManager::Exit()
{
	////Free Textures
	////End Scene Manager
	SceneManager::GetInstance()->Exit();
	////GameObjectManager::GetInstance()->Destroy();
	////Sound Manager will destroy itself when program ends
	
	SceneManager::Destroy();
	SoundManager::Destroy();

	delete Event::getInstance();
	//delete CombatManager::getInstance();
	//delete Database::getInstance();

	delete ParticleManager::GetInstance();
	delete RenderHelper::getInstance();

	// ensure that player destructor is called
	// before program ends as order of destruction
	// is not well defined in c++. the static database
	// instance could be being deleted before player destructor
	// is called, resulting in a read access violation
	player.reset();
}
