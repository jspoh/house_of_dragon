#ifndef SCENE_STAGES_H
#define SCENE_STAGES_H

#include "Scene.h"

#include "Pch.h"
#include "SceneManager.h"
#include "SceneLevelBuilder.h"

#define MOUSE_SENSITIVITY 5
#define MAX_LOAD_SCREEN_TIME 6.0
class SceneManager;
class SceneStages : public Scene
{
public:
	SceneStages();
	~SceneStages();

	virtual void Load();
	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	void Util_Camera_Shake(float duration, float strength);
	static SceneStages* sInstance; // The pointer to the object that gets registered
private:
	SceneStages(SceneManager* _sceneMgr); // This is used to register to SceneManager


	SceneLevelBuilder* m_LevelBuilder;

	double m_LoadScreenTimer;

	//////////////////////////////////////////////////////////////////////////
	// Animation
	const u32 spritesheet_rows = 7;
	const u32 spritesheet_cols = 9;
	const u32 spritesheet_max_sprites = 60;
	const f32 sprite_uv_width = 1.f / spritesheet_cols;
	const f32 sprite_uv_height = 1.f / spritesheet_rows;

	// (Not so good) hardcoded values about our animation
	f32 animation_timer = 0.f;
	f32 animation_duration_per_frame = 0.02f;
	u32 current_sprite_index = 0; // start from first sprite
	f32 current_sprite_uv_offset_x = 0.f;
	f32 current_sprite_uv_offset_y = 0.f;

	// Pointer to Mesh
	AEGfxVertexList* pMesh = 0;

	s8 pTextFont;

	int mouseX, mouseY; //For GetCursorPosition Function, s32

	////////////////////////////////////////////////////////////////////////
	//Screen shake
	double m_ScreenShakeTimer, m_ScreenShakeModifier;

};

#endif