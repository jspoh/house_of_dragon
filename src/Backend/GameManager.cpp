#include "Pch.h"
#include "GameManager.h"
#include "MyMath.h"
#include "CombatScene.h"

////Define an error callback
//static void error_callback(int error, const char* description)
//{
//	fputs(description, stderr);
//	_fgetchar();
//}

GameManager::GameManager()
{
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

	if (AEInputCheckTriggered(AEVK_LBUTTON)) {
		SoundPlayer::GlobalAudio::getInstance().playSfxClick();
	}

	AEInputGetCursorPosition(&mouseX, &mouseY);
	Point wMouse = stow(mouseX, mouseY);
	wMouseX = static_cast<int>(wMouse.x);
	wMouseY = static_cast<int>(wMouse.y);
	AEGfxGetCamPosition(&camOffset.x, &camOffset.y);

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
	delete Database::getInstance();

	delete ParticleManager::GetInstance();
	delete RenderHelper::getInstance();
}
