#include "SceneStages.h"
#include <iostream>

SceneStages* SceneStages::sInstance = new SceneStages(SceneManager::GetInstance());

SceneStages::SceneStages() : m_LevelBuilder(nullptr), m_LoadScreenTimer(MAX_LOAD_SCREEN_TIME)
{
}	

SceneStages::SceneStages(SceneManager* _sceneMgr) : m_LevelBuilder{nullptr}, m_LoadScreenTimer(MAX_LOAD_SCREEN_TIME)
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
	std::cout << "Loading Scene Stages" << std::endl;

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
}

void SceneStages::Update(double dt)
{	
	m_LoadScreenTimer -= dt;
	//This is to ensure that the game is on loading screen 
	//before the lag hits than on menu screen
	if ((m_LoadScreenTimer <= MAX_LOAD_SCREEN_TIME - 0.5)
		&& m_LevelBuilder == nullptr)
		m_LevelBuilder = new SceneLevelBuilder();
	
	//When level builder is created
	if (m_LevelBuilder != nullptr)
	{
		m_LevelBuilder->Update(dt);
		//Start the camera movement when load screen is finished
		if (m_LoadScreenTimer <= 0)
		{
			int mouseX, mouseY;
			AEInputGetCursorPosition(&mouseX, &mouseY);
			AEGfxSetCamPosition(static_cast<f32>((mouseX - AEGfxGetWindowWidth() / 2) / MOUSE_SENSITIVITY),
				static_cast<f32>(-(mouseY - AEGfxGetWindowHeight()) / MOUSE_SENSITIVITY));
		}
	}


	// Update the animation timer.
   // animation_timer should go up to animation_duration_per_frame.
	animation_timer += dt;
	if (animation_timer >= animation_duration_per_frame)
	{
		// When the time is up go to the next sprite.

		// Reset the timer.
		animation_timer = 0;

		// Calculate the next sprite UV
		current_sprite_index = ++current_sprite_index % spritesheet_max_sprites;

		u32 current_sprite_row = current_sprite_index / spritesheet_cols;
		u32 current_sprite_col = current_sprite_index % spritesheet_cols;

		current_sprite_uv_offset_x = sprite_uv_width * current_sprite_col;
		current_sprite_uv_offset_y = sprite_uv_height * current_sprite_row;

	}
}


void SceneStages::Render()
{
	if (m_LoadScreenTimer <= 1.0 && m_LevelBuilder != nullptr)
		m_LevelBuilder->Render();

	/////////////////////////////////////////////////////
	// Render Load Screen
	if (m_LoadScreenTimer >= -1.0)
	{
		//Fade out load screen(I just quickly make this)
		static f32 transparency = 1.8f;
		if (m_LoadScreenTimer < 1.0)
			transparency -= static_cast<f32>(AEFrameRateControllerGetFrameTime()) * 2.f;

		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 1.0f);
		AEGfxSetTransparency(transparency);
		AEMtx33 t_curr;
		AEMtx33Identity(&t_curr);
		AEMtx33ScaleApply(&t_curr, &t_curr, 99999, 99999);
		AEGfxSetTransform(t_curr.m);
		AEGfxMeshDraw(RenderHelper::getInstance()->GetdefaultMesh(), AE_GFX_MDM_TRIANGLES);

		AEGfxTextureSet(NULL, 0, 0);
		f32 TextWidth = 0, TextHeight = 0;
		char strBuffer[1024];
		sprintf_s(strBuffer, "LOADING");
		AEGfxGetPrintSize(pTextFont, strBuffer, 0.8f, &TextWidth, &TextHeight);
		AEGfxPrint(pTextFont, strBuffer, -TextWidth + 0.4f, 0.25f, 0.8f, 1.0f, 1.0f, 1.0f, transparency - 0.8f);

		//Load animation
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
		AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		AEGfxSetTransparency(transparency - 0.8f);
		AEGfxTextureSet(RenderHelper::getInstance()->getTextureByRef("LoadAnimationRoad"), current_sprite_uv_offset_x, current_sprite_uv_offset_y);
		AEMtx33Identity(&t_curr);
		AEMtx33ScaleApply(&t_curr, &t_curr, 1000.0f, 800.0f);
		f32 x{}, y{};
		AEGfxGetCamPosition(&x, &y);
		AEMtx33TransApply(&t_curr, &t_curr, x, y - 100.0f);
		AEGfxSetTransform(t_curr.m);
		AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
	}
}

void SceneStages::Exit()
{
	std::cout << "Exiting Scene SplashScreen" << std::endl;

	AEGfxMeshFree(pMesh);
	//Destroy Font
	AEGfxDestroyFont(pTextFont);

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