#ifndef SCENE_MENU_H
#define SCENE_MENU_H

#include "Scene.h"
#include "SceneManager.h"
#include "Pch.h"

#define ScreenTransitionTimer 3.0f

class SceneManager;
class SceneMenu : public Scene
{
public:
	SceneMenu();
	~SceneMenu();

	virtual void Load();
	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();


	typedef struct {
		AEGfxTexture* bg;
		AEGfxTexture* title;
		AEGfxTexture* transitionBG;
		AEGfxVertexList* mesh;
		s8 fontID;

		float buttonX[4];
		float buttonY[4];
		AEGfxTexture* button[4];

		s32 mouseX, mouseY;
		float buttonWidth, buttonHeight;
		bool hovering[4];
		bool levelSelecting;
		int levelActived[4];

		bool transitionEnd;
		float transitionTimer;
		float transitionElapse;
		int nextLevel;
	}Menu;



private:

	Menu myMenu;

	SceneMenu(SceneManager* _sceneMgr); // This is used to register to SceneManager

	static SceneMenu* sInstance; // The pointer to the object that gets registered
};

#endif