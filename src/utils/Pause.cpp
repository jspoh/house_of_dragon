/* Start Header ************************************************************************/
/*!
\file Pause.cpp
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

#include "Pch.h"
#include "Pause.h"
#include "SceneManager.h"
#include "HowToPlay.h"

Pause::Pause() : m_isPaused{ false }, m_isConfirming{ false }, m_confirmTimer{ 0.0f }, m_confirmingButton{ "" } {
}

Pause& Pause::getInstance() {
    static Pause instance;
    return instance;
}

void Pause::init() {
    m_isConfirming = false;
    m_confirmTimer = 0.0f;
    m_confirmingButton = "";
}

void Pause::update([[maybe_unused]] double dt) {
    if (HowToPlay::getInstance().m_isActive) {
        HowToPlay::getInstance().update(dt);
        return;
    }

    if (AEInputCheckTriggered(m_PAUSE_KEY)) {
        m_isPaused = !m_isPaused;
        if (m_isPaused) {
            SoundManager::getInstance()->pauseGroup(false);
        }
        else {
            SoundManager::getInstance()->resumeGroup(false);
        }
    }

    if (!m_isPaused) {
        return;
    }

    updateGlobals();

    // check for clicks
    constexpr int yOffset = m_BTN_HEIGHT + m_BTN_Y_GAP;
    constexpr int btnYStart = 0 + (m_NUM_BTNS * yOffset - m_BTN_Y_GAP) / 2;
    int btnY = btnYStart;

    for (auto& [b, scale] : m_btns) {
        AEVec2 pos = wtos(0, static_cast<float>(btnY));
        if (CollisionChecker::isMouseInRect(pos.x, pos.y, m_BTN_WIDTH * scale, m_BTN_HEIGHT * scale, static_cast<float>(mouseX), static_cast<float>(mouseY))) {
            scale = m_HOVER_BTN_SCALE;
            if (AEInputCheckTriggered(AEVK_LBUTTON)) {
                if (b == "RESTART" || b == "EXIT") {
                    if (!m_isConfirming) {
                        m_isConfirming = true;
                        m_confirmingButton = b;
                        m_confirmTimer = 0.0f;
                    }
                    else if (m_confirmingButton == b) {
                        if (b == "RESTART") {
                            SoundManager::getInstance()->resumeGroup(false);
                            m_isPaused = false;
                            SceneManager::getInstance()->restartScene();
                        }
                        else if (b == "EXIT") {
                            SoundManager::getInstance()->resumeGroup(false);
                            m_isPaused = false;
                            AEGfxSetCamPosition(0, 0);
                            SceneManager::getInstance()->SetActiveScene("SceneMenu");
                        }
                        m_isConfirming = false;
                    }
                }
                else {
                    if (b == "RESUME") {
                        m_isPaused = false;
                        SoundManager::getInstance()->resumeGroup(false);
                    }
                    else if (b == "HOWTOPLAY") {
                        HowToPlay::getInstance().m_isActive = true;
                    }
                }
            }
        }
        else {
            scale = 1.f;
        }
        btnY -= yOffset;
    }

    // Update confirmation timer
    if (m_isConfirming) {
        m_confirmTimer += static_cast<float>(dt);
        if (m_confirmTimer >= m_CONFIRMATION_TIME) {
            m_isConfirming = false;
        }
    }
}

void Pause::render() const {
    if (!m_isPaused) {
        return;
    }

    if (HowToPlay::getInstance().m_isActive) {
        HowToPlay::getInstance().render();
        return;
    }

    updateGlobals();

    RenderHelper::getInstance()->rect("invis", 0, 0, 9999.f, 9999.f, 0, Color{ 0,0,0,0.7f }, 0.7f);

    constexpr int btnX = 0;
    constexpr int yOffset = m_BTN_HEIGHT + m_BTN_Y_GAP;
    constexpr int btnYStart = 0 + (m_NUM_BTNS * yOffset - m_BTN_Y_GAP) / 2;
    int btnY = btnYStart;

    for (const auto& [b, scale] : m_btns) {
        if (m_isConfirming && m_confirmingButton == b) {
            RenderHelper::getInstance()->texture("button", btnX + camOffset.x, btnY + camOffset.y, scale * m_BTN_WIDTH, scale * m_BTN_HEIGHT);
            RenderHelper::getInstance()->text("Confirm ?", AEGfxGetWindowWidth() / 2.f, -btnY + AEGfxGetWindowHeight() / 2.f);
        }
        else {
            RenderHelper::getInstance()->texture("button", btnX + camOffset.x, btnY + camOffset.y, scale * m_BTN_WIDTH, scale * m_BTN_HEIGHT);
            RenderHelper::getInstance()->text(b, AEGfxGetWindowWidth() / 2.f, -btnY + AEGfxGetWindowHeight() / 2.f);
        }
        btnY -= yOffset;
    }
}