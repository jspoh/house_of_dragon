/* Start Header ************************************************************************/
/*!
\file SceneLevelBuilder.h
\author Soh Wei Jie, weijie.soh, 2301289
\par weijie.soh\@digipen.edu
\date 21 Feb 2024
\brief Handles the update and rendering of the levels

/*
Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#pragma once

#include "Pch.h"
#include "GameObjectManager.h"
#include "ParticleManager.h"
#include "CombatScene.h"
#include "SceneMenu.h"//For getting selected level

constexpr int SIZE_OF_FLOOR = 11;			//X Axis
constexpr int NUM_OF_TILES = 10 ;			//Z Axis
constexpr int NUM_OF_TILESPAWNPOINTS = 4;	//Num of Spawnpoints on x & y of each tile
constexpr float TOP_MOVEMENT_SPEED = 1.0f ;	//TOP MOVEMENT SPEED (Don't alter more than 2)
constexpr int MAX_NUM_SCENEOBJS_TILE = 8;
constexpr double MAX_LVLNAMETIMER = 4.0;
constexpr float LERPING_SPEED = 10.0f;
constexpr double TRY_TO_SPAWN_ENEMY_TIMER = 2.0;

namespace GameScene {
	extern bool combatAudioLoopIsPlaying;
	extern bool afterInit;		// after loading screen
}

class SceneLevelBuilder
{
public:
	SceneLevelBuilder();
	~SceneLevelBuilder();

	void update(double dt);
	void render();
private:
	void init(); //Init relevant values like floor translation
	void exit();

	void CreateRowOBJs(int t_tileNum);
	void DestroyRowOBJs(int t_tileNum);

	void SpawnLvlName();
	void UpdateLvlName(f32 t_dt);
	void RenderLvlName();

	void UpdateScreenTransition(f32 t_dt);
	void FadeINBlack(); //Call when u want fade into black
	void FadeOutBlack(); //Call when u want fade out from black

	void UpdateLevelGameplay(f32 t_dt);
	void UpdateLensFlare(f32 t_dt);
	void UpdateClouds(f32 t_dt);
	void UpdateBackdrop(f32 t_dt);

	std::vector<std::string> GenerateEnemyToSpawn();


	struct v_FloorData
	{
		v_FloorData();
		////////////////////////////////////////////////////////////////////////
		/*
		There are two different transforms that each floor tile stores.
		- m_transformFloorData -> the Original Floor Tile transform data(example: Floor[6][8] transform data)
		- m_transformFloorCurr -> the Current Floor Tile transform data that is incremented
		                        to the Original Floor Tile transform data

		And the other variables does the following:
		- m_currFloorNum -> determines the current tile number that it is on.
		- m_currFloorSpeed -> determines the value to increment for each value in the transform matrix.
		*/
		////////////////////////////////////////////////////////////////////////
		AEMtx33 m_transformFloorData;
		AEMtx33 m_transformFloorCurr;
		int m_currFloorNum;
		int m_floorNum;
		AEMtx33 m_currFloorSpeed;

		////////////////////////////////////////////////////////////////////////
        /*
        Determines the time taken before switching tiles.
		Pls don't alter.
		A more efficient check than consistently checking AABB/checking distance.
        */
        ////////////////////////////////////////////////////////////////////////
		f32 m_currFloorTimer;
		f32 m_floorSpeedTimer;

		//And this is basically Render or Not
		bool m_isRender;
		AEMtx33 m_scale, m_trans, m_originalTrans;
		int m_type; //0 for grass, 1 for rocky
	};

	struct v_TileSpawnPoint
	{
		f32 m_X;
		f32 m_Y;
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
		FIRST_TYPE,
		///////////////////////////////////////////////////////////////////////////////////////////////
		TYPE_Grass, //To define objs that are grass
		EType_Grass_Foliage_1,
		EType_Grass_Foliage_2,
		EType_Grass_Foliage_3,
		EType_Grass_1,
		EType_Grass_2,
		EType_Grass_3,
		TYPE_End_Grass, //To define the end of objs that are grass
		TYPE_Rock, //To define objs that are grass
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
		TYPE_End_Rock, //To define the end of objs that are grass
		///////////////////////////////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////////////////////////
		TYPE_Tree, // To define objs to be rand spawned on tile
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
		TYPE_End_Tree, // To define the end of objs to be rand spawned on tile
		///////////////////////////////////////////////////////////////////////////////////////////////
		LAST_TYPE,

		
	};

	struct v_SceneObject
	{
		v_SceneObject();
		AEMtx33 m_TransformData;
		AEMtx33 m_scale, m_trans;
		int m_RenderOrder;
		float m_Transparency;
		v_SceneObjectTypes m_type;
		std::string m_TexRef; //Used with draw
		bool m_tobeCentered;
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
		std::string m_LevelName;
		double m_LevelCompletionRate;
		bool m_Completed;
		bool m_Unlocked;
		int m_MaxEnemies;
		int m_EnemySpawnRate;
		std::vector<std::string> m_EnemyTypes;
		std::vector<int> m_EnemySpawnWeight;
		std::vector<std::string> m_SceneObjTypes;
		std::vector<int> m_SceneObjSpawnWeight;

		bool m_DayTime;
	};					  

	v_FloorData** m_floor;
	v_TileSpawnPoint** m_tileSP; //0 
	std::list<v_SceneObject>** m_floorOBJs;
	int t_centerFloorNum;

	v_SceneLevelData* m_sceneLevelDataList;
	double m_completionStatus; //0 - 100%
	int m_currLevel;
	int m_maxLevel;

	Color m_lighting;

	///////////////////////////////////////////////////////
	//LEVEL NAME DISPLAY
	f32 m_lvlNameTimer;
	f32 m_lvlNameTransparency;
	s8 m_pTextFont;
	
	///////////////////////////////////////////////////////
	//Screen Transition
	f32 m_currTransitionTransparency;
	f32 m_setTransitionTransparency;

	///////////////////////////////////////////////////////
	//TRANSFORM DATA
	AEMtx33 m_transformSkyData;
	std::vector<AEMtx33> m_transformCloudsData;
	std::vector<AEMtx33> m_transformBackDrops1Data;
	std::vector<AEMtx33> m_transformBackDrops2Data;
	std::vector<AEMtx33> m_transformBackDrops3Data;
	AEMtx33 m_transformSunData;
	AEMtx33 m_transformSunOverlayData;
	std::vector<AEMtx33> m_transformSunLensData;
	AEVec2 m_sunPos, m_sunOverlayScale;
	AEMtx33 m_transformFogData;

	///////////////////////////////////////////////////////
	//Combat 
	bool m_stopMovement;
	bool m_panCloseToGround;
	int m_panCloseToGroundValue = 80;
	int m_currentTileNumFurthest;
	bool m_combatPhase;
	bool m_combatAnimationComp;
	double m_combatBufferingTime;
	GameObject_Misc_Enemy* m_sceneEnemy; // Just as a reference to easily start the combat
	std::vector<std::string> m_combatNames;
};