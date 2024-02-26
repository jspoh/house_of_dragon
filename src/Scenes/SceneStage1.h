#ifndef SCENE_STAGE1_H
#define SCENE_STAGE1_H

#include "Scene.h"

#include "../Backend/Pch.h"
#include "SceneManager.h"
#include "SceneLevelBuilder.h"

//Remove upon test finish
#include <iostream> 
#include "../utils/utils.h"
#include "../Event/Event.h"
#include <vector>
#include <unordered_map>

class SceneManager;
class SceneStage1 : public Scene
{
public:
	SceneStage1();
	~SceneStage1();

	virtual void Load();
	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

private:
	SceneStage1(SceneManager* _sceneMgr); // This is used to register to SceneManager

	static SceneStage1* sInstance; // The pointer to the object that gets registered

	SceneLevelBuilder* m_LevelBuilder;
	//AEGfxVertexList* pMesh = 0;

	//AEGfxTexture* pFloorTex;
	//AEGfxTexture* pSideLeftFloorTex;
	//AEGfxTexture* pSideRightFloorTex;
	//AEGfxTexture* pSkyTex;
	//AEGfxTexture* pSunOverlayTex;
	//AEGfxTexture* pFogTex;


	//struct Floor
	//{
	//	AEMtx33 m_TransformFloorData;
	//	AEMtx33 m_TransformFloorCurr;
	//	int m_currFloorNum = 0;
	//	double m_currFloorTimer = 0;
	//	double m_FloorSpeedTimer = 0.5;
	//	AEMtx33 m_currFloorSpeed = { 0 };
	//	bool m_IsRender = true;
	//};
	//Floor m_Floor[10];
	//Floor m_RightSideFloor[10], m_RightSecondSideFloor[10], m_RightThirdSideFloor[10], m_RightFourthSideFloor[10];
	//Floor m_LeftSideFloor[10], m_LeftSecondSideFloor[10], m_LeftThirdSideFloor[10], m_LeftFourthSideFloor[10];
	//AEMtx33 m_TransformSkyData, m_TransformSunData, m_TransformSunOverlayData, m_TransformFogData;
};

#endif