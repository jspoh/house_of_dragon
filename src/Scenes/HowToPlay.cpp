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


HowToPlay::HowToPlay() {
    m_slideshow.buttonWidth = 100.0f;
    m_slideshow.buttonHeight = 50.0f;
    m_slideshow.leftButtonX = -AEGfxGetWindowWidth() / 2.0f + m_slideshow.buttonWidth / 2.0f + 20.0f;
    m_slideshow.leftButtonY = 0.0f;
    m_slideshow.rightButtonX = AEGfxGetWindowWidth() / 2.0f - m_slideshow.buttonWidth / 2.0f - 20.0f;
    m_slideshow.rightButtonY = 0.0f;
    m_slideshow.backButtonX = -AEGfxGetWindowWidth() / 2.0f + m_slideshow.buttonWidth / 2.0f + 20.0f;
    m_slideshow.backButtonY = AEGfxGetWindowHeight() / 2.0f - m_slideshow.buttonHeight / 2.0f - 20.0f;
    m_slideshow.hoveringLeft = false;
    m_slideshow.hoveringRight = false;
    m_slideshow.hoveringBack = false;
    m_slideshow.currentImageIndex = 0;
}

HowToPlay::~HowToPlay() {}

HowToPlay& HowToPlay::getInstance() {
    static HowToPlay instance;
    return instance;
}

void HowToPlay::load() {
    m_slideshow.background = "menuBg";
    m_slideshow.leftButton = "leftButton";
    m_slideshow.rightButton = "rightButton";
    m_slideshow.backButton = "backButton";
    m_slideshow.images[0] = "image0";
    m_slideshow.images[1] = "image1";
    m_slideshow.images[2] = "image2";
    m_slideshow.images[3] = "image3";
    m_slideshow.images[4] = "image4";
    m_slideshow.images[4] = "image5";
    m_slideshow.images[5] = "image6";
    m_slideshow.images[6] = "image7";
    m_slideshow.images[7] = "image8";
    m_slideshow.images[8] = "image9";
    m_slideshow.images[9] = "image10";
    m_slideshow.images[10] = "image11";
    m_slideshow.images[11] = "image12";




    RenderHelper::getInstance()->registerTexture("menuBg", "Assets/Menu/bg.png");
    RenderHelper::getInstance()->registerTexture("leftButton", "Assets/Menu/leftButton.png");
    RenderHelper::getInstance()->registerTexture("rightButton", "Assets/Menu/rightButton.png");
    RenderHelper::getInstance()->registerTexture("backButton", "Assets/Menu/back1.png");
    RenderHelper::getInstance()->registerTexture("image0", "Assets/Menu/00.png");
    RenderHelper::getInstance()->registerTexture("image1", "Assets/Menu/01.png");
    RenderHelper::getInstance()->registerTexture("image2", "Assets/Menu/02.png");
    RenderHelper::getInstance()->registerTexture("image3", "Assets/Menu/03.png");
    RenderHelper::getInstance()->registerTexture("image4", "Assets/Menu/04.png");
    RenderHelper::getInstance()->registerTexture("image5", "Assets/Menu/05.png");
    RenderHelper::getInstance()->registerTexture("image6", "Assets/Menu/06.png");
    RenderHelper::getInstance()->registerTexture("image7", "Assets/Menu/07.png");
    RenderHelper::getInstance()->registerTexture("image8", "Assets/Menu/08.png");
    RenderHelper::getInstance()->registerTexture("image9", "Assets/Menu/09.png");
    RenderHelper::getInstance()->registerTexture("image10", "Assets/Menu/10.png");
    RenderHelper::getInstance()->registerTexture("image11", "Assets/Menu/11.png");
    RenderHelper::getInstance()->registerTexture("image12", "Assets/Menu/12.png");
}

void HowToPlay::init() {
    AEGfxSetCamPosition(0, 0);
}

void HowToPlay::update([[maybe_unused]] double dt) {
    if (m_isActive && !m_prevIsActive) {
        init();
        m_slideshow.currentImageIndex = 0;
    }

    float mx = static_cast<float>(mouseX);
    float my = static_cast<float>(mouseY);

    // convert mouse position into world coords (not a very good way to do it)
    mx -= AEGfxGetWindowWidth() / 2.f;
    my = -my;
    my += AEGfxGetWindowHeight() / 2.f;

    if (AEInputCheckTriggered(AEVK_LBUTTON)) {
        AEVec2 leftP1 = { m_slideshow.leftButtonX - m_slideshow.buttonWidth / 2.0f, m_slideshow.leftButtonY + m_slideshow.buttonHeight / 2.0f };
        AEVec2 leftP2 = { m_slideshow.leftButtonX + m_slideshow.buttonWidth / 2.0f, m_slideshow.leftButtonY - m_slideshow.buttonHeight / 2.0f };

        if (leftP1.x < mx && leftP1.y > my && leftP2.x > mx && leftP2.y < my) {
            m_slideshow.currentImageIndex = (m_slideshow.currentImageIndex - 1 + 13) % 13;
        }

        AEVec2 rightP1 = { m_slideshow.rightButtonX - m_slideshow.buttonWidth / 2.0f, m_slideshow.rightButtonY + m_slideshow.buttonHeight / 2.0f };
        AEVec2 rightP2 = { m_slideshow.rightButtonX + m_slideshow.buttonWidth / 2.0f, m_slideshow.rightButtonY - m_slideshow.buttonHeight / 2.0f };

        if (rightP1.x < mx && rightP1.y > my && rightP2.x > mx && rightP2.y < my) {
            m_slideshow.currentImageIndex = (m_slideshow.currentImageIndex + 1) % 13;
        }

        AEVec2 backP1 = { m_slideshow.backButtonX - m_slideshow.buttonWidth / 2.0f, m_slideshow.backButtonY + m_slideshow.buttonHeight / 2.0f };
        AEVec2 backP2 = { m_slideshow.backButtonX + m_slideshow.buttonWidth / 2.0f, m_slideshow.backButtonY - m_slideshow.buttonHeight / 2.0f };

        if (backP1.x < mx && backP1.y > my && backP2.x > mx && backP2.y < my) {
            m_isActive = false;
        }
    }

    m_prevIsActive = m_isActive;
}

void HowToPlay::render() {
    RenderHelper::getInstance()->texture(m_slideshow.background, 0, 0, static_cast<float>(AEGfxGetWindowWidth()), static_cast<float>(AEGfxGetWindowHeight()));

    RenderHelper::getInstance()->texture(m_slideshow.images[m_slideshow.currentImageIndex], 0, 0, static_cast<float>(AEGfxGetWindowWidth()), static_cast<float>(AEGfxGetWindowHeight()));

    RenderHelper::getInstance()->texture(m_slideshow.leftButton, m_slideshow.leftButtonX, m_slideshow.leftButtonY, m_slideshow.buttonWidth, m_slideshow.buttonHeight);
    RenderHelper::getInstance()->texture(m_slideshow.rightButton, m_slideshow.rightButtonX, m_slideshow.rightButtonY, m_slideshow.buttonWidth, m_slideshow.buttonHeight);
    RenderHelper::getInstance()->texture(m_slideshow.backButton, m_slideshow.backButtonX, m_slideshow.backButtonY, m_slideshow.buttonWidth, m_slideshow.buttonHeight);
}

void HowToPlay::exit() {
    cout << "Exiting Scene Slideshow" << "\n";
}