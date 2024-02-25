#include "SceneMenu.h"

SceneMenu* SceneMenu::sInstance = new SceneMenu(SceneManager::GetInstance());

SceneMenu::SceneMenu()
{
}

SceneMenu::SceneMenu(SceneManager* _sceneMgr)
{
	_sceneMgr->AddScene("SceneMenu", this);
}

SceneMenu::~SceneMenu()
{
}

void SceneMenu::Load()
{
	std::cout << "Loading Scene Menu" << std::endl;
}

void SceneMenu::Init()
{

}

void SceneMenu::Update(double dt)
{
	if (AEInputCheckCurr(AEVK_1))
		SceneManager::GetInstance()->SetActiveScene("SceneStage1");
	if (AEInputCheckCurr(AEVK_2))
		SceneManager::GetInstance()->SetActiveScene("CombatScene");
}

void SceneMenu::Render()
{
	// Set the background to black.
	AEGfxSetBackgroundColor(0.0f, 0.2f, 1.0f);

	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);

	// Set the the color to multiply to white, so that the sprite can 
	// display the full range of colors (default is black).
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);

	// Set the color to add to nothing, so that we don't alter the sprite's color
	AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 1.0f);

	// Set blend mode to AE_GFX_BM_BLEND
	// This will allow transparency.
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);

	//PART CAUSING MEMORY LEAKS (JS GO FIX STOP DOING SIDE PROJECTS)
	Draw::getInstance()->text("MENU", AEGfxGetWindowWidth() / 2, AEGfxGetWindowHeight() / 2 - AEGfxGetWindowHeight() / 4);
	Draw::getInstance()->text("DO HERE JAY", AEGfxGetWindowWidth() / 2, AEGfxGetWindowHeight() / 2 - AEGfxGetWindowHeight() / 8);
	Draw::getInstance()->text("Press 1: GO TO Stage 1, 2: Go To Combat Scene, 3: NOT YET", AEGfxGetWindowWidth() / 2, AEGfxGetWindowHeight() / 2 + AEGfxGetWindowHeight() / 4);
}

void SceneMenu::Exit()
{
	std::cout << "Exiting Scene Menu" << std::endl;
}