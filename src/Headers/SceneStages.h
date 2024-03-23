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

	void Util_Window_Update(void); 
	void Util_Window_MoveTo(AEVec2 newPos);
	void Util_Window_Resize(float width, float height);
	void Util_Window_Shake(float duration, float strength);
private:
	SceneStages(SceneManager* _sceneMgr); // This is used to register to SceneManager

	static SceneStages* sInstance; // The pointer to the object that gets registered

	SceneLevelBuilder* m_LevelBuilder;

	double m_LoadScreenTimer;

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
	AEVec2 m_ScreenCentre;
	AEVec2 m_ScreenTopRight, m_ScreenTopLeft, m_ScreenBottomRight, m_ScreenBottomLeft;

	AEVec2 m_WindowPos;
	AEVec2 m_WindowScale;
	AEVec2 m_NewWindowPos;
	AEVec2 m_NewWindowScale;

	float m_MovingSpeed;
	float m_ScalingSpeed;

	bool m_ActivateOrNot;
	double m_ScreenShakeTimer, m_ScreenShakeModifier;

};

#endif