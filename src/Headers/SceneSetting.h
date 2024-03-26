//#ifndef SCENE_MENU_H
//#define SCENE_MENU_H

#include "Scene.h"
#include "SceneManager.h"
#include "Pch.h"

//#define ScreenTransitionTimer 3.0f

class SceneManager;
class SceneSetting : public Scene {
public:
	SceneSetting();
	SceneSetting(SceneManager* _sceneMgr);
	~SceneSetting();



	virtual void Load();
	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();


	typedef struct {
		AEGfxTexture* bg;
		AEGfxTexture* bar[2];
		AEGfxTexture* barhover[2];


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
	}Setting;

	void texture1(AEGfxTexture* texture, f32 scaleX, f32 scaleY, f32 rotation, f32 positionX, f32 positionY, AEGfxVertexList* mesh, f32 transparency);


private:

	Setting mySetting{ 0 };

	//SceneMenu(SceneManager* _sceneMgr); // This is used to register to SceneManager

	static SceneSetting* sInstance; // The pointer to the object that gets registered
};

//#endif