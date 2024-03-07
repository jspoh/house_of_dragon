#include "SceneLevelBuilder.h"

int t_CenterFloorNum = static_cast<int>(SIZE_OF_FLOOR / 2);

SceneLevelBuilder::v_SceneObject::v_SceneObject()
	:m_TexRef(""), m_RenderOrder(0), m_Transparency(-1.5f)
{
	AEMtx33Identity(&m_TransformData);
	AEMtx33Identity(&m_Scale);
	AEMtx33Identity(&m_Trans);
}

SceneLevelBuilder::v_SceneLevelData::v_SceneLevelData()
	:m_LevelName(""),
	m_Completed(false),
	m_MaxEnemies(0),
	m_DayTime(0),
	m_EnemyTypes(),
	m_EnemySpawnWeight(),
	m_SceneObjTypes(),
	m_SceneObjSpawnWeight() {}


SceneLevelBuilder::SceneLevelBuilder():
	m_StopMovement(false),
	m_PanCloseToGround(false)
{
	RenderHelper::getInstance()->registerTexture("TEST", "Assets/TEST.png");
	/////////////////////////////////////////////////////////////////////////////////
	//LOAD ALL TEXTURES - SHIFT TO RENDERHELPER
	/*********************************************
	//Forest
	**********************************************/
	RenderHelper::getInstance()->registerTexture("FLOOR_LEFT_1", "Assets/SceneObjects/FLOOR/Scene_FloorSideLeft_Grass.png");
	RenderHelper::getInstance()->registerTexture("FLOOR_CENTER_1", "Assets/SceneObjects/FLOOR/Scene_Floor_Path.png");
	RenderHelper::getInstance()->registerTexture("FLOOR_RIGHT_1", "Assets/SceneObjects/FLOOR/Scene_FloorSideRight_Grass.png");

	/*********************************************
	//Sky
	**********************************************/
	RenderHelper::getInstance()->registerTexture("SKY_1", "Assets/SceneObjects/SKY/Scene_Sky_Clear.png");
	RenderHelper::getInstance()->registerTexture("SUN_OVERLAY_1", "Assets/SceneObjects/SKY/Scene_Sun_Overlaylighting.png");

	/*********************************************
	//Fog
	**********************************************/
	RenderHelper::getInstance()->registerTexture("FOG_1", "Assets/SceneObjects/BACKGROUND/Scene_Fog_Color.png");

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
	//RenderHelper::getInstance()->registerTexture("FORESTTREE_XL_1_SHADOW", "Assets/SceneObjects/SCENE_OBJECTS/GreenTreeXL_SHADOW.png");
	//RenderHelper::getInstance()->registerTexture("FORESTTREE_XL_1_DEAD", "Assets/SceneObjects/SCENE_OBJECTS/GreenTreeXL_DEAD.png");
	RenderHelper::getInstance()->registerTexture(EType_ForestTree_XL_2, "Assets/SceneObjects/SCENE_OBJECTS/GreenTreeXL_Dark.png");
	//RenderHelper::getInstance()->registerTexture("FORESTTREE_XL_2_SHADOW", "Assets/SceneObjects/SCENE_OBJECTS/GreenTreeXL_Dark_SHADOW.png");
	//RenderHelper::getInstance()->registerTexture("FORESTTREE_XL_2_DEAD", "Assets/SceneObjects/SCENE_OBJECTS/GreenTreeXL_Dark_DEAD.png");
	RenderHelper::getInstance()->registerTexture(EType_ForestTree_L_1, "Assets/SceneObjects/SCENE_OBJECTS/GreenTreeL.png");
	//RenderHelper::getInstance()->registerTexture("FORESTTREE_L_1_SHADOW", "Assets/SceneObjects/SCENE_OBJECTS/GreenTreeL_SHADOW.png");
	//RenderHelper::getInstance()->registerTexture("FORESTTREE_L_1_DEAD", "Assets/SceneObjects/SCENE_OBJECTS/GreenTreeL_DEAD.png");
	RenderHelper::getInstance()->registerTexture(EType_ForestTree_L_2, "Assets/SceneObjects/SCENE_OBJECTS/GreenTreeL_Dark.png");
	//RenderHelper::getInstance()->registerTexture("FORESTTREE_L_2_SHADOW", "Assets/SceneObjects/SCENE_OBJECTS/GreenTreeL_Dark_SHADOW.png");
	//RenderHelper::getInstance()->registerTexture("FORESTTREE_L_2_DEAD", "Assets/SceneObjects/SCENE_OBJECTS/GreenTreeL_Dark_DEAD.png");
	RenderHelper::getInstance()->registerTexture(EType_ForestTree_M_1, "Assets/SceneObjects/SCENE_OBJECTS/GreenTreeM.png");
	//RenderHelper::getInstance()->registerTexture("FORESTTREE_M_1_SHADOW", "Assets/SceneObjects/SCENE_OBJECTS/GreenTreeM_SHADOW.png");
	//RenderHelper::getInstance()->registerTexture("FORESTTREE_M_1_DEAD", "Assets/SceneObjects/SCENE_OBJECTS/GreenTreeM_DEAD.png");
	RenderHelper::getInstance()->registerTexture(EType_ForestTree_M_2, "Assets/SceneObjects/SCENE_OBJECTS/GreenTreeM_Dark.png");
	//RenderHelper::getInstance()->registerTexture("FORESTTREE_M_2_SHADOW", "Assets/SceneObjects/SCENE_OBJECTS/GreenTreeM_Dark_SHADOW.png");
	//RenderHelper::getInstance()->registerTexture("FORESTTREE_M_2_DEAD", "Assets/SceneObjects/SCENE_OBJECTS/GreenTreeM_Dark_DEAD.png");
	RenderHelper::getInstance()->registerTexture(EType_ForestTree_S_1, "Assets/SceneObjects/SCENE_OBJECTS/GreenTreeS.png");
	//RenderHelper::getInstance()->registerTexture("FORESTTREE_S_1_SHADOW", "Assets/SceneObjects/SCENE_OBJECTS/GreenTreeS_SHADOW.png");
	//RenderHelper::getInstance()->registerTexture("FORESTTREE_S_1_DEAD", "Assets/SceneObjects/SCENE_OBJECTS/GreenTreeS_DEAD.png");
	RenderHelper::getInstance()->registerTexture(EType_ForestTree_S_2, "Assets/SceneObjects/SCENE_OBJECTS/GreenTreeS_Dark.png");
	//RenderHelper::getInstance()->registerTexture("FORESTTREE_S_2_SHADOW", "Assets/SceneObjects/SCENE_OBJECTS/GreenTreeS_Dark_SHADOW.png");
	//RenderHelper::getInstance()->registerTexture("FORESTTREE_S_2_DEAD", "Assets/SceneObjects/SCENE_OBJECTS/GreenTreeS_Dark_DEAD.png");

	RenderHelper::getInstance()->registerTexture(EType_NightTree_XL_1, "Assets/SceneObjects/SCENE_OBJECTS/NightTreeXL.png");
	//RenderHelper::getInstance()->registerTexture("NIGHTTREE_XL_1_SHADOW", "Assets/SceneObjects/SCENE_OBJECTS/NightTreeXL_SHADOW.png");
	//RenderHelper::getInstance()->registerTexture("NIGHTTREE_XL_1_DEAD", "Assets/SceneObjects/SCENE_OBJECTS/NightTreeXL_DEAD.png");
	RenderHelper::getInstance()->registerTexture(EType_NightTree_XL_2, "Assets/SceneObjects/SCENE_OBJECTS/NightTreeXL_Dark.png");
	//RenderHelper::getInstance()->registerTexture("NIGHTTREE_XL_2_SHADOW", "Assets/SceneObjects/SCENE_OBJECTS/NightTreeXL_Dark_SHADOW.png");
	//RenderHelper::getInstance()->registerTexture("NIGHTTREE_XL_2_DEAD", "Assets/SceneObjects/SCENE_OBJECTS/NightTreeXL_Dark_DEAD.png");
	RenderHelper::getInstance()->registerTexture(EType_NightTree_L_1, "Assets/SceneObjects/SCENE_OBJECTS/NightTreeL.png");
	//RenderHelper::getInstance()->registerTexture("NIGHTTREE_L_1_SHADOW", "Assets/SceneObjects/SCENE_OBJECTS/NightTreeL_SHADOW.png");
	//RenderHelper::getInstance()->registerTexture("NIGHTTREE_L_1_DEAD", "Assets/SceneObjects/SCENE_OBJECTS/NightTreeL_DEAD.png");
	RenderHelper::getInstance()->registerTexture(EType_NightTree_L_2, "Assets/SceneObjects/SCENE_OBJECTS/NightTreeL_Dark.png");
	//RenderHelper::getInstance()->registerTexture("NIGHTTREE_L_2_SHADOW", "Assets/SceneObjects/SCENE_OBJECTS/NightTreeL_Dark_SHADOW.png");
	//RenderHelper::getInstance()->registerTexture("NIGHTTREE_L_2_DEAD", "Assets/SceneObjects/SCENE_OBJECTS/NightTreeL_Dark_DEAD.png");
	RenderHelper::getInstance()->registerTexture(EType_NightTree_M_1, "Assets/SceneObjects/SCENE_OBJECTS/NightTreeM.png");
	//RenderHelper::getInstance()->registerTexture("NIGHTTREE_M_1_SHADOW", "Assets/SceneObjects/SCENE_OBJECTS/NightTreeM_SHADOW.png");
	//RenderHelper::getInstance()->registerTexture("NIGHTTREE_M_1_DEAD", "Assets/SceneObjects/SCENE_OBJECTS/NightTreeM_DEAD.png");
	RenderHelper::getInstance()->registerTexture(EType_NightTree_M_2, "Assets/SceneObjects/SCENE_OBJECTS/NightTreeM_Dark.png");
	//RenderHelper::getInstance()->registerTexture("NIGHTTREE_M_2_SHADOW", "Assets/SceneObjects/SCENE_OBJECTS/NightTreeM_Dark_SHADOW.png");
	//RenderHelper::getInstance()->registerTexture("NIGHTTREE_M_2_DEAD", "Assets/SceneObjects/SCENE_OBJECTS/NightTreeM_Dark_DEAD.png");
	RenderHelper::getInstance()->registerTexture(EType_NightTree_S_1, "Assets/SceneObjects/SCENE_OBJECTS/NightTreeS.png");
	//RenderHelper::getInstance()->registerTexture("NIGHTTREE_S_1_SHADOW", "Assets/SceneObjects/SCENE_OBJECTS/NightTreeS_SHADOW.png");
	//RenderHelper::getInstance()->registerTexture("NIGHTTREE_S_1_DEAD", "Assets/SceneObjects/SCENE_OBJECTS/NightTreeS_DEAD.png");
	RenderHelper::getInstance()->registerTexture(EType_NightTree_S_2, "Assets/SceneObjects/SCENE_OBJECTS/NightTreeS_Dark.png");
	//RenderHelper::getInstance()->registerTexture("NIGHTTREE_S_2_SHADOW", "Assets/SceneObjects/SCENE_OBJECTS/NightTreeS_Dark_SHADOW.png");
	//RenderHelper::getInstance()->registerTexture("NIGHTTREE_S_2_DEAD", "Assets/SceneObjects/SCENE_OBJECTS/NightTreeS_Dark_DEAD.png");

	//TO BE DELETED
	pFloorTex = AEGfxTextureLoad("Assets/SceneObjects/FLOOR/Scene_Floor_Grass_3D.png");
	pSideRightFloorTex = AEGfxTextureLoad("Assets/SceneObjects/FLOOR/Scene_FloorSideRight_Sand_3D.png");
	pSideLeftFloorTex = AEGfxTextureLoad("Assets/SceneObjects/FLOOR/Scene_FloorSideLeft_Sand_3D.png");
	pSkyTex = AEGfxTextureLoad("Assets/SceneObjects/SKY/Scene_Sky_Clear.png");
	pSunOverlayTex = AEGfxTextureLoad("Assets/SceneObjects/SKY/Scene_Sun_Overlaylighting.png");
	pFogTex = AEGfxTextureLoad("Assets/SceneObjects/BACKGROUND/Scene_Fog_Color.png");
	pEnemyTex = AEGfxTextureLoad("Assets/SceneObjects/SCENE_OBJECTS/GreenTreeXL.png");

	m_Floor = new v_FloorData * [SIZE_OF_FLOOR];
	m_FloorOBJs = new std::list<v_SceneObject>* [SIZE_OF_FLOOR];
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
			m_tileSP[i][j].m_X = (36 / NUM_OF_TILESPAWNPOINTS * j) - 18;
			m_tileSP[i][j].m_Y = (50 / NUM_OF_TILESPAWNPOINTS * i) - 25;
		}
	}

	///////////////////////////////////////////////////////////////////
	// Load Level Data from Json
	for (int i = 0; i < Database::getInstance()->data["levels"].size(); i++)
	{
		v_SceneLevelData t_curr{};
		t_curr.m_LevelName = Database::getInstance()->data["levels"][i]["levelName"];
		m_SceneLevelDataList.push_back(t_curr);
	}


	Init();

	bool tex = Database::getInstance()->data["levels"][0]["unlocked"];
}
SceneLevelBuilder::~SceneLevelBuilder()
{                          
	Exit();
}

