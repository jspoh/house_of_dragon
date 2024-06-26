/* Start Header ************************************************************************/
/*!
\file SceneStages.cpp
\author Soh Wei Jie, weijie.soh, 2301289
\par weijie.soh\@digipen.edu
\date 01 Apr 2024
\brief Staging scene between menu and combat
/*
Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/


#include "Pch.h"
#include "SceneStages.h"

bool SceneStagesAudio::loopIsPlaying = false;

SceneStages* SceneStages::m_sInstance = new SceneStages(SceneManager::getInstance());

SceneStages::SceneStages() : 
	m_LevelBuilder(nullptr), 
	m_LoadScreenTimer(MAX_LOAD_SCREEN_TIME),
	m_ScreenShakeModifier(0),
	m_ScreenShakeTimer(0),
	pTextFont(0),
	m_transparency(1.8f),
	m_StartGame(false)
{
}	

SceneStages::SceneStages(SceneManager* _sceneMgr) :
	m_LevelBuilder{ nullptr },
	m_LoadScreenTimer(MAX_LOAD_SCREEN_TIME),
	m_ScreenShakeModifier(0),
	m_ScreenShakeTimer(0),
	pTextFont(0),
	m_transparency(1.8f),
	m_StartGame(false)
{
	_sceneMgr->AddScene("SceneStages", this);
}

SceneStages::~SceneStages()
{
}

void SceneStages::load()
{
}

void SceneStages::init()
{
	cout << "Loading Scene Stages" << "\n";

	// Informing the library that we're about to start adding triangles
	AEGfxMeshStart();

	// This shape has 2 triangles that makes up a square
	// Color parameters represent colours as ARGB
	// UV coordinates to read from loaded textures
	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFFFFFFFF, 0.0f, sprite_uv_height, // bottom left
		0.5f, -0.5f, 0xFFFFFFFF, sprite_uv_width, sprite_uv_height, // bottom right 
		-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f);  // top left

	AEGfxTriAdd(
		0.5f, -0.5f, 0xFFFFFFFF, sprite_uv_width, sprite_uv_height, // bottom right 
		0.5f, 0.5f, 0xFFFFFFFF, sprite_uv_width, 0.0f,   // top right
		-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f);  // bottom left


	// Saving the mesh (list of triangles) in pMesh
	pMesh = AEGfxMeshEnd();

	RenderHelper::getInstance()->registerTexture("LoadAnimationRoad", "Assets/LoadingAnimation.png");

	pTextFont = AEGfxCreateFont("Assets/Fonts/TokyoMidnight.otf", 100);

	mouseX = AEGfxGetWindowWidth() / 2;
	mouseY = AEGfxGetWindowHeight();

	m_ScreenShakeTimer = 0.0f;
	m_ScreenShakeModifier = 1.0f;

	m_StartGame = false;
	m_transparency = 1.8f;
}

void SceneStages::update(double dt)
{	
	m_LoadScreenTimer -= dt;
	//This is to ensure that the game is on loading screen 
	//before the lag hits than on menu screen
	if ((m_LoadScreenTimer <= (MAX_LOAD_SCREEN_TIME - 0.5))
		&& m_LevelBuilder == nullptr)
		m_LevelBuilder = new SceneLevelBuilder();

	m_StartGame = m_LoadScreenTimer < 1.0 ? AEInputCheckTriggered(AEVK_LBUTTON) || m_StartGame : m_StartGame;
	////Fade out load screen(I just quickly make this)
	if (m_LoadScreenTimer < 1.0 && m_StartGame)
		m_transparency -= static_cast<f32>(AEFrameRateControllerGetFrameTime()) * 2.f;

	//When level builder is created
	if (m_LevelBuilder != nullptr)
	{
		m_LevelBuilder->update(dt);

		//Start the camera movement
		AEInputGetCursorPosition(&mouseX, &mouseY);
		//                                   |                Move Based On Mouse Pos                |  |                          Screen Shake                                     |
		if (Pause::getInstance().m_isPaused) {
			return;
		}
		AEGfxSetCamPosition(static_cast<f32>((mouseX - AEGfxGetWindowWidth() / 2) / MOUSE_SENSITIVITY - AERandFloat() * static_cast<f32>(m_ScreenShakeTimer * m_ScreenShakeModifier)),
				            static_cast<f32>(-(mouseY - AEGfxGetWindowHeight()) / MOUSE_SENSITIVITY) - AERandFloat() * static_cast<f32>(m_ScreenShakeTimer * m_ScreenShakeModifier));

		//Shaking Camera additional logic
		if (m_ScreenShakeTimer > 0.0f)
		{
			m_ScreenShakeTimer -= dt;
		}
		else
		{
			m_ScreenShakeTimer = 0.f;
		}
	}

	//Call this for screen shake
	{
		//if (AEInputCheckTriggered(AEVK_M))
		//	Util_Camera_Shake(0.5, 100);
	}

	// Update the animation timer.
	m_animationTimer += static_cast<float>(dt);
	if (m_animationTimer >= m_animationDurationPerFrame)
	{
		m_animationTimer = 0;
		m_currentSpriteIndex = ++m_currentSpriteIndex % spritesheet_max_sprites;

		u32 current_sprite_row = m_currentSpriteIndex / spritesheet_cols;
		u32 current_sprite_col = m_currentSpriteIndex % spritesheet_cols;

		m_currentSpriteUvOffsetX = sprite_uv_width * current_sprite_col;
		m_currentSpriteUvOffsetY = sprite_uv_height * current_sprite_row;

	}

	if (m_transparency <= 0 && !SceneStagesAudio::loopIsPlaying && !GameScene::afterInit) {
		SoundPlayer::GameAudio::getInstance().playLoop();
		SceneStagesAudio::loopIsPlaying = true;
		GameScene::afterInit = true;
	}
}


void SceneStages::render()
{
	if (m_LoadScreenTimer <= 1.0 && m_LevelBuilder != nullptr && m_StartGame)
		m_LevelBuilder->render();

	/////////////////////////////////////////////////////
	// Render load Screen
	if (m_LoadScreenTimer >= -1.0 || !m_StartGame || (m_StartGame && m_transparency >= -2.0f))
	{
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 1.0f);
		AEGfxSetTransparency(m_transparency);
		AEMtx33 t_curr;
		AEMtx33Identity(&t_curr);
		AEMtx33ScaleApply(&t_curr, &t_curr, 99999, 99999);
		AEGfxSetTransform(t_curr.m);
		AEGfxMeshDraw(RenderHelper::getInstance()->getdefaultMesh(), AE_GFX_MDM_TRIANGLES);

		AEGfxTextureSet(NULL, 0, 0);
		f32 TextWidth = 0, TextHeight = 0;
		char strBuffer[1024];
		if (m_LoadScreenTimer < 1.0)
		{
			sprintf_s(strBuffer, "CLICK TO START");
			AEGfxGetPrintSize(pTextFont, strBuffer, 0.8f, &TextWidth, &TextHeight);
			AEGfxPrint(pTextFont, strBuffer, static_cast<float>(-mouseX) / 2750.f - 0.4f, static_cast<float>(mouseY) / 2000.f - 0.2f, 0.65f, 1.0f, 1.0f, 1.0f, m_transparency - 0.8f);

		}
		else
		{
			sprintf_s(strBuffer, "LOADING");
			AEGfxGetPrintSize(pTextFont, strBuffer, 0.8f, &TextWidth, &TextHeight);
			AEGfxPrint(pTextFont, strBuffer, static_cast<float>(-mouseX) / 2750.f - 0.2f, static_cast<float>(mouseY) / 2000.f - 0.2f, 0.8f, 1.0f, 1.0f, 1.0f, m_transparency - 0.8f);

		}
			
		//load animation
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
		AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		AEGfxSetTransparency(m_transparency - 0.8f);
		AEGfxTextureSet(RenderHelper::getInstance()->getTextureByRef("LoadAnimationRoad"), m_currentSpriteUvOffsetX, m_currentSpriteUvOffsetY);
		AEMtx33Identity(&t_curr);
		AEMtx33ScaleApply(&t_curr, &t_curr, 1000.0f, 800.0f);
		AEMtx33TransApply(&t_curr, &t_curr, 0.f, -100.0f);
		AEGfxSetTransform(t_curr.m);
		AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
	}
}

void SceneStages::exit()
{
	cout << "Exiting Scene SplashScreen" << "\n";

	AEGfxMeshFree(pMesh);
	//Destroy Font
	AEGfxDestroyFont(pTextFont);

	delete m_LevelBuilder;
	m_LevelBuilder = nullptr;

	GameScene::afterInit = false;
	SceneStagesAudio::loopIsPlaying = false;
	SoundPlayer::stopAll();

	// use loading screen again after exiting to menu
	m_LoadScreenTimer = MIN_LOAD_SCREEN_TIME;
}

//////////////////////////////////////////////////////////////////////////////
//Screen Shake
void SceneStages::Util_Camera_Shake(float duration, float strength)
{
	m_ScreenShakeTimer = duration;
	m_ScreenShakeModifier = strength;
}

/**********************************
TODO:
MERGE EVERYTHING AND FIXING BUGS
***/