#pragma once

//#ifndef SCENE_MENU_H
//#define SCENE_MENU_H

#include "Scene.h"
#include "SceneManager.h"
#include "Pch.h"

//#define ScreenTransitionTimer 3.0f

class SceneManager;
class SceneSetting : public Scene {
public:
	SceneSetting(SceneManager* _sceneMgr);
	~SceneSetting();



	virtual void Load();
	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	//struct bar {
	//	AEVec2 pos;
	//	AEVec2 size;
	//};

	//typedef struct {
		//AEGfxTexture* bg;
		//AEGfxTexture* bar[2];
		//AEGfxTexture* barhover[2];


		//AEGfxVertexList* mesh;
		//s8 fontID;

		//float buttonX[4];
		//float buttonY[4];
		//AEGfxTexture* button[4];

		//s32 mouseX, mouseY;
		//float buttonWidth, buttonHeight;
		//bool hovering[4];
		//bool levelSelecting;
		//int levelActived[4];

		//bool transitionEnd;
		//float transitionTimer;
		//float transitionElapse;
		//int nextLevel;
	//}Setting;

	//void texture1(AEGfxTexture* texture, f32 scaleX, f32 scaleY, f32 rotation, f32 positionX, f32 positionY, AEGfxVertexList* mesh, f32 transparency);


private:

	//Setting mySetting{ 0 };

	//bar barsound{ 0 };

	//SceneMenu(SceneManager* _sceneMgr); // This is used to register to SceneManager

	static SceneSetting* sInstance; // The pointer to the object that gets registered

	AEVec2 soundBarPos{ 170, -2 };
	const AEVec2 soundBarScale{ 600, 15 };
	AEVec2 musicBarPos{ 170, -95 };
	const Color soundBarColor{ 1, 0.82f, 0.62f, 1.f };

	const AEVec2 sliderScale{ 20, 30 };
	AEVec2 soundSliderPos{ 0, -2 };
	AEVec2 musicSliderPos{ 0, -95 };

	float sfxVolume{};
	float musicVolume{};
};

//#endif