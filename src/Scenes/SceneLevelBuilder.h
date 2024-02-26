#pragma once

#include "../Backend/Pch.h"

#define SIZE_OF_FLOOR 9 //X Axis
#define NUM_OF_TILES 10 //Z Axis
class SceneLevelBuilder
{
public:
	SceneLevelBuilder();
	~SceneLevelBuilder();

	void Update(double dt);
	void Render();
private:
	//Init relevant values like floor translation
	void Init(); //DONT NEED TO CALL
	void Exit(); //DONT NEED TO CALL

	struct v_FloorData
	{
		AEMtx33 m_TransformFloorData;
		AEMtx33 m_TransformFloorCurr;
		int m_currFloorNum = 0;
		double m_currFloorTimer = 0;
		double m_FloorSpeedTimer = 0.5;
		AEMtx33 m_currFloorSpeed = { 0 };
		bool m_IsRender = true;
	};

	AEGfxVertexList* pMesh;

	AEGfxTexture* pFloorTex;
	AEGfxTexture* pSideLeftFloorTex;
	AEGfxTexture* pSideRightFloorTex;
	AEGfxTexture* pSkyTex;
	AEGfxTexture* pSunOverlayTex;
	AEGfxTexture* pFogTex;

	v_FloorData** m_Floor;
	AEMtx33 m_TransformSkyData;
	AEMtx33 m_TransformSunData;
	AEMtx33 m_TransformSunOverlayData;
	AEMtx33 m_TransformFogData;
};