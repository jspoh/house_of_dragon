/* Start Header ************************************************************************/
/*!
\file SceneMenu.cpp
\author Yang yujie, yujie.yang, 2301383
\par yujie.yang\@digipen.edu
\date 01 Apr 2024
\brief menu scene implementation
/*
Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/


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

	myMenu.bg = "./Assets/Menu/bg.png";
	myMenu.bg1 = "./Assets/Menu/levelselector.png";
    myMenu.pointer = "./Assets/Menu/buttons/dagger.png";
    myMenu.button[0] = "./Assets/Menu/buttons/playy.png";
    myMenu.button[1] = "./Assets/Menu/buttons/credits.png";
    myMenu.button[2] = "./Assets/Menu/buttons/settings.png";
    myMenu.button[3] = "./Assets/Menu/buttons/howtoplay.png";
    myMenu.button[4] = "./Assets/Menu/buttons/exits.png";
    myMenu.back = "./Assets/Menu/back1.png";


    myMenu.buttonSelect[0] = "./Assets/Menu/level1.png";
    myMenu.buttonSelect[1] = "./Assets/Menu/level2.png";
    myMenu.buttonSelect[2] = "./Assets/Menu/level3.png";
    myMenu.buttonSelect[3] = "./Assets/Menu/level4.png";
    myMenu.buttonSelect[4] = "./Assets/Menu/level5.png";
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
        myMenu.buttonScale[i] = 1.0f; // Initialize button scales to 1.0f

        myMenu.buttonX[i] = 0;
        myMenu.buttonY[i] = -i * (myMenu.buttonHeight - buttonGap) + 250;
    }

    myMenu.buttonSelectWidth = 250.0f;
    myMenu.buttonSelectHeight = 400.0f;


    const float buttonSelectGap = 15.0f; // Adjust the value as needed
    for (int i = 0; i < 5; ++i)
    {
        myMenu.buttonSelectScale[i] = 1.0f; // Initialize button select scales to 1.0f


        myMenu.buttonSelectX[i] = -500.0f + i * (myMenu.buttonSelectWidth - buttonSelectGap); // Adjust the x-coordinate calculation
        myMenu.buttonSelectY[i] = 0.0f; // Adjust the y-coordinate as needed
        myMenu.hoveringSelect[i] = false;
    }
    ParticleManager::GetInstance()->init();

    if (!loopIsPlaying) {
        SoundPlayer::MenuAudio::getInstance().playLoopMenu();
        loopIsPlaying = true;
    }

    // Initialize back button variables
    myMenu.backButtonWidth = 100.0f;
    myMenu.backButtonHeight = 50.0f;
    myMenu.backButtonX = -AEGfxGetWindowWidth() / 2.0f + myMenu.backButtonWidth / 2.0f + 20.0f;
    myMenu.backButtonY = AEGfxGetWindowHeight() / 2.0f - myMenu.backButtonHeight / 2.0f - 20.0f;
    myMenu.hoveringBack = false;



    AEGfxSetCamPosition(0, 0);
}




void SceneMenu::Update(double dt)
{
    int mX, mY;
    AEInputGetCursorPosition(&mX, &mY);

    ParticleManager::GetInstance()->setParticlePos(static_cast<float>(mX), static_cast<float>(mY));
    ParticleManager::GetInstance()->update(dt);

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
                AEVec2 p1 = { myMenu.buttonX[i] - myMenu.buttonWidth / 2.3f * myMenu.buttonScale[i], myMenu.buttonY[i] + myMenu.buttonHeight / 7.5f * myMenu.buttonScale[i] };
                AEVec2 p2 = { myMenu.buttonX[i] + myMenu.buttonWidth / 2.3f * myMenu.buttonScale[i], myMenu.buttonY[i] - myMenu.buttonHeight / 7.5f * myMenu.buttonScale[i] };

                if (p1.x < mx && p1.y > my && p2.x > mx && p2.y < my)
                {
                    myMenu.hovering[i] = true;
                    myMenu.buttonScale[i] = 1.2f; // Increase the scale when hovering
                    switch (i)
                    {
                    case 0:
                        myMenu.levelSelecting = true;
                        //SoundPlayer::stopAll();
                        //SoundPlayer::MenuAudio::getInstance().playLoopLevelSelect();
                        //loopIsPlaying = false;
                        break;
                    case 1:
                        SceneManager::GetInstance()->SetActiveScene("SceneCredits");
                        break;
                    case 2:
                        SceneManager::GetInstance()->SetActiveScene("SceneSetting");
                        break;
                    case 3:
                        SceneManager::GetInstance()->SetActiveScene("HowToPlay");
                        break;
                    case 4:
                        gGameRunning = false;
                        break;
                    }
                }
                else
                {
                    myMenu.hovering[i] = false;
                    myMenu.buttonScale[i] = 1.0f; // Reset the scale when not hovering
                }
            }
        }
        else
        {
            for (int i = 0; i < 5; ++i)
            {
                AEVec2 p1 = { myMenu.buttonSelectX[i] - myMenu.buttonWidth / 6.f, myMenu.buttonSelectY[i] + myMenu.buttonHeight / 4.f };
                AEVec2 p2 = { myMenu.buttonSelectX[i] + myMenu.buttonWidth / 6.f, myMenu.buttonSelectY[i] - myMenu.buttonHeight / 4.f };

                if (p1.x < mx && p1.y > my && p2.x > mx && p2.y < my)
                {
                    myMenu.hoveringSelect[i] = true;
                    myMenu.buttonSelectScale[i] = 1.2f; // Increase the scale when hovering
                    switch (i)
                    {
                    case 0:
                        SceneManager::GetInstance()->SetActiveScene("SceneStages");
                        SoundPlayer::stopAll();
                        loopIsPlaying = false;
                        break;
                    case 1:
                        SceneManager::GetInstance()->SetActiveScene("SceneStages");
                        SoundPlayer::stopAll();
                        loopIsPlaying = false;
                        break;
                    case 2:
                        SceneManager::GetInstance()->SetActiveScene("SceneStages");
                        SoundPlayer::stopAll();
                        loopIsPlaying = false;
                        break;
                    case 3:
                        SceneManager::GetInstance()->SetActiveScene("SceneStages");
                        SoundPlayer::stopAll();
                        loopIsPlaying = false;
                        break;
                    case 4:
                        SceneManager::GetInstance()->SetActiveScene("SceneStages");
                        SoundPlayer::stopAll();
                        loopIsPlaying = false;
                        break;

                    }
                }
                else
                {
                    myMenu.hoveringSelect[i] = false;
                    myMenu.buttonSelectScale[i] = 1.0f; // Reset the scale when not hovering
                }
            }

            // Check if the back button is clicked
            AEVec2 p1 = { myMenu.backButtonX - myMenu.backButtonWidth / 2.0f, myMenu.backButtonY + myMenu.backButtonHeight / 2.0f };
            AEVec2 p2 = { myMenu.backButtonX + myMenu.backButtonWidth / 2.0f, myMenu.backButtonY - myMenu.backButtonHeight / 2.0f };

            if (p1.x < mx && p1.y > my && p2.x > mx && p2.y < my)
            {
                myMenu.levelSelecting = false;
            }
        }

        return;
    }
    else
    {
        for (int i = 0; i < 5; ++i)
        {
            myMenu.hovering[i] = false;
            myMenu.buttonScale[i] = 1.0f; // Reset the scale when not hovering
        }

        myMenu.hoveringBack = false;
    }
}



void SceneMenu::Render()
{
    RenderHelper::getInstance()->texture("./Assets/Menu/bg.png", 0, 0, static_cast<float>(AEGfxGetWindowWidth()), static_cast<float>(AEGfxGetWindowHeight()));

    // Render the menu buttons
    for (int i = 4; i >= 0; --i)
    {
        RenderHelper::getInstance()->texture(myMenu.button[i], myMenu.buttonX[i], myMenu.buttonY[i], myMenu.buttonWidth * myMenu.buttonScale[i], myMenu.buttonHeight * myMenu.buttonScale[i]);

        if (myMenu.hovering[i])
        {
            RenderHelper::getInstance()->texture(myMenu.pointer, myMenu.buttonX[i] - myMenu.buttonWidth / 2.0f - 30.0f, myMenu.buttonY[i], 40, 40);
        }
    }

    // Render the background image again before rendering buttonSelect
    if (myMenu.levelSelecting)
    {
        RenderHelper::getInstance()->texture("./Assets/Menu/levelselector.png", 0, 0, static_cast<float>(AEGfxGetWindowWidth()), static_cast<float>(AEGfxGetWindowHeight()));

        // Render the buttonSelect buttons
        for (int i = 0; i < 5; ++i)
        {
            RenderHelper::getInstance()->texture(myMenu.buttonSelect[i], myMenu.buttonSelectX[i], myMenu.buttonSelectY[i], myMenu.buttonWidth * myMenu.buttonSelectScale[i], myMenu.buttonHeight * myMenu.buttonSelectScale[i]);
        }

        // Render the back button
        RenderHelper::getInstance()->texture(myMenu.back, myMenu.backButtonX, myMenu.backButtonY, myMenu.backButtonWidth , myMenu.backButtonHeight );

        if (myMenu.hoveringBack)
        {
            RenderHelper::getInstance()->texture(myMenu.pointer, myMenu.backButtonX - myMenu.backButtonWidth / 2.0f - 30.0f, myMenu.backButtonY, 40, 40);
        }

    }

    ParticleManager::GetInstance()->render();

    //RenderHelper::getInstance()->texture("./Assets/Menu/bg.png", 0, 0, static_cast<float>(AEGfxGetWindowWidth()), static_cast<float>(AEGfxGetWindowHeight()));
}


void SceneMenu::Exit()
{
	cout << "Exiting Scene Menu" << "\n";

}