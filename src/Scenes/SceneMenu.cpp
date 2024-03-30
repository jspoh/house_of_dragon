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

	myMenu.bg = "menuBg";
	myMenu.bg1 = "menuBg1";
	myMenu.pointer = "dagger";
	myMenu.button[0] = "olay";
	myMenu.button[1] = "xredit";
	myMenu.button[2] = "aetting";
	myMenu.button[3] = "howtoplay";
	myMenu.button[4] = "quit";



	myMenu.buttonSelect[0] = "level1";
	myMenu.buttonSelect[1] = "level2";
	myMenu.buttonSelect[2] = "level3";
	myMenu.buttonSelect[3] = "level4";
	myMenu.buttonSelect[4] = "level5";


	RenderHelper::getInstance()->registerTexture("menuBg", "Assets/Menu/bg.png");
	RenderHelper::getInstance()->registerTexture("menuBg1", "Assets/Menu/levelselector.png");
	RenderHelper::getInstance()->registerTexture("dagger", "Assets/Menu/buttons/dagger.png");
	RenderHelper::getInstance()->registerTexture("olay", "Assets/Menu/buttons/playy.png");
	RenderHelper::getInstance()->registerTexture("xredit", "Assets/Menu/buttons/credits.png");
	RenderHelper::getInstance()->registerTexture("aetting", "Assets/Menu/buttons/settings.png");
	RenderHelper::getInstance()->registerTexture("howtoplay", "Assets/Menu/buttons/howtoplay.png");
	RenderHelper::getInstance()->registerTexture("quit", "Assets/Menu/buttons/exits.png");

	RenderHelper::getInstance()->registerTexture("level1", "Assets/Menu/level1.png");
	RenderHelper::getInstance()->registerTexture("level2", "Assets/Menu/level2.png");
	RenderHelper::getInstance()->registerTexture("level3", "Assets/Menu/level3.png");
	RenderHelper::getInstance()->registerTexture("level4", "Assets/Menu/level4.png");
	RenderHelper::getInstance()->registerTexture("level5", "Assets/Menu/level5.png");

}

