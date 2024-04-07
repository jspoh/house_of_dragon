/* Start Header ************************************************************************/
/*!
\file SceneLevelBuilder.cpp
\author Soh Wei Jie, weijie.soh, 2301289 (90%, 3D rendering)
\par weijie.soh\@digipen.edu
\author Poh Jing Seng, jingseng.poh, 2301363 (10%, integration, bugfixes)
\par jingseng.poh\@digipen.edu
\date 21 Feb 2024
\brief The 3D engine, the heart of the game and my pain. Read the comments I have written
	   for indepth description of the files.

/*
Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#include "Pch.h"
#include "SceneLevelBuilder.h"
#include "CombatPlayer.h"
#include "Pause.h"
#include "SceneStages.h"

// DONT MANUALLY FREE THIS POINTER
std::unique_ptr<Player> player = nullptr;

// states for audio
bool GameScene::combatAudioLoopIsPlaying = false;
bool GameScene::afterInit = false;

////////////////////////////////////////////////////////////////////////////////////////
/*
Global Variables Init
*/
namespace {
	double LerpSpeed = 10.0;
	constexpr double FLEEING_COMPLETION_PENALTY = 20.0;		// percentage penatly for completion status when fleeing fight [0,100]

	int PLAYER_BASE_HEALTH = static_cast<int>(Database::getInstance().data["player"]["baseHealth"]);
	int PLAYER_BASE_DAMAGE = static_cast<int>(Database::getInstance().data["player"]["baseDamage"]);

	// show game end screen
	bool showGameEnd = false;

	float elapsedTime = 0;
	constexpr float SHOW_GAME_END_DURATION = 3.f;

	// cleanup misc states
	void cleanupSlb() {
		showGameEnd = false;
		GameScene::combatAudioLoopIsPlaying = false;
		SceneStagesAudio::loopIsPlaying = true;
		elapsedTime = 0;
	}
}
////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////
/*
Struct Constructor Area
*/
SceneLevelBuilder::v_FloorData::v_FloorData() :
	m_currFloorNum{ 0 },
	m_floorNum{ 0 },
	m_currFloorTimer{ 0.0 },
	m_floorSpeedTimer{ 0.5 },
	m_isRender{ true },
	m_type{ 0 }
{
	AEMtx33Identity(&m_transformFloorData);
	AEMtx33Identity(&m_transformFloorCurr);
	AEMtx33Identity(&m_currFloorSpeed);
	AEMtx33Identity(&m_scale);
	AEMtx33Identity(&m_trans);
	AEMtx33Identity(&m_originalTrans);
}
SceneLevelBuilder::v_SceneObject::v_SceneObject()
	:m_TexRef{ "" },
	m_RenderOrder{ 0 },
	m_Transparency{ -1.5f },
	m_type{ v_SceneObjectTypes::EType_Grass_Foliage_1 },
	m_tobeCentered{ false }
{
	AEMtx33Identity(&m_TransformData);
	AEMtx33Identity(&m_scale);
	AEMtx33Identity(&m_trans);
}
SceneLevelBuilder::v_SceneLevelData::v_SceneLevelData()
	:m_LevelName{ "" },
	m_Completed{ false },
	m_MaxEnemies{ 0 },
	m_DayTime{ 0 },
	m_EnemyTypes{},
	m_EnemySpawnWeight{},
	m_SceneObjTypes{},
	m_SceneObjSpawnWeight{},
	m_LevelCompletionRate{ 0.0 },
	m_Unlocked{ false },
	m_EnemySpawnRate{ 0 } {}
////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/***************************************************************************************************************************************
		  _____             _____                    _____                    _____             _____
		 /\    \           /\    \                  /\    \                  /\    \           /\    \
		/::\    \         /::\    \                /::\    \                /::\    \         /::\    \
	   /::::\    \        \:::\    \              /::::\    \              /::::\    \        \:::\    \
	  /::::::\    \        \:::\    \            /::::::\    \            /::::::\    \        \:::\    \
	 /:::/\:::\    \        \:::\    \          /:::/\:::\    \          /:::/\:::\    \        \:::\    \
	/:::/__\:::\    \        \:::\    \        /:::/__\:::\    \        /:::/__\:::\    \        \:::\    \
	\:::\   \:::\    \       /::::\    \      /::::\   \:::\    \      /::::\   \:::\    \       /::::\    \
  ___\:::\   \:::\    \     /::::::\    \    /::::::\   \:::\    \    /::::::\   \:::\    \     /::::::\    \
 /\   \:::\   \:::\    \   /:::/\:::\    \  /:::/\:::\   \:::\    \  /:::/\:::\   \:::\____\   /:::/\:::\    \
/::\   \:::\   \:::\____\ /:::/  \:::\____\/:::/  \:::\   \:::\____\/:::/  \:::\   \:::|    | /:::/  \:::\____\
\:::\   \:::\   \::/    //:::/    \::/    /\::/    \:::\  /:::/    /\::/   |::::\  /:::|____|/:::/    \::/    /
 \:::\   \:::\   \/____//:::/    / \/____/  \/____/ \:::\/:::/    /  \/____|:::::\/:::/    //:::/    / \/____/
  \:::\   \:::\    \   /:::/    /                    \::::::/    /         |:::::::::/    //:::/    /
   \:::\   \:::\____\ /:::/    /                      \::::/    /          |::|\::::/    //:::/    /
	\:::\  /:::/    / \::/    /                       /:::/    /           |::| \::/____/ \::/    /
	 \:::\/:::/    /   \/____/                       /:::/    /            |::|  ~|        \/____/
	  \::::::/    /                                 /:::/    /             |::|   |
	   \::::/    /                                 /:::/    /              \::|   |
		\::/    /                                  \::/    /                \:|   |
		 \/____/                                    \/____/                  \|___|

