/* Start Header ************************************************************************/
/*!
\file HowToPlay.h
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


#pragma once

#include "Pch.h"

class HowToPlay {
private:
    HowToPlay();
    ~HowToPlay();

public:
    // used to control if tutorial overlay is shown
    bool m_isActive = false;

    static HowToPlay& getInstance();

    void load();
    void init();
    void update(double dt);
    void render();
    void exit();

    typedef struct {
        s8 fontID;
        std::string background;
        std::string leftButton;
        std::string rightButton;
        std::string backButton;
        std::string images[13];
        s32 cursorX, cursorY;
        float leftButtonX, leftButtonY;
        float rightButtonX, rightButtonY;
        float backButtonX, backButtonY;
        float buttonWidth, buttonHeight;
        bool hoveringLeft, hoveringRight, hoveringBack;
        int currentImageIndex;
    } Slideshow;

private:
    Slideshow m_slideshow{ 0 };
    bool m_prevIsActive = m_isActive;
};
