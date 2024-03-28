#include "Pch.h"
#include "SceneMenu.h"
#include "SoundPlayer.h"
#include "ParticleManager.h"

namespace {
	bool loopIsPlaying = false;
}

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


}

void SceneMenu::Init()
{

	myMenu.buttonWidth = 400.0f; // Example initialization
	myMenu.buttonHeight = 120.0f; // Example initialization
	myMenu.transitionEnd = false; // Example initialization
	myMenu.transitionTimer = 0.0f; // Example initialization
	myMenu.transitionElapse = 0.0f; // Example initialization
	myMenu.nextLevel = 0; // Example initialization
	for (int i = 0; i < 4; ++i)
	{
		myMenu.buttonX[i] = 0;
		myMenu.buttonY[i] = -i * (myMenu.buttonHeight + 10) + 200;
	}

	ParticleManager::GetInstance()->init();

	if (!loopIsPlaying) {
		SoundPlayer::MenuAudio::getInstance().playLoopMenu();
		loopIsPlaying = true;
	}

	AEGfxSetCamPosition(0, 0);
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

	//if (AEInputCheckTriggered(AEVK_F))
	//{
	//	// Get the current mouse position
	//	s32 mxx, myy;
	//	AEInputGetCursorPosition(&mxx, &myy);
	//	float mx = static_cast<float>(mxx);
	//	float my = static_cast<float>(myy);


	//	// Create fireworks particles at the mouse position
	//	ParticleManager::GetInstance()->createExplosionParticle(mx, my);
	//}


	if (AEInputCheckTriggered(AEVK_LBUTTON))
	{

		s32 mxx, myy;
		AEInputGetCursorPosition(&mxx, &myy);
		float mx = static_cast<float>(mxx);
		float my = static_cast<float>(myy);
		mx -= AEGfxGetWindowWidth() / 2;

		my = -my;
		my += AEGfxGetWindowHeight() / 2.0f;

		Point cursorPos = { mx, my };


		for (int i = 0; i < 4; ++i)
		{
			AEVec2 p1 = { myMenu.buttonX[i] - myMenu.buttonWidth / 2.f, myMenu.buttonY[i] + myMenu.buttonHeight / 2.f };
			AEVec2 p2 = { myMenu.buttonX[i] + myMenu.buttonWidth / 2.f, myMenu.buttonY[i] - myMenu.buttonHeight / 2.f };

			if (p1.x<mx && p1.y>my && p2.x > mx && p2.y < my)
			{
				myMenu.hovering[i] = true;
				switch (i)
				{
				case 0:
					SceneManager::GetInstance()->SetActiveScene("SceneStages");
					SoundPlayer::stopAll();
					loopIsPlaying = false;
					break;
				case 1:
					SceneManager::GetInstance()->SetActiveScene("SceneCredits");
					break;
				case 2:
					SceneManager::GetInstance()->SetActiveScene("");
					break;
				case 3:
					SceneManager::GetInstance()->Exit();
					break;
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


	AEMtx33 scale = { 0 };
	AEMtx33Scale(&scale, AEGfxGetWindowWidth(), AEGfxGetWindowHeight());

	AEMtx33 transform;
	AEMtx33Trans(&transform, 0, 0);

	AEMtx33 model = { 0 };
	AEMtx33Concat(&model, &scale, &transform);

	// prepare to draw
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetColorToAdd(0, 0, 0, 1);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(1);
	AEGfxSetTransform(model.m);
	AEGfxTextureSet(myMenu.bg, 0, 0);
	AEGfxMeshDraw(myMenu.mesh, AE_GFX_MDM_TRIANGLES);

	//ParticleManager* particleManager = ParticleManager::emit_Particle();
	//particleManager->update_particles(AEFrameRateControllerGetFrameTime());
	//particleManager.render_particles();


	for (int i = 3; i >= 0; --i)
	{

		scale = { 0 };
		AEMtx33Scale(&scale, myMenu.buttonWidth, myMenu.buttonHeight);

		AEMtx33Trans(&transform, myMenu.buttonX[i], myMenu.buttonY[i]);

		model = { 0 };
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
	
}