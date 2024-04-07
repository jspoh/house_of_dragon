/* Start Header ************************************************************************/
/*!
\file GameManager.cpp
\author Soh Wei Jie, weijie.soh, 2301289
\par weijie.soh\@digipen.edu
\date 28 feb 2024
\brief Game Manager that handles the basic call of update and render to the managers in
       the game. Also handles destruction of said managers.
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
}

GameManager::~GameManager()
{
}

void GameManager::init()
{
	// seed PRNG
	Math::InitRNG();

	//Init Scene Manager
	SceneManager::getInstance()->SetActiveScene("SceneSplashScreen");

	initGlobals();
	RenderHelper::getInstance()->load();
	// cant load opengl textures on a different thread (technically can but no access to opengl, only alpha engine)
	//std::thread t1(loadAllTextures);
	//t1.detach();
}

void GameManager::run()
{
	if (AEInputCheckTriggered(AEVK_LBUTTON)) {
		SoundPlayer::GlobalAudio::getInstance().playSfxClick();
	}

	updateGlobals();

	SceneManager::getInstance()->update((f32)AEFrameRateControllerGetFrameTime());

	//Render
	SceneManager::getInstance()->render();
}

void GameManager::exit()
{
	SceneManager::getInstance()->exit();
	
	SceneManager::destroy();
	SoundManager::destroy();

	delete Event::getInstance();

	delete ParticleManager::getInstance();
	delete RenderHelper::getInstance();

	// ensure that player destructor is called
	// before program ends as order of destruction
	// is not well defined in c++. the static database
	// instance could be being deleted before player destructor
	// is called, resulting in a read access violation
	player.reset();
}
