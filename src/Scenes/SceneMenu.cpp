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
	myMenu.pointer = "dagger";
	myMenu.button[0] = "olay";
	myMenu.button[1] = "xredit";
	myMenu.button[2] = "aetting";
	myMenu.button[3] = "quit";


	myMenu.buttonSelect[0] = "chicken";
	myMenu.buttonSelect[1] = "tiger";
	myMenu.buttonSelect[2] = "dragon_level_select";


	RenderHelper::getInstance()->registerTexture("menuBg", "Assets/Menu/bg.png");
	RenderHelper::getInstance()->registerTexture("dagger", "Assets/Menu/buttons/dagger.png");
	RenderHelper::getInstance()->registerTexture("olay", "Assets/Menu/buttons/olay.png");
	RenderHelper::getInstance()->registerTexture("xredit", "Assets/Menu/buttons/xredit.png");
	RenderHelper::getInstance()->registerTexture("aetting", "Assets/Menu/buttons/aetting.png");
	RenderHelper::getInstance()->registerTexture("quit", "Assets/Menu/buttons/quit.png");

	RenderHelper::getInstance()->registerTexture("chicken", "Assets/Menu/chicken.png");
	RenderHelper::getInstance()->registerTexture("tiger", "Assets/Menu/tiger.png");
	RenderHelper::getInstance()->registerTexture("dragon_level_select", "Assets/Menu/dragon.png");

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
		myMenu.buttonY[i] = -i * (myMenu.buttonHeight) + 200;
	}

	for (int i = 0; i < 3; ++i)
	{
		myMenu.buttonSelectX[i] = -300.0f + i * 300.0f; // Adjust the x-coordinate as needed
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
						SoundPlayer::stopAll();
						loopIsPlaying = false;
						break;
					case 1:
						SceneManager::GetInstance()->SetActiveScene("SceneCredits");
						break;
					case 2:
						SceneManager::GetInstance()->SetActiveScene("SceneSetting");
						break;
					case 3:
						gGameRunning = false;
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
	RenderHelper::getInstance()->texture("menuBg", 0, 0, static_cast<float>(AEGfxGetWindowWidth()), static_cast<float>(AEGfxGetWindowHeight()));

	// Render the menu buttons
	for (int i = 3; i >= 0; --i)
	{
		RenderHelper::getInstance()->texture(myMenu.button[i], myMenu.buttonX[i], myMenu.buttonY[i], myMenu.buttonWidth, myMenu.buttonHeight);

		if (myMenu.hovering[i])
		{
			RenderHelper::getInstance()->texture(myMenu.pointer, myMenu.buttonX[i] - myMenu.buttonWidth / 2.0f - 30.0f, myMenu.buttonY[i], 40, 40);
		}
	}

	// Render the background image again before rendering buttonSelect - hmm why render again?
	if (myMenu.levelSelecting)
	{
		RenderHelper::getInstance()->texture("menuBg", 0, 0, static_cast<float>(AEGfxGetWindowWidth()), static_cast<float>(AEGfxGetWindowHeight()));

		// Render the buttonSelect buttons
		for (int i = 0; i < 3; ++i)
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