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

Pause::Pause() : isPaused{ false }, isConfirming{ false }, confirmTimer{ 0.0f }, confirmingButton{ "" } {
}

Pause& Pause::getInstance() {
    static Pause instance;
    return instance;
}

void Pause::init() {
    isConfirming = false;
    confirmTimer = 0.0f;
    confirmingButton = "";
}

void Pause::update([[maybe_unused]] double dt) {
    if (AEInputCheckTriggered(PAUSE_KEY)) {
        isPaused = !isPaused;
        if (isPaused) {
            SoundManager::GetInstance()->pauseGroup(false);
        }
        else {
            SoundManager::GetInstance()->resumeGroup(false);
        }
    }

    if (!isPaused) {
        return;
    }

    updateGlobals();

    // check for clicks
    constexpr int yOffset = BTN_HEIGHT + BTN_Y_GAP;
    constexpr int btnYStart = 0 + (NUM_BTNS * yOffset - BTN_Y_GAP) / 2;
    int btnY = btnYStart;

    for (auto& [b, scale] : btns) {
        AEVec2 pos = wtos(0, static_cast<float>(btnY));
        if (CollisionChecker::isMouseInRect(pos.x, pos.y, BTN_WIDTH * scale, BTN_HEIGHT * scale, static_cast<float>(mouseX), static_cast<float>(mouseY))) {
            scale = HOVER_BTN_SCALE;
            if (AEInputCheckTriggered(AEVK_LBUTTON)) {
                if (b == "RESTART" || b == "EXIT" || b == "HOWTOPLAY") {
                    if (!isConfirming) {
                        isConfirming = true;
                        confirmingButton = b;
                        confirmTimer = 0.0f;
                    }
                    else if (confirmingButton == b) {
                        if (b == "RESTART") {
                            // !TODO: call state or get scenelevelbuilder to restart combatscene
                            //SceneManager::GetInstance()->SetActiveScene("SceneMenu");
                            isPaused = false;
                            SceneManager::GetInstance()->restartScene();
                        }
                        else if (b == "EXIT") {
                            isPaused = false;
                            AEGfxSetCamPosition(0, 0);
                            SceneManager::GetInstance()->SetActiveScene("SceneMenu");
                        }
                        else if (b == "HOWTOPLAY") {
                            SceneManager::GetInstance()->SetActiveScene("HowToPlay");
                        }
                        isConfirming = false;
                    }
                }
                else {
                    if (b == "RESUME") {
                        isPaused = false;
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
    if (isConfirming) {
        confirmTimer += dt;
        if (confirmTimer >= 3.0f) {
            isConfirming = false;
        }
    }
}

void Pause::render() const {
    if (!isPaused) {
        return;
    }

    updateGlobals();

    RenderHelper::getInstance()->rect("invis", 0, 0, 9999.f, 9999.f, 0, Color{ 0,0,0,0.7f }, 0.7f);

    constexpr int btnX = 0;
    constexpr int yOffset = BTN_HEIGHT + BTN_Y_GAP;
    constexpr int btnYStart = 0 + (NUM_BTNS * yOffset - BTN_Y_GAP) / 2;
    int btnY = btnYStart;

    for (const auto& [b, scale] : btns) {
        if (isConfirming && confirmingButton == b) {
            RenderHelper::getInstance()->texture("button", btnX + camOffset.x, btnY + camOffset.y, scale * BTN_WIDTH, scale * BTN_HEIGHT);
            RenderHelper::getInstance()->text("Confirm ?", AEGfxGetWindowWidth() / 2.f, -btnY + AEGfxGetWindowHeight() / 2.f);
        }
        else {
            RenderHelper::getInstance()->texture("button", btnX + camOffset.x, btnY + camOffset.y, scale * BTN_WIDTH, scale * BTN_HEIGHT);
            RenderHelper::getInstance()->text(b, AEGfxGetWindowWidth() / 2.f, -btnY + AEGfxGetWindowHeight() / 2.f);
        }
        btnY -= yOffset;
    }
}