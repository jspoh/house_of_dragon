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

SceneMenu::SceneMenu() : m_SelectedLevel{0}
{

}

SceneMenu::SceneMenu(SceneManager* _sceneMgr) : m_SelectedLevel{ 0 }
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
    myMenu.back = "back";

    myMenu.bg2 = "taichi";


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

    RenderHelper::getInstance()->registerTexture("back", "Assets/Menu/back1.png");

    RenderHelper::getInstance()->registerTexture("taichi", "Assets/Menu/download.png");
}

void SceneMenu::Init()
{
     myMenu.cameraX = 0.0f;
    myMenu.cameraY = 0.0f;
    myMenu.cameraSpeed = 200.0f; // Adjust the camera speed as needed

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

    myMenu.taichiRotation = 0.0f;
    myMenu.taichiScale = 1.0f;
    myMenu.taichiScaleSpeed = 0.5f;
    myMenu.taichiMinScale = 0.8f;
    myMenu.taichiMaxScale = 1.2f;

    AEGfxSetCamPosition(0, 0);
}



void SceneMenu::Update(double dt)
{
    int mX, mY;
    AEInputGetCursorPosition(&mX, &mY);

    ParticleManager::GetInstance()->setParticlePos(static_cast<float>(mX), static_cast<float>(mY));
    ParticleManager::GetInstance()->update(dt);

    s32 mxx, myy;
    AEInputGetCursorPosition(&mxx, &myy);
    float mx = static_cast<float>(mxx);
    float my = static_cast<float>(myy);
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
                myMenu.buttonScale[i] = 1.2f; // Increase the scale when hovering
            }
            else
            {
                myMenu.hovering[i] = false;
                myMenu.buttonScale[i] = 1.0f; // Reset the scale when not hovering
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
                        SceneManager::GetInstance()->SetActiveScene("HowToPlay");
                        break;
                    case 4:
                        gGameRunning = false;
                        break;
                    }
                }
            }
        }
    }
    else
    {
        // Handle WASD key input for camera movement
        if (AEInputCheckCurr(AEVK_W))
        {
            myMenu.cameraY += myMenu.cameraSpeed * dt;
        }
        if (AEInputCheckCurr(AEVK_S))
        {
            myMenu.cameraY -= myMenu.cameraSpeed * dt;
        }
        if (AEInputCheckCurr(AEVK_A))
        {
            myMenu.cameraX -= myMenu.cameraSpeed * dt;
        }
        if (AEInputCheckCurr(AEVK_D))
        {
            myMenu.cameraX += myMenu.cameraSpeed * dt;
        }
        // Button hovering logic for level selection buttons
        for (int i = 0; i < 5; ++i)
        {
            AEVec2 p1 = { myMenu.buttonSelectX[i] - myMenu.buttonWidth / 6.f, myMenu.buttonSelectY[i] + myMenu.buttonHeight / 4.f };
            AEVec2 p2 = { myMenu.buttonSelectX[i] + myMenu.buttonWidth / 6.f, myMenu.buttonSelectY[i] - myMenu.buttonHeight / 4.f };

            if (p1.x < mx && p1.y > my && p2.x > mx && p2.y < my)
            {
                myMenu.hoveringSelect[i] = true;
                myMenu.buttonSelectScale[i] = 1.2f; // Increase the scale when hovering
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
            for (int i = 0; i < 5; ++i)
            {
                AEVec2 p1 = { myMenu.buttonSelectX[i] - myMenu.buttonWidth / 6.f, myMenu.buttonSelectY[i] + myMenu.buttonHeight / 4.f };
                AEVec2 p2 = { myMenu.buttonSelectX[i] + myMenu.buttonWidth / 6.f, myMenu.buttonSelectY[i] - myMenu.buttonHeight / 4.f };

                if (p1.x < mx && p1.y > my && p2.x > mx && p2.y < my)
                {
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

        if (p1.x < mx && p1.y > my && p2.x > mx && p2.y < my)
        {
            myMenu.hoveringBack = true;
        }
        else
        {
            myMenu.hoveringBack = false;
        }
    }

    myMenu.taichiRotation += 5.0f * dt; // Adjust the rotation speed as needed
    if (myMenu.taichiRotation >= 360.0f)
    {
        myMenu.taichiRotation -= 360.0f;
    }

    myMenu.taichiScale += myMenu.taichiScaleSpeed * dt;
    if (myMenu.taichiScale > myMenu.taichiMaxScale || myMenu.taichiScale < myMenu.taichiMinScale)
    {
        myMenu.taichiScaleSpeed = -myMenu.taichiScaleSpeed;
    }
}


void SceneMenu::Render()
{
    RenderHelper::getInstance()->texture("menuBg", 0, 0, static_cast<float>(AEGfxGetWindowWidth()), static_cast<float>(AEGfxGetWindowHeight()));

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
      

        // Apply camera transformation
        AEGfxSetCamPosition(myMenu.cameraX, myMenu.cameraY);

        RenderHelper::getInstance()->texture("menuBg1", 0, 0, static_cast<float>(AEGfxGetWindowWidth()), static_cast<float>(AEGfxGetWindowHeight()));

        // Render the buttonSelect buttons
        for (int i = 0; i < 5; ++i)
        { 
            RenderHelper::getInstance()->texture(myMenu.buttonSelect[i], myMenu.buttonSelectX[i], myMenu.buttonSelectY[i], myMenu.buttonWidth * myMenu.buttonSelectScale[i], myMenu.buttonHeight * myMenu.buttonSelectScale[i]);
        }



        // Render the back button
        RenderHelper::getInstance()->texture(myMenu.back, myMenu.backButtonX, myMenu.backButtonY, myMenu.backButtonWidth, myMenu.backButtonHeight);

        if (myMenu.hoveringBack)
        {
            RenderHelper::getInstance()->texture(myMenu.pointer, myMenu.backButtonX - myMenu.backButtonWidth / 2.0f - 30.0f, myMenu.backButtonY, 40, 40);
        }

    
   /*     float taichiX = 0.0f;
        float taichiY = 0.0f;
        float taichiWidth = 200.0f;
        float taichiHeight = 200.0f;

        RenderHelper::getInstance()->texture("taichi", taichiX, taichiY, taichiWidth * myMenu.taichiScale, taichiHeight * myMenu.taichiScale, 1.0f, Color{ 1.0f, 1.0f, 1.0f, 1.0f }, myMenu.taichiRotation);*/


    }

    ParticleManager::GetInstance()->render();

    //RenderHelper::getInstance()->texture("menuBg", 0, 0, static_cast<float>(AEGfxGetWindowWidth()), static_cast<float>(AEGfxGetWindowHeight()));
}


void SceneMenu::Exit()
{
    cout << "Exiting Scene Menu" << "\n";

}