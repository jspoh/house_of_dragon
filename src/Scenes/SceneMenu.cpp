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
#include "ParticleManager.h"
#include "HowToPlay.h"

namespace {
    bool loopIsPlaying = false;
}

SceneMenu* SceneMenu::sInstance = new SceneMenu(SceneManager::GetInstance());

SceneMenu::SceneMenu() : m_SelectedLevel{0}
{

}

SceneMenu::SceneMenu(SceneManager* _sceneMgr) : m_SelectedLevel{ 0 }
{
    _sceneMgr->AddScene("SceneMenu", this);
    nextSceneName = "SceneMenu";
}

SceneMenu::~SceneMenu()
{
}

/**
 * @brief Loads assets and initializes variables for the menu scene.
 *
 * Loads textures and sets up initial values for the main menu scene, such as
 * background images, button textures, and audio.
 */
void SceneMenu::Load()
{
    // Load textures
    myMenu.bg = "menuBg";
    myMenu.bg1 = "menuBg1";
    myMenu.pointer = "dagger";
    myMenu.button[0] = "olay";
    myMenu.button[1] = "xredit";
    myMenu.button[2] = "aetting";
    myMenu.button[3] = "howtoplay";
    myMenu.button[4] = "quit";
    myMenu.back = "back";
    myMenu.confirm = "confirm";


    // Level selection buttons
    myMenu.buttonSelect[0] = "level1";
    myMenu.buttonSelect[1] = "level2";
    myMenu.buttonSelect[2] = "level3";
    myMenu.buttonSelect[3] = "level4";
    myMenu.buttonSelect[4] = "level5";
    myMenu.buttonSelect[5] = "level6";
    myMenu.buttonSelect[6] = "level7";


    RenderHelper::getInstance()->registerTexture("menuBg", "Assets/Menu/bg.png");
    RenderHelper::getInstance()->registerTexture("menuBg1", "Assets/Menu/levelselector.png");
    RenderHelper::getInstance()->registerTexture("dagger", "Assets/Menu/buttons/dagger.png");

    RenderHelper::getInstance()->registerTexture("olay", "Assets/Menu/buttons/playy.png");
    RenderHelper::getInstance()->registerTexture("xredit", "Assets/Menu/buttons/credits.png");
    RenderHelper::getInstance()->registerTexture("aetting", "Assets/Menu/buttons/settings.png");
    RenderHelper::getInstance()->registerTexture("howtoplay", "Assets/Menu/buttons/howtoplay.png");
    RenderHelper::getInstance()->registerTexture("quit", "Assets/Menu/buttons/exits.png");
    RenderHelper::getInstance()->registerTexture("confirm", "Assets/Menu/buttons/confirm.png");


    RenderHelper::getInstance()->registerTexture("level1", "Assets/Menu/level1.png");
    RenderHelper::getInstance()->registerTexture("level2", "Assets/Menu/level2.png");
    RenderHelper::getInstance()->registerTexture("level3", "Assets/Menu/level3.png");
    RenderHelper::getInstance()->registerTexture("level4", "Assets/Menu/level4.png");
    RenderHelper::getInstance()->registerTexture("level5", "Assets/Menu/level5.png");
    RenderHelper::getInstance()->registerTexture("level6", "Assets/Menu/level6.png");
    RenderHelper::getInstance()->registerTexture("level7", "Assets/Menu/level7.png");

    RenderHelper::getInstance()->registerTexture("back", "Assets/Menu/back1.png");

    HowToPlay::getInstance().Load();
}

/**
 * @brief Initializes variables for the menu scene.
 *
 * Initializes variables for the main menu scene, such as camera position,
 * button positions, and button scales.
 */
void SceneMenu::Init()
{
    myMenu.quitConfirm = false;
    myMenu.quitConfirmTimer = 0.0f;

    myMenu.buttonWidth = 300.0f; 
    myMenu.buttonHeight = 230.0f; 
    myMenu.transitionEnd = false; 
    myMenu.transitionTimer = 0.0f; 
    myMenu.transitionElapse = 0.0f; 
    myMenu.nextLevel = 0; 
    const float buttonGap = 130.0f; 
    for (int i = 0; i < 5; ++i)
    {
        myMenu.buttonScale[i] = 1.0f; 
        myMenu.buttonX[i] = 0;
        myMenu.buttonY[i] = -i * (myMenu.buttonHeight - buttonGap) + 150;
    }

    myMenu.buttonSelectWidth = 250.0f;
    myMenu.buttonSelectHeight = 400.0f;
    const float buttonSelectGap = 100.0f;
    const float buttonSelectStartX = -((7 - 1) * (myMenu.buttonSelectWidth - buttonSelectGap)) / 2.0f;  // Calculate the starting X position
    for (int i = 0; i < 7; ++i)
    {
        myMenu.buttonSelectScale[i] = 1.0f;

        myMenu.buttonSelectX[i] = buttonSelectStartX + i * (myMenu.buttonSelectWidth - buttonSelectGap);  // Adjust the positioning
        myMenu.buttonSelectY[i] = 0.0f;
        myMenu.hoveringSelect[i] = false;
    }
    ParticleManager::GetInstance()->init();

    if (!loopIsPlaying) {
        SoundPlayer::MenuAudio::getInstance().playLoopMenu();
        loopIsPlaying = true;
    }

    myMenu.backButtonWidth = 100.0f;
    myMenu.backButtonHeight = 50.0f;
    myMenu.backButtonX = -AEGfxGetWindowWidth() / 2.0f + myMenu.backButtonWidth / 2.0f + 20.0f;
    myMenu.backButtonY = AEGfxGetWindowHeight() / 2.0f - myMenu.backButtonHeight / 2.0f - 20.0f;
    myMenu.hoveringBack = false;

    AEGfxSetCamPosition(0, 0);
}