void SceneMenu::Init()
{

	myMenu.buttonWidth = 300.0f; // Example initialization
	myMenu.buttonHeight = 230.0f; // Example initialization
	myMenu.transitionEnd = false; // Example initialization
	myMenu.transitionTimer = 0.0f; // Example initialization
	myMenu.transitionElapse = 0.0f; // Example initialization
	myMenu.nextLevel = 0; // Example initialization
	const float buttonGap = 100.0f; // Adjust the value as needed
	for (int i = 0; i < 5; ++i)
	{
		myMenu.buttonX[i] = 0;
		myMenu.buttonY[i] = -i * (myMenu.buttonHeight - buttonGap) + 250;
	}

	myMenu.buttonSelectWidth = 250.0f; // Example initialization
	myMenu.buttonSelectHeight = 400.0f; // Example initialization

	const float buttonSelectGap = 15.0f; // Adjust the value as needed
	for (int i = 0; i < 5; ++i)
	{
		myMenu.buttonSelectX[i] = -500.0f + i * (myMenu.buttonSelectWidth + buttonSelectGap); // Adjust the x-coordinate calculation
		myMenu.buttonSelectY[i] = 0.0f; // Adjust the y-coordinate as needed
		myMenu.hoveringSelect[i] = false;
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


	//if (AEInputCheckTriggered(AEVK_1)) {
	//	SceneManager::GetInstance()->SetActiveScene("TestScene");
	//}
	//else if (AEInputCheckTriggered(AEVK_2)) {
	//	SceneManager::GetInstance()->SetActiveScene("CombatScene");
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

		if (!myMenu.levelSelecting)
		{
			for (int i = 0; i < 5; ++i)
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
						SoundPlayer::stopAll();
						SoundPlayer::MenuAudio::getInstance().playLoopLevelSelect();
						loopIsPlaying = false;
						break;
					case 1:
						SceneManager::GetInstance()->SetActiveScene("SceneCredits");
						break;
					case 2:
						SceneManager::GetInstance()->SetActiveScene("SceneSetting");
						break;
					case 3:
						SceneManager::GetInstance()->SetActiveScene("HowToPlay");

					case 4:
						gGameRunning = false;
						break;
					}
				}
			}
		}
		else
		{

			for (int i = 0; i < 5; ++i)
			{
				AEVec2 p1 = { myMenu.buttonSelectX[i] - myMenu.buttonWidth / 2.f, myMenu.buttonSelectY[i] + myMenu.buttonHeight / 2.f };
				AEVec2 p2 = { myMenu.buttonSelectX[i] + myMenu.buttonWidth / 2.f, myMenu.buttonSelectY[i] - myMenu.buttonHeight / 2.f };

				if (p1.x < mx && p1.y > my && p2.x > mx && p2.y < my)
				{
					myMenu.hoveringSelect[i] = true;
					switch (i)
					{
					case 0:SceneManager::GetInstance()->SetActiveScene("SceneStages");
						SoundPlayer::stopAll();
						loopIsPlaying = false;
						break;
					case 1:SceneManager::GetInstance()->SetActiveScene("SceneStages");
						SoundPlayer::stopAll();
						loopIsPlaying = false;
						break;
					case 2:SceneManager::GetInstance()->SetActiveScene("SceneStages");
						SoundPlayer::stopAll();
						loopIsPlaying = false;
						break;
					case 3:SceneManager::GetInstance()->SetActiveScene("SceneStages");
						SoundPlayer::stopAll();
						loopIsPlaying = false;
						break;
					case 4:SceneManager::GetInstance()->SetActiveScene("SceneStages");
						SoundPlayer::stopAll();
						loopIsPlaying = false;
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
		for (int i = 0; i < 5; ++i)
		{
			myMenu.hovering[i] = false;
		}
	}

	// !TODO: implement back button too
	if (AEInputCheckTriggered(AEVK_Q) && myMenu.levelSelecting) {
		SoundPlayer::stopAll();
		SoundPlayer::MenuAudio::getInstance().playLoopMenu();
		myMenu.levelSelecting = false;
		return;
	}
}


void SceneMenu::Render()
{
	RenderHelper::getInstance()->texture("menuBg", 0, 0, static_cast<float>(AEGfxGetWindowWidth()), static_cast<float>(AEGfxGetWindowHeight()));

	// Render the menu buttons
	for (int i = 5; i >= 0; --i)
	{
		RenderHelper::getInstance()->texture(myMenu.button[i], myMenu.buttonX[i], myMenu.buttonY[i], myMenu.buttonWidth, myMenu.buttonHeight);

		if (myMenu.hovering[i])
		{
			RenderHelper::getInstance()->texture(myMenu.pointer, myMenu.buttonX[i] - myMenu.buttonWidth / 2.0f - 30.0f, myMenu.buttonY[i], 40, 40);
		}
	}

	// Render the background image again before rendering buttonSelect - should not do it like this, implement inner states in this scene
	if (myMenu.levelSelecting)
	{
		RenderHelper::getInstance()->texture("menuBg1", 0, 0, static_cast<float>(AEGfxGetWindowWidth()), static_cast<float>(AEGfxGetWindowHeight()));

		// Render the buttonSelect buttons
		for (int i = 0; i < 5; ++i)
		{
			RenderHelper::getInstance()->texture(myMenu.buttonSelect[i], myMenu.buttonSelectX[i], myMenu.buttonSelectY[i], myMenu.buttonWidth, myMenu.buttonHeight);
		}
	}

	ParticleManager::GetInstance()->render();
}

void SceneMenu::Exit()
{
	std::cout << "Exiting Scene Menu" << std::endl;

}