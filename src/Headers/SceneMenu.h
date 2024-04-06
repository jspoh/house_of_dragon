/* Start Header ************************************************************************/
/*!
\file SceneMenu.h
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

#pragma once

#ifndef SCENE_MENU_H
#define SCENE_MENU_H

#include "Scene.h"
#include "SceneManager.h"
#include "Pch.h"

#define ScreenTransitionTimer 3.0f

class SceneManager;

/**
 * @class SceneMenu
 * @brief Handles the main menu scene of the game.
 *
 * Responsible for handling the display and interaction within the
 * main menu. It includes navigation, selection of options,
 * and other related menu functionalities.
 */
class SceneMenu : public Scene {
public:
    SceneMenu();
    SceneMenu(SceneManager* _sceneMgr);
    ~SceneMenu();

    std::string nextSceneName;

    virtual void load();
    virtual void init();
    virtual void update(double dt);
    virtual void render();
    virtual void exit();

    typedef struct {
        s8 fontID;
        std::string bg;
        std::string bg1;
        std::string bg2;
        std::string pointer;
        std::string title;
        std::string transitionBG;
        std::string confirm;
        float buttonX[5];
        float buttonY[5];
        std::string button[7];
        std::string buttonSelect[7];
        s32 mouseX, mouseY;
        float buttonWidth, buttonHeight;
        bool hovering[5];
        bool levelSelecting;
        int levelActived[5];
        bool transitionEnd;
        float transitionTimer;
        float transitionElapse;
        int nextLevel;
        float buttonScale[7]; // Add this line
        // Stage selection button variables
        float buttonSelectWidth, buttonSelectHeight;
        float buttonSelectX[7];
        float buttonSelectY[7];
        bool hoveringSelect[7];
        float buttonSelectScale[7]; // Add this line

        std::string back;
        float backButtonX, backButtonY;
        float backButtonWidth, backButtonHeight;
        bool hoveringBack;
        float cameraX, cameraY;
        float cameraSpeed;

        float daggerRotation;

        bool quitConfirm;
        float quitConfirmTimer;

    } Menu;

    int m_SelectedLevel;
    static SceneMenu* m_sInstance;
private:
    Menu myMenu{ 0 };

};

#endif