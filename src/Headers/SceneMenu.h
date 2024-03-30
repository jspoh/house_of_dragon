#pragma once

#ifndef SCENE_MENU_H
#define SCENE_MENU_H

#include "Scene.h"
#include "SceneManager.h"
#include "Pch.h"

#define ScreenTransitionTimer 3.0f

class SceneManager;

class SceneMenu : public Scene {
public:
    SceneMenu();
    SceneMenu(SceneManager* _sceneMgr);
    ~SceneMenu();

    virtual void Load();
    virtual void Init();
    virtual void Update(double dt);
    virtual void Render();
    virtual void Exit();

    typedef struct {
        s8 fontID;
        std::string bg;
        std::string bg1;
        std::string pointer;
        std::string title;
        std::string transitionBG;
        float buttonX[5];
        float buttonY[5];
        std::string button[5];
        std::string buttonSelect[5];
        s32 mouseX, mouseY;
        float buttonWidth, buttonHeight;
        bool hovering[5];
        bool levelSelecting;
        int levelActived[5];
        bool transitionEnd;
        float transitionTimer;
        float transitionElapse;
        int nextLevel;

        // Stage selection button variables
        float buttonSelectWidth, buttonSelectHeight;
        float buttonSelectX[5];
        float buttonSelectY[5];
        bool hoveringSelect[5];
    } Menu;

private:
    Menu myMenu{ 0 };

    static SceneMenu* sInstance;
};

#endif