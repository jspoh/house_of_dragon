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
        std::string pointer;
        std::string title;
        std::string transitionBG;
        float buttonX[4];
        float buttonY[4];
        std::string button[4];
        std::string buttonSelect[3];
        s32 mouseX, mouseY;
        float buttonWidth, buttonHeight;
        bool hovering[4];
        bool levelSelecting;
        int levelActived[4];
        bool transitionEnd;
        float transitionTimer;
        float transitionElapse;
        int nextLevel;

        // Stage selection button variables
        float buttonSelectX[3];
        float buttonSelectY[3];
        bool hoveringSelect[3];
    } Menu;

private:
    Menu myMenu{ 0 };

    static SceneMenu* sInstance;
};

#endif