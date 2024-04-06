/* Start Header ************************************************************************/
/*!
\file SceneStages.h
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

#ifndef SCENE_STAGES_H
#define SCENE_STAGES_H

#include "Scene.h"

#include "Pch.h"
#include "SceneManager.h"
#include "SceneLevelBuilder.h"

#define MOUSE_SENSITIVITY 5
#define MAX_LOAD_SCREEN_TIME 6.0
#define MIN_LOAD_SCREEN_TIME 1.5

namespace SceneStagesAudio {
	extern bool loopIsPlaying;
}

class SceneManager;
class SceneStages : public Scene
{
public:
	SceneStages();
	~SceneStages();

	virtual void load();
	virtual void init();
	virtual void update(double dt);
	virtual void render();
	virtual void exit();

	void Util_Camera_Shake(float duration, float strength);
	static SceneStages* sInstance; // The pointer to the object that gets registered
	bool m_StartGame;

private:
	SceneStages(SceneManager* _sceneMgr); // This is used to register to SceneManager


	SceneLevelBuilder* m_LevelBuilder;

	f32 m_transparency;
	double m_LoadScreenTimer;
	

	//////////////////////////////////////////////////////////////////////////
	// Animation
	const u32 spritesheet_rows = 7;
	const u32 spritesheet_cols = 9;
	const u32 spritesheet_max_sprites = 60;
	const f32 sprite_uv_width = 1.f / spritesheet_cols;
	const f32 sprite_uv_height = 1.f / spritesheet_rows;

	// (Not so good) hardcoded values about our animation
	f32 m_animationTimer = 0.f;
	f32 m_animationDurationPerFrame = 0.02f;
	u32 m_currentSpriteIndex = 0; // start from first sprite
	f32 m_currentSpriteUvOffsetX = 0.f;
	f32 m_currentSpriteUvOffsetY = 0.f;

	// Pointer to Mesh
	AEGfxVertexList* pMesh = 0;

	s8 pTextFont;


	////////////////////////////////////////////////////////////////////////
	//Screen shake
	double m_ScreenShakeTimer, m_ScreenShakeModifier;

};

#endif