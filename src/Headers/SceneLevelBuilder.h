#pragma once

#include "Pch.h"
#include "GameObjectManager.h"

#define SIZE_OF_FLOOR 11 //X Axis
#define NUM_OF_TILES 10 //Z Axis
#define NUM_OF_TILESPAWNPOINTS 4//Num of Spawnpoints on x & y of each tile
#define TOP_MOVEMENT_SPEED 1.0f //TOP MOVEMENT SPEED (Don't alter more than 2)
#define MAX_NUM_SCENEOBJS_TILE 8

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

	void CreateRowOBJs(int t_tileNum);
	void DestroyRowOBJs(int t_tileNum);

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

	struct v_TileSpawnPoint
	{
		double m_X;
		double m_Y;
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
	enum v_SceneObjectTypes
	{
		EType_MysteryEnemyStrong,
		EType_MysteryEnemyWeak,

		ETYPE_NONE,
		EType_ForestTree_XL_1,
		EType_ForestTree_XL_2,
		EType_ForestTree_L_1,
		EType_ForestTree_L_2,
		EType_ForestTree_M_1,
		EType_ForestTree_M_2,
		EType_ForestTree_S_1,
		EType_ForestTree_S_2,
		EType_NightTree_XL_1,
		EType_NightTree_XL_2,
		EType_NightTree_L_1,
		EType_NightTree_L_2,
		EType_NightTree_M_1,
		EType_NightTree_M_2,
		EType_NightTree_S_1,
		EType_NightTree_S_2,
		EType_Grass_Foliage_1,
		EType_Grass_Foliage_2,
		EType_Grass_Foliage_3,
		EType_Grass_1,
		EType_Grass_2,
		EType_Grass_3,
		EType_Rock_BROWN_1,
		EType_Rock_BROWN_2,
		EType_Rock_BROWN_3,
		EType_Rock_BROWN_4,
		EType_Rock_BROWN_5,
		EType_Rock_BROWN_6,
		EType_Rock_WHITE_1,
		EType_Rock_WHITE_2,
		EType_Rock_WHITE_3,
		EType_Rock_WHITE_4,
		EType_Rock_WHITE_5,
		EType_Rock_WHITE_6,
		ETYPE_LAST,

		
	};

	struct v_SceneObject
	{
		v_SceneObject();
		AEMtx33 m_TransformData;
		AEMtx33 m_Scale, m_Trans;
		int m_RenderOrder;
		float m_Transparency;
		v_SceneObjectTypes m_Type;
		string m_TexRef; //Used with draw
	};

	//////////////////////////////////////////////////////////////////////////////////////////////
	//                       Scene Level Data (Read from Json)
	/*
	* 
	*                      DESCRIBE YOUR SHIT LAH
	* 
	*/
	////////////////////////////////////////////////////////////////////////////////////////////////
	struct v_SceneLevelData
	{
		v_SceneLevelData();
		string m_LevelName;
		bool m_Completed;
		int m_MaxEnemies;
		std::list<string> m_EnemyTypes;
		std::list<int> m_EnemySpawnWeight;
		std::list<string> m_SceneObjTypes;
		std::list<int> m_SceneObjSpawnWeight;

		bool m_DayTime;
	};					  

	//Change soon
	AEGfxTexture* pFloorTex;
	AEGfxTexture* pSideLeftFloorTex;
	AEGfxTexture* pSideRightFloorTex;
	AEGfxTexture* pSkyTex;
	AEGfxTexture* pSunOverlayTex;
	AEGfxTexture* pFogTex;

	AEGfxTexture* pEnemyTex;

	v_FloorData** m_Floor;
	v_TileSpawnPoint** m_tileSP; //0 
	std::list<v_SceneObject>** m_FloorOBJs;

	std::list<v_SceneLevelData> m_SceneLevelDataList;

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