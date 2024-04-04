/* Start Header ************************************************************************/
/*!
\file SceneLevelBuilder.cpp
\author Soh Wei Jie, weijie.soh, 2301289
\par weijie.soh\@digipen.edu
\date 21 Feb 2024
\brief The 3D engine, the heart of the game and my pain.

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

namespace {
	//First data is left, second data is right
	std::pair<AEMtx33, AEMtx33> Hand1PosData{};
	std::pair<AEMtx33, AEMtx33> Hand2PosData{};
	std::pair<AEMtx33, AEMtx33> Hand3PosData{};
	std::pair<AEMtx33, AEMtx33> Hand4PosData{};

	static int t_AnimationFrame = 0;
	static double t_AnimationDuration = 0.0;
	static bool LeftSide = false;
	double LerpSpeed = 10.0;
	AEVec2 targetPos{};
	f32 camX, camY;

	int PLAYER_BASE_HEALTH = static_cast<int>(Database::getInstance()->data["player"]["baseHealth"]);
	int PLAYER_BASE_DAMAGE = static_cast<int>(Database::getInstance()->data["player"]["baseDamage"]);

	bool showGameEnd = false;
}

bool GameScene::combatAudioLoopIsPlaying = false;
bool GameScene::afterInit = false;

SceneLevelBuilder::v_FloorData::v_FloorData() :
	m_currFloorNum{ 0 },
	m_FloorNum{ 0 },
	m_currFloorTimer{ 0.0 },
	m_FloorSpeedTimer{ 0.5 },
	m_IsRender{ true },
	m_Type{0}
{
	AEMtx33Identity(&m_TransformFloorData);
	AEMtx33Identity(&m_TransformFloorCurr);
	AEMtx33Identity(&m_currFloorSpeed);
	AEMtx33Identity(&m_Scale);
	AEMtx33Identity(&m_Trans);
	AEMtx33Identity(&m_OriginalTrans);
}
SceneLevelBuilder::v_SceneObject::v_SceneObject()
	:m_TexRef{ "" },
	m_RenderOrder{ 0 },
	m_Transparency{ -1.5f },
	m_Type{ v_SceneObjectTypes::EType_Grass_Foliage_1 },
	m_tobeCentered {false}
{
	AEMtx33Identity(&m_TransformData);
	AEMtx33Identity(&m_Scale);
	AEMtx33Identity(&m_Trans);
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
	m_EnemySpawnRate{0}{}

SceneLevelBuilder::SceneLevelBuilder() :
	m_StopMovement{ false },
	m_PanCloseToGround{ false },
	m_CompletionStatus{ 98 },
	m_currLevel{ 0 },
	m_LvlNameTimer{ 0.0 },
	m_LvlNameTransparency{ 0.0 },
	m_currTransitionTransparency{ 1.0 },
	m_setTransitionTransparency{ -1.0 },
	m_SceneEnemy{ nullptr },
	m_CombatPhase{ false },
	m_CombatAnimationComp{ false },
	m_CombatBufferingTime{ 0.0 },
	m_Lighting {1.0f,1.0f,1.0f,1.0f}
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
		pTextFont = AEGfxCreateFont("Assets/Fonts/TokyoMidnight.otf", 50);

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
		m_Floor = new v_FloorData * [SIZE_OF_FLOOR];
		m_FloorOBJs = new std::list<v_SceneObject>*[SIZE_OF_FLOOR];
		for (int i = 0; i < SIZE_OF_FLOOR; i++)
		{
			m_Floor[i] = new v_FloorData[NUM_OF_TILES];
			m_FloorOBJs[i] = new std::list<v_SceneObject>[NUM_OF_TILES];
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
		t_CenterFloorNum = static_cast<int>(SIZE_OF_FLOOR / 2);
	}


	//////////////////////////////////////////////////////////////////////////////////////////////
	//                       Load data from Json
	/*
	*     Load All Level Data from Json, for instance, enemies to spawn and title name and objs
	*/
	//////////////////////////////////////////////////////////////////////////////////////////////
	{
		if (Database::getInstance()->data["levels"].size() > 0)
		{
			m_SceneLevelDataList = new v_SceneLevelData[Database::getInstance()->data["levels"].size()];
			m_MAXLevel = static_cast<int>(Database::getInstance()->data["levels"].size()) - 1;
			for (int i = 0; i < Database::getInstance()->data["levels"].size(); i++)
			{
				v_SceneLevelData t_curr{};
				t_curr.m_LevelName = Database::getInstance()->data["levels"][i]["levelName"];
				t_curr.m_LevelCompletionRate = Database::getInstance()->data["levels"][i]["levelCompletionRate"];
				t_curr.m_Completed = Database::getInstance()->data["levels"][i]["completed"];
				t_curr.m_EnemySpawnRate = Database::getInstance()->data["levels"][i]["enemySpawnRate"];
				t_curr.m_Unlocked = Database::getInstance()->data["levels"][i]["unlocked"]; 
				t_curr.m_MaxEnemies = Database::getInstance()->data["levels"][i]["maxEnemies"];
				t_curr.m_DayTime = Database::getInstance()->data["levels"][i]["DayTime"];

				for (auto& map : Database::getInstance()->data["levels"][i]["enemySpawnWeight"].items())
				{
					for (auto type = map.value().begin(); type != map.value().end(); type++)
					{
						t_curr.m_EnemyTypes.push_back(type.key());
						t_curr.m_EnemySpawnWeight.push_back(type.value());
					}
				}

				for (auto& map : Database::getInstance()->data["levels"][i]["SceneOBJSpawnWeight"].items())
				{
					for (auto type = map.value().begin(); type != map.value().end(); type++)
					{
						t_curr.m_SceneObjTypes.push_back(type.key());
						t_curr.m_SceneObjSpawnWeight.push_back(type.value());
					}
				}

				*m_SceneLevelDataList = t_curr;
				m_SceneLevelDataList++;
			}
			m_SceneLevelDataList -= Database::getInstance()->data["levels"].size();
		}
	}
	

	//////////////////////////////////////////////////////////////////////////////////////////////
	//                       Load Combat elements
	//////////////////////////////////////////////////////////////////////////////////////////////
	CombatScene::getInstance().Load();

	//////////////////////////////////////////////////////////////////////////////////////////////
	//                       Initialise 3D engine & relevant values
	//////////////////////////////////////////////////////////////////////////////////////////////// 
	Init();
}
SceneLevelBuilder::~SceneLevelBuilder()
{
	Exit(); //Hehe, dont allow sneaky memory leak
}

