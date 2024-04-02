#pragma once

#ifndef SCENE_SLIDESHOW_H
#define SCENE_SLIDESHOW_H

#include "Scene.h"
#include "SceneManager.h"
#include "Pch.h"

class SceneManager;

class HowToPlay : public Scene {
public:
    HowToPlay();
    HowToPlay(SceneManager* _sceneMgr);
    ~HowToPlay();

    virtual void Load();
    virtual void Init();
    virtual void Update(double dt);
    virtual void Render();
    virtual void Exit();

    typedef struct {
        s8 fontID;
        std::string background;
        std::string leftButton;
        std::string rightButton;
        std::string backButton;
        std::string images[8];
        s32 cursorX, cursorY;
        float leftButtonX, leftButtonY;
        float rightButtonX, rightButtonY;
        float backButtonX, backButtonY;
        float buttonWidth, buttonHeight;
        bool hoveringLeft, hoveringRight, hoveringBack;
        int currentImageIndex;
    } Slideshow;

private:
    Slideshow slideshow{ 0 };
    static HowToPlay* sInstance;
};

#endif