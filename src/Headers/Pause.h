/* Start Header ************************************************************************/
/*!
\file Pause.h
\author Poh Jing Seng, jingseng.poh, 2301363
\par jingseng.poh\@digipen.edu
\date 3 Feb 2024
\brief pause screen overlay
/*
Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#pragma once

#include "Pch.h"

class Pause {
private:
    Pause();

    static constexpr u8 PAUSE_KEY = AEVK_ESCAPE;
    static constexpr int NUM_BTNS = 4;
    std::array<std::pair<std::string, float>, NUM_BTNS> btns{
        std::pair{"RESUME", 1.f},
        {"HOWTOPLAY", 1.f},
        {"RESTART", 1.f},
        {"EXIT", 1.f}
    };

    static constexpr int BTN_WIDTH = 250;
    static constexpr int BTN_HEIGHT = 100;
    static constexpr int BTN_Y_GAP = 30;
    static constexpr float HOVER_BTN_SCALE = 1.1f;

    bool isConfirming;
    float confirmTimer;
    std::string confirmingButton;

public:
    static Pause& getInstance();
    bool isPaused;

    ~Pause() = default;

    void init();
    void update(double dt);
    void render() const;
};