***************************************************************************************************************************************/
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
SceneLevelBuilder::SceneLevelBuilder() :
	m_stopMovement{ false },
	m_panCloseToGround{ false },
	m_completionStatus{ 98 },
	m_currLevel{ 0 },
	m_lvlNameTimer{ 0.0 },
	m_lvlNameTransparency{ 0.0 },
	m_currTransitionTransparency{ 1.0 },
	m_setTransitionTransparency{ -1.0 },
	m_sceneEnemy{ nullptr },
	m_combatPhase{ false },
	m_combatAnimationComp{ false },
	m_combatBufferingTime{ 0.0 },
	m_lighting{ 1.0f,1.0f,1.0f,1.0f }
{
	//////////////////////////////////////////////////////////////////////////////////////////////
	//                       Loading of ALL Scene Textures
	/*
	*     All assets in scene are to be loaded here to allow loading screen to do its magic.
	*/
	//////////////////////////////////////////////////////////////////////////////////////////////
	{
		/*********************************************
		//Level Header
		**********************************************/
		RenderHelper::getInstance()->registerTexture("LVL_HEADER", "Assets/SceneObjects/LvlHeader.png");
		m_pTextFont = AEGfxCreateFont("Assets/Fonts/TokyoMidnight.otf", 50);

		/*********************************************
		//Floor
		**********************************************/
		RenderHelper::getInstance()->registerTexture("FLOOR_LEFT_1", "Assets/SceneObjects/FLOOR/Scene_FloorSideLeft_Grass1.png");
		RenderHelper::getInstance()->registerTexture("FLOOR_CENTER_1", "Assets/SceneObjects/FLOOR/Scene_Floor_Path1.png");
		RenderHelper::getInstance()->registerTexture("FLOOR_RIGHT_1", "Assets/SceneObjects/FLOOR/Scene_FloorSideRight_Grass1.png");
		RenderHelper::getInstance()->registerTexture("FLOOR_LEFT_2", "Assets/SceneObjects/FLOOR/Scene_FloorSideLeft_Rocky1.png");
		RenderHelper::getInstance()->registerTexture("FLOOR_CENTER_2", "Assets/SceneObjects/FLOOR/Scene_Floor_Path2.png");
		RenderHelper::getInstance()->registerTexture("FLOOR_RIGHT_2", "Assets/SceneObjects/FLOOR/Scene_FloorSideRight_Rocky1.png");

		/*********************************************
		//Sky
		**********************************************/
		RenderHelper::getInstance()->registerTexture("SKY_TEST_1", "Assets/SceneObjects/SKY/1.png");
		RenderHelper::getInstance()->registerTexture("SKY_TEST_2", "Assets/SceneObjects/SKY/2.png");
		RenderHelper::getInstance()->registerTexture("SKY_TEST_3", "Assets/SceneObjects/SKY/3.png");
		RenderHelper::getInstance()->registerTexture("SKY_TEST_4", "Assets/SceneObjects/SKY/4.png");
		RenderHelper::getInstance()->registerTexture("SUN_OVERLAY_1", "Assets/SceneObjects/SKY/Scene_Sun_Overlaylighting.png");

		RenderHelper::getInstance()->registerTexture("SUN_LENS_1", "Assets/SceneObjects/SKY/lense_ghost_3.png");
		RenderHelper::getInstance()->registerTexture("SUN_LENS_2", "Assets/SceneObjects/SKY/lense_ghost_1.png");
		RenderHelper::getInstance()->registerTexture("SUN_LENS_3", "Assets/SceneObjects/SKY/lense_ghost_2.png");
		RenderHelper::getInstance()->registerTexture("SUN_LENS_4", "Assets/SceneObjects/SKY/lense_chroma_ring.png");
		RenderHelper::getInstance()->registerTexture("SUN_LENS_5", "Assets/SceneObjects/SKY/lense_bokeh.png");

		/*********************************************
		//Fog
		**********************************************/
		RenderHelper::getInstance()->registerTexture("FOG_1", "Assets/SceneObjects/BACKGROUND/Scene_Fog_NEW_Color.png");
		RenderHelper::getInstance()->registerTexture("FOG_2", "Assets/SceneObjects/BACKGROUND/Scene_Fog_Color_MASSIVE.png");

		/*********************************************
		//BackDrop
		**********************************************/
		RenderHelper::getInstance()->registerTexture("BACKDROP_1", "Assets/SceneObjects/BACKGROUND/BackDrop_01.png");
		RenderHelper::getInstance()->registerTexture("BACKDROP_2", "Assets/SceneObjects/BACKGROUND/BackDrop_02.png");
		RenderHelper::getInstance()->registerTexture("BACKDROP_3", "Assets/SceneObjects/BACKGROUND/BackDrop_03.png");

		/*********************************************
		//SceneObjects (Ref NAME: SCENEOBJECT_*ENUMID*)
		**********************************************/
		///////////////////////////////////////////
		//Grass
		RenderHelper::getInstance()->registerTexture(EType_Grass_1, "Assets/SceneObjects/SCENE_OBJECTS/Grass_1.png");
		RenderHelper::getInstance()->registerTexture(EType_Grass_2, "Assets/SceneObjects/SCENE_OBJECTS/Grass_2.png");
		RenderHelper::getInstance()->registerTexture(EType_Grass_3, "Assets/SceneObjects/SCENE_OBJECTS/Grass_3.png");
		RenderHelper::getInstance()->registerTexture(EType_Grass_Foliage_1, "Assets/SceneObjects/SCENE_OBJECTS/Grass_Foliage1.png");
		RenderHelper::getInstance()->registerTexture(EType_Grass_Foliage_2, "Assets/SceneObjects/SCENE_OBJECTS/Grass_Foliage2.png");
		RenderHelper::getInstance()->registerTexture(EType_Grass_Foliage_3, "Assets/SceneObjects/SCENE_OBJECTS/Grass_Foliage3.png");

		//Rocks
		RenderHelper::getInstance()->registerTexture(EType_Rock_BROWN_1, "Assets/SceneObjects/SCENE_OBJECTS/RockPile1_BROWN.png");
		RenderHelper::getInstance()->registerTexture(EType_Rock_BROWN_2, "Assets/SceneObjects/SCENE_OBJECTS/RockPile2_BROWN.png");
		RenderHelper::getInstance()->registerTexture(EType_Rock_BROWN_3, "Assets/SceneObjects/SCENE_OBJECTS/RockPile3_BROWN.png");
		RenderHelper::getInstance()->registerTexture(EType_Rock_BROWN_4, "Assets/SceneObjects/SCENE_OBJECTS/RockPile4_BROWN.png");
		RenderHelper::getInstance()->registerTexture(EType_Rock_BROWN_5, "Assets/SceneObjects/SCENE_OBJECTS/RockPile5_BROWN.png");
		RenderHelper::getInstance()->registerTexture(EType_Rock_BROWN_6, "Assets/SceneObjects/SCENE_OBJECTS/RockPile6_BROWN.png");
		RenderHelper::getInstance()->registerTexture(EType_Rock_WHITE_1, "Assets/SceneObjects/SCENE_OBJECTS/RockPile1_WHITE.png");
		RenderHelper::getInstance()->registerTexture(EType_Rock_WHITE_2, "Assets/SceneObjects/SCENE_OBJECTS/RockPile2_WHITE.png");
		RenderHelper::getInstance()->registerTexture(EType_Rock_WHITE_3, "Assets/SceneObjects/SCENE_OBJECTS/RockPile3_WHITE.png");
		RenderHelper::getInstance()->registerTexture(EType_Rock_WHITE_4, "Assets/SceneObjects/SCENE_OBJECTS/RockPile4_WHITE.png");
		RenderHelper::getInstance()->registerTexture(EType_Rock_WHITE_5, "Assets/SceneObjects/SCENE_OBJECTS/RockPile5_WHITE.png");
		RenderHelper::getInstance()->registerTexture(EType_Rock_WHITE_6, "Assets/SceneObjects/SCENE_OBJECTS/RockPile6_WHITE.png");

		//Trees
		RenderHelper::getInstance()->registerTexture(EType_ForestTree_XL_1, "Assets/SceneObjects/SCENE_OBJECTS/GreenTreeXL.png");
		RenderHelper::getInstance()->registerTexture(EType_ForestTree_XL_2, "Assets/SceneObjects/SCENE_OBJECTS/GreenTreeXL_Dark.png");
		RenderHelper::getInstance()->registerTexture(EType_ForestTree_L_1, "Assets/SceneObjects/SCENE_OBJECTS/GreenTreeL.png");
		RenderHelper::getInstance()->registerTexture(EType_ForestTree_L_2, "Assets/SceneObjects/SCENE_OBJECTS/GreenTreeL_Dark.png");
		RenderHelper::getInstance()->registerTexture(EType_ForestTree_M_1, "Assets/SceneObjects/SCENE_OBJECTS/GreenTreeM.png");
		RenderHelper::getInstance()->registerTexture(EType_ForestTree_M_2, "Assets/SceneObjects/SCENE_OBJECTS/GreenTreeM_Dark.png");
		RenderHelper::getInstance()->registerTexture(EType_ForestTree_S_1, "Assets/SceneObjects/SCENE_OBJECTS/GreenTreeS.png");
		RenderHelper::getInstance()->registerTexture(EType_ForestTree_S_2, "Assets/SceneObjects/SCENE_OBJECTS/GreenTreeS_Dark.png");

		RenderHelper::getInstance()->registerTexture(EType_NightTree_XL_1, "Assets/SceneObjects/SCENE_OBJECTS/NightTreeXL.png");
		RenderHelper::getInstance()->registerTexture(EType_NightTree_XL_2, "Assets/SceneObjects/SCENE_OBJECTS/NightTreeXL_Dark.png");
		RenderHelper::getInstance()->registerTexture(EType_NightTree_L_1, "Assets/SceneObjects/SCENE_OBJECTS/NightTreeL.png");
		RenderHelper::getInstance()->registerTexture(EType_NightTree_L_2, "Assets/SceneObjects/SCENE_OBJECTS/NightTreeL_Dark.png");
		RenderHelper::getInstance()->registerTexture(EType_NightTree_M_1, "Assets/SceneObjects/SCENE_OBJECTS/NightTreeM.png");
		RenderHelper::getInstance()->registerTexture(EType_NightTree_M_2, "Assets/SceneObjects/SCENE_OBJECTS/NightTreeM_Dark.png");
		RenderHelper::getInstance()->registerTexture(EType_NightTree_S_1, "Assets/SceneObjects/SCENE_OBJECTS/NightTreeS.png");
		RenderHelper::getInstance()->registerTexture(EType_NightTree_S_2, "Assets/SceneObjects/SCENE_OBJECTS/NightTreeS_Dark.png");
		/*********************************************
		//GameObjects
		**********************************************/
		RenderHelper::getInstance()->registerTexture("MISC_ENEMY_STRONG", "Assets/SceneObjects/GAME_OBJECTS/Scene_Enemy_Strong.png");
		RenderHelper::getInstance()->registerTexture("MISC_ENEMY_WEAK", "Assets/SceneObjects/GAME_OBJECTS/Scene_Enemy_Weak.png");
		RenderHelper::getInstance()->registerTexture("MISC_ENEMYJAW_UPPER", "Assets/SceneObjects/GAME_OBJECTS/Scene_Enemy_UpperJaw.png");
		RenderHelper::getInstance()->registerTexture("MISC_ENEMYJAW_LOWER", "Assets/SceneObjects/GAME_OBJECTS/Scene_Enemy_LowerJaw.png");

		RenderHelper::getInstance()->registerTexture("FIREBALL", "Assets/Combat_Enemy/Projectiles/FireBall.png");
		RenderHelper::getInstance()->registerTexture("ENERGYBALL", "Assets/Combat_Enemy/Projectiles/EnergyBall.png");

	}

	//////////////////////////////////////////////////////////////////////////////////////////////
	//                       Allocate Memory to the variable pointers
	/*
	*     Allocate relevant memory depending on defined values (Check #define in .hpp)
	*/
	//////////////////////////////////////////////////////////////////////////////////////////////
	{
		m_floor = new v_FloorData * [SIZE_OF_FLOOR];
		m_floorOBJs = new std::list<v_SceneObject>*[SIZE_OF_FLOOR];
		for (int i = 0; i < SIZE_OF_FLOOR; i++)
		{
			m_floor[i] = new v_FloorData[NUM_OF_TILES];
			m_floorOBJs[i] = new std::list<v_SceneObject>[NUM_OF_TILES];
		}

		m_tileSP = new v_TileSpawnPoint * [NUM_OF_TILESPAWNPOINTS];
		for (int i = 0; i < NUM_OF_TILESPAWNPOINTS; i++)
		{
			m_tileSP[i] = new v_TileSpawnPoint[NUM_OF_TILESPAWNPOINTS];
			for (int j = 0; j < NUM_OF_TILESPAWNPOINTS; j++)
			{
				m_tileSP[i][j].m_X = (36.0f / NUM_OF_TILESPAWNPOINTS * static_cast<float>(j)) - 18.0f;
				m_tileSP[i][j].m_Y = (50.0f / NUM_OF_TILESPAWNPOINTS * static_cast<float>(i)) - 25.0f;
			}
		}
		//Set the Center floor num - the one the player is traversing on
		t_centerFloorNum = static_cast<int>(SIZE_OF_FLOOR / 2);
	}


	//////////////////////////////////////////////////////////////////////////////////////////////
	//                       load data from Json
	/*
	*     load All Level Data from Json, for instance, enemies to spawn and title name and objs
	*/
	//////////////////////////////////////////////////////////////////////////////////////////////
	{
		if (Database::getInstance().data["levels"].size() > 0)
		{
			m_sceneLevelDataList = new v_SceneLevelData[Database::getInstance().data["levels"].size()];
			m_maxLevel = static_cast<int>(Database::getInstance().data["levels"].size()) - 1;
			for (int i = 0; i < Database::getInstance().data["levels"].size(); i++)
			{
				v_SceneLevelData t_curr{};
				t_curr.m_LevelName = Database::getInstance().data["levels"][i]["levelName"];
				t_curr.m_LevelCompletionRate = Database::getInstance().data["levels"][i]["levelCompletionRate"];
				t_curr.m_Completed = Database::getInstance().data["levels"][i]["completed"];
				t_curr.m_EnemySpawnRate = Database::getInstance().data["levels"][i]["enemySpawnRate"];
				t_curr.m_Unlocked = Database::getInstance().data["levels"][i]["unlocked"];
				t_curr.m_MaxEnemies = Database::getInstance().data["levels"][i]["maxEnemies"];
				t_curr.m_DayTime = Database::getInstance().data["levels"][i]["DayTime"];

				for (auto& map : Database::getInstance().data["levels"][i]["enemySpawnWeight"].items())
				{
					// you cannot iterate like this! wouldve caused a crash!
					//for (auto type = map.value().begin(); type != map.value().end(); type++)
					for (const auto& [animal, spawnWeight] : map.value().items())
					{
						t_curr.m_EnemyTypes.push_back(animal);
						t_curr.m_EnemySpawnWeight.push_back(spawnWeight);
					}
				}

				for (auto& map : Database::getInstance().data["levels"][i]["SceneOBJSpawnWeight"].items())
				{
					for (auto type = map.value().begin(); type != map.value().end(); type++)
					{
						t_curr.m_SceneObjTypes.push_back(type.key());
						t_curr.m_SceneObjSpawnWeight.push_back(type.value());
					}
				}

				*m_sceneLevelDataList = t_curr;
				m_sceneLevelDataList++;
			}
			m_sceneLevelDataList -= Database::getInstance().data["levels"].size();
		}
	}


	//////////////////////////////////////////////////////////////////////////////////////////////
	//                       load Combat elements
	//////////////////////////////////////////////////////////////////////////////////////////////
	CombatScene::getInstance().load();
	SoundPlayer::CombatAudio::getInstance();

	//////////////////////////////////////////////////////////////////////////////////////////////
	//                       Initialise 3D engine & relevant values
	//////////////////////////////////////////////////////////////////////////////////////////////// 
	init();
}
SceneLevelBuilder::~SceneLevelBuilder()
{
	exit(); //Hehe, dont allow sneaky memory leak
}