/**
 * @brief Updates the menu scene.
 *
 * Updates the menu scene by handling input, updating the camera position,
 * and updating the rotation and scale of the taichi symbol.
 *
 * @param dt Delta time
 */
void SceneMenu::Update(double dt)
{
    if (HowToPlay::getInstance().isActive) {
        HowToPlay::getInstance().Update(dt);
        return;
    }

    ParticleManager::GetInstance()->setParticlePos(static_cast<float>(mouseX), static_cast<float>(mouseY));
    ParticleManager::GetInstance()->update(dt);

    float mx = static_cast<float>(mouseX);
    float my = static_cast<float>(mouseY);
    mx -= AEGfxGetWindowWidth() / 2;

    my = -my;
    my += AEGfxGetWindowHeight() / 2.0f;

    AEVec2 cursorPos = { mx, my };

    if (!myMenu.levelSelecting)
    {
        // Button hovering logic
        for (int i = 0; i < 5; ++i)
        {
            AEVec2 p1 = { myMenu.buttonX[i] - myMenu.buttonWidth / 2.3f * myMenu.buttonScale[i], myMenu.buttonY[i] + myMenu.buttonHeight / 7.5f * myMenu.buttonScale[i] };
            AEVec2 p2 = { myMenu.buttonX[i] + myMenu.buttonWidth / 2.3f * myMenu.buttonScale[i], myMenu.buttonY[i] - myMenu.buttonHeight / 7.5f * myMenu.buttonScale[i] };

            if (p1.x < mx && p1.y > my && p2.x > mx && p2.y < my)
            {
                myMenu.hovering[i] = true;
                myMenu.buttonScale[i] = 1.2f; 
            }
            else
            {
                myMenu.hovering[i] = false;
                myMenu.buttonScale[i] = 1.0f; 
            }
        }

        // Button clicking logic
        if (AEInputCheckTriggered(AEVK_LBUTTON))
        {
            for (int i = 0; i < 5; ++i)
            {
                AEVec2 p1 = { myMenu.buttonX[i] - myMenu.buttonWidth / 2.3f * myMenu.buttonScale[i], myMenu.buttonY[i] + myMenu.buttonHeight / 7.5f * myMenu.buttonScale[i] };
                AEVec2 p2 = { myMenu.buttonX[i] + myMenu.buttonWidth / 2.3f * myMenu.buttonScale[i], myMenu.buttonY[i] - myMenu.buttonHeight / 7.5f * myMenu.buttonScale[i] };

                if (p1.x < mx && p1.y > my && p2.x > mx && p2.y < my)
                {
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
                        HowToPlay::getInstance().isActive = true;
                        break;
                    case 4:
                        if (!myMenu.quitConfirm)
                        {
                            myMenu.quitConfirm = true;
                            myMenu.quitConfirmTimer = 0.0f;
                        }
                        else
                        {
                            gGameRunning = false;
                        }
                        break;

                    }
                }
            }
        }




        if (myMenu.quitConfirm)
        {
			myMenu.quitConfirmTimer += static_cast<float>(dt);
            if (myMenu.quitConfirmTimer >= 5.0f)
            {
				myMenu.quitConfirm = false;
			}
		}
    }


    else
    {
  

        // Button hovering logic for level selection buttons
        for (int i = 0; i < 7; ++i)
        {
            AEVec2 p1 = { myMenu.buttonSelectX[i] - myMenu.buttonWidth / 6.f, myMenu.buttonSelectY[i] + myMenu.buttonHeight / 4.f };
            AEVec2 p2 = { myMenu.buttonSelectX[i] + myMenu.buttonWidth / 6.f, myMenu.buttonSelectY[i] - myMenu.buttonHeight / 4.f };

            if (p1.x < mx && p1.y > my && p2.x > mx && p2.y < my)
            {
                myMenu.hoveringSelect[i] = true;
                myMenu.buttonSelectScale[i] = 1.5f; // Increase the scale when hovering
            }
            else
            {
                myMenu.hoveringSelect[i] = false;
                myMenu.buttonSelectScale[i] = 1.0f; // Reset the scale when not hovering
            }
        }

        // Button clicking logic for level selection buttons
        if (AEInputCheckTriggered(AEVK_LBUTTON))
        {
            for (int i = 0; i < 7; ++i)
            {
                AEVec2 p1 = { myMenu.buttonSelectX[i] - myMenu.buttonWidth / 6.f, myMenu.buttonSelectY[i] + myMenu.buttonHeight / 4.f };
                AEVec2 p2 = { myMenu.buttonSelectX[i] + myMenu.buttonWidth / 6.f, myMenu.buttonSelectY[i] - myMenu.buttonHeight / 4.f };

                if (p1.x < mx && p1.y > my && p2.x > mx && p2.y < my)
                {
                    switch (i)
                    {
                    case 0:
                    case 1:
                    case 2:
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                        nextSceneName = "SceneStages";
                        SoundPlayer::stopAll();
                        loopIsPlaying = false;
                        myMenu.levelSelecting = false;
                        SceneManager::GetInstance()->SetActiveScene(nextSceneName);
                        break;
                    default:
                        cout << "Error in selection\n";
                    }
                    m_SelectedLevel = i;
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

        // Button hovering logic for back button
        AEVec2 p1 = { myMenu.backButtonX - myMenu.backButtonWidth / 2.0f, myMenu.backButtonY + myMenu.backButtonHeight / 2.0f };
        AEVec2 p2 = { myMenu.backButtonX + myMenu.backButtonWidth / 2.0f, myMenu.backButtonY - myMenu.backButtonHeight / 2.0f };

        // Check if the cursor is hovering over the back button
        if (p1.x < mx && p1.y > my && p2.x > mx && p2.y < my)
        {
            myMenu.hoveringBack = true;
        }
        else
        {
            myMenu.hoveringBack = false;
        }
    }

}

/**
 * @brief Renders the menu scene.
 *
 * Renders the menu scene by rendering the background image, menu buttons,
 */
void SceneMenu::Render()
{
    if (HowToPlay::getInstance().isActive) {
        HowToPlay::getInstance().Render();
        return;
    }

    RenderHelper::getInstance()->texture("menuBg", 0, 0, static_cast<float>(AEGfxGetWindowWidth()), static_cast<float>(AEGfxGetWindowHeight()));

    // Render the menu buttons
    for (int i = 4; i >= 0; --i)
    {
        if (i == 4 && myMenu.quitConfirm)
        {
            RenderHelper::getInstance()->texture(myMenu.confirm, myMenu.buttonX[i], myMenu.buttonY[i], myMenu.buttonWidth * myMenu.buttonScale[i], myMenu.buttonHeight * myMenu.buttonScale[i]);
        }
        else
        {
            RenderHelper::getInstance()->texture(myMenu.button[i], myMenu.buttonX[i], myMenu.buttonY[i], myMenu.buttonWidth * myMenu.buttonScale[i], myMenu.buttonHeight * myMenu.buttonScale[i]);
        }

        if (myMenu.hovering[i] && !myMenu.quitConfirm)
        {
            RenderHelper::getInstance()->texture(myMenu.pointer, myMenu.buttonX[i] - myMenu.buttonWidth / 2.0f - 30.0f, myMenu.buttonY[i], 40, 40);
        }
    }

    // Render the background image again before rendering buttonSelect
    if (myMenu.levelSelecting)
    { 

        RenderHelper::getInstance()->texture("menuBg1", 0, 0, static_cast<float>(AEGfxGetWindowWidth()), static_cast<float>(AEGfxGetWindowHeight()));

        // Render the buttonSelect buttons
        for (int i = 0; i < 7; ++i)
        { 
            RenderHelper::getInstance()->texture(myMenu.buttonSelect[i], myMenu.buttonSelectX[i], myMenu.buttonSelectY[i], myMenu.buttonWidth * myMenu.buttonSelectScale[i], myMenu.buttonHeight * myMenu.buttonSelectScale[i]);
        }

        // Render the back button
        RenderHelper::getInstance()->texture(myMenu.back, myMenu.backButtonX, myMenu.backButtonY, myMenu.backButtonWidth, myMenu.backButtonHeight);

     /*   if (myMenu.hoveringBack)
        {
            RenderHelper::getInstance()->texture(myMenu.pointer, myMenu.backButtonX - myMenu.backButtonWidth / 2.0f - 30.0f, myMenu.backButtonY, 40, 40);
        }*/
    }




    ParticleManager::GetInstance()->render();

}

/**
 * @brief Exits the menu scene.
 *
 * Exits the menu scene by stopping the menu audio loop.
 */
void SceneMenu::Exit()
{
    cout << "Exiting Scene Menu" << "\n";

}