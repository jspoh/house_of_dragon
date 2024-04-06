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

    static constexpr u8 m_PAUSE_KEY = AEVK_ESCAPE;
    static constexpr int m_NUM_BTNS = 4;
    std::array<std::pair<std::string, float>, m_NUM_BTNS> m_btns{
        std::pair{"RESUME", 1.f},
        {"HOWTOPLAY", 1.f},
        {"RESTART", 1.f},
        {"EXIT", 1.f}
    };

    static constexpr int m_BTN_WIDTH = 250;
    static constexpr int m_BTN_HEIGHT = 100;
    static constexpr int m_BTN_Y_GAP = 30;
    static constexpr float m_HOVER_BTN_SCALE = 1.1f;

    bool m_isConfirming;
    float m_confirmTimer;
    std::string m_confirmingButton;
    static constexpr float m_CONFIRMATION_TIME = 0.5f;

public:
    static Pause& getInstance();
    bool m_isPaused;

    ~Pause() = default;

    void init();
    void update(double dt);
    void render() const;
};