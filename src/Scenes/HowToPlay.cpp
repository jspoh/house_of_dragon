/* Start Header ************************************************************************/
/*!
\file HowToPlay.cpp
\author Yang yujie, yujie.yang, 2301383
\par yujie.yang\@digipen.edu
\date 01 Apr 2024
\brief  show how to play the game
/*
Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/


#include "Pch.h"
#include "HowToPlay.h"
#include "SoundPlayer.h"
#include "ParticleManager.h"

HowToPlay* HowToPlay::sInstance = new HowToPlay(SceneManager::GetInstance());

HowToPlay::HowToPlay() {}

HowToPlay::HowToPlay(SceneManager* _sceneMgr) {
    _sceneMgr->AddScene("HowToPlay", this);
}

HowToPlay::~HowToPlay() {}

void HowToPlay::Load() {
    slideshow.background = "menuBg";
    slideshow.leftButton = "leftButton";
    slideshow.rightButton = "rightButton";
    slideshow.backButton = "backButton";
    slideshow.images[0] = "image1";
    slideshow.images[1] = "image2";
    slideshow.images[2] = "image3";
    slideshow.images[3] = "image4";
    slideshow.images[4] = "image5";
    slideshow.images[5] = "image6";
    slideshow.images[6] = "image7";
    slideshow.images[7] = "image8";

    RenderHelper::getInstance()->registerTexture("menuBg", "Assets/Menu/bg.png");
    RenderHelper::getInstance()->registerTexture("leftButton", "Assets/Menu/leftButton.png");
    RenderHelper::getInstance()->registerTexture("rightButton", "Assets/Menu/rightButton.png");
    RenderHelper::getInstance()->registerTexture("backButton", "Assets/Menu/back1.png");
    RenderHelper::getInstance()->registerTexture("image1", "Assets/Menu/01.png");
    RenderHelper::getInstance()->registerTexture("image2", "Assets/Menu/02.png");
    RenderHelper::getInstance()->registerTexture("image3", "Assets/Menu/03.png");
    RenderHelper::getInstance()->registerTexture("image4", "Assets/Menu/04.png");
    RenderHelper::getInstance()->registerTexture("image5", "Assets/Menu/05.png");
    RenderHelper::getInstance()->registerTexture("image6", "Assets/Menu/06.png");
    RenderHelper::getInstance()->registerTexture("image7", "Assets/Menu/07.png");
    RenderHelper::getInstance()->registerTexture("image8", "Assets/Menu/08.png");
}

void HowToPlay::Init() {
    slideshow.buttonWidth = 100.0f;
    slideshow.buttonHeight = 50.0f;
    slideshow.leftButtonX = -AEGfxGetWindowWidth() / 2.0f + slideshow.buttonWidth / 2.0f + 20.0f;
    slideshow.leftButtonY = 0.0f;
    slideshow.rightButtonX = AEGfxGetWindowWidth() / 2.0f - slideshow.buttonWidth / 2.0f - 20.0f;
    slideshow.rightButtonY = 0.0f;
    slideshow.backButtonX = -AEGfxGetWindowWidth() / 2.0f + slideshow.buttonWidth / 2.0f + 20.0f;
    slideshow.backButtonY = AEGfxGetWindowHeight() / 2.0f - slideshow.buttonHeight / 2.0f - 20.0f;
    slideshow.hoveringLeft = false;
    slideshow.hoveringRight = false;
    slideshow.hoveringBack = false;
    slideshow.currentImageIndex = 0;

    AEGfxSetCamPosition(0, 0);
}

void HowToPlay::Update(double dt) {
    UNREFERENCED_PARAMETER(dt);
    int mX, mY;
    AEInputGetCursorPosition(&mX, &mY);

    if (AEInputCheckTriggered(AEVK_LBUTTON)) {
        s32 mxx, myy;
        AEInputGetCursorPosition(&mxx, &myy);
        float mx = static_cast<float>(mxx);
        float my = static_cast<float>(myy);
        mx -= AEGfxGetWindowWidth() / 2;
        my = -my;
        my += AEGfxGetWindowHeight() / 2.0f;

        AEVec2 leftP1 = { slideshow.leftButtonX - slideshow.buttonWidth / 2.0f, slideshow.leftButtonY + slideshow.buttonHeight / 2.0f };
        AEVec2 leftP2 = { slideshow.leftButtonX + slideshow.buttonWidth / 2.0f, slideshow.leftButtonY - slideshow.buttonHeight / 2.0f };

        if (leftP1.x < mx && leftP1.y > my && leftP2.x > mx && leftP2.y < my) {
            slideshow.currentImageIndex = (slideshow.currentImageIndex - 1 + 8) % 8;
        }

        AEVec2 rightP1 = { slideshow.rightButtonX - slideshow.buttonWidth / 2.0f, slideshow.rightButtonY + slideshow.buttonHeight / 2.0f };
        AEVec2 rightP2 = { slideshow.rightButtonX + slideshow.buttonWidth / 2.0f, slideshow.rightButtonY - slideshow.buttonHeight / 2.0f };

        if (rightP1.x < mx && rightP1.y > my && rightP2.x > mx && rightP2.y < my) {
            slideshow.currentImageIndex = (slideshow.currentImageIndex + 1) % 8;
        }

        AEVec2 backP1 = { slideshow.backButtonX - slideshow.buttonWidth / 2.0f, slideshow.backButtonY + slideshow.buttonHeight / 2.0f };
        AEVec2 backP2 = { slideshow.backButtonX + slideshow.buttonWidth / 2.0f, slideshow.backButtonY - slideshow.buttonHeight / 2.0f };

        if (backP1.x < mx && backP1.y > my && backP2.x > mx && backP2.y < my) {
            SceneManager::GetInstance()->SetActiveScene("SceneMenu");
        }
    }
}

void HowToPlay::Render() {
    RenderHelper::getInstance()->texture(slideshow.background, 0, 0, static_cast<float>(AEGfxGetWindowWidth()), static_cast<float>(AEGfxGetWindowHeight()));

    RenderHelper::getInstance()->texture(slideshow.images[slideshow.currentImageIndex], 0, 0, static_cast<float>(AEGfxGetWindowWidth()), static_cast<float>(AEGfxGetWindowHeight()));

    RenderHelper::getInstance()->texture(slideshow.leftButton, slideshow.leftButtonX, slideshow.leftButtonY, slideshow.buttonWidth, slideshow.buttonHeight);
    RenderHelper::getInstance()->texture(slideshow.rightButton, slideshow.rightButtonX, slideshow.rightButtonY, slideshow.buttonWidth, slideshow.buttonHeight);
    RenderHelper::getInstance()->texture(slideshow.backButton, slideshow.backButtonX, slideshow.backButtonY, slideshow.buttonWidth, slideshow.buttonHeight);
}

void HowToPlay::Exit() {
    cout << "Exiting Scene Slideshow" << "\n";
}