// !TODO: jspoh showGameEnd = false; reset this. my visual studio is bugged cant expand
void SceneLevelBuilder::Init()
{
	/////////////////////////////////////////////////////////////
	// Basic Setup of all necessary objects in scene
	{
		player = std::make_unique<Player>(Player((float)PLAYER_BASE_HEALTH, (float)PLAYER_BASE_DAMAGE));
		/////////////////////////////////////////////////////////////
		//Creating GameObjects
		Create::MiscEnemy();
		for (int i = 0; i < 10; i++)
		{
			Create::Projectiles();
		}

		m_CompletionStatus = 98;
		m_currLevel = SceneMenu::sInstance->m_SelectedLevel;
		m_Lighting = { 1.0f,1.0f,1.0f,1.0f };
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
					AEMtx33Scale(&m_Floor[j][i].m_Scale, 8000.f, 1262.f);
					AEMtx33Trans(&m_Floor[j][i].m_Trans, 16000.0f * static_cast<f32>(j - t_CenterFloorNum), -2829.0f);
					break;
				case 1:
					AEMtx33Scale(&m_Floor[j][i].m_Scale, 7000.f, 1262.f);
					AEMtx33Trans(&m_Floor[j][i].m_Trans, 5750.0f * static_cast<f32>(j - t_CenterFloorNum), -2229.0f);
					break;
				case 2:
					AEMtx33Scale(&m_Floor[j][i].m_Scale, 6000.f, 1262.f);
					AEMtx33Trans(&m_Floor[j][i].m_Trans, 4350.0f * static_cast<f32>(j - t_CenterFloorNum), -1629.0f);
					break;
					//First Floor
				case 3:
					AEMtx33Scale(&m_Floor[j][i].m_Scale, 2940.f, 616.f);
					AEMtx33Trans(&m_Floor[j][i].m_Trans, 2150.0f * static_cast<f32>(j - t_CenterFloorNum), -696.0f);
					break;
					//Second Floor
				case 4:
					AEMtx33Scale(&m_Floor[j][i].m_Scale, 1593.0f, 339.f);
					AEMtx33Trans(&m_Floor[j][i].m_Trans, 1150.0f * static_cast<f32>(j - t_CenterFloorNum), -282.0f);
					break;
					//Third floor
				case 5:
					AEMtx33Scale(&m_Floor[j][i].m_Scale, 779.0f, 133.f);
					AEMtx33Trans(&m_Floor[j][i].m_Trans, 555.0f * static_cast<f32>(j - t_CenterFloorNum), -50.0f);
					break;
					//Fourth floor
				case 6:
					AEMtx33Scale(&m_Floor[j][i].m_Scale, 381.0f, 47.f);
					AEMtx33Trans(&m_Floor[j][i].m_Trans, 270.0f * static_cast<f32>(j - t_CenterFloorNum), 39.0f);
					break;
					//Fifth floor
				case 7:
					AEMtx33Scale(&m_Floor[j][i].m_Scale, 181.0f, 14.f);
					AEMtx33Trans(&m_Floor[j][i].m_Trans, 130.0f * static_cast<f32>(j - t_CenterFloorNum), 69.0f);
					break;
					//Sixth floor
				case 8:
					AEMtx33Scale(&m_Floor[j][i].m_Scale, 85.0f, 4.f);
					AEMtx33Trans(&m_Floor[j][i].m_Trans, 59.0f * static_cast<f32>(j - t_CenterFloorNum), 78.0f);
					break;
					//Seventh floor
				case 9:
					AEMtx33Scale(&m_Floor[j][i].m_Scale, 33.0f, 1.f);
					AEMtx33Trans(&m_Floor[j][i].m_Trans, 25.0f * static_cast<f32>(j - t_CenterFloorNum), 80.0f);
					break;
				default:
					cout << "Error pls check floor" << "\n";
					break;
				}
				m_Floor[j][i].m_FloorNum = i;
				m_Floor[j][i].m_OriginalTrans = m_Floor[j][i].m_Trans;
				AEMtx33Concat(&m_Floor[j][i].m_TransformFloorData, &m_Floor[j][i].m_Trans, &m_Floor[j][i].m_Scale);
				m_Floor[j][i].m_currFloorNum = i;
				//Setting Movement Point To
				m_Floor[j][i].m_TransformFloorCurr = m_Floor[j][i].m_TransformFloorData;
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
		AEMtx33Concat(&m_TransformSkyData, &trans, &scale);
		for (int i = 0; i < 9; i++)
		{
			m_TransformCloudsData.push_back(m_temp);
			AEMtx33ScaleApply(&m_TransformCloudsData[i], &m_TransformCloudsData[i], 1700.0f, 600.f);
			AEMtx33TransApply(&m_TransformCloudsData[i], &m_TransformCloudsData[i], (i % 3 - 1) * 1700.0f, 220.f);
		}

		////////////////////////////////////////////////////////////////////////
		//Mountain, hills, whatnot
		//A Quick fix for retaining the previous transform of the backdrops
		for (int i = 0; i < 9; i++)
		{
			m_TransformBackDrops1Data.push_back(m_temp);
			m_TransformBackDrops2Data.push_back(m_temp);
			m_TransformBackDrops3Data.push_back(m_temp);
		}

		////////////////////////////////////////////////////////////////////////
		//Sun
		m_sunOverlayScale = { 120.f, 120.f };
		m_sunPos = { 350, 350 };
		AEMtx33Scale(&scale, 50.0f, 50.f);
		AEMtx33Trans(&trans, m_sunPos.x, m_sunPos.y);
		AEMtx33Concat(&m_TransformSunData, &trans, &scale);
		AEMtx33Scale(&scale, m_sunOverlayScale.x, m_sunOverlayScale.y);
		AEMtx33Trans(&trans, m_sunPos.x, m_sunPos.y);
		AEMtx33Concat(&m_TransformSunOverlayData, &trans, &scale);
		for (int i = 0; i < 8; i++)
			m_TransformSunLensData.push_back(m_temp);

		////////////////////////////////////////////////////////////////////////
		//Fog
		AEMtx33Scale(&scale, 2000.0f, 70.f);
		AEMtx33Trans(&trans, 0, 80);
		AEMtx33Concat(&m_TransformFogData, &trans, &scale);
	}
}