void SceneLevelBuilder::Init()
{
	/*******************************************************************************/
	//MAIN FLOOR
	for (int j = 0; j < SIZE_OF_FLOOR; j++)
	{
		for (int i = 0; i < NUM_OF_TILES; i++)
		{
			switch (i)
			{
				//Out of Screen Floor
			case 0:
				AEMtx33Scale(&m_Floor[j][i].m_Scale, 8000.f, 1262.f);
				AEMtx33Trans(&m_Floor[j][i].m_Trans, 16000 * (j-t_CenterFloorNum), -2829);
				break;
			case 1:
				AEMtx33Scale(&m_Floor[j][i].m_Scale, 7000.f, 1262.f);
				AEMtx33Trans(&m_Floor[j][i].m_Trans, 5750 * (j - t_CenterFloorNum), -2229);
				break;
			case 2:
				AEMtx33Scale(&m_Floor[j][i].m_Scale, 6000.f, 1262.f);
				AEMtx33Trans(&m_Floor[j][i].m_Trans, 4350 * (j - t_CenterFloorNum), -1629);
				break;
				//First Floor
			case 3:
				AEMtx33Scale(&m_Floor[j][i].m_Scale, 2940.f, 616.f);
				AEMtx33Trans(&m_Floor[j][i].m_Trans, 2150 * (j - t_CenterFloorNum), -696);
				break;
				//Second Floor
			case 4:
				AEMtx33Scale(&m_Floor[j][i].m_Scale, 1593.0f, 339.f);
				AEMtx33Trans(&m_Floor[j][i].m_Trans, 1150 * (j - t_CenterFloorNum), -282);
				break;
				//Third floor
			case 5:
				AEMtx33Scale(&m_Floor[j][i].m_Scale, 779.0f, 133.f);
				AEMtx33Trans(&m_Floor[j][i].m_Trans, 555 * (j - t_CenterFloorNum), -50);
				break;
				//Fourth floor
			case 6:
				AEMtx33Scale(&m_Floor[j][i].m_Scale, 381.0f, 47.f);
				AEMtx33Trans(&m_Floor[j][i].m_Trans, 270 * (j - t_CenterFloorNum), 39);
				break;
				//Fifth floor
			case 7:
				AEMtx33Scale(&m_Floor[j][i].m_Scale, 181.0f, 14.f);
				AEMtx33Trans(&m_Floor[j][i].m_Trans, 130 * (j - t_CenterFloorNum), 69);
				break;
				//Sixth floor
			case 8:
				AEMtx33Scale(&m_Floor[j][i].m_Scale, 85.0f, 4.f);
				AEMtx33Trans(&m_Floor[j][i].m_Trans, 59 * (j - t_CenterFloorNum), 78);
				break;
				//Seventh floor
			case 9:
				AEMtx33Scale(&m_Floor[j][i].m_Scale, 33.0f, 1.f);
				AEMtx33Trans(&m_Floor[j][i].m_Trans, 25 * (j - t_CenterFloorNum), 80);
				break;
			default:
				std::cout << "Error pls check floor" << std::endl;
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
	

	AEMtx33 scale, trans;
	//////////////////////////////////////////////////////////////////
	//// Create Scene Objects
	//// Use this to spawn objects into scene when starting
	//for (int j = 0; j < SIZE_OF_FLOOR; j++)
	//{
	//	for (int i = 0; i < NUM_OF_TILES; i++)
	//	{
	//		if (j == t_CenterFloorNum)
	//			continue;
	//	    v_SceneObject newObj;
	//		newObj.m_TexRef = "Mystery_S_Enemy";
	//		AEMtx33Trans(&newObj.m_Trans, (rand() % 20 - 10) * 1 ,20);
	//		AEMtx33Scale(&newObj.m_Scale, 0.2f, 0.2f);
	//		m_FloorOBJs[j][i].push_back(newObj);
	//	}
	//}

	/////////////////////////////////////////////////////////////
	// ETC Transformations
	//DO SKY DATA
	AEMtx33Scale(&scale, 2000.0f, 800.f);
	AEMtx33Trans(&trans, 0, 200);
	AEMtx33Concat(&m_TransformSkyData, &trans, &scale);

	//Do Sun Data
	AEMtx33Scale(&scale, 50.0f, 50.f);
	AEMtx33Trans(&trans, 150, 250);
	AEMtx33Concat(&m_TransformSunData, &trans, &scale);
	AEMtx33Scale(&scale, 120.0f, 120.f);
	AEMtx33Trans(&trans, 149, 250);
	AEMtx33Concat(&m_TransformSunOverlayData, &trans, &scale);

	//DO FOG DATA
	AEMtx33Scale(&scale, 2000.0f, 70.f);
	AEMtx33Trans(&trans, 0, 80);
	AEMtx33Concat(&m_TransformFogData, &trans, &scale);


}

void SceneLevelBuilder::Update(double dt)
{
	//Placement Tool (Remove once done)

	static double x = 1, y = 1;
	if (AEInputCheckCurr(AEVK_W))
	{
		y += 0.05;
	}
	if (AEInputCheckCurr(AEVK_S))
	{
		y -= 0.05;
	}
	if (AEInputCheckCurr(AEVK_A))
	{
		x -= 0.05;
	}
	if (AEInputCheckCurr(AEVK_D))
	{
		x += 0.05;
	}
	static double mx = 0, my = 200;
	if (AEInputCheckCurr(AEVK_UP))
	{
		mx += 0.55;
	}
	if (AEInputCheckCurr(AEVK_DOWN))
	{
		mx -= 0.55;
	}
	//AEMtx33 scale = { 0 }, trans = { 0 };
	//AEMtx33Scale(&scale, x, y);
	//AEMtx33Trans(&trans, mx, my);
	//AEMtx33Concat(&m_TransformFogData, &trans, &scale);
	//
	//cout << x << " " << y << " " << mx << endl;

	static float t_MovementSpeed = 1.0f;
	static int t_PanCloseToGroundValue = 80;
	static int t_PanSideWays = 80;
	static int PanDown = 0;
	t_MovementSpeed += t_MovementSpeed < TOP_MOVEMENT_SPEED ? static_cast<float>(dt * 5.f) : 0;

	if (AEInputCheckCurr(AEVK_Z))
	{
		m_StopMovement = true;
		t_MovementSpeed = 0.0f;
	}
	else
		m_StopMovement = false;

	if (AEInputCheckCurr(AEVK_V))
	{
		m_PanCloseToGround = true;
		t_PanCloseToGroundValue -= t_PanCloseToGroundValue > 30? 1 : 0;
		PanDown -= PanDown > -100 ? 1 : 0;
	}
	else
	{
		m_PanCloseToGround = false;
		t_PanCloseToGroundValue += t_PanCloseToGroundValue < 80 ? 1 : 0;
		PanDown += PanDown < 0 ? 1 : 0;
	}

	//NOT WORKING WILL GIVE UP
	if (AEInputCheckCurr(AEVK_C))
	{
		t_PanSideWays -= t_PanSideWays > 30 ? 1 : 0;
	}
	else
	{
		t_PanSideWays += t_PanSideWays < 80 ? 1 : 0;
	}

	AEGfxSetCamPosition(0, -PanDown);
	

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
				(m_NextFloorData.m[0][0] - m_CurrFloorData.m[0][0]) / t_PanCloseToGroundValue,
				(m_NextFloorData.m[0][1] - m_CurrFloorData.m[0][1]) / 80,
				(m_NextFloorData.m[0][2] - m_CurrFloorData.m[0][2]) / t_PanCloseToGroundValue,
				(m_NextFloorData.m[1][0] - m_CurrFloorData.m[1][0]) / 80,
				(m_NextFloorData.m[1][1] - m_CurrFloorData.m[1][1]) / 80,
				(m_NextFloorData.m[1][2] - m_CurrFloorData.m[1][2]) / 80,
				(m_NextFloorData.m[2][0] - m_CurrFloorData.m[2][0]) / 80,
				(m_NextFloorData.m[2][1] - m_CurrFloorData.m[2][1]) / 80,
				(m_NextFloorData.m[2][2] - m_CurrFloorData.m[2][2]) / 80
				};

				//Incrementing speed
				m_Floor[j][i].m_currFloorSpeed.m[0][0] += m_Floor[j][i].m_currFloorSpeed.m[0][0] < m_MinimumNextFloorSpeed.m[0][0] ? dt * m_MinimumNextFloorSpeed.m[0][0] * t_MovementSpeed : m_Floor[j][i].m_currFloorSpeed.m[0][0] > m_MinimumNextFloorSpeed.m[0][0] ? dt * m_MinimumNextFloorSpeed.m[0][0] * t_MovementSpeed : 0;
				m_Floor[j][i].m_currFloorSpeed.m[0][1] += m_Floor[j][i].m_currFloorSpeed.m[0][1] < m_MinimumNextFloorSpeed.m[0][1] ? dt * m_MinimumNextFloorSpeed.m[0][1] * t_MovementSpeed : m_Floor[j][i].m_currFloorSpeed.m[0][1] > m_MinimumNextFloorSpeed.m[0][1] ? dt * m_MinimumNextFloorSpeed.m[0][1] * t_MovementSpeed : 0;
				m_Floor[j][i].m_currFloorSpeed.m[0][2] += m_Floor[j][i].m_currFloorSpeed.m[0][2] < m_MinimumNextFloorSpeed.m[0][2] ? dt * m_MinimumNextFloorSpeed.m[0][2] * t_MovementSpeed : m_Floor[j][i].m_currFloorSpeed.m[0][2] > m_MinimumNextFloorSpeed.m[0][2] ? dt * m_MinimumNextFloorSpeed.m[0][2] * t_MovementSpeed : 0;
				m_Floor[j][i].m_currFloorSpeed.m[1][0] += m_Floor[j][i].m_currFloorSpeed.m[1][0] < m_MinimumNextFloorSpeed.m[1][0] ? dt * m_MinimumNextFloorSpeed.m[1][0] * t_MovementSpeed : m_Floor[j][i].m_currFloorSpeed.m[1][0] > m_MinimumNextFloorSpeed.m[1][0] ? dt * m_MinimumNextFloorSpeed.m[1][0] * t_MovementSpeed : 0;
				m_Floor[j][i].m_currFloorSpeed.m[1][1] += m_Floor[j][i].m_currFloorSpeed.m[1][1] < m_MinimumNextFloorSpeed.m[1][1] ? dt * m_MinimumNextFloorSpeed.m[1][1] * t_MovementSpeed : m_Floor[j][i].m_currFloorSpeed.m[1][1] > m_MinimumNextFloorSpeed.m[1][1] ? dt * m_MinimumNextFloorSpeed.m[1][1] * t_MovementSpeed : 0;
				m_Floor[j][i].m_currFloorSpeed.m[1][2] += m_Floor[j][i].m_currFloorSpeed.m[1][2] < m_MinimumNextFloorSpeed.m[1][2] ? dt * m_MinimumNextFloorSpeed.m[1][2] * t_MovementSpeed : m_Floor[j][i].m_currFloorSpeed.m[1][2] > m_MinimumNextFloorSpeed.m[1][2] ? dt * m_MinimumNextFloorSpeed.m[1][2] * t_MovementSpeed : 0;
				m_Floor[j][i].m_currFloorSpeed.m[2][0] += m_Floor[j][i].m_currFloorSpeed.m[2][0] < m_MinimumNextFloorSpeed.m[2][0] ? dt * m_MinimumNextFloorSpeed.m[2][0] * t_MovementSpeed : m_Floor[j][i].m_currFloorSpeed.m[2][0] > m_MinimumNextFloorSpeed.m[2][0] ? dt * m_MinimumNextFloorSpeed.m[2][0] * t_MovementSpeed : 0;
				m_Floor[j][i].m_currFloorSpeed.m[2][1] += m_Floor[j][i].m_currFloorSpeed.m[2][1] < m_MinimumNextFloorSpeed.m[2][1] ? dt * m_MinimumNextFloorSpeed.m[2][1] * t_MovementSpeed : m_Floor[j][i].m_currFloorSpeed.m[2][1] > m_MinimumNextFloorSpeed.m[2][1] ? dt * m_MinimumNextFloorSpeed.m[2][1] * t_MovementSpeed : 0;
				m_Floor[j][i].m_currFloorSpeed.m[2][2] += m_Floor[j][i].m_currFloorSpeed.m[2][2] < m_MinimumNextFloorSpeed.m[2][2] ? dt * m_MinimumNextFloorSpeed.m[2][2] * t_MovementSpeed : m_Floor[j][i].m_currFloorSpeed.m[2][2] > m_MinimumNextFloorSpeed.m[2][2] ? dt * m_MinimumNextFloorSpeed.m[2][2] * t_MovementSpeed : 0;
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

							CurrentTileNumFurthest = m_Floor[j][i].m_FloorNum;

							t_ShiftRow.push_back(m_Floor[j][i].m_FloorNum);
							
						}
					}
					else
						m_Floor[j][i].m_currFloorTimer += dt;
				}
			}

			if (t_ShiftRow.size()>=1)
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
		//UPDATE OBJs Pos and Logic
		//////////////////////////////////////////////////////////////////////////
		//GameObjectManager::GetInstance()->Update(dt);
		v_SceneObject temp;
		pair<int, int> t_TransScaleModifier = { 60, 48}; //For rand on tile pos
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

					//Skew on the tile
					if (!AEInputCheckCurr(AEVK_L))
					(*it).m_TransformData.m[1][0] = 0.30 * (j - t_CenterFloorNum) / (m_Floor[j][i].m_currFloorNum + 1) ;

					//Scale with the tile
					AEMtx33ScaleApply(&(*it).m_TransformData, &(*it).m_TransformData, m_Floor[j][i].m_TransformFloorCurr.m[0][0] / (1 / (*it).m_Scale.m[0][0]), m_Floor[j][i].m_TransformFloorCurr.m[0][0] / (1 / (*it).m_Scale.m[1][1]));

					//Translate to the tile
					//(*it).m_TransformData.m[0][2] = m_Floor[j][i].m_Trans.m[0][2] * 1.3* (*it).m_Scale.m[0][0];
					//(*it).m_TransformData.m[1][2] = m_Floor[j][i].m_Trans.m[1][2] * 0.7* (*it).m_Scale.m[1][1];

					//CHANGE TO THIS ANGLE
					(*it).m_TransformData.m[0][2] = m_Floor[j][i].m_Trans.m[0][2] * (0.55) * (*it).m_Scale.m[0][0];
					(*it).m_TransformData.m[1][2] = m_Floor[j][i].m_Trans.m[1][2] * (0.85) * (*it).m_Scale.m[1][1];

					//Translate to its specific position on the tile 
					AEMtx33TransApply(&(*it).m_TransformData, &(*it).m_TransformData, 
						(*it).m_Trans.m[0][2] * m_Floor[j][i].m_TransformFloorCurr.m[0][0] / ((t_TransScaleModifier.first) / (*it).m_Scale.m[0][0]), 
						0/*(*it).m_Trans.m[1][2] * m_Floor[j][i].m_TransformFloorCurr.m[0][0] / ((t_TransScaleModifier.second) / (*it).m_Scale.m[1][1])*/);
				
					//Adjusting Transparency
					(*it).m_Transparency += dt*1.5;
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
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);

	// Set the color to add to nothing, so that we don't alter the sprite's color
	AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 1.0f);

	// Set blend mode to AE_GFX_BM_BLEND
	// This will allow transparency.
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);

	////////////////////////////////////////////////////////////////////////
	//SKY RENDER
	AEGfxSetTransparency(1.0f);
	//Sky
	AEGfxTextureSet(RenderHelper::getInstance()->getTextureByRef("SKY_1"), 0, 0);
	AEGfxSetTransform(m_TransformSkyData.m);
	AEGfxMeshDraw(RenderHelper::getInstance()->GetdefaultMesh(), AE_GFX_MDM_TRIANGLES);

	//Sun
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	AEGfxSetColorToAdd(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetTransparency(0.0f);
	AEGfxSetTransform(m_TransformSunData.m);
	AEGfxMeshDraw(RenderHelper::getInstance()->GetdefaultMesh(), AE_GFX_MDM_TRIANGLES);
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetTransparency(1.0f);
	AEGfxTextureSet(RenderHelper::getInstance()->getTextureByRef("SUN_OVERLAY_1"), 0, 0);
	AEGfxSetTransform(m_TransformSunOverlayData.m);
	AEGfxMeshDraw(RenderHelper::getInstance()->GetdefaultMesh(), AE_GFX_MDM_TRIANGLES);

	//////////////////////////////////////////////////////////////////////////////////////////
	//// Floors
	//// //////////////////////////////////////////////////////////////////////////////////
	//// Tell the engine to get ready to draw something with texture.
	//// Set the color to add to nothing, so that we don't alter the sprite's color
	AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 1.0f);

	std::string texRef = "Floor_Center_";// + LEVELNUM
	//Main Floor
	AEGfxTextureSet(RenderHelper::getInstance()->getTextureByRef("FLOOR_CENTER_1"), 0, 0);
	for (int i = NUM_OF_TILES - 1; i > -1; i--)
	{
		if (m_Floor[t_CenterFloorNum][i].m_IsRender)
		{
			AEGfxSetTransform(m_Floor[t_CenterFloorNum][i].m_TransformFloorCurr.m);
			AEGfxMeshDraw(RenderHelper::getInstance()->GetdefaultMesh(), AE_GFX_MDM_TRIANGLES);
		}
	}

	////Left Floor
	AEGfxTextureSet(RenderHelper::getInstance()->getTextureByRef("FLOOR_LEFT_1"), 0, 0);
	for (int j = 0; j < SIZE_OF_FLOOR - (t_CenterFloorNum + 1); j++)
	{
		for (int i = NUM_OF_TILES - 1; i > -1; i--)
		{
			if (m_Floor[j][i].m_IsRender)
			{
				AEGfxSetTransform(m_Floor[j][i].m_TransformFloorCurr.m);
				AEGfxMeshDraw(RenderHelper::getInstance()->GetdefaultMesh(), AE_GFX_MDM_TRIANGLES);
			}
		}
	}
	//Right Floor
	AEGfxTextureSet(RenderHelper::getInstance()->getTextureByRef("FLOOR_RIGHT_1"), 0, 0);
	for (int j = (t_CenterFloorNum + 1); j < SIZE_OF_FLOOR; j++)
	{
		for (int i = NUM_OF_TILES - 1; i > -1; i--)
		{
			if (m_Floor[j][i].m_IsRender)
			{
				AEGfxSetTransform(m_Floor[j][i].m_TransformFloorCurr.m);
				AEGfxMeshDraw(RenderHelper::getInstance()->GetdefaultMesh(), AE_GFX_MDM_TRIANGLES);
			}
		}
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////
	//Fog
	AEGfxTextureSet(RenderHelper::getInstance()->getTextureByRef("FOG_1"), 0, 0);
	AEGfxSetTransform(m_TransformFogData.m);
	AEGfxMeshDraw(RenderHelper::getInstance()->GetdefaultMesh(), AE_GFX_MDM_TRIANGLES);


	/////////////////////////////////////////////////////////////////////////////////////////////////
	//SceneObj
	//Render Left Side
	for (int j = 0; j < SIZE_OF_FLOOR/2; j++)
	{
		for (int i = 0; i < NUM_OF_TILES - 1; i++)
		{
			////////////////////////////////////////////////
			// CurrentTileNumFurthest = 4
			// -> 4 3 2 1 0 9 8 7 6 5 -> Render in this way
			////////////////////////////////////////////////
			int tempTileNum = CurrentTileNumFurthest - i;
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
	for (int j = SIZE_OF_FLOOR - 1; j >= SIZE_OF_FLOOR/2; j--)
	{
		for (int i = 0; i < NUM_OF_TILES - 1; i++)
		{
			////////////////////////////////////////////////
			// CurrentTileNumFurthest = 4
			// -> 4 3 2 1 0 9 8 7 6 5 -> Render in this way
			////////////////////////////////////////////////
			int tempTileNum = CurrentTileNumFurthest - i;
			if (tempTileNum < 0)
				tempTileNum += NUM_OF_TILES - 1;

			for (std::list<v_SceneObject>::iterator it = m_FloorOBJs[j][tempTileNum].begin();
				it != m_FloorOBJs[j][tempTileNum].end();
				it++)
			{
				AEGfxTextureSet(RenderHelper::getInstance()->GetTexture((*it).m_Type),0,0);
				AEGfxSetTransparency((*it).m_Transparency);
				AEGfxSetTransform((*it).m_TransformData.m);
				AEGfxMeshDraw(RenderHelper::getInstance()->GetdefaultMesh(), AE_GFX_MDM_TRIANGLES);
			}
		}
	}

	//Enable later
	//GameObjectManager::GetInstance()->Render();
}
void SceneLevelBuilder::Exit()
{
	//Clear All Texture
	AEGfxTextureUnload(pFloorTex);
	AEGfxTextureUnload(pSideRightFloorTex);
	AEGfxTextureUnload(pSideLeftFloorTex);
	AEGfxTextureUnload(pSkyTex);
	AEGfxTextureUnload(pSunOverlayTex);
	AEGfxTextureUnload(pFogTex);
	AEGfxTextureUnload(pEnemyTex);

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

	//Clear Wall

	//Clear Object in scene
	GameObjectManager::GetInstance()->Exit();
	GameObjectManager::GetInstance()->Destroy();
}


void SceneLevelBuilder::CreateRowOBJs(int t_tileNum)
{
	srand(static_cast<unsigned> (time(0)));
	//if(false)
	for (int j = 0; j < SIZE_OF_FLOOR; j++)
	{
		//Skip centre
		if (j == t_CenterFloorNum)
			continue;

		for (int i = MAX_NUM_SCENEOBJS_TILE/*3*//*rand() % NUM_OF_TILESPAWNPOINTS + MAX_NUM_SCENEOBJS_TILE/10*/; i > 0; i--)
		{
			v_SceneObject newObj;

			//Selecting Entities to Spawn
			newObj.m_Type = static_cast<v_SceneObjectTypes>(AEClamp(rand()%(v_SceneObjectTypes::ETYPE_LAST - v_SceneObjectTypes::ETYPE_NONE) + v_SceneObjectTypes::ETYPE_NONE,
				v_SceneObjectTypes::ETYPE_NONE + 1,
				v_SceneObjectTypes::ETYPE_LAST - 1));

			//Random Selection of Spawn location on tile
			int t_RandX, t_RandY;
			if (j == t_CenterFloorNum - 1 || j == t_CenterFloorNum + 1)
			{
				t_RandX = rand() % NUM_OF_TILESPAWNPOINTS / 2;
				t_RandY = rand() % NUM_OF_TILESPAWNPOINTS / 2;
			}
			else
			{
				t_RandX = rand() % NUM_OF_TILESPAWNPOINTS;
				t_RandY = rand() % NUM_OF_TILESPAWNPOINTS;
			}
					
			
			if (j < t_CenterFloorNum)//Left Side
			{
				AEMtx33Trans(&newObj.m_Trans, m_tileSP[t_RandY][t_RandX].m_X + m_tileSP[t_RandY][t_RandX].m_Y / 3, m_tileSP[t_RandY][t_RandX].m_Y);
			}
			else//Right Side
			{
				AEMtx33Trans(&newObj.m_Trans, -m_tileSP[t_RandY][t_RandX].m_X - m_tileSP[t_RandY][t_RandX].m_Y / 3, m_tileSP[t_RandY][t_RandX].m_Y);
			}

			////Random Scaling
			//float scale = (rand() % 20) * 0.01f + 0.1f;
			//AEMtx33Scale(&newObj.m_Scale, scale, scale);
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