#pragma once

#include "../Backend/Pch.h"
#include "../GameObject/GameObjectManager.h"

#define SIZE_OF_FLOOR 11 //X Axis
#define NUM_OF_TILES 10 //Z Axis
#define TOP_MOVEMENT_SPEED 1.0f //TOP MOVEMENT SPEED (Don't alter more than 2)

class SceneLevelBuilder
{
public:
	SceneLevelBuilder();
	~SceneLevelBuilder();

	void Update(double dt);
	void Render();
private:
	//Init relevant values like floor translation
	void Init();
	void Exit();

	struct v_FloorData
	{
		////////////////////////////////////////////////////////////////////////
		/*
		There are two different transforms that each floor tile stores.
		- m_TransformFloorData -> the Original Floor Tile transform data(example: Floor[6][8] transform data)
		- m_TransformFloorCurr -> the Current Floor Tile transform data that is incremented
		                        to the Original Floor Tile transform data

		And the other variables does the following:
		- m_currFloorNum -> determines the current tile number that it is on.
		- m_currFloorSpeed -> determines the value to increment for each value in the transform matrix.
		*/
		////////////////////////////////////////////////////////////////////////
		AEMtx33 m_TransformFloorData;
		AEMtx33 m_TransformFloorCurr;
		int m_currFloorNum = 0;
		int m_FloorNum = 0;
		AEMtx33 m_currFloorSpeed = { 0 };

		////////////////////////////////////////////////////////////////////////
        /*
        Determines the time taken before switching tiles.
		Pls don't alter.
		A more efficient check than consistently checking AABB/checking distance.
        */
        ////////////////////////////////////////////////////////////////////////
		double m_currFloorTimer = 0;
		double m_FloorSpeedTimer = 0.5;

		//And this is basically Render or Not
		bool m_IsRender = true;
		AEMtx33 m_Scale, m_Trans, m_OriginalTrans;
	};

	//////////////////////////////////////////////////////////////////////////////////////////////
    //                       *DO NOT CONFUSE WITH GAMEOBJECTS*
    /*
    		   Scene Objects are simpler objects contrary to Game Objects.
    			 They are only meant to be used in rendering in scenes.
    			   Most, if not all, do not have much to be updated.
    			  There are meant for faster loading and generation
    			  during switching of scenes for seamless transition.
    
    		GameObjects would still be used for combat and more complex entities.
    */
    
    ////////////////////////////////////////////////////////////////////////////////////////////////
	struct SceneObject
	{
		SceneObject();
		AEMtx33 m_TransformData;
		AEMtx33 m_Scale, m_Trans;
		string m_TexRef; //Used with draw
	};

	AEGfxVertexList* pMesh;

	AEGfxTexture* pFloorTex;
	AEGfxTexture* pSideLeftFloorTex;
	AEGfxTexture* pSideRightFloorTex;
	AEGfxTexture* pSkyTex;
	AEGfxTexture* pSunOverlayTex;
	AEGfxTexture* pFogTex;

	AEGfxTexture* pEnemyTex;

	v_FloorData** m_Floor;
	std::list<SceneObject>** m_FloorOBJs;
	//v_WallData** m_Wall;
	AEMtx33 m_TransformSkyData;
	AEMtx33 m_TransformSunData;
	AEMtx33 m_TransformSunOverlayData;
	AEMtx33 m_TransformFogData;

	bool m_StopMovement;
	bool m_PanCloseToGround;
	/*bool m_PanLeft;
	bool m_PanRight;*/

	int CurrentTileNumFurthest;
};