void SceneLevelBuilder::Update(double dt)
{
	if (!SceneStages::sInstance->m_StartGame) dt *= 3; //JUST A SPEED UP TO WARM UP THE ENGINE BEFORE GAMEPLAY

	/////////////////////////////////////////////////////////////////////////////////
	// Generic Update calls can be placed here
	{
		updateGlobals();
		player->update(dt);
		UpdateScreenTransition(static_cast<f32>(dt)); // Screen Basic Transition
		Pause::getInstance().update(dt); //Pause
		if (Pause::getInstance().isPaused) {
			return;
		}
		UpdateLvlName(static_cast<f32>(dt));//Level Name Animation
		///////////////////////////////////////////////////////////////////////////////
		//Level Progression Update
		if(!m_CombatPhase)
		{
			if (m_CompletionStatus > 100.0 && SceneStages::sInstance->m_StartGame)
			{
				if (m_currLevel < m_MAXLevel) {
					++m_currLevel;
				}
				else {
					showGameEnd = true;
					if (AEInputCheckTriggered(AEVK_SPACE)) {
						SceneManager::GetInstance()->SetActiveScene("SceneMenu");
						return;			// terminate this scene state early
					}
				}
				//else
				//	m_currLevel; //ALL LEVELS DONE
				SceneLevelBuilder::SpawnLvlName();
				m_CompletionStatus = 0.0;
			}

			if (!showGameEnd) {
				m_CompletionStatus += SceneStages::sInstance->m_StartGame ? dt * m_SceneLevelDataList[m_currLevel].m_LevelCompletionRate : 0.0;
				if (AEInputCheckCurr(AEVK_1))
					m_CompletionStatus += SceneStages::sInstance->m_StartGame ? dt * m_SceneLevelDataList[m_currLevel].m_LevelCompletionRate * 50 : 0.0;
			}
			cout << "Level Done: " << m_CompletionStatus << "%\n";

		}
		UpdateLevelGameplay(static_cast<float>(dt));
		UpdateLensFlare(static_cast<float>(dt));
		UpdateClouds(static_cast<float>(dt));
		UpdateBackdrop(static_cast<float>(dt));
	}

	////////////////////////////////////////////////////////////////////////
	//Adjustments to camera if necessary
	{
		/**
		 * this set cam positioning is messing up the camera positioning
		 * for everything used after this. uncomment the code to see the print logs
		 * - JS
		 * 
		 */
		//updateGlobals();
		//cout << mouseX << "," << mouseY << " | " << camOffset.x << "," << camOffset.y << "\n";
		//AEGfxSetCamPosition(camX, camY - static_cast<f32>(m_PanDownCam));
		//updateGlobals();
		//cout << mouseX << "," << mouseY << " | " << camOffset.x << "," << camOffset.y << "\n";
		//cout << "\n";
	}

	////////////////////////////////////////////////////////////////////////////////
	// Combat & Player with Scene Interaction
	{
		static int t_whoseTurn = 0;
		if (!m_CombatPhase)
		{
			////////////////////////////////////////////////////////////////////////////
			// Player INPUT
			{
				if (AEInputCheckTriggered(AEVK_LBUTTON))
				{
					player->setHandStateAnimationType(Player::HandAnimationType::Punch);
				}
				//Not planning to implement for now
				//else if (AEInputCheckCurr(AEVK_RBUTTON)) {
				//	player->setHandStateAnimationType(Player::HandAnimationType::Ready);
				//}
				//else if (AEInputCheckTriggered(AEVK_SPACE))
				//{
				//	player->setHandStateAnimationType(Player::HandAnimationType::Block);
				//}
			}

			/////////////////////////////////////////////////////////////////////////////
			// Settings to lerp to the movement view
			{
				if (m_currTransitionTransparency >= 0.5f)
				{
					m_CombatBufferingTime -= m_CombatBufferingTime > 0.0 ? static_cast<f32>(dt) : 0;
					if (m_CombatBufferingTime > 0.2f)
					{
						dt *= 15; //SPEEDUP SPECIFICALLY
						m_StopMovement = false;
						m_PanCloseToGround = false;
						m_PanCloseToGroundValue += m_PanCloseToGroundValue < 80 ? static_cast<int>(LERPING_SPEED) : 0;
					}
				}
				if (m_CombatBufferingTime <= 0.0f)
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
				if (m_SceneEnemy != nullptr)
				{
					if (m_SceneEnemy->m_StartCombat)
					{
						switch (m_SceneEnemy->m_StartCombat)
						{
						case 1:
							m_SceneEnemy->m_Active = false;
							m_SceneEnemy = nullptr;
							m_CombatBufferingTime = 2.0f;
							CombatScene::getInstance().spawnEnemies(GenerateEnemyToSpawn());
							t_whoseTurn = CombatManager::PLAYER;
							m_CombatPhase = true;
							m_CombatAnimationComp = false;
							break;
						case 2:
							m_SceneEnemy->m_Active = false;
							m_SceneEnemy = nullptr;
							m_currTransitionTransparency = 1.0f;
							m_setTransitionTransparency = 1.0f;
							m_CombatBufferingTime = 0.8f;
							CombatScene::getInstance().spawnEnemies(GenerateEnemyToSpawn());
							t_whoseTurn = CombatManager::ENEMY;
							m_CombatPhase = true;
							m_CombatAnimationComp = false;
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
			if (m_CombatAnimationComp)
			{
				// check if combat is over and update accordingly
				bool prevCombatPhaseState = m_CombatPhase;
				m_CombatPhase = CombatManager::getInstance().isInCombat;

				// switch combat audio back to game audio
				if (prevCombatPhaseState && !m_CombatPhase) {
					SoundPlayer::stopAll();
					SoundPlayer::GameAudio::getInstance().playLoop();
					GameScene::combatAudioLoopIsPlaying = false;
					SceneStagesAudio::loopIsPlaying = true;
					
				}

				CombatScene::getInstance().Update(dt);
				if (!m_CombatPhase)
				{
					m_CombatBufferingTime = 0.6f;
					FadeINBlack();
					if (CombatScene::getInstance().getWinFlag())
						m_CompletionStatus += 30; // Increment Progression of level for completing the battle

				}
					
			}

			/////////////////////////////////////////////////////////////////////
			// Settings to lerp to the combat view
			{
				m_CombatBufferingTime -= m_CombatBufferingTime > 0.0 ? static_cast<f32>(dt) : 0;
				if (m_CombatBufferingTime < 0.0f && !m_CombatAnimationComp)
				{
					m_CombatAnimationComp = true;
					m_StopMovement = true;
					CombatScene::getInstance().Init((CombatManager::TURN)t_whoseTurn);
					FadeOutBlack();
				}
				//For faster setup
				if (m_CombatBufferingTime > 0.4f && !m_CombatAnimationComp)
				{
					if ((CombatManager::TURN)t_whoseTurn - 1) dt *= 5; //SPEEDUP SPECIFICALLY FOR ENEMY START TURN
				}

				m_PanCloseToGround = true;
				m_PanCloseToGroundValue -= m_PanCloseToGroundValue > 30 ? static_cast<int>(LERPING_SPEED) : 0;

				if (!GameScene::combatAudioLoopIsPlaying && m_CombatPhase) {
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
	if (!m_StopMovement)
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
			AEMtx33 m_LastFloorData = m_Floor[j][8].m_TransformFloorData;
			///////////////////////////////////////////////////////////////////////////
			// Checks from Up to Down
			///////////////////////////////////////////////////////////////////////////
			for (int i = NUM_OF_TILES - 1; i > -1; i--)
			{
				AEMtx33 m_NextFloorData = m_Floor[j][i].m_currFloorNum != 0 ? m_Floor[j][m_Floor[j][i].m_currFloorNum - 1].m_TransformFloorData : m_Floor[j][i].m_TransformFloorCurr = m_LastFloorData;
				AEMtx33 m_CurrFloorData = m_Floor[j][m_Floor[j][i].m_currFloorNum].m_TransformFloorData;

				//Minimum Speed of next floor
				AEMtx33 m_MinimumNextFloorSpeed = {
				(m_NextFloorData.m[0][0] - m_CurrFloorData.m[0][0]) / m_PanCloseToGroundValue,
				(m_NextFloorData.m[0][1] - m_CurrFloorData.m[0][1]) / 80,
				(m_NextFloorData.m[0][2] - m_CurrFloorData.m[0][2]) / m_PanCloseToGroundValue,
				(m_NextFloorData.m[1][0] - m_CurrFloorData.m[1][0]) / 80,
				(m_NextFloorData.m[1][1] - m_CurrFloorData.m[1][1]) / 80,
				(m_NextFloorData.m[1][2] - m_CurrFloorData.m[1][2]) / 80,
				(m_NextFloorData.m[2][0] - m_CurrFloorData.m[2][0]) / 80,
				(m_NextFloorData.m[2][1] - m_CurrFloorData.m[2][1]) / 80,
				(m_NextFloorData.m[2][2] - m_CurrFloorData.m[2][2]) / 80
				};

				//Incrementing speed
				m_Floor[j][i].m_currFloorSpeed.m[0][0] += m_Floor[j][i].m_currFloorSpeed.m[0][0] < m_MinimumNextFloorSpeed.m[0][0] ? static_cast<f32>(dt) * m_MinimumNextFloorSpeed.m[0][0] * TOP_MOVEMENT_SPEED : m_Floor[j][i].m_currFloorSpeed.m[0][0] > m_MinimumNextFloorSpeed.m[0][0] ? static_cast<f32>(dt) * m_MinimumNextFloorSpeed.m[0][0] * TOP_MOVEMENT_SPEED : 0;
				m_Floor[j][i].m_currFloorSpeed.m[0][1] += m_Floor[j][i].m_currFloorSpeed.m[0][1] < m_MinimumNextFloorSpeed.m[0][1] ? static_cast<f32>(dt) * m_MinimumNextFloorSpeed.m[0][1] * TOP_MOVEMENT_SPEED : m_Floor[j][i].m_currFloorSpeed.m[0][1] > m_MinimumNextFloorSpeed.m[0][1] ? static_cast<f32>(dt) * m_MinimumNextFloorSpeed.m[0][1] * TOP_MOVEMENT_SPEED : 0;
				m_Floor[j][i].m_currFloorSpeed.m[0][2] += m_Floor[j][i].m_currFloorSpeed.m[0][2] < m_MinimumNextFloorSpeed.m[0][2] ? static_cast<f32>(dt) * m_MinimumNextFloorSpeed.m[0][2] * TOP_MOVEMENT_SPEED : m_Floor[j][i].m_currFloorSpeed.m[0][2] > m_MinimumNextFloorSpeed.m[0][2] ? static_cast<f32>(dt) * m_MinimumNextFloorSpeed.m[0][2] * TOP_MOVEMENT_SPEED : 0;
				m_Floor[j][i].m_currFloorSpeed.m[1][0] += m_Floor[j][i].m_currFloorSpeed.m[1][0] < m_MinimumNextFloorSpeed.m[1][0] ? static_cast<f32>(dt) * m_MinimumNextFloorSpeed.m[1][0] * TOP_MOVEMENT_SPEED : m_Floor[j][i].m_currFloorSpeed.m[1][0] > m_MinimumNextFloorSpeed.m[1][0] ? static_cast<f32>(dt) * m_MinimumNextFloorSpeed.m[1][0] * TOP_MOVEMENT_SPEED : 0;
				m_Floor[j][i].m_currFloorSpeed.m[1][1] += m_Floor[j][i].m_currFloorSpeed.m[1][1] < m_MinimumNextFloorSpeed.m[1][1] ? static_cast<f32>(dt) * m_MinimumNextFloorSpeed.m[1][1] * TOP_MOVEMENT_SPEED : m_Floor[j][i].m_currFloorSpeed.m[1][1] > m_MinimumNextFloorSpeed.m[1][1] ? static_cast<f32>(dt) * m_MinimumNextFloorSpeed.m[1][1] * TOP_MOVEMENT_SPEED : 0;
				m_Floor[j][i].m_currFloorSpeed.m[1][2] += m_Floor[j][i].m_currFloorSpeed.m[1][2] < m_MinimumNextFloorSpeed.m[1][2] ? static_cast<f32>(dt) * m_MinimumNextFloorSpeed.m[1][2] * TOP_MOVEMENT_SPEED : m_Floor[j][i].m_currFloorSpeed.m[1][2] > m_MinimumNextFloorSpeed.m[1][2] ? static_cast<f32>(dt) * m_MinimumNextFloorSpeed.m[1][2] * TOP_MOVEMENT_SPEED : 0;
				m_Floor[j][i].m_currFloorSpeed.m[2][0] += m_Floor[j][i].m_currFloorSpeed.m[2][0] < m_MinimumNextFloorSpeed.m[2][0] ? static_cast<f32>(dt) * m_MinimumNextFloorSpeed.m[2][0] * TOP_MOVEMENT_SPEED : m_Floor[j][i].m_currFloorSpeed.m[2][0] > m_MinimumNextFloorSpeed.m[2][0] ? static_cast<f32>(dt) * m_MinimumNextFloorSpeed.m[2][0] * TOP_MOVEMENT_SPEED : 0;
				m_Floor[j][i].m_currFloorSpeed.m[2][1] += m_Floor[j][i].m_currFloorSpeed.m[2][1] < m_MinimumNextFloorSpeed.m[2][1] ? static_cast<f32>(dt) * m_MinimumNextFloorSpeed.m[2][1] * TOP_MOVEMENT_SPEED : m_Floor[j][i].m_currFloorSpeed.m[2][1] > m_MinimumNextFloorSpeed.m[2][1] ? static_cast<f32>(dt) * m_MinimumNextFloorSpeed.m[2][1] * TOP_MOVEMENT_SPEED : 0;
				m_Floor[j][i].m_currFloorSpeed.m[2][2] += m_Floor[j][i].m_currFloorSpeed.m[2][2] < m_MinimumNextFloorSpeed.m[2][2] ? static_cast<f32>(dt) * m_MinimumNextFloorSpeed.m[2][2] * TOP_MOVEMENT_SPEED : m_Floor[j][i].m_currFloorSpeed.m[2][2] > m_MinimumNextFloorSpeed.m[2][2] ? static_cast<f32>(dt) * m_MinimumNextFloorSpeed.m[2][2] * TOP_MOVEMENT_SPEED : 0;
				//Adding to floor
				m_Floor[j][i].m_TransformFloorCurr.m[0][0] += m_Floor[j][i].m_currFloorSpeed.m[0][0];
				m_Floor[j][i].m_TransformFloorCurr.m[0][1] += m_Floor[j][i].m_currFloorSpeed.m[0][1];
				m_Floor[j][i].m_TransformFloorCurr.m[0][2] += m_Floor[j][i].m_currFloorSpeed.m[0][2];
				m_Floor[j][i].m_TransformFloorCurr.m[1][0] += m_Floor[j][i].m_currFloorSpeed.m[1][0];
				m_Floor[j][i].m_TransformFloorCurr.m[1][1] += m_Floor[j][i].m_currFloorSpeed.m[1][1];
				m_Floor[j][i].m_TransformFloorCurr.m[1][2] += m_Floor[j][i].m_currFloorSpeed.m[1][2];
				m_Floor[j][i].m_TransformFloorCurr.m[2][0] += m_Floor[j][i].m_currFloorSpeed.m[2][0];
				m_Floor[j][i].m_TransformFloorCurr.m[2][1] += m_Floor[j][i].m_currFloorSpeed.m[2][1];
				m_Floor[j][i].m_TransformFloorCurr.m[2][2] += m_Floor[j][i].m_currFloorSpeed.m[2][2];

				///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				m_Floor[j][i].m_Trans.m[0][2] += m_Floor[j][i].m_currFloorSpeed.m[0][2];
				m_Floor[j][i].m_Trans.m[1][2] += m_Floor[j][i].m_currFloorSpeed.m[1][2];

				if (!m_StopMovement)
				{
					if (m_Floor[j][i].m_currFloorTimer > m_Floor[j][i].m_FloorSpeedTimer)
					{
						m_Floor[j][i].m_currFloorTimer = 0.0;
						if (m_Floor[j][i].m_currFloorNum > 1)
						{
							m_Floor[j][i].m_currFloorNum--;
							m_Floor[j][i].m_IsRender = true;
						}
						else
						{
							//Loop to the top
							m_Floor[j][i].m_currFloorNum = 8;
							m_Floor[j][i].m_currFloorSpeed = { 0 };
							m_Floor[j][i].m_TransformFloorCurr = m_LastFloorData;
							m_Floor[j][i].m_IsRender = false;

							m_Floor[j][i].m_Trans.m[0][2] = m_Floor[j][8].m_OriginalTrans.m[0][2];
							m_Floor[j][i].m_Trans.m[1][2] = m_Floor[j][8].m_OriginalTrans.m[1][2];

							m_CurrentTileNumFurthest = m_Floor[j][i].m_FloorNum;

							t_ShiftRow.push_back(m_Floor[j][i].m_FloorNum);

							//FORCE CHANGE OF FLOORING TYPE
							//Change of flooring at Stage 6
							if (m_currLevel >= 6 && m_currLevel <= 7)
								m_Floor[j][i].m_Type = 1;
							else
								m_Floor[j][i].m_Type = 0;
						}
					}
					else
						m_Floor[j][i].m_currFloorTimer += static_cast<f32>(dt);
				}
			}

			if (t_ShiftRow.size() >= 1)
			{
				for (std::list<int>::iterator i = t_ShiftRow.begin(); i != t_ShiftRow.end(); i++)
				{
					//Clear Scene Objects in row
					DestroyRowOBJs(*i);

					//Only spawn on tile 0, 2->8 (1 & 9 tiles are hidden below another tile, so visual error)
					if (m_Floor[j][*i].m_FloorNum > -1 && m_Floor[j][*i].m_FloorNum < 9 && m_Floor[j][*i].m_FloorNum != 1)
						CreateRowOBJs(*i);
				}
				t_ShiftRow.clear();
			}
		}

		///////////////////////////////////////////////////////////////////////////
		//UPDATE SCENEOBJs Pos and Logic
		//////////////////////////////////////////////////////////////////////////
		GameObjectManager::GetInstance()->Update(dt);
		v_SceneObject temp;
		std::pair<int, int> t_TransScaleModifier = { 60, 48 }; //For rand on tile pos
		static double t_XModifier = 2, t_YModifier = 225, t_MXModifier = 45, t_MYModifier = 1;
		for (int j = 0; j < SIZE_OF_FLOOR; j++)
		{
			for (int i = NUM_OF_TILES - 1; i > -1; i--)
			{
				for (std::list<v_SceneObject>::iterator it = m_FloorOBJs[j][i].begin();
					it != m_FloorOBJs[j][i].end();
					it++)
				{
					//Reset Transform data
					AEMtx33Identity(&(*it).m_TransformData);
					
					if (!m_PanCloseToGround)
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
					AEMtx33ScaleApply(&(*it).m_TransformData, &(*it).m_TransformData, m_Floor[j][i].m_TransformFloorCurr.m[0][0] * 2, m_Floor[j][i].m_TransformFloorCurr.m[0][0] * 2);

					//Translate with the tile
					if (j < t_CenterFloorNum)
						AEMtx33TransApply(&(*it).m_TransformData, &(*it).m_TransformData, static_cast<float>(m_Floor[j][i].m_Trans.m[0][2] * t_XModifier + t_MXModifier), static_cast<float>(t_YModifier - m_Floor[j][i].m_Trans.m[1][2] * t_MYModifier));
					else
						AEMtx33TransApply(&(*it).m_TransformData, &(*it).m_TransformData, static_cast<float>(m_Floor[j][i].m_Trans.m[0][2] * t_XModifier - t_MXModifier), static_cast<float>(t_YModifier - m_Floor[j][i].m_Trans.m[1][2] * t_MYModifier));


					AEMtx33TransApply(&(*it).m_TransformData, &(*it).m_TransformData,
						(*it).m_Trans.m[0][2] * m_Floor[j][i].m_TransformFloorCurr.m[0][0] / ((t_TransScaleModifier.first) / (*it).m_Scale.m[0][0]),
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
				for (std::list<v_SceneObject>::iterator it = m_FloorOBJs[j][i].begin();
					it != m_FloorOBJs[j][i].end();
					it++)
				{
					(*it).m_Transparency += static_cast<f32>(dt) * 1.5f;
				}
			}
		}
	}
}

void SceneLevelBuilder::Render()
{
	// Set the background to black.
	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);

	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);

	// Set the the color to multiply to white, so that the sprite can 
	// display the full range of colors (default is black).
	AEGfxSetColorToMultiply(m_Lighting.r, m_Lighting.g, m_Lighting.b, m_Lighting.a);

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
		AEGfxSetTransform(m_TransformSkyData.m);
		AEGfxTextureSet(RenderHelper::getInstance()->getTextureByRef("SKY_TEST_1"), 0, 0);
		AEGfxMeshDraw(RenderHelper::getInstance()->GetdefaultMesh(), AE_GFX_MDM_TRIANGLES);

		/////////////////////////////////////////////////////////////////////////////////
		//Sun
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		AEGfxSetColorToAdd(1.0f, 1.0f, 1.0f, 1.0f);
		AEGfxSetTransparency(1.0f);
		AEGfxSetTransform(m_TransformSunData.m);
		AEGfxMeshDraw(RenderHelper::getInstance()->GetdefaultMesh(), AE_GFX_MDM_TRIANGLES);
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxSetTransparency(1.0f);
		AEGfxTextureSet(RenderHelper::getInstance()->getTextureByRef("SUN_OVERLAY_1"), 0, 0);
		AEGfxSetTransform(m_TransformSunOverlayData.m);
		AEGfxMeshDraw(RenderHelper::getInstance()->GetdefaultMesh(), AE_GFX_MDM_TRIANGLES);

		//////////////////////////////////////////////////////////////////////////////////////
		//Cloud
		//First Layer
		AEGfxSetTransform(m_TransformCloudsData[0].m);
		AEGfxTextureSet(RenderHelper::getInstance()->getTextureByRef("SKY_TEST_2"), 0, 0);
		AEGfxMeshDraw(RenderHelper::getInstance()->GetdefaultMesh(), AE_GFX_MDM_TRIANGLES);
		AEGfxSetTransform(m_TransformCloudsData[1].m);
		AEGfxTextureSet(RenderHelper::getInstance()->getTextureByRef("SKY_TEST_2"), 0, 0);
		AEGfxMeshDraw(RenderHelper::getInstance()->GetdefaultMesh(), AE_GFX_MDM_TRIANGLES);
		AEGfxSetTransform(m_TransformCloudsData[2].m);
		AEGfxTextureSet(RenderHelper::getInstance()->getTextureByRef("SKY_TEST_2"), 0, 0);
		AEGfxMeshDraw(RenderHelper::getInstance()->GetdefaultMesh(), AE_GFX_MDM_TRIANGLES);
		//Second Layer
		AEGfxSetTransform(m_TransformCloudsData[3].m);
		AEGfxTextureSet(RenderHelper::getInstance()->getTextureByRef("SKY_TEST_3"), 0, 0);
		AEGfxMeshDraw(RenderHelper::getInstance()->GetdefaultMesh(), AE_GFX_MDM_TRIANGLES);
		AEGfxSetTransform(m_TransformCloudsData[4].m);
		AEGfxTextureSet(RenderHelper::getInstance()->getTextureByRef("SKY_TEST_3"), 0, 0);
		AEGfxMeshDraw(RenderHelper::getInstance()->GetdefaultMesh(), AE_GFX_MDM_TRIANGLES);
		AEGfxSetTransform(m_TransformCloudsData[5].m);
		AEGfxTextureSet(RenderHelper::getInstance()->getTextureByRef("SKY_TEST_3"), 0, 0);
		AEGfxMeshDraw(RenderHelper::getInstance()->GetdefaultMesh(), AE_GFX_MDM_TRIANGLES);
		//Third Layer
		AEGfxSetTransform(m_TransformCloudsData[6].m);
		AEGfxTextureSet(RenderHelper::getInstance()->getTextureByRef("SKY_TEST_4"), 0, 0);
		AEGfxMeshDraw(RenderHelper::getInstance()->GetdefaultMesh(), AE_GFX_MDM_TRIANGLES);
		AEGfxSetTransform(m_TransformCloudsData[7].m);
		AEGfxTextureSet(RenderHelper::getInstance()->getTextureByRef("SKY_TEST_4"), 0, 0);
		AEGfxMeshDraw(RenderHelper::getInstance()->GetdefaultMesh(), AE_GFX_MDM_TRIANGLES);
		AEGfxSetTransform(m_TransformCloudsData[8].m);
		AEGfxTextureSet(RenderHelper::getInstance()->getTextureByRef("SKY_TEST_4"), 0, 0);
		AEGfxMeshDraw(RenderHelper::getInstance()->GetdefaultMesh(), AE_GFX_MDM_TRIANGLES);
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////
	// BACKDROP RENDER
	{
		AEGfxTextureSet(RenderHelper::getInstance()->getTextureByRef("BACKDROP_1"), 0, 0);
		for (int i = 0; i < 9; i++)
		{
			AEGfxSetTransform(m_TransformBackDrops1Data[i].m);
			AEGfxMeshDraw(RenderHelper::getInstance()->GetdefaultMesh(), AE_GFX_MDM_TRIANGLES);
		}
		AEGfxTextureSet(RenderHelper::getInstance()->getTextureByRef("BACKDROP_2"), 0, 0);
		for (int i = 0; i < 5; i++)
		{
			AEGfxSetTransform(m_TransformBackDrops2Data[i].m);
			AEGfxMeshDraw(RenderHelper::getInstance()->GetdefaultMesh(), AE_GFX_MDM_TRIANGLES);
		}
		AEGfxTextureSet(RenderHelper::getInstance()->getTextureByRef("BACKDROP_3"), 0, 0);
		for (int i = 0; i < 5; i++)
		{
			AEGfxSetTransform(m_TransformBackDrops3Data[i].m);
			AEGfxMeshDraw(RenderHelper::getInstance()->GetdefaultMesh(), AE_GFX_MDM_TRIANGLES);
		}

	}
	/////////////////////////////////////////////////////////////////////////////////////////////////
	// FLOOR RENDER
	{
		//// Tell the engine to get ready to draw something with texture.
		//// Set the color to add to nothing, so that we don't alter the sprite's color
		AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 1.0f);

		//Main Floor
		
		for (int i = NUM_OF_TILES - 1; i > -1; i--)
		{
			if(m_Floor[t_CenterFloorNum][i].m_Type ==0)
				AEGfxTextureSet(RenderHelper::getInstance()->getTextureByRef("FLOOR_CENTER_1"), 0, 0);
			else
				AEGfxTextureSet(RenderHelper::getInstance()->getTextureByRef("FLOOR_CENTER_2"), 0, 0);

			if (m_Floor[t_CenterFloorNum][i].m_IsRender)
			{
				AEGfxSetTransform(m_Floor[t_CenterFloorNum][i].m_TransformFloorCurr.m);
				AEGfxMeshDraw(RenderHelper::getInstance()->GetdefaultMesh(), AE_GFX_MDM_TRIANGLES);
			}
		}

		////Left Floor
		for (int j = 0; j < SIZE_OF_FLOOR - (t_CenterFloorNum + 1); j++)
		{
			for (int i = NUM_OF_TILES - 1; i > -1; i--)
			{
				if (m_Floor[j][i].m_Type == 0)
					AEGfxTextureSet(RenderHelper::getInstance()->getTextureByRef("FLOOR_LEFT_1"), 0, 0);
				else
					AEGfxTextureSet(RenderHelper::getInstance()->getTextureByRef("FLOOR_LEFT_2"), 0, 0);

				if (m_Floor[j][i].m_IsRender)
				{
					AEGfxSetTransform(m_Floor[j][i].m_TransformFloorCurr.m);
					AEGfxMeshDraw(RenderHelper::getInstance()->GetdefaultMesh(), AE_GFX_MDM_TRIANGLES);
				}
			}
		}
		//Right Floor
		for (int j = (t_CenterFloorNum + 1); j < SIZE_OF_FLOOR; j++)
		{
			for (int i = NUM_OF_TILES - 1; i > -1; i--)
			{
				if (m_Floor[j][i].m_Type == 0)
					AEGfxTextureSet(RenderHelper::getInstance()->getTextureByRef("FLOOR_RIGHT_1"), 0, 0);
				else
					AEGfxTextureSet(RenderHelper::getInstance()->getTextureByRef("FLOOR_RIGHT_2"), 0, 0);

				if (m_Floor[j][i].m_IsRender)
				{
					AEGfxSetTransform(m_Floor[j][i].m_TransformFloorCurr.m);
					AEGfxMeshDraw(RenderHelper::getInstance()->GetdefaultMesh(), AE_GFX_MDM_TRIANGLES);
				}
			}
		}

	}
	/////////////////////////////////////////////////////////////////////////////////////////////////
	// FOG RENDER
	{
		AEGfxTextureSet(RenderHelper::getInstance()->getTextureByRef("FOG_1"), 0, 0);
		AEGfxSetTransform(m_TransformFogData.m);
		AEGfxMeshDraw(RenderHelper::getInstance()->GetdefaultMesh(), AE_GFX_MDM_TRIANGLES);
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
				// m_CurrentTileNumFurthest = 4
				// -> 4 3 2 1 0 9 8 7 6 5 -> Render in this way
				////////////////////////////////////////////////
				int tempTileNum = m_CurrentTileNumFurthest - i;
				if (tempTileNum < 0)
					tempTileNum += NUM_OF_TILES - 1;

				for (std::list<v_SceneObject>::iterator it = m_FloorOBJs[j][tempTileNum].begin();
					it != m_FloorOBJs[j][tempTileNum].end();
					it++)
				{
					AEGfxTextureSet(RenderHelper::getInstance()->GetTexture((*it).m_Type), 0, 0);
					AEGfxSetTransparency((*it).m_Transparency);
					AEGfxSetTransform((*it).m_TransformData.m);
					AEGfxMeshDraw(RenderHelper::getInstance()->GetdefaultMesh(), AE_GFX_MDM_TRIANGLES);
				}
			}
		}
		//Render Right Side
		for (int j = SIZE_OF_FLOOR - 1; j >= SIZE_OF_FLOOR / 2; j--)
		{
			for (int i = 0; i < NUM_OF_TILES - 1; i++)
			{
				////////////////////////////////////////////////
				// m_CurrentTileNumFurthest = 4
				// -> 4 3 2 1 0 9 8 7 6 5 -> Render in this way
				////////////////////////////////////////////////
				int tempTileNum = m_CurrentTileNumFurthest - i;
				if (tempTileNum < 0)
					tempTileNum += NUM_OF_TILES - 1;

				for (std::list<v_SceneObject>::iterator it = m_FloorOBJs[j][tempTileNum].begin();
					it != m_FloorOBJs[j][tempTileNum].end();
					it++)
				{
					AEGfxTextureSet(RenderHelper::getInstance()->GetTexture((*it).m_Type), 0, 0);
					AEGfxSetTransparency((*it).m_Transparency);
					AEGfxSetTransform((*it).m_TransformData.m);
					AEGfxMeshDraw(RenderHelper::getInstance()->GetdefaultMesh(), AE_GFX_MDM_TRIANGLES);
				}
			}
		}
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////
	// Light Flare
	if(m_SceneLevelDataList[m_currLevel].m_DayTime)
	{
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		static f32 transparency[8] = { 1.07f, -0.75f, 0.2f, -0.05f , -0.36f, 0.9f ,1.1f,2.2f };
		//Lens Flare
		AEGfxSetTransparency(transparency[7]);
		AEGfxTextureSet(RenderHelper::getInstance()->getTextureByRef("SUN_LENS_1"), 0, 0);
		AEGfxSetTransform(m_TransformSunLensData[7].m);
		AEGfxMeshDraw(RenderHelper::getInstance()->GetdefaultMesh(), AE_GFX_MDM_TRIANGLES);
		AEGfxSetTransparency(transparency[6]);
		AEGfxTextureSet(RenderHelper::getInstance()->getTextureByRef("SUN_LENS_2"), 0, 0);
		AEGfxSetTransform(m_TransformSunLensData[6].m);
		AEGfxMeshDraw(RenderHelper::getInstance()->GetdefaultMesh(), AE_GFX_MDM_TRIANGLES);
		AEGfxSetTransparency(transparency[5]);
		AEGfxTextureSet(RenderHelper::getInstance()->getTextureByRef("SUN_LENS_2"), 0, 0);
		AEGfxSetTransform(m_TransformSunLensData[5].m);
		AEGfxMeshDraw(RenderHelper::getInstance()->GetdefaultMesh(), AE_GFX_MDM_TRIANGLES);
		AEGfxSetTransparency(transparency[4]);
		AEGfxTextureSet(RenderHelper::getInstance()->getTextureByRef("SUN_LENS_2"), 0, 0);
		AEGfxSetTransform(m_TransformSunLensData[4].m);
		AEGfxMeshDraw(RenderHelper::getInstance()->GetdefaultMesh(), AE_GFX_MDM_TRIANGLES);
		AEGfxSetTransparency(transparency[3]);
		AEGfxTextureSet(RenderHelper::getInstance()->getTextureByRef("SUN_LENS_2"), 0, 0);
		AEGfxSetTransform(m_TransformSunLensData[3].m);
		AEGfxMeshDraw(RenderHelper::getInstance()->GetdefaultMesh(), AE_GFX_MDM_TRIANGLES);
		AEGfxSetTransparency(transparency[2]);
		AEGfxTextureSet(RenderHelper::getInstance()->getTextureByRef("SUN_LENS_3"), 0, 0);
		AEGfxSetTransform(m_TransformSunLensData[2].m);
		AEGfxMeshDraw(RenderHelper::getInstance()->GetdefaultMesh(), AE_GFX_MDM_TRIANGLES);
		AEGfxSetTransparency(transparency[1]);
		AEGfxTextureSet(RenderHelper::getInstance()->getTextureByRef("SUN_LENS_4"), 0, 0);
		AEGfxSetTransform(m_TransformSunLensData[1].m);
		AEGfxMeshDraw(RenderHelper::getInstance()->GetdefaultMesh(), AE_GFX_MDM_TRIANGLES);
		AEGfxSetTransparency(transparency[0]);
		AEGfxTextureSet(RenderHelper::getInstance()->getTextureByRef("SUN_LENS_5"), 0, 0);
		AEGfxSetTransform(m_TransformSunLensData[0].m);
		AEGfxMeshDraw(RenderHelper::getInstance()->GetdefaultMesh(), AE_GFX_MDM_TRIANGLES);
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
	//   	AEGfxSetColorToMultiply(m_Lighting.r, m_Lighting.g, m_Lighting.b, m_Lighting.a);
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
	//		AEGfxMeshDraw(RenderHelper::getInstance()->GetdefaultMesh(), AE_GFX_MDM_TRIANGLES);
	//	}
	//	else
	//		t_Transparency = -1.1;
	//}

	/////////////////////////////////////////////////////////////////////////////////////////////////
	// Combat Render
	if (m_CombatPhase)
		CombatScene::getInstance().Render();
	/////////////////////////////////////////////////////////////////////////////////////////////////
	// UI / MISC RENDER
	{
		//Lvl Name
		RenderLvlName();

		//Border
		f32 camX, camY;
		AEGfxGetCamPosition(&camX, &camY);
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 1.0f);
		AEGfxSetTransparency(1.0);
		AEMtx33 t_curr;
		AEMtx33Identity(&t_curr);
		AEMtx33ScaleApply(&t_curr, &t_curr, 99999, 90);
		AEMtx33TransApply(&t_curr, &t_curr, camX, -AEGfxGetWindowHeight() / 2 + camY);
		AEGfxSetTransform(t_curr.m);
		AEGfxMeshDraw(RenderHelper::getInstance()->GetdefaultMesh(), AE_GFX_MDM_TRIANGLES);
		AEMtx33Identity(&t_curr);
		AEMtx33ScaleApply(&t_curr, &t_curr, 99999, 90);
		AEMtx33TransApply(&t_curr, &t_curr, camX, AEGfxGetWindowHeight() / 2 + camY);
		AEGfxSetTransform(t_curr.m);
		AEGfxMeshDraw(RenderHelper::getInstance()->GetdefaultMesh(), AE_GFX_MDM_TRIANGLES);

		//Screen Transition
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 1.0f);
		AEGfxSetTransparency(m_currTransitionTransparency);
		AEMtx33Identity(&t_curr);
		AEMtx33ScaleApply(&t_curr, &t_curr, 99999, 99999);
		AEGfxSetTransform(t_curr.m);
		AEGfxMeshDraw(RenderHelper::getInstance()->GetdefaultMesh(), AE_GFX_MDM_TRIANGLES);

	}
	/////////////////////////////////////////////////////////////////////////////////////////////////
	// GAMEOBJ RENDER
	GameObjectManager::GetInstance()->Render();

	////////////////////////////////////////////////////////////////////////////////////////////////
	// UI / MISC RENDER PART 2
	{
		// down here because player should be drawn on top of everything else, save pause screen
		if (!m_CombatPhase) {
			player->render();
		}
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////
	// show game end screen if no more levels
	if (showGameEnd) {
		cout << "show game end screen\n";
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////
	// UI / MISC RENDER PART 3
	{
		Pause::getInstance().render();
	}

}

void SceneLevelBuilder::Exit()
{
	//Clear Floor
	for (int i = 0; i < SIZE_OF_FLOOR; i++)
	{
		delete[] m_Floor[i];
		delete[] m_FloorOBJs[i];
	}
	delete[] m_Floor;
	delete[] m_FloorOBJs;

	for (int i = 0; i < NUM_OF_TILESPAWNPOINTS; i++)
	{
		delete[] m_tileSP[i];
	}
	delete[] m_tileSP;

	//Clear Object in scene
	GameObjectManager::GetInstance()->Exit();
	GameObjectManager::GetInstance()->Destroy();

	delete[] m_SceneLevelDataList;

	//Destroy Font
	AEGfxDestroyFont(pTextFont);

	CombatScene::getInstance().Exit();
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
		if (j == t_CenterFloorNum)
			continue;

		//Spawn objs based on MAX_NUM_SCENEOBJS_TILE
		for (int i = static_cast<int>(AEClamp(static_cast<f32>(rand() % MAX_NUM_SCENEOBJS_TILE), 1.0f, static_cast<f32>(MAX_NUM_SCENEOBJS_TILE))); i > 0; i--)
		{
			v_SceneObject newObj;

			//Selecting Entity Group to Spawn
			int TotalProb = 0; //Get total probability
			for (int curr : m_SceneLevelDataList[m_currLevel].m_SceneObjSpawnWeight)
			{
				TotalProb += curr;
			}
			std::string Ref = "";
			int randnum = static_cast<int>(AEClamp(static_cast<f32>(rand() % TotalProb), 1.0f, static_cast<f32>(TotalProb)));//This is the rand probability of which type of sceneobjects to spawn
			int temp = 0;//Disregard this: for loop below
			for (int curr : m_SceneLevelDataList[m_currLevel].m_SceneObjSpawnWeight)
			{
				randnum -= curr;
				if (randnum < 0)
				{
					Ref = m_SceneLevelDataList[m_currLevel].m_SceneObjTypes[temp];
					break;
				}
				temp++;
			}

			//Selecting Entity from entity group to spawn
			if (Ref == "Grass")
			{
				newObj.m_Type = static_cast<v_SceneObjectTypes>(AEClamp(static_cast<f32>(rand() % (v_SceneObjectTypes::TYPE_End_Grass - v_SceneObjectTypes::TYPE_Grass) + v_SceneObjectTypes::TYPE_Grass),
					static_cast<f32>(v_SceneObjectTypes::TYPE_Grass + 1),
					static_cast<f32>(v_SceneObjectTypes::TYPE_End_Grass - 1)));
				newObj.m_tobeCentered = true;
			}
			else if (Ref == "Tree")
			{
				newObj.m_Type = static_cast<v_SceneObjectTypes>(AEClamp(static_cast<f32>(rand() % (v_SceneObjectTypes::TYPE_End_Tree - v_SceneObjectTypes::TYPE_Tree) + v_SceneObjectTypes::TYPE_Tree),
					static_cast<f32>(v_SceneObjectTypes::TYPE_Tree + 1),
					static_cast<f32>(v_SceneObjectTypes::TYPE_End_Tree - 1)));
				newObj.m_tobeCentered = false;
			}
			else if (Ref == "Rock")
			{
				newObj.m_Type = static_cast<v_SceneObjectTypes>(AEClamp(static_cast<f32>(rand() % (v_SceneObjectTypes::TYPE_End_Rock - v_SceneObjectTypes::TYPE_Rock) + v_SceneObjectTypes::TYPE_Rock),
					static_cast<f32>(v_SceneObjectTypes::TYPE_Rock + 1),
					static_cast<f32>(v_SceneObjectTypes::TYPE_End_Rock - 1)));
				newObj.m_tobeCentered = true;
			}

			//Random Selection of Spawn location on tile
			int t_RandX, t_RandY;
			if (!newObj.m_tobeCentered)
			{
				if (j == t_CenterFloorNum - 1 || j == t_CenterFloorNum + 1)
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


			if (j < t_CenterFloorNum)//Left Side
			{
				AEMtx33Trans(&newObj.m_Trans, m_tileSP[t_RandY][t_RandX].m_X + m_tileSP[t_RandY][t_RandX].m_Y / 3, m_tileSP[t_RandY][t_RandX].m_Y);
			}
			else//Right Side
			{
				AEMtx33Trans(&newObj.m_Trans, -m_tileSP[t_RandY][t_RandX].m_X - m_tileSP[t_RandY][t_RandX].m_Y / 3, m_tileSP[t_RandY][t_RandX].m_Y);
			}

			//Scaling (Uniform Scaling)
			AEMtx33Scale(&newObj.m_Scale, 2.5f, 2.5f);

			//Push into OBJlist in tile (Determine which to render first based on Spawnpoint m_Y
			std::list<v_SceneObject>::iterator it = m_FloorOBJs[j][t_tileNum].begin();
			newObj.m_RenderOrder = t_RandY; //Spawnpoint m_y
			for (; it != m_FloorOBJs[j][t_tileNum].end(); it++)
			{
				if (newObj.m_RenderOrder <= (*it).m_RenderOrder)
					break;
			}
			m_FloorOBJs[j][t_tileNum].insert(it, newObj);
		}
	}
}
void SceneLevelBuilder::DestroyRowOBJs(int t_tileNum)
{
	for (int j = 0; j < SIZE_OF_FLOOR; j++)
	{
		m_FloorOBJs[j][t_tileNum].clear();
	}
}


/*********************************************************************************
Level Name
**********************************************************************************/
void SceneLevelBuilder::SpawnLvlName()
{
	m_LvlNameTimer = MAX_LVLNAMETIMER;
	m_LvlNameTransparency = -1.2f;
}
void SceneLevelBuilder::UpdateLvlName(f32 t_dt)
{
	if (m_LvlNameTimer > MAX_LVLNAMETIMER - 1.0)
	{
		m_LvlNameTransparency += m_LvlNameTransparency > 1.0f ? 0.0f : 0.08f;
	}
	else if (m_LvlNameTimer < 1.0)
	{
		m_LvlNameTransparency -= m_LvlNameTransparency < -1.0f ? 0.0f : 0.08f;
	}
	m_LvlNameTimer -= t_dt;
}
void SceneLevelBuilder::RenderLvlName()
{
	f32 t_camX, t_camY;
	AEGfxGetCamPosition(&t_camX, &t_camY);

	AEVec2 RightOriginalHeaderPos{ 27.5f, 205.7f };
	AEVec2 LeftOriginalHeaderPos{ -27.5f, 205.7f };
	AEVec2 RightMaxHeaderPos{ -15.0f + m_SceneLevelDataList[m_currLevel].m_LevelName.size() * 13.7f, 205.7f };
	AEVec2 LeftMaxHeaderPos{ 15.0f - m_SceneLevelDataList[m_currLevel].m_LevelName.size() * 13.7f, 205.7f };
	static AEVec2 currRightHeaderPos{ RightOriginalHeaderPos };
	static AEVec2 currLeftHeaderPos{ LeftOriginalHeaderPos };
	currRightHeaderPos.x += currRightHeaderPos.x < RightMaxHeaderPos.x ? (RightMaxHeaderPos.x - currRightHeaderPos.x) / 30 : 0;
	currLeftHeaderPos.x += currLeftHeaderPos.x > LeftMaxHeaderPos.x ? (LeftMaxHeaderPos.x - currLeftHeaderPos.x) / 30 : 0;

	if (m_LvlNameTimer > 0.0 && m_currLevel > -1)
	{
		AEGfxTextureSet(NULL, 0, 0);
		f32 TextWidth = 0, TextHeight = 0;
		char strBuffer[1024];
		sprintf_s(strBuffer, m_SceneLevelDataList[m_currLevel].m_LevelName.c_str());
		AEGfxGetPrintSize(pTextFont, strBuffer, 0.8f, &TextWidth, &TextHeight);
		AEGfxPrint(pTextFont, strBuffer, -TextWidth / 2.0f - 0.01f, 0.61f, 0.8f, 0.f, 0.f, 0.f, m_LvlNameTransparency);
		AEGfxPrint(pTextFont, strBuffer, -TextWidth / 2.0f, 0.6f, 0.8f, 1.0f, 0.75f, 0.0f, m_LvlNameTransparency);


		AEGfxTextureSet(RenderHelper::getInstance()->getTextureByRef("LVL_HEADER"), 0, 0);
		if (m_LvlNameTimer < 1.0) AEGfxSetTransparency(m_LvlNameTransparency - 0.9f);
		else AEGfxSetTransparency(m_LvlNameTransparency + 0.2f);

		AEMtx33 trans{};
		AEMtx33Identity(&trans);
		AEMtx33ScaleApply(&trans, &trans, 214.5, 32.5);
		AEMtx33TransApply(&trans, &trans, currRightHeaderPos.x + t_camX - 3.75f, currRightHeaderPos.y + t_camY + 4.0f);
		AEGfxSetTransform(trans.m);
		AEGfxSetColorToMultiply(0.0f, 0.0f, 0.0f, 1.0f);
		AEGfxMeshDraw(RenderHelper::getInstance()->GetdefaultMesh(), AE_GFX_MDM_TRIANGLES);
		AEMtx33TransApply(&trans, &trans, 3.75f, -4.0f);
		AEGfxSetTransform(trans.m);
		AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
		AEGfxMeshDraw(RenderHelper::getInstance()->GetdefaultMesh(), AE_GFX_MDM_TRIANGLES);


		AEMtx33Identity(&trans);
		AEMtx33RotDeg(&trans, 180);
		AEMtx33ScaleApply(&trans, &trans, 214.5, 32.5);
		AEMtx33TransApply(&trans, &trans, currLeftHeaderPos.x + t_camX - 3.75f, currLeftHeaderPos.y + t_camY + 4.0f);
		AEGfxSetTransform(trans.m);
		AEGfxSetColorToMultiply(0.0f, 0.0f, 0.0f, 1.0f);
		AEGfxMeshDraw(RenderHelper::getInstance()->GetdefaultMesh(), AE_GFX_MDM_TRIANGLES);
		AEMtx33TransApply(&trans, &trans, 3.75f, -4.0f);
		AEGfxSetTransform(trans.m);
		AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
		AEGfxMeshDraw(RenderHelper::getInstance()->GetdefaultMesh(), AE_GFX_MDM_TRIANGLES);
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
	if (m_TryTimer < 0)
	{
		if (m_SceneEnemy == nullptr && !m_CombatPhase && SceneStages::sInstance->m_StartGame)
		{
			if (rand() % 100 < m_SceneLevelDataList[m_currLevel].m_EnemySpawnRate)
			{
				m_SceneEnemy = dynamic_cast<GameObject_Misc_Enemy*>(GameObjectManager::GetInstance()->FindObjectByReference("MiscEnemy"));
				m_SceneEnemy->ActivateEnemy(m_Floor[t_CenterFloorNum][m_CurrentTileNumFurthest].m_TransformFloorCurr);
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
	if (m_currLevel < m_MAXLevel) //Check the stats for next level
	{
		/////////////////////////////////////////////////////////////////////
		//Change to nighttime
		if (!m_SceneLevelDataList[m_currLevel].m_DayTime)
		{
			t_r = -0.39; t_g = 0.06; t_b = 0.3;
		}
		/////////////////////////////////////////////////////////////////////
		//Change to Dawn
		else if (!m_SceneLevelDataList[m_currLevel - 1].m_DayTime && m_SceneLevelDataList[m_currLevel].m_DayTime)
		{
			t_r = 1.0; t_g = 0.7; t_b = 1.0;
		}
		/////////////////////////////////////////////////////////////////////
		//Change to Dusk
		else if (!m_SceneLevelDataList[m_currLevel + 1].m_DayTime && m_SceneLevelDataList[m_currLevel].m_DayTime)
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

	m_Lighting.r += m_Lighting.r > static_cast<float>(t_r) && abs(t_r - m_Lighting.r) > dt ? -dt / LERPING_SPEED : m_Lighting.r < static_cast<float>(t_r) && abs(m_Lighting.r - t_r) > dt ? dt / LERPING_SPEED : 0;
	m_Lighting.g += m_Lighting.g > static_cast<float>(t_g) && abs(t_g - m_Lighting.g) > dt ? -dt / LERPING_SPEED : m_Lighting.g < static_cast<float>(t_g) && abs(m_Lighting.g - t_g) > dt ? dt / LERPING_SPEED : 0;
	m_Lighting.b += m_Lighting.b > static_cast<float>(t_b) && abs(t_b - m_Lighting.b) > dt ? -dt / LERPING_SPEED : m_Lighting.b < static_cast<float>(t_b) && abs(m_Lighting.b - t_b) > dt ? dt / LERPING_SPEED : 0;

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
	for (int i = 0; i < m_TransformSunLensData.size(); i++)
	{
		AEMtx33Identity(&m_TransformSunLensData[i]);
		AEMtx33ScaleApply(&m_TransformSunLensData[i], &m_TransformSunLensData[i], m_sunOverlayScale.x + scaleIncr[i], m_sunOverlayScale.y + scaleIncr[i]);
		AEMtx33TransApply(&m_TransformSunLensData[i], &m_TransformSunLensData[i], mX + (m_sunPos.x - mX) * (i + varience[i] + 1) / 8, mY + (m_sunPos.y - mY) * (i + varience[i] + 1) / 8);
	}
}
void SceneLevelBuilder::UpdateClouds(f32 t_dt)
{
	float t_CloudMaxSpeed = 10.0f;// - to go left, + to go right
	for (int i = 0; i < m_TransformCloudsData.size(); i++)
	{
		if (i / 3 != 1)//Special case for this texture pack
		{
			AEMtx33Identity(&m_TransformCloudsData[i]);
			AEMtx33ScaleApply(&m_TransformCloudsData[i], &m_TransformCloudsData[i], 1700.0f, 600.f);
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
			AEMtx33TransApply(&m_TransformCloudsData[i], &m_TransformCloudsData[i], (i % 3 - 1) * 1700.0f - static_cast<f32>(mouseX) / static_cast<f32>(((65) / (3 - i / 3))),
				305.f + static_cast<f32>(mouseY) / static_cast<f32>(((100) / (3 - i / 3))));
		}
		else
		{
			AEMtx33TransApply(&m_TransformCloudsData[i], &m_TransformCloudsData[i], t_CloudMaxSpeed * t_dt, 0.0f);;
		}

		//To do infinite Interpolation
		if (m_TransformCloudsData[i].m[0][2] > m_TransformCloudsData[i].m[0][0])
			m_TransformCloudsData[i].m[0][2] -= m_TransformCloudsData[i].m[0][0] * 2;
		if (m_TransformCloudsData[i].m[0][2] < -m_TransformCloudsData[i].m[0][0])
			m_TransformCloudsData[i].m[0][2] += m_TransformCloudsData[i].m[0][0] * 2;
	}
}
void SceneLevelBuilder::UpdateBackdrop(f32 t_dt)
{
	UNREFERENCED_PARAMETER(t_dt);
	for (int i = 0; i < 9; i++)
	{
		AEMtx33Identity(&m_TransformBackDrops1Data[i]);
		AEMtx33ScaleApply(&m_TransformBackDrops1Data[i], &m_TransformBackDrops1Data[i], 240.0f, 360.f);
		AEMtx33TransApply(&m_TransformBackDrops1Data[i], &m_TransformBackDrops1Data[i], (i - 4) * 240.0f - static_cast<f32>(mouseX / 50.0f),
			205.f + static_cast<f32>(mouseY / 90.0f));

	}

	for (int i = 0; i < 5; i++)
	{
		AEMtx33Identity(&m_TransformBackDrops2Data[i]);
		AEMtx33ScaleApply(&m_TransformBackDrops2Data[i], &m_TransformBackDrops2Data[i], 640.0f, 360.0f);
		AEMtx33TransApply(&m_TransformBackDrops2Data[i], &m_TransformBackDrops2Data[i], (i - 2) * 640.0f - static_cast<f32>(mouseX / 35.0f),
			205.f + static_cast<f32>(mouseY / 70.0f));
	}


	if (m_currLevel <= 5)
	{
		for (int i = 0; i < 5; i++)
		{
			AEMtx33Identity(&m_TransformBackDrops3Data[i]);
			AEMtx33ScaleApply(&m_TransformBackDrops3Data[i], &m_TransformBackDrops3Data[i], 480.0f, 360.f);
			AEMtx33TransApply(&m_TransformBackDrops3Data[i], &m_TransformBackDrops3Data[i], (i - 2) * 480.0f - static_cast<f32>(mouseX / 27.0f),
				205.f + static_cast<f32>(mouseY / 50.0f));
		}
	}
	else
		for (int i = 0; i < 5; i++) AEMtx33Identity(&m_TransformBackDrops3Data[i]);
}

std::vector<std::string> SceneLevelBuilder::GenerateEnemyToSpawn()
{
	m_CombatNames.clear();
	int TotalProb = 0; //Get total probability
	for (int curr : m_SceneLevelDataList[m_currLevel].m_EnemySpawnWeight)
	{
		TotalProb += curr;
	}
	for (int i = 0; i < m_SceneLevelDataList[m_currLevel].m_MaxEnemies; i++)
	{
		std::string Ref = "";
		int randnum = static_cast<int>(AEClamp(static_cast<f32>(rand() % TotalProb), 1.0f, static_cast<f32>(TotalProb)));//This is the rand probability of which type of sceneobjects to spawn
		int temp = 0;//Disregard this: for loop below
		for (int curr : m_SceneLevelDataList[m_currLevel].m_EnemySpawnWeight)
		{
			randnum -= curr;
			if (randnum < 0)
			{
				Ref = m_SceneLevelDataList[m_currLevel].m_EnemyTypes[temp];
				break;
			}
			temp++;
		}
		m_CombatNames.push_back(Ref);

		if (rand() % 100 < 10)
		{
			break;
		}
	}
	

	return m_CombatNames;
}

////////////////////////////////////////////////////////////////////////////
/* TO BE DELETED
//Placement Tool (Remove once done)
static double x = 1, y = 1;
if (AEInputCheckCurr(AEVK_W))
{
	y += 0.5;
}
if (AEInputCheckCurr(AEVK_S))
{
	y -= 0.5;
}
if (AEInputCheckCurr(AEVK_A))
{
	x -= 0.55;
}
if (AEInputCheckCurr(AEVK_D))
{
	x += 0.55;
}
static double mx = 0, my = 0;
if (AEInputCheckCurr(AEVK_UP))
{
	mx += 0.55;
}
if (AEInputCheckCurr(AEVK_DOWN))
{
	mx -= 0.55;
}
if (AEInputCheckCurr(AEVK_RIGHT))
{
	my += 12.55;
}
if (AEInputCheckCurr(AEVK_LEFT))
{
	my -= 12.55;
}
//AEMtx33 scale = { 0 }, trans = { 0 };
//AEMtx33Scale(&scale, x, y);
//AEMtx33Trans(&trans, mx, my);
//AEMtx33Concat(&m_TransformFogData, &trans, &scale);
//
//cout << x << " " << y << " " << mx << endl;
*/