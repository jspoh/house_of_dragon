#include "Pch.h"
#include "SceneMenu.h"
#include "SoundManager.h"
#include "ParticleManager.h"
#include "SingletonTemplate.h"

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
	AEGfxMeshStart();
	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFFFF00FF, 0.0f, 1.0f,
		0.5f, -0.5f, 0xFFFFFF00, 1.0f, 1.0f,
		-0.5f, 0.5f, 0xFF00FFFF, 0.0f, 0.0f);
	AEGfxTriAdd(
		0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f,
		0.5f, 0.5f, 0xFFFFFFFF, 1.0f, 0.0f,
		-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f);

	myMenu.mesh = AEGfxMeshEnd();

	myMenu.bg = AEGfxTextureLoad("Assets/Menu/bg.png");
	myMenu.pointer = AEGfxTextureLoad("Assets/Menu/buttons/dagger.png");
	myMenu.button[0] = AEGfxTextureLoad("Assets/Menu/buttons/olay.png");
	myMenu.button[1] = AEGfxTextureLoad("Assets/Menu/buttons/xredit.png");
	myMenu.button[2] = AEGfxTextureLoad("Assets/Menu/buttons/aetting.png");
	myMenu.button[3] = AEGfxTextureLoad("Assets/Menu/buttons/quit.png");


	myMenu.buttonSelect[0] = AEGfxTextureLoad("Assets/Menu/chicken.png");
	myMenu.buttonSelect[1] = AEGfxTextureLoad("Assets/Menu/tiger.png");
	myMenu.buttonSelect[2] = AEGfxTextureLoad("Assets/Menu/dragon.png");



	SoundManager::GetInstance()->registerAudio("btnClickSound", "./Assets/Audio/SFX/button_click.mp3");
	//SoundManager::GetInstance()->registerAudio("titleMusic", "./Assets/Audio/Music/sample.mp3");

}

void SceneMenu::Init()
{

	myMenu.buttonWidth = 400.0f; // Example initialization
	myMenu.buttonHeight = 130.0f; // Example initialization
	myMenu.transitionEnd = false; // Example initialization
	myMenu.transitionTimer = 0.0f; // Example initialization
	myMenu.transitionElapse = 0.0f; // Example initialization
	myMenu.nextLevel = 0; // Example initialization
	for (int i = 0; i < 4; ++i)
	{
		myMenu.buttonX[i] = 0;
		myMenu.buttonY[i] = -i * (myMenu.buttonHeight ) + 200;
	}

	//SoundManager::GetInstance()->playAudio("titleMusic", 1, -1, true);

	for (int i = 0; i < 3; ++i)
	{
		myMenu.buttonSelectX[i] = -300.0f + i * 300.0f; // Adjust the x-coordinate as needed
		myMenu.buttonSelectY[i] = 0.0f; // Adjust the y-coordinate as needed
		myMenu.hoveringSelect[i] = false;
	}

	ParticleManager::GetInstance()->init();

}

void SceneMenu::Update(double dt)
{
	int mX, mY;
	AEInputGetCursorPosition(&mX, &mY);

	ParticleManager::GetInstance()->setParticlePos(static_cast<float>(mX), static_cast<float>(mY));
	ParticleManager::GetInstance()->update(dt);


	if (AEInputCheckTriggered(AEVK_1)) {
		SceneManager::GetInstance()->SetActiveScene("TestScene");
	}
	else if (AEInputCheckTriggered(AEVK_2)) {
		SceneManager::GetInstance()->SetActiveScene("CombatScene");
	}


	if (AEInputCheckTriggered(AEVK_LBUTTON))
	{
		s32 mxx, myy;
		AEInputGetCursorPosition(&mxx, &myy);
		float mx = static_cast<float>(mxx);
		float my = static_cast<float>(myy);
		mx -= AEGfxGetWindowWidth() / 2;

		my = -my;
		my += AEGfxGetWindowHeight() / 2.0f;

		SoundManager::GetInstance()->playAudio("btnClickSound");

		Point cursorPos = { mx, my };

		if (!myMenu.levelSelecting)
		{
			for (int i = 0; i < 4; ++i)
			{
				AEVec2 p1 = { myMenu.buttonX[i] - myMenu.buttonWidth / 2.f, myMenu.buttonY[i] + myMenu.buttonHeight / 2.f };
				AEVec2 p2 = { myMenu.buttonX[i] + myMenu.buttonWidth / 2.f, myMenu.buttonY[i] - myMenu.buttonHeight / 2.f };

				if (p1.x < mx && p1.y > my && p2.x > mx && p2.y < my)
				{
					myMenu.hovering[i] = true;
					switch (i)
					{
					case 0:
						myMenu.levelSelecting = true;
						break;
					case 1:
						SceneManager::GetInstance()->SetActiveScene("SceneCredits");
						break;
					case 2:
						SceneManager::GetInstance()->SetActiveScene("SceneSetting");
						break;
					case 3:
						SceneManager::GetInstance()->Exit();
						break;
					}
				}
			}
		}
		else
		{
			for (int i = 0; i < 3; ++i)
			{
				AEVec2 p1 = { myMenu.buttonSelectX[i] - myMenu.buttonWidth / 2.f, myMenu.buttonSelectY[i] + myMenu.buttonHeight / 2.f };
				AEVec2 p2 = { myMenu.buttonSelectX[i] + myMenu.buttonWidth / 2.f, myMenu.buttonSelectY[i] - myMenu.buttonHeight / 2.f };

				if (p1.x < mx && p1.y > my && p2.x > mx && p2.y < my)
				{
					myMenu.hoveringSelect[i] = true;
					switch (i)
					{
					case 0:
					case 1:
					case 2:
						SceneManager::GetInstance()->SetActiveScene("SceneStages");
						break;
					}
				}
				else
				{
					myMenu.hoveringSelect[i] = false;
				}
			}
		}

		return;
	}
	else
	{
		for (int i = 0; i < 4; ++i)
		{
			myMenu.hovering[i] = false;
		}
	}
}


