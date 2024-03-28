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
        AEGfxTexture* bg;
        AEGfxTexture* pointer;
        AEGfxTexture* title;
        AEGfxTexture* transitionBG;
        AEGfxVertexList* mesh;
        s8 fontID;
        float buttonX[4];
        float buttonY[4];
        AEGfxTexture* button[4];
        AEGfxTexture* buttonSelect[3];
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