void SceneLevelBuilder::init()
{
	// cleanup again just in case
	//cleanupSlb();

	/////////////////////////////////////////////////////////////
	// Basic Setup of all necessary objects in scene
	{
		if (!player) {
			player = std::make_unique<Player>(Player((float)PLAYER_BASE_HEALTH, (float)PLAYER_BASE_DAMAGE));
		}
		player->resetHealth();

		/////////////////////////////////////////////////////////////
		//Creating GameObjects
		Create::miscEnemy();
		for (int i = 0; i < 10; i++)
		{
			Create::projectiles();
		}

		m_completionStatus = 98;
		m_currLevel = SceneMenu::m_sInstance->m_SelectedLevel;
		m_lighting = { 1.0f,1.0f,1.0f,1.0f };
	}

	//////////////////////////////////////////////////////////////////////////////////////////////
	//                       GENERATION OF TRANSFORMATION DATA
	/*
	*         Here lies the beginning of the 3D engine initialization, starting with the outset of
	*         my sanity degradation \(-_-)/ and also the creation of all the relevant transformations
	*         for each tile, the tiles will then interpolate towards the player then return
	*         to the back of the list at the end of its cycle, resulting in the movement of the
	*         player.
	*
	*         This engine was inspired by the world shader in Animal Crossing on DS to have
	*         infinite scrolling by the player.
	*/
	////////////////////////////////////////////////////////////////////////////////////////////////
	AEMtx33 scale, trans;
	/////////////////////////////////////////////////////////////
	//MAIN FLOOR
	{
		for (int j = 0; j < SIZE_OF_FLOOR; j++)
		{
			for (int i = 0; i < NUM_OF_TILES; i++)
			{
				switch (i)
				{
					//Out of Screen Floor
				case 0:
					AEMtx33Scale(&m_floor[j][i].m_scale, 8000.f, 1262.f);
					AEMtx33Trans(&m_floor[j][i].m_trans, 16000.0f * static_cast<f32>(j - t_centerFloorNum), -2829.0f);
					break;
				case 1:
					AEMtx33Scale(&m_floor[j][i].m_scale, 7000.f, 1262.f);
					AEMtx33Trans(&m_floor[j][i].m_trans, 5750.0f * static_cast<f32>(j - t_centerFloorNum), -2229.0f);
					break;
				case 2:
					AEMtx33Scale(&m_floor[j][i].m_scale, 6000.f, 1262.f);
					AEMtx33Trans(&m_floor[j][i].m_trans, 4350.0f * static_cast<f32>(j - t_centerFloorNum), -1629.0f);
					break;
					//First Floor
				case 3:
					AEMtx33Scale(&m_floor[j][i].m_scale, 2940.f, 616.f);
					AEMtx33Trans(&m_floor[j][i].m_trans, 2150.0f * static_cast<f32>(j - t_centerFloorNum), -696.0f);
					break;
					//Second Floor
				case 4:
					AEMtx33Scale(&m_floor[j][i].m_scale, 1593.0f, 339.f);
					AEMtx33Trans(&m_floor[j][i].m_trans, 1150.0f * static_cast<f32>(j - t_centerFloorNum), -282.0f);
					break;
					//Third floor
				case 5:
					AEMtx33Scale(&m_floor[j][i].m_scale, 779.0f, 133.f);
					AEMtx33Trans(&m_floor[j][i].m_trans, 555.0f * static_cast<f32>(j - t_centerFloorNum), -50.0f);
					break;
					//Fourth floor
				case 6:
					AEMtx33Scale(&m_floor[j][i].m_scale, 381.0f, 47.f);
					AEMtx33Trans(&m_floor[j][i].m_trans, 270.0f * static_cast<f32>(j - t_centerFloorNum), 39.0f);
					break;
					//Fifth floor
				case 7:
					AEMtx33Scale(&m_floor[j][i].m_scale, 181.0f, 14.f);
					AEMtx33Trans(&m_floor[j][i].m_trans, 130.0f * static_cast<f32>(j - t_centerFloorNum), 69.0f);
					break;
					//Sixth floor
				case 8:
					AEMtx33Scale(&m_floor[j][i].m_scale, 85.0f, 4.f);
					AEMtx33Trans(&m_floor[j][i].m_trans, 59.0f * static_cast<f32>(j - t_centerFloorNum), 78.0f);
					break;
					//Seventh floor
				case 9:
					AEMtx33Scale(&m_floor[j][i].m_scale, 33.0f, 1.f);
					AEMtx33Trans(&m_floor[j][i].m_trans, 25.0f * static_cast<f32>(j - t_centerFloorNum), 80.0f);
					break;
				default:
					cout << "Error pls check floor" << "\n";
					break;
				}
				m_floor[j][i].m_floorNum = i;
				m_floor[j][i].m_originalTrans = m_floor[j][i].m_trans;
				AEMtx33Concat(&m_floor[j][i].m_transformFloorData, &m_floor[j][i].m_trans, &m_floor[j][i].m_scale);
				m_floor[j][i].m_currFloorNum = i;
				//Setting Movement Point To
				m_floor[j][i].m_transformFloorCurr = m_floor[j][i].m_transformFloorData;
			}
		}


	}

	/////////////////////////////////////////////////////////////
	// BACKDROP
	{
		////////////////////////////////////////////////////////////////////////
		//Sky
		AEMtx33 m_temp;
		AEMtx33Identity(&m_temp);
		//DO SKY DATA
		AEMtx33Scale(&scale, 1700.0f, 600.f);
		AEMtx33Trans(&trans, 0, 220);
		AEMtx33Concat(&m_transformSkyData, &trans, &scale);
		for (int i = 0; i < 9; i++)
		{
			m_transformCloudsData.push_back(m_temp);
			AEMtx33ScaleApply(&m_transformCloudsData[i], &m_transformCloudsData[i], 1700.0f, 600.f);
			AEMtx33TransApply(&m_transformCloudsData[i], &m_transformCloudsData[i], (i % 3 - 1) * 1700.0f, 220.f);
		}

		////////////////////////////////////////////////////////////////////////
		//Mountain, hills, whatnot
		//A Quick fix for retaining the previous transform of the backdrops
		for (int i = 0; i < 9; i++)
		{
			m_transformBackDrops1Data.push_back(m_temp);
			m_transformBackDrops2Data.push_back(m_temp);
			m_transformBackDrops3Data.push_back(m_temp);
		}

		////////////////////////////////////////////////////////////////////////
		//Sun
		m_sunOverlayScale = { 120.f, 120.f };
		m_sunPos = { 350, 350 };
		AEMtx33Scale(&scale, 50.0f, 50.f);
		AEMtx33Trans(&trans, m_sunPos.x, m_sunPos.y);
		AEMtx33Concat(&m_transformSunData, &trans, &scale);
		AEMtx33Scale(&scale, m_sunOverlayScale.x, m_sunOverlayScale.y);
		AEMtx33Trans(&trans, m_sunPos.x, m_sunPos.y);
		AEMtx33Concat(&m_transformSunOverlayData, &trans, &scale);
		for (int i = 0; i < 8; i++)
			m_transformSunLensData.push_back(m_temp);

		////////////////////////////////////////////////////////////////////////
		//Fog
		AEMtx33Scale(&scale, 2000.0f, 70.f);
		AEMtx33Trans(&trans, 0, 80);
		AEMtx33Concat(&m_transformFogData, &trans, &scale);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/***************************************************************************************************************************************

		  _____                    _____                    _____                    _____             _____                    _____
		 /\    \                  /\    \                  /\    \                  /\    \           /\    \                  /\    \
		/::\____\                /::\    \                /::\    \                /::\    \         /::\    \                /::\    \
	   /:::/    /               /::::\    \              /::::\    \              /::::\    \        \:::\    \              /::::\    \
	  /:::/    /               /::::::\    \            /::::::\    \            /::::::\    \        \:::\    \            /::::::\    \
	 /:::/    /               /:::/\:::\    \          /:::/\:::\    \          /:::/\:::\    \        \:::\    \          /:::/\:::\    \
	/:::/    /               /:::/__\:::\    \        /:::/  \:::\    \        /:::/__\:::\    \        \:::\    \        /:::/__\:::\    \
   /:::/    /               /::::\   \:::\    \      /:::/    \:::\    \      /::::\   \:::\    \       /::::\    \      /::::\   \:::\    \
  /:::/    /      _____    /::::::\   \:::\    \    /:::/    / \:::\    \    /::::::\   \:::\    \     /::::::\    \    /::::::\   \:::\    \
 /:::/____/      /\    \  /:::/\:::\   \:::\____\  /:::/    /   \:::\ ___\  /:::/\:::\   \:::\    \   /:::/\:::\    \  /:::/\:::\   \:::\    \
|:::|    /      /::\____\/:::/  \:::\   \:::|    |/:::/____/     \:::|    |/:::/  \:::\   \:::\____\ /:::/  \:::\____\/:::/__\:::\   \:::\____\
|:::|____\     /:::/    /\::/    \:::\  /:::|____|\:::\    \     /:::|____|\::/    \:::\  /:::/    //:::/    \::/    /\:::\   \:::\   \::/    /
 \:::\    \   /:::/    /  \/_____/\:::\/:::/    /  \:::\    \   /:::/    /  \/____/ \:::\/:::/    //:::/    / \/____/  \:::\   \:::\   \/____/
  \:::\    \ /:::/    /            \::::::/    /    \:::\    \ /:::/    /            \::::::/    //:::/    /            \:::\   \:::\    \
   \:::\    /:::/    /              \::::/    /      \:::\    /:::/    /              \::::/    //:::/    /              \:::\   \:::\____\
	\:::\__/:::/    /                \::/____/        \:::\  /:::/    /               /:::/    / \::/    /                \:::\   \::/    /
	 \::::::::/    /                  ~~               \:::\/:::/    /               /:::/    /   \/____/                  \:::\   \/____/
	  \::::::/    /                                     \::::::/    /               /:::/    /                              \:::\    \
	   \::::/    /                                       \::::/    /               /:::/    /                                \:::\____\
		\::/____/                                         \::/____/                \::/    /                                  \::/    /
		 ~~                                                ~~                       \/____/                                    \/____/

***************************************************************************************************************************************/
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SceneLevelBuilder::update(double dt)
{
	if (!SceneStages::m_sInstance->m_StartGame) dt *= 3; //SPEEDUP SPECIFICALLY TO FIT THE TRANSITION WITHIN THE SMALL TIMEFRAME

	/////////////////////////////////////////////////////////////////////////////////
	// Generic Update calls can be placed here
	{
		updateGlobals();
		player->update(dt);
		UpdateScreenTransition(static_cast<f32>(dt)); // Screen Basic Transition
		Pause::getInstance().update(dt); //Pause
		if (Pause::getInstance().m_isPaused) {
			return;
		}
		UpdateLvlName(static_cast<f32>(dt));//Level Name Animation
		///////////////////////////////////////////////////////////////////////////////
		//Level Progression Update
		if (!m_combatPhase)
		{
			if (m_completionStatus > 100.0 && SceneStages::m_sInstance->m_StartGame)
			{
				if (m_currLevel < m_maxLevel) {
					++m_currLevel;

					// mark next level as unlocked
					Database::getInstance().data["levels"][m_currLevel]["unlocked"] = true;
				}
				else {
					showGameEnd = true;
					elapsedTime += static_cast<float>(dt);
					if (elapsedTime >= SHOW_GAME_END_DURATION) {
						SceneManager::getInstance()->SetActiveScene("SceneCredits");
						return;			// terminate this scene state early
					}
					return;
				}

				if (!showGameEnd) {
					SceneLevelBuilder::SpawnLvlName();
					m_completionStatus = 0.0;
				}
			}

			if (!showGameEnd) {
				if (CombatManager::getInstance().m_playerFledLastFight) {
					CombatManager::getInstance().m_playerFledLastFight = false;		// reset flag
					m_completionStatus -= FLEEING_COMPLETION_PENALTY;		// penalize player for fleeing fight
					m_completionStatus = m_completionStatus < 0 ? 0 : m_completionStatus;
				}

				m_completionStatus += SceneStages::m_sInstance->m_StartGame ? dt * m_sceneLevelDataList[m_currLevel].m_LevelCompletionRate : 0.0;

				if (DEBUG && AEInputCheckTriggered(AEVK_1)) {
					m_completionStatus = 99.0;		// skip to completion when debugging
				}
			}

			///////////////////////////////////////////////////////////////////////////////////////////////////
			// This is a way to speed up playtesting, uncomment this if you would like to progress and unlock the 
			// levels faster.
			//if (AEInputCheckCurr(AEVK_TAB))
			//	m_completionStatus += dt * m_sceneLevelDataList[m_currLevel].m_LevelCompletionRate * 10;
		}
		UpdateLevelGameplay(static_cast<float>(dt));
		UpdateLensFlare(static_cast<float>(dt));
		UpdateClouds(static_cast<float>(dt));
		UpdateBackdrop(static_cast<float>(dt));
	}

	////////////////////////////////////////////////////////////////////////
	//Adjustments to camera if necessary
	{
	}

	////////////////////////////////////////////////////////////////////////////////
	// Combat & Player with Scene Interaction
	{
		static int t_whoseTurn = 0;
		if (!m_combatPhase)
		{
			////////////////////////////////////////////////////////////////////////////
			// Player INPUT
			{
				if (AEInputCheckTriggered(AEVK_LBUTTON))
				{
					SoundPlayer::CombatAudio::getInstance().playSfxPunch();
					player->setHandStateAnimationType(Player::HandAnimationType::Punch);
				}
			}

			/////////////////////////////////////////////////////////////////////////////
			// Settings to lerp to the movement view
			{
				if (m_currTransitionTransparency >= 0.5f)
				{
					m_combatBufferingTime -= m_combatBufferingTime > 0.0 ? static_cast<f32>(dt) : 0;
					if (m_combatBufferingTime > 0.2f)
					{
						dt *= 15; //SPEEDUP SPECIFICALLY TO FIT THE TRANSITION WITHIN THE SMALL TIMEFRAME
						m_stopMovement = false;
						m_panCloseToGround = false;
						m_panCloseToGroundValue += m_panCloseToGroundValue < 80 ? static_cast<int>(LERPING_SPEED) : 0;
					}
				}
				if (m_combatBufferingTime <= 0.0f)
				{
					////////////////////////////////////////////////////////////////////////
					// Fade out after combat
					//might give potential errors if using fade in/fade out in other areas, so watch out
					FadeOutBlack();
					//////////////////////////////////////////////////////////////////////////
					if (GameScene::combatAudioLoopIsPlaying && !SceneStagesAudio::loopIsPlaying && GameScene::afterInit) {
						SoundPlayer::stopAll();
						SoundPlayer::GameAudio::getInstance().playLoop();
						GameScene::combatAudioLoopIsPlaying = false;
						SceneStagesAudio::loopIsPlaying = true;
					}
				}
			}

			/////////////////////////////////////////////////////////////////////////////
			// Check to start Combat Phase
			{
				/////////////////////////////////////////////////////////////////////////////////
				// Two scenarios in which combat starts : Your turn or OUR TURN ( Insert R*****N Flag meme here )
				// 1) Player turn starts first
				// 2) Enemy turn starts first
				if (m_sceneEnemy != nullptr && !showGameEnd)
				{
					if (m_sceneEnemy->m_StartCombat)
					{
						switch (m_sceneEnemy->m_StartCombat)
						{
						case 1:
							m_sceneEnemy->m_Active = false;
							m_sceneEnemy = nullptr;
							m_combatBufferingTime = 2.0f;
							CombatScene::getInstance().spawnEnemies(GenerateEnemyToSpawn());
							t_whoseTurn = CombatManager::PLAYER;
							m_combatPhase = true;
							m_combatAnimationComp = false;
							break;
						case 2:
							m_sceneEnemy->m_Active = false;
							m_sceneEnemy = nullptr;
							m_currTransitionTransparency = 1.0f;
							m_setTransitionTransparency = 1.0f;
							m_combatBufferingTime = 0.8f;
							CombatScene::getInstance().spawnEnemies(GenerateEnemyToSpawn());
							t_whoseTurn = CombatManager::ENEMY;
							m_combatPhase = true;
							m_combatAnimationComp = false;
							break;
						}
					}
				}
			}
		}
		else
		{
			//////////////////////////////////////////////////////////////////
			// Combat Update
			if (m_combatAnimationComp)
			{
				// check if combat is over and update accordingly
				bool prevCombatPhaseState = m_combatPhase;
				m_combatPhase = CombatManager::getInstance().m_isInCombat;

				// switch combat audio back to game audio
				if (prevCombatPhaseState && !m_combatPhase) {
					SoundPlayer::stopAll();
					SoundPlayer::GameAudio::getInstance().playLoop();
					GameScene::combatAudioLoopIsPlaying = false;
					SceneStagesAudio::loopIsPlaying = true;

				}

				CombatScene::getInstance().update(dt);

				/////////////////////////////////////////////////////////////////////////////////
				// 
				if (!m_combatPhase)
				{
					m_combatBufferingTime = 0.6f;
					FadeINBlack();
					if (CombatScene::getInstance().getWinFlag())
						m_completionStatus += 30; // Increment Progression of level for completing the battle

				}

			}

			/////////////////////////////////////////////////////////////////////
			// Settings to lerp to the combat view
			{
				m_combatBufferingTime -= m_combatBufferingTime > 0.0 ? static_cast<f32>(dt) : 0;
				if (m_combatBufferingTime < 0.0f && !m_combatAnimationComp)
				{
					m_combatAnimationComp = true;
					m_stopMovement = true;
					CombatScene::getInstance().init((CombatManager::TURN)t_whoseTurn);
					FadeOutBlack();
				}
				//For faster setup
				if (m_combatBufferingTime > 0.4f && !m_combatAnimationComp)
				{
					if ((CombatManager::TURN)t_whoseTurn - 1) dt *= 5; //SPEEDUP SPECIFICALLY TO FIT THE TRANSITION WITHIN THE SMALL TIMEFRAME
				}

				m_panCloseToGround = true;
				m_panCloseToGroundValue -= m_panCloseToGroundValue > 30 ? static_cast<int>(LERPING_SPEED) : 0;

				if (!GameScene::combatAudioLoopIsPlaying && m_combatPhase) {
					SoundPlayer::stopAll();
					SoundPlayer::CombatAudio::getInstance().playLoop();
					GameScene::combatAudioLoopIsPlaying = true;
					SceneStagesAudio::loopIsPlaying = false;
				}
			}
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Game 3D Environment Update Cycle
	if (!m_stopMovement)
	{
		///////////////////////////////////////////////////////////////////////////
		//UPDATE FLOOR MOVEMENT
		//////////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////////////
		// Checks from Left to Right
		///////////////////////////////////////////////////////////////////////////
		for (int j = 0; j < SIZE_OF_FLOOR; j++)
		{
			std::list<int> t_ShiftRow{};
			AEMtx33 m_LastFloorData = m_floor[j][8].m_transformFloorData;
			///////////////////////////////////////////////////////////////////////////
			// Checks from Up to Down
			///////////////////////////////////////////////////////////////////////////
			for (int i = NUM_OF_TILES - 1; i > -1; i--)
			{
				AEMtx33 m_NextFloorData = m_floor[j][i].m_currFloorNum != 0 ? m_floor[j][m_floor[j][i].m_currFloorNum - 1].m_transformFloorData : m_floor[j][i].m_transformFloorCurr = m_LastFloorData;
				AEMtx33 m_CurrFloorData = m_floor[j][m_floor[j][i].m_currFloorNum].m_transformFloorData;

				//Minimum Speed of next floor
				AEMtx33 m_MinimumNextFloorSpeed = {
				(m_NextFloorData.m[0][0] - m_CurrFloorData.m[0][0]) / m_panCloseToGroundValue,
				(m_NextFloorData.m[0][1] - m_CurrFloorData.m[0][1]) / 80,
				(m_NextFloorData.m[0][2] - m_CurrFloorData.m[0][2]) / m_panCloseToGroundValue,
				(m_NextFloorData.m[1][0] - m_CurrFloorData.m[1][0]) / 80,
				(m_NextFloorData.m[1][1] - m_CurrFloorData.m[1][1]) / 80,
				(m_NextFloorData.m[1][2] - m_CurrFloorData.m[1][2]) / 80,
				(m_NextFloorData.m[2][0] - m_CurrFloorData.m[2][0]) / 80,
				(m_NextFloorData.m[2][1] - m_CurrFloorData.m[2][1]) / 80,
				(m_NextFloorData.m[2][2] - m_CurrFloorData.m[2][2]) / 80
				};

				//Incrementing speed
				m_floor[j][i].m_currFloorSpeed.m[0][0] += m_floor[j][i].m_currFloorSpeed.m[0][0] < m_MinimumNextFloorSpeed.m[0][0] ? static_cast<f32>(dt) * m_MinimumNextFloorSpeed.m[0][0] * TOP_MOVEMENT_SPEED : m_floor[j][i].m_currFloorSpeed.m[0][0] > m_MinimumNextFloorSpeed.m[0][0] ? static_cast<f32>(dt) * m_MinimumNextFloorSpeed.m[0][0] * TOP_MOVEMENT_SPEED : 0;
				m_floor[j][i].m_currFloorSpeed.m[0][1] += m_floor[j][i].m_currFloorSpeed.m[0][1] < m_MinimumNextFloorSpeed.m[0][1] ? static_cast<f32>(dt) * m_MinimumNextFloorSpeed.m[0][1] * TOP_MOVEMENT_SPEED : m_floor[j][i].m_currFloorSpeed.m[0][1] > m_MinimumNextFloorSpeed.m[0][1] ? static_cast<f32>(dt) * m_MinimumNextFloorSpeed.m[0][1] * TOP_MOVEMENT_SPEED : 0;
				m_floor[j][i].m_currFloorSpeed.m[0][2] += m_floor[j][i].m_currFloorSpeed.m[0][2] < m_MinimumNextFloorSpeed.m[0][2] ? static_cast<f32>(dt) * m_MinimumNextFloorSpeed.m[0][2] * TOP_MOVEMENT_SPEED : m_floor[j][i].m_currFloorSpeed.m[0][2] > m_MinimumNextFloorSpeed.m[0][2] ? static_cast<f32>(dt) * m_MinimumNextFloorSpeed.m[0][2] * TOP_MOVEMENT_SPEED : 0;
				m_floor[j][i].m_currFloorSpeed.m[1][0] += m_floor[j][i].m_currFloorSpeed.m[1][0] < m_MinimumNextFloorSpeed.m[1][0] ? static_cast<f32>(dt) * m_MinimumNextFloorSpeed.m[1][0] * TOP_MOVEMENT_SPEED : m_floor[j][i].m_currFloorSpeed.m[1][0] > m_MinimumNextFloorSpeed.m[1][0] ? static_cast<f32>(dt) * m_MinimumNextFloorSpeed.m[1][0] * TOP_MOVEMENT_SPEED : 0;
				m_floor[j][i].m_currFloorSpeed.m[1][1] += m_floor[j][i].m_currFloorSpeed.m[1][1] < m_MinimumNextFloorSpeed.m[1][1] ? static_cast<f32>(dt) * m_MinimumNextFloorSpeed.m[1][1] * TOP_MOVEMENT_SPEED : m_floor[j][i].m_currFloorSpeed.m[1][1] > m_MinimumNextFloorSpeed.m[1][1] ? static_cast<f32>(dt) * m_MinimumNextFloorSpeed.m[1][1] * TOP_MOVEMENT_SPEED : 0;
				m_floor[j][i].m_currFloorSpeed.m[1][2] += m_floor[j][i].m_currFloorSpeed.m[1][2] < m_MinimumNextFloorSpeed.m[1][2] ? static_cast<f32>(dt) * m_MinimumNextFloorSpeed.m[1][2] * TOP_MOVEMENT_SPEED : m_floor[j][i].m_currFloorSpeed.m[1][2] > m_MinimumNextFloorSpeed.m[1][2] ? static_cast<f32>(dt) * m_MinimumNextFloorSpeed.m[1][2] * TOP_MOVEMENT_SPEED : 0;
				m_floor[j][i].m_currFloorSpeed.m[2][0] += m_floor[j][i].m_currFloorSpeed.m[2][0] < m_MinimumNextFloorSpeed.m[2][0] ? static_cast<f32>(dt) * m_MinimumNextFloorSpeed.m[2][0] * TOP_MOVEMENT_SPEED : m_floor[j][i].m_currFloorSpeed.m[2][0] > m_MinimumNextFloorSpeed.m[2][0] ? static_cast<f32>(dt) * m_MinimumNextFloorSpeed.m[2][0] * TOP_MOVEMENT_SPEED : 0;
				m_floor[j][i].m_currFloorSpeed.m[2][1] += m_floor[j][i].m_currFloorSpeed.m[2][1] < m_MinimumNextFloorSpeed.m[2][1] ? static_cast<f32>(dt) * m_MinimumNextFloorSpeed.m[2][1] * TOP_MOVEMENT_SPEED : m_floor[j][i].m_currFloorSpeed.m[2][1] > m_MinimumNextFloorSpeed.m[2][1] ? static_cast<f32>(dt) * m_MinimumNextFloorSpeed.m[2][1] * TOP_MOVEMENT_SPEED : 0;
				m_floor[j][i].m_currFloorSpeed.m[2][2] += m_floor[j][i].m_currFloorSpeed.m[2][2] < m_MinimumNextFloorSpeed.m[2][2] ? static_cast<f32>(dt) * m_MinimumNextFloorSpeed.m[2][2] * TOP_MOVEMENT_SPEED : m_floor[j][i].m_currFloorSpeed.m[2][2] > m_MinimumNextFloorSpeed.m[2][2] ? static_cast<f32>(dt) * m_MinimumNextFloorSpeed.m[2][2] * TOP_MOVEMENT_SPEED : 0;
				//Adding to floor
				m_floor[j][i].m_transformFloorCurr.m[0][0] += m_floor[j][i].m_currFloorSpeed.m[0][0];
				m_floor[j][i].m_transformFloorCurr.m[0][1] += m_floor[j][i].m_currFloorSpeed.m[0][1];
				m_floor[j][i].m_transformFloorCurr.m[0][2] += m_floor[j][i].m_currFloorSpeed.m[0][2];
				m_floor[j][i].m_transformFloorCurr.m[1][0] += m_floor[j][i].m_currFloorSpeed.m[1][0];
				m_floor[j][i].m_transformFloorCurr.m[1][1] += m_floor[j][i].m_currFloorSpeed.m[1][1];
				m_floor[j][i].m_transformFloorCurr.m[1][2] += m_floor[j][i].m_currFloorSpeed.m[1][2];
				m_floor[j][i].m_transformFloorCurr.m[2][0] += m_floor[j][i].m_currFloorSpeed.m[2][0];
				m_floor[j][i].m_transformFloorCurr.m[2][1] += m_floor[j][i].m_currFloorSpeed.m[2][1];
				m_floor[j][i].m_transformFloorCurr.m[2][2] += m_floor[j][i].m_currFloorSpeed.m[2][2];

				///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				m_floor[j][i].m_trans.m[0][2] += m_floor[j][i].m_currFloorSpeed.m[0][2];
				m_floor[j][i].m_trans.m[1][2] += m_floor[j][i].m_currFloorSpeed.m[1][2];

				if (!m_stopMovement)
				{
					if (m_floor[j][i].m_currFloorTimer > m_floor[j][i].m_floorSpeedTimer)
					{
						m_floor[j][i].m_currFloorTimer = 0.0;
						if (m_floor[j][i].m_currFloorNum > 1)
						{
							m_floor[j][i].m_currFloorNum--;
							m_floor[j][i].m_isRender = true;
						}
						else
						{
							//Loop to the top
							m_floor[j][i].m_currFloorNum = 8;
							m_floor[j][i].m_currFloorSpeed = { 0 };
							m_floor[j][i].m_transformFloorCurr = m_LastFloorData;
							m_floor[j][i].m_isRender = false;

							m_floor[j][i].m_trans.m[0][2] = m_floor[j][8].m_originalTrans.m[0][2];
							m_floor[j][i].m_trans.m[1][2] = m_floor[j][8].m_originalTrans.m[1][2];

							m_currentTileNumFurthest = m_floor[j][i].m_floorNum;

							t_ShiftRow.push_back(m_floor[j][i].m_floorNum);

							//FORCE CHANGE OF FLOORING TYPE
							//Change of flooring at Stage 6
							if (m_currLevel >= 6 && m_currLevel <= 7)
								m_floor[j][i].m_type = 1;
							else
								m_floor[j][i].m_type = 0;
						}
					}
					else
						m_floor[j][i].m_currFloorTimer += static_cast<f32>(dt);
				}
			}

			if (t_ShiftRow.size() >= 1)
			{
				for (std::list<int>::iterator i = t_ShiftRow.begin(); i != t_ShiftRow.end(); i++)
				{
					//Clear Scene Objects in row
					DestroyRowOBJs(*i);

					//Only spawn on tile 0, 2->8 (1 & 9 tiles are hidden below another tile, so visual error)
					if (m_floor[j][*i].m_floorNum > -1 && m_floor[j][*i].m_floorNum < 9 && m_floor[j][*i].m_floorNum != 1)
						CreateRowOBJs(*i);
				}
				t_ShiftRow.clear();
			}
		}

		///////////////////////////////////////////////////////////////////////////
		//UPDATE SCENEOBJs Pos and Logic
		//////////////////////////////////////////////////////////////////////////
		GameObjectManager::getInstance()->update(dt);
		v_SceneObject temp;
		std::pair<int, int> t_TransScaleModifier = { 60, 48 }; //For rand on tile pos
		static double t_XModifier = 2, t_YModifier = 225, t_MXModifier = 45, t_MYModifier = 1;
		for (int j = 0; j < SIZE_OF_FLOOR; j++)
		{
			for (int i = NUM_OF_TILES - 1; i > -1; i--)
			{
				for (std::list<v_SceneObject>::iterator it = m_floorOBJs[j][i].begin();
					it != m_floorOBJs[j][i].end();
					it++)
				{
					//Reset Transform data
					AEMtx33Identity(&(*it).m_TransformData);

					if (!m_panCloseToGround)
					{
						t_XModifier = t_XModifier > 2.0 ? t_XModifier - dt : t_XModifier;
						t_YModifier = t_YModifier > 225 ? t_YModifier - dt * LerpSpeed : t_YModifier;
						t_MXModifier = t_MXModifier < 45 ? t_MXModifier + dt * LerpSpeed : t_MXModifier;
						t_MYModifier = t_MYModifier > 1 ? t_MYModifier - dt : t_MYModifier;
					}
					else
					{
						t_XModifier = t_XModifier < 2.3 ? t_XModifier + dt / 400 : t_XModifier;
						t_YModifier = t_YModifier < 938 ? t_YModifier + dt / 12.7 * LerpSpeed : t_YModifier;
						t_MXModifier = t_MXModifier > -10.45 ? t_MXModifier - dt / 100 * LerpSpeed : t_MXModifier;
						t_MYModifier = t_MYModifier < 10 ? t_MYModifier + dt / 100 : t_MYModifier;
					}

					//Scale with the tile
					AEMtx33ScaleApply(&(*it).m_TransformData, &(*it).m_TransformData, m_floor[j][i].m_transformFloorCurr.m[0][0] * 2, m_floor[j][i].m_transformFloorCurr.m[0][0] * 2);

					//Translate with the tile
					if (j < t_centerFloorNum)
						AEMtx33TransApply(&(*it).m_TransformData, &(*it).m_TransformData, static_cast<float>(m_floor[j][i].m_trans.m[0][2] * t_XModifier + t_MXModifier), static_cast<float>(t_YModifier - m_floor[j][i].m_trans.m[1][2] * t_MYModifier));
					else
						AEMtx33TransApply(&(*it).m_TransformData, &(*it).m_TransformData, static_cast<float>(m_floor[j][i].m_trans.m[0][2] * t_XModifier - t_MXModifier), static_cast<float>(t_YModifier - m_floor[j][i].m_trans.m[1][2] * t_MYModifier));


					AEMtx33TransApply(&(*it).m_TransformData, &(*it).m_TransformData,
						(*it).m_trans.m[0][2] * m_floor[j][i].m_transformFloorCurr.m[0][0] / ((t_TransScaleModifier.first) / (*it).m_scale.m[0][0]),
						0);

					//Adjusting Transparency
					(*it).m_Transparency += static_cast<f32>(dt) * 1.5f;
				}
			}
		}
	}
	else
	{
		////////////////////////////////////////////////////////////////////////////
		// Make all Scene Objs visible if possible if stop movement
		for (int j = 0; j < SIZE_OF_FLOOR; j++)
		{
			for (int i = NUM_OF_TILES - 1; i > -1; i--)
			{
				for (std::list<v_SceneObject>::iterator it = m_floorOBJs[j][i].begin();
					it != m_floorOBJs[j][i].end();
					it++)
				{
					(*it).m_Transparency += static_cast<f32>(dt) * 1.5f;
				}
			}
		}
	}
}

/*********************************************************************************
Scene Object Spawning
**********************************************************************************/
void SceneLevelBuilder::CreateRowOBJs(int t_tileNum)
{
	//srand(static_cast<unsigned> (time(0)));

	//if(false) //Delete if not used
	for (int j = 0; j < SIZE_OF_FLOOR; j++)
	{
		//Skip centre
		if (j == t_centerFloorNum)
			continue;

		//Spawn objs based on MAX_NUM_SCENEOBJS_TILE
		for (int i = static_cast<int>(AEClamp(static_cast<f32>(rand() % MAX_NUM_SCENEOBJS_TILE), 1.0f, static_cast<f32>(MAX_NUM_SCENEOBJS_TILE))); i > 0; i--)
		{
			v_SceneObject newObj;

			//Selecting Entity Group to Spawn
			int TotalProb = 0; //Get total probability
			for (int curr : m_sceneLevelDataList[m_currLevel].m_SceneObjSpawnWeight)
			{
				TotalProb += curr;
			}
			std::string Ref = "";
			int randnum = static_cast<int>(AEClamp(static_cast<f32>(rand() % TotalProb), 1.0f, static_cast<f32>(TotalProb)));//This is the rand probability of which type of sceneobjects to spawn
			int temp = 0;//Disregard this: for loop below
			for (int curr : m_sceneLevelDataList[m_currLevel].m_SceneObjSpawnWeight)
			{
				randnum -= curr;
				if (randnum < 0)
				{
					Ref = m_sceneLevelDataList[m_currLevel].m_SceneObjTypes[temp];
					break;
				}
				temp++;
			}

			//Selecting Entity from entity group to spawn
			if (Ref == "Grass")
			{
				newObj.m_type = static_cast<v_SceneObjectTypes>(AEClamp(static_cast<f32>(rand() % (v_SceneObjectTypes::TYPE_End_Grass - v_SceneObjectTypes::TYPE_Grass) + v_SceneObjectTypes::TYPE_Grass),
					static_cast<f32>(v_SceneObjectTypes::TYPE_Grass + 1),
					static_cast<f32>(v_SceneObjectTypes::TYPE_End_Grass - 1)));
				newObj.m_tobeCentered = true;
			}
			else if (Ref == "Tree")
			{
				newObj.m_type = static_cast<v_SceneObjectTypes>(AEClamp(static_cast<f32>(rand() % (v_SceneObjectTypes::TYPE_End_Tree - v_SceneObjectTypes::TYPE_Tree) + v_SceneObjectTypes::TYPE_Tree),
					static_cast<f32>(v_SceneObjectTypes::TYPE_Tree + 1),
					static_cast<f32>(v_SceneObjectTypes::TYPE_End_Tree - 1)));
				newObj.m_tobeCentered = false;
			}
			else if (Ref == "Rock")
			{
				newObj.m_type = static_cast<v_SceneObjectTypes>(AEClamp(static_cast<f32>(rand() % (v_SceneObjectTypes::TYPE_End_Rock - v_SceneObjectTypes::TYPE_Rock) + v_SceneObjectTypes::TYPE_Rock),
					static_cast<f32>(v_SceneObjectTypes::TYPE_Rock + 1),
					static_cast<f32>(v_SceneObjectTypes::TYPE_End_Rock - 1)));
				newObj.m_tobeCentered = true;
			}

			//Random Selection of Spawn location on tile
			int t_RandX, t_RandY;
			if (!newObj.m_tobeCentered)
			{
				if (j == t_centerFloorNum - 1 || j == t_centerFloorNum + 1)
				{
					t_RandX = static_cast<int>(rand() % NUM_OF_TILESPAWNPOINTS / 2);
					t_RandY = static_cast<int>(rand() % NUM_OF_TILESPAWNPOINTS / 2);
				}
				else
				{
					t_RandX = rand() % NUM_OF_TILESPAWNPOINTS;
					t_RandY = rand() % NUM_OF_TILESPAWNPOINTS;
				}
			}
			else
			{
				t_RandX = t_RandY = 0;
			}


			if (j < t_centerFloorNum)//Left Side
			{
				AEMtx33Trans(&newObj.m_trans, m_tileSP[t_RandY][t_RandX].m_X + m_tileSP[t_RandY][t_RandX].m_Y / 3, m_tileSP[t_RandY][t_RandX].m_Y);
			}
			else//Right Side
			{
				AEMtx33Trans(&newObj.m_trans, -m_tileSP[t_RandY][t_RandX].m_X - m_tileSP[t_RandY][t_RandX].m_Y / 3, m_tileSP[t_RandY][t_RandX].m_Y);
			}

			//Scaling (Uniform Scaling)
			AEMtx33Scale(&newObj.m_scale, 2.5f, 2.5f);

			//Push into OBJlist in tile (Determine which to render first based on Spawnpoint m_Y
			std::list<v_SceneObject>::iterator it = m_floorOBJs[j][t_tileNum].begin();
			newObj.m_RenderOrder = t_RandY; //Spawnpoint m_y
			for (; it != m_floorOBJs[j][t_tileNum].end(); it++)
			{
				if (newObj.m_RenderOrder <= (*it).m_RenderOrder)
					break;
			}
			m_floorOBJs[j][t_tileNum].insert(it, newObj);
		}
	}
}
void SceneLevelBuilder::DestroyRowOBJs(int t_tileNum)
{
	for (int j = 0; j < SIZE_OF_FLOOR; j++)
	{
		m_floorOBJs[j][t_tileNum].clear();
	}
}


/*********************************************************************************
Level Name
**********************************************************************************/
void SceneLevelBuilder::SpawnLvlName()
{
	m_lvlNameTimer = MAX_LVLNAMETIMER;
	m_lvlNameTransparency = -1.2f;
}
void SceneLevelBuilder::UpdateLvlName(f32 t_dt)
{
	if (m_lvlNameTimer > MAX_LVLNAMETIMER - 1.0)
	{
		m_lvlNameTransparency += m_lvlNameTransparency > 1.0f ? 0.0f : 0.08f;
	}
	else if (m_lvlNameTimer < 1.0)
	{
		m_lvlNameTransparency -= m_lvlNameTransparency < -1.0f ? 0.0f : 0.08f;
	}
	m_lvlNameTimer -= t_dt;
}
void SceneLevelBuilder::RenderLvlName()
{
	f32 t_camX, t_camY;
	AEGfxGetCamPosition(&t_camX, &t_camY);

	AEVec2 RightOriginalHeaderPos{ 27.5f, 205.7f };
	AEVec2 LeftOriginalHeaderPos{ -27.5f, 205.7f };
	AEVec2 RightMaxHeaderPos{ -15.0f + m_sceneLevelDataList[m_currLevel].m_LevelName.size() * 13.7f, 205.7f };
	AEVec2 LeftMaxHeaderPos{ 15.0f - m_sceneLevelDataList[m_currLevel].m_LevelName.size() * 13.7f, 205.7f };
	static AEVec2 currRightHeaderPos{ RightOriginalHeaderPos };
	static AEVec2 currLeftHeaderPos{ LeftOriginalHeaderPos };
	currRightHeaderPos.x += currRightHeaderPos.x < RightMaxHeaderPos.x ? (RightMaxHeaderPos.x - currRightHeaderPos.x) / 30 : 0;
	currLeftHeaderPos.x += currLeftHeaderPos.x > LeftMaxHeaderPos.x ? (LeftMaxHeaderPos.x - currLeftHeaderPos.x) / 30 : 0;

	if (m_lvlNameTimer > 0.0 && m_currLevel > -1)
	{
		AEGfxTextureSet(NULL, 0, 0);
		f32 TextWidth = 0, TextHeight = 0;
		char strBuffer[1024];
		sprintf_s(strBuffer, m_sceneLevelDataList[m_currLevel].m_LevelName.c_str());
		AEGfxGetPrintSize(m_pTextFont, strBuffer, 0.8f, &TextWidth, &TextHeight);
		AEGfxPrint(m_pTextFont, strBuffer, -TextWidth / 2.0f - 0.01f, 0.61f, 0.8f, 0.f, 0.f, 0.f, m_lvlNameTransparency);
		AEGfxPrint(m_pTextFont, strBuffer, -TextWidth / 2.0f, 0.6f, 0.8f, 1.0f, 0.75f, 0.0f, m_lvlNameTransparency);


		AEGfxTextureSet(RenderHelper::getInstance()->getTextureByRef("LVL_HEADER"), 0, 0);
		if (m_lvlNameTimer < 1.0) AEGfxSetTransparency(m_lvlNameTransparency - 0.9f);
		else AEGfxSetTransparency(m_lvlNameTransparency + 0.2f);

		AEMtx33 trans{};
		AEMtx33Identity(&trans);
		AEMtx33ScaleApply(&trans, &trans, 214.5, 32.5);
		AEMtx33TransApply(&trans, &trans, currRightHeaderPos.x + t_camX - 3.75f, currRightHeaderPos.y + t_camY + 4.0f);
		AEGfxSetTransform(trans.m);
		AEGfxSetColorToMultiply(0.0f, 0.0f, 0.0f, 1.0f);
		AEGfxMeshDraw(RenderHelper::getInstance()->getdefaultMesh(), AE_GFX_MDM_TRIANGLES);
		AEMtx33TransApply(&trans, &trans, 3.75f, -4.0f);
		AEGfxSetTransform(trans.m);
		AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
		AEGfxMeshDraw(RenderHelper::getInstance()->getdefaultMesh(), AE_GFX_MDM_TRIANGLES);


		AEMtx33Identity(&trans);
		AEMtx33RotDeg(&trans, 180);
		AEMtx33ScaleApply(&trans, &trans, 214.5, 32.5);
		AEMtx33TransApply(&trans, &trans, currLeftHeaderPos.x + t_camX - 3.75f, currLeftHeaderPos.y + t_camY + 4.0f);
		AEGfxSetTransform(trans.m);
		AEGfxSetColorToMultiply(0.0f, 0.0f, 0.0f, 1.0f);
		AEGfxMeshDraw(RenderHelper::getInstance()->getdefaultMesh(), AE_GFX_MDM_TRIANGLES);
		AEMtx33TransApply(&trans, &trans, 3.75f, -4.0f);
		AEGfxSetTransform(trans.m);
		AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
		AEGfxMeshDraw(RenderHelper::getInstance()->getdefaultMesh(), AE_GFX_MDM_TRIANGLES);
	}
	else
	{
		currRightHeaderPos = RightOriginalHeaderPos;
		currLeftHeaderPos = LeftOriginalHeaderPos;
	}
}

/*********************************************************************************
Screen Transition
**********************************************************************************/
void SceneLevelBuilder::UpdateScreenTransition(f32 t_dt)
{
	m_currTransitionTransparency += (m_setTransitionTransparency - m_currTransitionTransparency) / (600 * t_dt);
}
void SceneLevelBuilder::FadeINBlack() { m_setTransitionTransparency = 1.0f; }
void SceneLevelBuilder::FadeOutBlack() { m_setTransitionTransparency = -1.0f; }

/*********************************************************************************
GENERIC UPDATE FUNCTIONS (PARALLAX SCROLLING)
**********************************************************************************/
void SceneLevelBuilder::UpdateLevelGameplay(f32 dt)
{
	/////////////////////////////////////////////////////////////////////////
	// Auto Spawning of enemies
	static double m_TryTimer = TRY_TO_SPAWN_ENEMY_TIMER;
	m_TryTimer -= dt;
	if (m_TryTimer < 0 || (DEBUG && AEInputCheckTriggered(AEVK_RBUTTON)))
	{
		if (m_sceneEnemy == nullptr && !m_combatPhase && SceneStages::m_sInstance->m_StartGame)
		{
			if (rand() % 100 < m_sceneLevelDataList[m_currLevel].m_EnemySpawnRate || (DEBUG && AEInputCheckTriggered(AEVK_RBUTTON)))
			{
				m_sceneEnemy = dynamic_cast<GameObject_Misc_Enemy*>(GameObjectManager::getInstance()->findObjectByReference("MiscEnemy"));
				m_sceneEnemy->ActivateEnemy(m_floor[t_centerFloorNum][m_currentTileNumFurthest].m_transformFloorCurr);
			}
		}
		m_TryTimer = TRY_TO_SPAWN_ENEMY_TIMER;
	}

	/////////////////////////////////////////////////////////////////////
	/*
			  Anything written in this part is meant for generic changes
			  like lighting
	*/
	/////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////
	// Update backdrop main position or light filter 
	// betweem each level to seamlessly transit
	static double t_r, t_g, t_b;
	if (m_currLevel < m_maxLevel) //Check the stats for next level
	{
		/////////////////////////////////////////////////////////////////////
		//Change to nighttime
		if (!m_sceneLevelDataList[m_currLevel].m_DayTime)
		{
			t_r = -0.39; t_g = 0.06; t_b = 0.3;
		}
		/////////////////////////////////////////////////////////////////////
		//Change to Dawn
		else if (!m_sceneLevelDataList[m_currLevel - 1].m_DayTime && m_sceneLevelDataList[m_currLevel].m_DayTime)
		{
			t_r = 1.0; t_g = 0.7; t_b = 1.0;
		}
		/////////////////////////////////////////////////////////////////////
		//Change to Dusk
		else if (!m_sceneLevelDataList[m_currLevel + 1].m_DayTime && m_sceneLevelDataList[m_currLevel].m_DayTime)
		{
			t_r = 1.0; t_g = 0.34; t_b = 0.3;
		}
		/////////////////////////////////////////////////////////////////////
		//Change to DayTime
		else
		{
			t_r = 1.0; t_g = 1.0; t_b = 1.0;
		}
	}
	/////////////////////////////////////////////////////////////////////
	//Change to DayTime
	else
	{
		t_r = 1.0; t_g = 1.0; t_b = 1.0;
	}

	m_lighting.r += m_lighting.r > static_cast<float>(t_r) && abs(t_r - m_lighting.r) > dt ? -dt / LERPING_SPEED : m_lighting.r < static_cast<float>(t_r) && abs(m_lighting.r - t_r) > dt ? dt / LERPING_SPEED : 0;
	m_lighting.g += m_lighting.g > static_cast<float>(t_g) && abs(t_g - m_lighting.g) > dt ? -dt / LERPING_SPEED : m_lighting.g < static_cast<float>(t_g) && abs(m_lighting.g - t_g) > dt ? dt / LERPING_SPEED : 0;
	m_lighting.b += m_lighting.b > static_cast<float>(t_b) && abs(t_b - m_lighting.b) > dt ? -dt / LERPING_SPEED : m_lighting.b < static_cast<float>(t_b) && abs(m_lighting.b - t_b) > dt ? dt / LERPING_SPEED : 0;

	/////////////////////////////////////////////////////////////////////
	/*
			  Anything written below this part is meant for specific
			  stages. SO BASICALLY HARD CODED FOR THAT STAGE
	*/
	/////////////////////////////////////////////////////////////////////
	{

	}
}
void SceneLevelBuilder::UpdateLensFlare(f32 t_dt)
{
	UNREFERENCED_PARAMETER(t_dt);

	int mX, mY;
	AEInputGetCursorPosition(&mX, &mY);

	mX -= AEGfxGetWindowWidth() / 2;
	mX = static_cast<int>(mX * 1.5);
	static int y = -120;
	mY = static_cast<int>(mY / 1.5 + y);
	mY *= -1;

	//Furthest from sun -> Closest to sun
	static f32 varience[8] = { -2.7f, -2.7f, -1.9f,-2.f, -2.55f, 0.72f, 0.25f, -0.1f };
	static f32 scaleIncr[8] = { 320,240 ,350 ,30 ,-100 ,-90,-40,60 };
	for (int i = 0; i < m_transformSunLensData.size(); i++)
	{
		AEMtx33Identity(&m_transformSunLensData[i]);
		AEMtx33ScaleApply(&m_transformSunLensData[i], &m_transformSunLensData[i], m_sunOverlayScale.x + scaleIncr[i], m_sunOverlayScale.y + scaleIncr[i]);
		AEMtx33TransApply(&m_transformSunLensData[i], &m_transformSunLensData[i], mX + (m_sunPos.x - mX) * (i + varience[i] + 1) / 8, mY + (m_sunPos.y - mY) * (i + varience[i] + 1) / 8);
	}
}
void SceneLevelBuilder::UpdateClouds(f32 t_dt)
{
	float t_CloudMaxSpeed = 10.0f;// - to go left, + to go right
	for (int i = 0; i < m_transformCloudsData.size(); i++)
	{
		if (i / 3 != 1)//Special case for this texture pack
		{
			AEMtx33Identity(&m_transformCloudsData[i]);
			AEMtx33ScaleApply(&m_transformCloudsData[i], &m_transformCloudsData[i], 1700.0f, 600.f);
			/***********************************************************************************************************************
			The Clouds interpolation works by having three different tiles for each layer.

			When the tiles reach or exceed their position by their size, they move to the back by teleporting
			(300 move right ->)     Tile 1         (1700 value apart)            Tile 2             (1700 value apart)             Tile 3
											  ||
											  \/
			Tile 3     (1700 value apart)       Tile 1         (1700 value apart)            Tile 2
			and vice versa.

			The second calculation is the parallax movement relative to the mouse position. The movement of the mouse creates slight movement
			in the clouds, creating depth.
			************************************************************************************************************************/
			//                                                                      |   Pos for each tile |          | Parallax Movement based on mouse position |     
			AEMtx33TransApply(&m_transformCloudsData[i], &m_transformCloudsData[i], (i % 3 - 1) * 1700.0f - static_cast<f32>(mouseX) / static_cast<f32>(((65) / (3 - i / 3))),
				305.f + static_cast<f32>(mouseY) / static_cast<f32>(((100) / (3 - i / 3))));
		}
		else
		{
			AEMtx33TransApply(&m_transformCloudsData[i], &m_transformCloudsData[i], t_CloudMaxSpeed * t_dt, 0.0f);;
		}

		//To do infinite Interpolation
		if (m_transformCloudsData[i].m[0][2] > m_transformCloudsData[i].m[0][0])
			m_transformCloudsData[i].m[0][2] -= m_transformCloudsData[i].m[0][0] * 2;
		if (m_transformCloudsData[i].m[0][2] < -m_transformCloudsData[i].m[0][0])
			m_transformCloudsData[i].m[0][2] += m_transformCloudsData[i].m[0][0] * 2;
	}
}
void SceneLevelBuilder::UpdateBackdrop(f32 t_dt)
{
	UNREFERENCED_PARAMETER(t_dt);
	for (int i = 0; i < 9; i++)
	{
		AEMtx33Identity(&m_transformBackDrops1Data[i]);
		AEMtx33ScaleApply(&m_transformBackDrops1Data[i], &m_transformBackDrops1Data[i], 240.0f, 360.f);
		AEMtx33TransApply(&m_transformBackDrops1Data[i], &m_transformBackDrops1Data[i], (i - 4) * 240.0f - static_cast<f32>(mouseX / 50.0f),
			205.f + static_cast<f32>(mouseY / 90.0f));

	}

	for (int i = 0; i < 5; i++)
	{
		AEMtx33Identity(&m_transformBackDrops2Data[i]);
		AEMtx33ScaleApply(&m_transformBackDrops2Data[i], &m_transformBackDrops2Data[i], 640.0f, 360.0f);
		AEMtx33TransApply(&m_transformBackDrops2Data[i], &m_transformBackDrops2Data[i], (i - 2) * 640.0f - static_cast<f32>(mouseX / 35.0f),
			205.f + static_cast<f32>(mouseY / 70.0f));
	}


	if (m_currLevel <= 5)
	{
		for (int i = 0; i < 5; i++)
		{
			AEMtx33Identity(&m_transformBackDrops3Data[i]);
			AEMtx33ScaleApply(&m_transformBackDrops3Data[i], &m_transformBackDrops3Data[i], 480.0f, 360.f);
			AEMtx33TransApply(&m_transformBackDrops3Data[i], &m_transformBackDrops3Data[i], (i - 2) * 480.0f - static_cast<f32>(mouseX / 27.0f),
				205.f + static_cast<f32>(mouseY / 50.0f));
		}
	}
	else
		for (int i = 0; i < 5; i++) AEMtx33Identity(&m_transformBackDrops3Data[i]);
}

std::vector<std::string> SceneLevelBuilder::GenerateEnemyToSpawn()
{
	m_combatNames.clear();
	int TotalProb = 0; //Get total probability
	for (int curr : m_sceneLevelDataList[m_currLevel].m_EnemySpawnWeight)
	{
		TotalProb += curr;
	}
	for (int i = 0; i < m_sceneLevelDataList[m_currLevel].m_MaxEnemies; i++)
	{
		std::string Ref = "";
		int randnum = static_cast<int>(AEClamp(static_cast<f32>(rand() % TotalProb), 1.0f, static_cast<f32>(TotalProb)));//This is the rand probability of which type of sceneobjects to spawn
		int temp = 0;//Disregard this: for loop below
		for (int curr : m_sceneLevelDataList[m_currLevel].m_EnemySpawnWeight)
		{
			randnum -= curr;
			if (randnum <= 0)
			{
				Ref = m_sceneLevelDataList[m_currLevel].m_EnemyTypes[temp];
				break;
			}
			temp++;
		}
		if (Ref == "") {
			cerr << "No enemies generated!\n";
			//throw std::exception();
		}

		m_combatNames.push_back(Ref);

		if (rand() % 100 < 10)
		{
			break;
		}
	}


	return m_combatNames;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/***************************************************************************************************************************************

		  _____                    _____                    _____                    _____                    _____                    _____
		 /\    \                  /\    \                  /\    \                  /\    \                  /\    \                  /\    \
		/::\    \                /::\    \                /::\____\                /::\    \                /::\    \                /::\    \
	   /::::\    \              /::::\    \              /::::|   |               /::::\    \              /::::\    \              /::::\    \
	  /::::::\    \            /::::::\    \            /:::::|   |              /::::::\    \            /::::::\    \            /::::::\    \
	 /:::/\:::\    \          /:::/\:::\    \          /::::::|   |             /:::/\:::\    \          /:::/\:::\    \          /:::/\:::\    \
	/:::/__\:::\    \        /:::/__\:::\    \        /:::/|::|   |            /:::/  \:::\    \        /:::/__\:::\    \        /:::/__\:::\    \
   /::::\   \:::\    \      /::::\   \:::\    \      /:::/ |::|   |           /:::/    \:::\    \      /::::\   \:::\    \      /::::\   \:::\    \
  /::::::\   \:::\    \    /::::::\   \:::\    \    /:::/  |::|   | _____    /:::/    / \:::\    \    /::::::\   \:::\    \    /::::::\   \:::\    \
 /:::/\:::\   \:::\____\  /:::/\:::\   \:::\    \  /:::/   |::|   |/\    \  /:::/    /   \:::\ ___\  /:::/\:::\   \:::\    \  /:::/\:::\   \:::\____\
/:::/  \:::\   \:::|    |/:::/__\:::\   \:::\____\/:: /    |::|   /::\____\/:::/____/     \:::|    |/:::/__\:::\   \:::\____\/:::/  \:::\   \:::|    |
\::/   |::::\  /:::|____|\:::\   \:::\   \::/    /\::/    /|::|  /:::/    /\:::\    \     /:::|____|\:::\   \:::\   \::/    /\::/   |::::\  /:::|____|
 \/____|:::::\/:::/    /  \:::\   \:::\   \/____/  \/____/ |::| /:::/    /  \:::\    \   /:::/    /  \:::\   \:::\   \/____/  \/____|:::::\/:::/    /
	   |:::::::::/    /    \:::\   \:::\    \              |::|/:::/    /    \:::\    \ /:::/    /    \:::\   \:::\    \            |:::::::::/    /
	   |::|\::::/    /      \:::\   \:::\____\             |::::::/    /      \:::\    /:::/    /      \:::\   \:::\____\           |::|\::::/    /
	   |::| \::/____/        \:::\   \::/    /             |:::::/    /        \:::\  /:::/    /        \:::\   \::/    /           |::| \::/____/
	   |::|  ~|               \:::\   \/____/              |::::/    /          \:::\/:::/    /          \:::\   \/____/            |::|  ~|
	   |::|   |                \:::\    \                  /:::/    /            \::::::/    /            \:::\    \                |::|   |
	   \::|   |                 \:::\____\                /:::/    /              \::::/    /              \:::\____\               \::|   |
		\:|   |                  \::/    /                \::/    /                \::/____/                \::/    /                \:|   |
		 \|___|                   \/____/                  \/____/                  ~~                       \/____/                  \|___|

***************************************************************************************************************************************/
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SceneLevelBuilder::render()
{
	// Set the background to black.
	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);

	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);

	// Set the the color to multiply to white, so that the sprite can 
	// display the full range of colors (default is black).
	AEGfxSetColorToMultiply(m_lighting.r, m_lighting.g, m_lighting.b, m_lighting.a);

	// Set the color to add to nothing, so that we don't alter the sprite's color
	AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 1.0f);

	// Set blend mode to AE_GFX_BM_BLEND
	// This will allow transparency.
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);

	/////////////////////////////////////////////////////////////////////////////////////////////////
	//SKY RENDER
	{
		AEGfxSetTransparency(1.0f);
		//Sky
		AEGfxSetTransform(m_transformSkyData.m);
		AEGfxTextureSet(RenderHelper::getInstance()->getTextureByRef("SKY_TEST_1"), 0, 0);
		AEGfxMeshDraw(RenderHelper::getInstance()->getdefaultMesh(), AE_GFX_MDM_TRIANGLES);

		/////////////////////////////////////////////////////////////////////////////////
		//Sun
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		AEGfxSetColorToAdd(1.0f, 1.0f, 1.0f, 1.0f);
		AEGfxSetTransparency(1.0f);
		AEGfxSetTransform(m_transformSunData.m);
		AEGfxMeshDraw(RenderHelper::getInstance()->getdefaultMesh(), AE_GFX_MDM_TRIANGLES);
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxSetTransparency(1.0f);
		AEGfxTextureSet(RenderHelper::getInstance()->getTextureByRef("SUN_OVERLAY_1"), 0, 0);
		AEGfxSetTransform(m_transformSunOverlayData.m);
		AEGfxMeshDraw(RenderHelper::getInstance()->getdefaultMesh(), AE_GFX_MDM_TRIANGLES);

		//////////////////////////////////////////////////////////////////////////////////////
		//Cloud
		//First Layer
		AEGfxSetTransform(m_transformCloudsData[0].m);
		AEGfxTextureSet(RenderHelper::getInstance()->getTextureByRef("SKY_TEST_2"), 0, 0);
		AEGfxMeshDraw(RenderHelper::getInstance()->getdefaultMesh(), AE_GFX_MDM_TRIANGLES);
		AEGfxSetTransform(m_transformCloudsData[1].m);
		AEGfxTextureSet(RenderHelper::getInstance()->getTextureByRef("SKY_TEST_2"), 0, 0);
		AEGfxMeshDraw(RenderHelper::getInstance()->getdefaultMesh(), AE_GFX_MDM_TRIANGLES);
		AEGfxSetTransform(m_transformCloudsData[2].m);
		AEGfxTextureSet(RenderHelper::getInstance()->getTextureByRef("SKY_TEST_2"), 0, 0);
		AEGfxMeshDraw(RenderHelper::getInstance()->getdefaultMesh(), AE_GFX_MDM_TRIANGLES);
		//Second Layer
		AEGfxSetTransform(m_transformCloudsData[3].m);
		AEGfxTextureSet(RenderHelper::getInstance()->getTextureByRef("SKY_TEST_3"), 0, 0);
		AEGfxMeshDraw(RenderHelper::getInstance()->getdefaultMesh(), AE_GFX_MDM_TRIANGLES);
		AEGfxSetTransform(m_transformCloudsData[4].m);
		AEGfxTextureSet(RenderHelper::getInstance()->getTextureByRef("SKY_TEST_3"), 0, 0);
		AEGfxMeshDraw(RenderHelper::getInstance()->getdefaultMesh(), AE_GFX_MDM_TRIANGLES);
		AEGfxSetTransform(m_transformCloudsData[5].m);
		AEGfxTextureSet(RenderHelper::getInstance()->getTextureByRef("SKY_TEST_3"), 0, 0);
		AEGfxMeshDraw(RenderHelper::getInstance()->getdefaultMesh(), AE_GFX_MDM_TRIANGLES);
		//Third Layer
		AEGfxSetTransform(m_transformCloudsData[6].m);
		AEGfxTextureSet(RenderHelper::getInstance()->getTextureByRef("SKY_TEST_4"), 0, 0);
		AEGfxMeshDraw(RenderHelper::getInstance()->getdefaultMesh(), AE_GFX_MDM_TRIANGLES);
		AEGfxSetTransform(m_transformCloudsData[7].m);
		AEGfxTextureSet(RenderHelper::getInstance()->getTextureByRef("SKY_TEST_4"), 0, 0);
		AEGfxMeshDraw(RenderHelper::getInstance()->getdefaultMesh(), AE_GFX_MDM_TRIANGLES);
		AEGfxSetTransform(m_transformCloudsData[8].m);
		AEGfxTextureSet(RenderHelper::getInstance()->getTextureByRef("SKY_TEST_4"), 0, 0);
		AEGfxMeshDraw(RenderHelper::getInstance()->getdefaultMesh(), AE_GFX_MDM_TRIANGLES);
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////
	// BACKDROP RENDER
	{
		AEGfxTextureSet(RenderHelper::getInstance()->getTextureByRef("BACKDROP_1"), 0, 0);
		for (int i = 0; i < 9; i++)
		{
			AEGfxSetTransform(m_transformBackDrops1Data[i].m);
			AEGfxMeshDraw(RenderHelper::getInstance()->getdefaultMesh(), AE_GFX_MDM_TRIANGLES);
		}
		AEGfxTextureSet(RenderHelper::getInstance()->getTextureByRef("BACKDROP_2"), 0, 0);
		for (int i = 0; i < 5; i++)
		{
			AEGfxSetTransform(m_transformBackDrops2Data[i].m);
			AEGfxMeshDraw(RenderHelper::getInstance()->getdefaultMesh(), AE_GFX_MDM_TRIANGLES);
		}
		AEGfxTextureSet(RenderHelper::getInstance()->getTextureByRef("BACKDROP_3"), 0, 0);
		for (int i = 0; i < 5; i++)
		{
			AEGfxSetTransform(m_transformBackDrops3Data[i].m);
			AEGfxMeshDraw(RenderHelper::getInstance()->getdefaultMesh(), AE_GFX_MDM_TRIANGLES);
		}

	}
	/////////////////////////////////////////////////////////////////////////////////////////////////
	// FLOOR RENDER
	{
		AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 1.0f);

		//Main Floor

		for (int i = NUM_OF_TILES - 1; i > -1; i--)
		{
			if (m_floor[t_centerFloorNum][i].m_type == 0)
				AEGfxTextureSet(RenderHelper::getInstance()->getTextureByRef("FLOOR_CENTER_1"), 0, 0);
			else
				AEGfxTextureSet(RenderHelper::getInstance()->getTextureByRef("FLOOR_CENTER_2"), 0, 0);

			if (m_floor[t_centerFloorNum][i].m_isRender)
			{
				AEGfxSetTransform(m_floor[t_centerFloorNum][i].m_transformFloorCurr.m);
				AEGfxMeshDraw(RenderHelper::getInstance()->getdefaultMesh(), AE_GFX_MDM_TRIANGLES);
			}
		}

		////Left Floor
		for (int j = 0; j < SIZE_OF_FLOOR - (t_centerFloorNum + 1); j++)
		{
			for (int i = NUM_OF_TILES - 1; i > -1; i--)
			{
				if (m_floor[j][i].m_type == 0)
					AEGfxTextureSet(RenderHelper::getInstance()->getTextureByRef("FLOOR_LEFT_1"), 0, 0);
				else
					AEGfxTextureSet(RenderHelper::getInstance()->getTextureByRef("FLOOR_LEFT_2"), 0, 0);

				if (m_floor[j][i].m_isRender)
				{
					AEGfxSetTransform(m_floor[j][i].m_transformFloorCurr.m);
					AEGfxMeshDraw(RenderHelper::getInstance()->getdefaultMesh(), AE_GFX_MDM_TRIANGLES);
				}
			}
		}
		//Right Floor
		for (int j = (t_centerFloorNum + 1); j < SIZE_OF_FLOOR; j++)
		{
			for (int i = NUM_OF_TILES - 1; i > -1; i--)
			{
				if (m_floor[j][i].m_type == 0)
					AEGfxTextureSet(RenderHelper::getInstance()->getTextureByRef("FLOOR_RIGHT_1"), 0, 0);
				else
					AEGfxTextureSet(RenderHelper::getInstance()->getTextureByRef("FLOOR_RIGHT_2"), 0, 0);

				if (m_floor[j][i].m_isRender)
				{
					AEGfxSetTransform(m_floor[j][i].m_transformFloorCurr.m);
					AEGfxMeshDraw(RenderHelper::getInstance()->getdefaultMesh(), AE_GFX_MDM_TRIANGLES);
				}
			}
		}

	}
	/////////////////////////////////////////////////////////////////////////////////////////////////
	// FOG RENDER
	{
		AEGfxTextureSet(RenderHelper::getInstance()->getTextureByRef("FOG_1"), 0, 0);
		AEGfxSetTransform(m_transformFogData.m);
		AEGfxMeshDraw(RenderHelper::getInstance()->getdefaultMesh(), AE_GFX_MDM_TRIANGLES);
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////
	// SCENEOBJ RENDER
	{
		//Render Left Side
		for (int j = 0; j < SIZE_OF_FLOOR / 2; j++)
		{
			for (int i = 0; i < NUM_OF_TILES - 1; i++)
			{
				////////////////////////////////////////////////
				// m_currentTileNumFurthest = 4
				// -> 4 3 2 1 0 9 8 7 6 5 -> Render in this way
				////////////////////////////////////////////////
				int tempTileNum = m_currentTileNumFurthest - i;
				if (tempTileNum < 0)
					tempTileNum += NUM_OF_TILES - 1;

				for (std::list<v_SceneObject>::iterator it = m_floorOBJs[j][tempTileNum].begin();
					it != m_floorOBJs[j][tempTileNum].end();
					it++)
				{
					AEGfxTextureSet(RenderHelper::getInstance()->getTextureByRef((*it).m_type), 0, 0);
					AEGfxSetTransparency((*it).m_Transparency);
					AEGfxSetTransform((*it).m_TransformData.m);
					AEGfxMeshDraw(RenderHelper::getInstance()->getdefaultMesh(), AE_GFX_MDM_TRIANGLES);
				}
			}
		}
		//Render Right Side
		for (int j = SIZE_OF_FLOOR - 1; j >= SIZE_OF_FLOOR / 2; j--)
		{
			for (int i = 0; i < NUM_OF_TILES - 1; i++)
			{
				////////////////////////////////////////////////
				// m_currentTileNumFurthest = 4
				// -> 4 3 2 1 0 9 8 7 6 5 -> Render in this way
				////////////////////////////////////////////////
				int tempTileNum = m_currentTileNumFurthest - i;
				if (tempTileNum < 0)
					tempTileNum += NUM_OF_TILES - 1;

				for (std::list<v_SceneObject>::iterator it = m_floorOBJs[j][tempTileNum].begin();
					it != m_floorOBJs[j][tempTileNum].end();
					it++)
				{
					AEGfxTextureSet(RenderHelper::getInstance()->getTextureByRef((*it).m_type), 0, 0);
					AEGfxSetTransparency((*it).m_Transparency);
					AEGfxSetTransform((*it).m_TransformData.m);
					AEGfxMeshDraw(RenderHelper::getInstance()->getdefaultMesh(), AE_GFX_MDM_TRIANGLES);
				}
			}
		}
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////
	// Light Flare
	if (m_sceneLevelDataList[m_currLevel].m_DayTime)
	{
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		static f32 transparency[8] = { 1.07f, -0.75f, 0.2f, -0.05f , -0.36f, 0.9f ,1.1f,2.2f };
		//Lens Flare
		AEGfxSetTransparency(transparency[7]);
		AEGfxTextureSet(RenderHelper::getInstance()->getTextureByRef("SUN_LENS_1"), 0, 0);
		AEGfxSetTransform(m_transformSunLensData[7].m);
		AEGfxMeshDraw(RenderHelper::getInstance()->getdefaultMesh(), AE_GFX_MDM_TRIANGLES);
		AEGfxSetTransparency(transparency[6]);
		AEGfxTextureSet(RenderHelper::getInstance()->getTextureByRef("SUN_LENS_2"), 0, 0);
		AEGfxSetTransform(m_transformSunLensData[6].m);
		AEGfxMeshDraw(RenderHelper::getInstance()->getdefaultMesh(), AE_GFX_MDM_TRIANGLES);
		AEGfxSetTransparency(transparency[5]);
		AEGfxTextureSet(RenderHelper::getInstance()->getTextureByRef("SUN_LENS_2"), 0, 0);
		AEGfxSetTransform(m_transformSunLensData[5].m);
		AEGfxMeshDraw(RenderHelper::getInstance()->getdefaultMesh(), AE_GFX_MDM_TRIANGLES);
		AEGfxSetTransparency(transparency[4]);
		AEGfxTextureSet(RenderHelper::getInstance()->getTextureByRef("SUN_LENS_2"), 0, 0);
		AEGfxSetTransform(m_transformSunLensData[4].m);
		AEGfxMeshDraw(RenderHelper::getInstance()->getdefaultMesh(), AE_GFX_MDM_TRIANGLES);
		AEGfxSetTransparency(transparency[3]);
		AEGfxTextureSet(RenderHelper::getInstance()->getTextureByRef("SUN_LENS_2"), 0, 0);
		AEGfxSetTransform(m_transformSunLensData[3].m);
		AEGfxMeshDraw(RenderHelper::getInstance()->getdefaultMesh(), AE_GFX_MDM_TRIANGLES);
		AEGfxSetTransparency(transparency[2]);
		AEGfxTextureSet(RenderHelper::getInstance()->getTextureByRef("SUN_LENS_3"), 0, 0);
		AEGfxSetTransform(m_transformSunLensData[2].m);
		AEGfxMeshDraw(RenderHelper::getInstance()->getdefaultMesh(), AE_GFX_MDM_TRIANGLES);
		AEGfxSetTransparency(transparency[1]);
		AEGfxTextureSet(RenderHelper::getInstance()->getTextureByRef("SUN_LENS_4"), 0, 0);
		AEGfxSetTransform(m_transformSunLensData[1].m);
		AEGfxMeshDraw(RenderHelper::getInstance()->getdefaultMesh(), AE_GFX_MDM_TRIANGLES);
		AEGfxSetTransparency(transparency[0]);
		AEGfxTextureSet(RenderHelper::getInstance()->getTextureByRef("SUN_LENS_5"), 0, 0);
		AEGfxSetTransform(m_transformSunLensData[0].m);
		AEGfxMeshDraw(RenderHelper::getInstance()->getdefaultMesh(), AE_GFX_MDM_TRIANGLES);
	}

	///////////////////////////////////////////////////////////////////////////////////////////
	//MASSIVE FOG AT LEVEL 7 (REMOVED FROM FINAL PRODUCT DUE TO PROBLEMS WITH RENDERING)
	//{
	//	static double t_Transparency = -1.1;
	//	if (m_currLevel == 7)
	//	{
	// 		/////////////////////////////////////////////////////////////////////////
	//   // RESET SETTINGS
	//   	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	//   	AEGfxSetColorToMultiply(m_lighting.r, m_lighting.g, m_lighting.b, m_lighting.a);
	//   	AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 1.0f);
	//   	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	//		static double t_Transparency = -1.1;
	//		t_Transparency += AEFrameRateControllerGetFrameTime() / LERPING_SPEED;
	//		AEMtx33 transform;
	//		AEMtx33Identity(&transform);
	//		AEMtx33ScaleApply(&transform, &transform, 1800, 1440);
	//		AEMtx33TransApply(&transform, &transform, 0, 315);
	//		AEGfxTextureSet(RenderHelper::getInstance()->getTextureByRef("FOG_2"), 0, 0);
	//		AEGfxSetTransparency(t_Transparency);
	//		AEGfxSetTransform(transform.m);
	//		AEGfxMeshDraw(RenderHelper::getInstance()->getdefaultMesh(), AE_GFX_MDM_TRIANGLES);
	//	}
	//	else
	//		t_Transparency = -1.1;
	//}

	/////////////////////////////////////////////////////////////////////////////////////////////////
	// Combat Render
	if (m_combatPhase)
		CombatScene::getInstance().render();
	/////////////////////////////////////////////////////////////////////////////////////////////////
	// UI / MISC RENDER
	{
		updateGlobals();//Had to force update to update the positions to as correct as possible

		//Lvl Name
		RenderLvlName();

		//Border
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 1.0f);
		AEGfxSetTransparency(1.0);
		AEMtx33 t_curr;
		AEMtx33Identity(&t_curr);
		AEMtx33ScaleApply(&t_curr, &t_curr, 99999, 90);
		AEMtx33TransApply(&t_curr, &t_curr, camOffset.x, -AEGfxGetWindowHeight() / 2 + camOffset.y);
		AEGfxSetTransform(t_curr.m);
		AEGfxMeshDraw(RenderHelper::getInstance()->getdefaultMesh(), AE_GFX_MDM_TRIANGLES);
		AEMtx33Identity(&t_curr);
		AEMtx33ScaleApply(&t_curr, &t_curr, 99999, 90);
		AEMtx33TransApply(&t_curr, &t_curr, camOffset.x, AEGfxGetWindowHeight() / 2 + camOffset.y);
		AEGfxSetTransform(t_curr.m);
		AEGfxMeshDraw(RenderHelper::getInstance()->getdefaultMesh(), AE_GFX_MDM_TRIANGLES);

		//Screen Transition
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 1.0f);
		AEGfxSetTransparency(m_currTransitionTransparency);
		AEMtx33Identity(&t_curr);
		AEMtx33ScaleApply(&t_curr, &t_curr, 99999, 99999);
		AEGfxSetTransform(t_curr.m);
		AEGfxMeshDraw(RenderHelper::getInstance()->getdefaultMesh(), AE_GFX_MDM_TRIANGLES);

	}
	/////////////////////////////////////////////////////////////////////////////////////////////////
	// GAMEOBJ RENDER
	GameObjectManager::getInstance()->render();

	////////////////////////////////////////////////////////////////////////////////////////////////
	// UI / MISC RENDER PART 2
	{
		// down here because player should be drawn on top of everything else, save pause screen
		if (!m_combatPhase) {
			player->render();
		}
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////
	// show game end screen if no more levels
	if (showGameEnd) {
		//cout << "show game end screen\n";

		float opacity = elapsedTime / SHOW_GAME_END_DURATION;
		// cover screen with very large rect
		RenderHelper::getInstance()->rect("invis", 0, 0, 9999, 9999, 0, Color{ 0,0,0, opacity }, opacity);
		RenderHelper::getInstance()->text("Thanks for playing!", AEGfxGetWindowWidth() / 2.f, AEGfxGetWindowHeight() / 2.f);
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////
	// UI / MISC RENDER PART 3
	{
		Pause::getInstance().render();
	}

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/***************************************************************************************************************************************
	  _____                    _____                    _____                            _____                    _____                    _____
	 /\    \                  /\    \                  /\    \                          /\    \                  /\    \                  /\    \
	/::\    \                /::\____\                /::\    \                        /::\    \                /::\____\                /::\    \
	\:::\    \              /:::/    /               /::::\    \                      /::::\    \              /::::|   |               /::::\    \
	 \:::\    \            /:::/    /               /::::::\    \                    /::::::\    \            /:::::|   |              /::::::\    \
	  \:::\    \          /:::/    /               /:::/\:::\    \                  /:::/\:::\    \          /::::::|   |             /:::/\:::\    \
	   \:::\    \        /:::/____/               /:::/__\:::\    \                /:::/__\:::\    \        /:::/|::|   |            /:::/  \:::\    \
	   /::::\    \      /::::\    \              /::::\   \:::\    \              /::::\   \:::\    \      /:::/ |::|   |           /:::/    \:::\    \
	  /::::::\    \    /::::::\    \   _____    /::::::\   \:::\    \            /::::::\   \:::\    \    /:::/  |::|   | _____    /:::/    / \:::\    \
	 /:::/\:::\    \  /:::/\:::\    \ /\    \  /:::/\:::\   \:::\    \          /:::/\:::\   \:::\    \  /:::/   |::|   |/\    \  /:::/    /   \:::\ ___\
	/:::/  \:::\____\/:::/  \:::\    /::\____\/:::/__\:::\   \:::\____\        /:::/__\:::\   \:::\____\/:: /    |::|   /::\____\/:::/____/     \:::|    |
   /:::/    \::/    /\::/    \:::\  /:::/    /\:::\   \:::\   \::/    /        \:::\   \:::\   \::/    /\::/    /|::|  /:::/    /\:::\    \     /:::|____|
  /:::/    / \/____/  \/____/ \:::\/:::/    /  \:::\   \:::\   \/____/          \:::\   \:::\   \/____/  \/____/ |::| /:::/    /  \:::\    \   /:::/    /
 /:::/    /                    \::::::/    /    \:::\   \:::\    \               \:::\   \:::\    \              |::|/:::/    /    \:::\    \ /:::/    /
/:::/    /                      \::::/    /      \:::\   \:::\____\               \:::\   \:::\____\             |::::::/    /      \:::\    /:::/    /
\::/    /                       /:::/    /        \:::\   \::/    /                \:::\   \::/    /             |:::::/    /        \:::\  /:::/    /
 \/____/                       /:::/    /          \:::\   \/____/                  \:::\   \/____/              |::::/    /          \:::\/:::/    /
							  /:::/    /            \:::\    \                       \:::\    \                  /:::/    /            \::::::/    /
							 /:::/    /              \:::\____\                       \:::\____\                /:::/    /              \::::/    /
							 \::/    /                \::/    /                        \::/    /                \::/    /                \::/____/
							  \/____/                  \/____/                          \/____/                  \/____/                  ~~

***************************************************************************************************************************************/
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SceneLevelBuilder::exit()
{
	//Clear Floor
	for (int i = 0; i < SIZE_OF_FLOOR; i++)
	{
		delete[] m_floor[i];
		delete[] m_floorOBJs[i];
	}
	delete[] m_floor;
	delete[] m_floorOBJs;

	for (int i = 0; i < NUM_OF_TILESPAWNPOINTS; i++)
	{
		delete[] m_tileSP[i];
	}
	delete[] m_tileSP;

	//Clear Object in scene
	GameObjectManager::getInstance()->exit();
	GameObjectManager::getInstance()->destroy();

	delete[] m_sceneLevelDataList;

	//Destroy Font
	AEGfxDestroyFont(m_pTextFont);

	CombatScene::getInstance().exit();

	cleanupSlb();
}


/*******************************************************************************************************************************************
*                                                THE END OF THE FILE ;)
*                  Good morning, and in case I don't see ya, good afternoon, good evening, and good night!
*******************************************************************************************************************************************/