void SceneMenu::Render()
{
	AEGfxSetBackgroundColor(0.0f, 0.2f, 1.0f);
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 1.0f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);

	// Render the background image to fit the whole window
	AEMtx33 bgScale = { 0 };
	AEMtx33Scale(&bgScale, (float)AEGfxGetWindowWidth(), (float)AEGfxGetWindowHeight());

	AEMtx33 bgTransform = { 0 };
	AEMtx33Trans(&bgTransform, 0.0f, 0.0f);

	AEMtx33 bgModel = { 0 };
	AEMtx33Concat(&bgModel, &bgScale, &bgTransform);

	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetColorToAdd(0, 0, 0, 1);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(1);
	AEGfxSetTransform(bgModel.m);
	AEGfxTextureSet(myMenu.bg, 0, 0);
	AEGfxMeshDraw(myMenu.mesh, AE_GFX_MDM_TRIANGLES);

	// Render the menu buttons
	for (int i = 3; i >= 0; --i)
	{
		AEMtx33 scale = { 0 };
		AEMtx33Scale(&scale, myMenu.buttonWidth, myMenu.buttonHeight);

		AEMtx33 transform = { 0 };
		AEMtx33Trans(&transform, myMenu.buttonX[i], myMenu.buttonY[i]);

		AEMtx33 model = { 0 };
		AEMtx33Concat(&model, &transform, &scale);

		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
		AEGfxSetColorToAdd(0, 0, 0, 1);
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		AEGfxSetTransparency(1);
		AEGfxSetTransform(model.m);
		AEGfxTextureSet(myMenu.button[i], 0, 0);
		AEGfxMeshDraw(myMenu.mesh, AE_GFX_MDM_TRIANGLES);

		if (myMenu.hovering[i])
		{
			AEMtx33 daggerScale = { 0 };
			AEMtx33Scale(&daggerScale, 40.0f, 40.0f);

			AEMtx33 daggerTranslate = { 0 };
			AEMtx33Trans(&daggerTranslate, myMenu.buttonX[i] - myMenu.buttonWidth / 2.0f - 30.0f, myMenu.buttonY[i]);

			AEMtx33 daggerTransform = { 0 };
			AEMtx33Concat(&daggerTransform, &daggerScale, &daggerTranslate);

			AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
			AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
			AEGfxSetColorToAdd(0, 0, 0, 1);
			AEGfxSetBlendMode(AE_GFX_BM_BLEND);
			AEGfxSetTransparency(1);
			AEGfxSetTransform(daggerTransform.m);
			AEGfxTextureSet(myMenu.pointer, 0, 0);
			AEGfxMeshDraw(myMenu.mesh, AE_GFX_MDM_TRIANGLES);
		}
	}

	// Render the background image again before rendering buttonSelect
	if (myMenu.levelSelecting)
	{
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
		AEGfxSetColorToAdd(0, 0, 0, 1);
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		AEGfxSetTransparency(1);
		AEGfxSetTransform(bgModel.m);
		AEGfxTextureSet(myMenu.bg, 0, 0);
		AEGfxMeshDraw(myMenu.mesh, AE_GFX_MDM_TRIANGLES);

		// Render the buttonSelect buttons
		for (int i = 0; i < 3; ++i)
		{
			AEMtx33 selectScale = { 0 };
			AEMtx33Scale(&selectScale, myMenu.buttonWidth, myMenu.buttonHeight);

			AEMtx33 selectTransform = { 0 };
			AEMtx33Trans(&selectTransform, myMenu.buttonSelectX[i], myMenu.buttonSelectY[i]);

			AEMtx33 selectModel = { 0 };
			AEMtx33Concat(&selectModel, &selectTransform, &selectScale);

			AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
			AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
			AEGfxSetColorToAdd(0, 0, 0, 1);
			AEGfxSetBlendMode(AE_GFX_BM_BLEND);
			AEGfxSetTransparency(1);
			AEGfxSetTransform(selectModel.m);
			AEGfxTextureSet(myMenu.buttonSelect[i], 0, 0);
			AEGfxMeshDraw(myMenu.mesh, AE_GFX_MDM_TRIANGLES);
		}
	}

	ParticleManager::GetInstance()->render();
}

void SceneMenu::Exit()
{
	std::cout << "Exiting Scene Menu" << std::endl;
	for (int i = 0; i < 4; i++)
	{
		AEGfxTextureUnload(myMenu.button[i]);
	}
	AEGfxMeshFree(myMenu.mesh);
	AEGfxTextureUnload(myMenu.pointer);
	AEGfxTextureUnload(myMenu.bg);
	
	SoundManager::GetInstance()->removeAudio("btnClickSound");
}