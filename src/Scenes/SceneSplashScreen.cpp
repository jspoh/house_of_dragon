#include "SceneSplashScreen.h"

#include "../Backend/GameManager.h" //? Still thinking
#include "SceneManager.h"
#include "CombatScene/CombatScene.h"
#include <iostream> //Remove upon test finish

SceneSplashScreen* SceneSplashScreen::sInstance = new SceneSplashScreen(SceneManager::GetInstance());
AEGfxVertexList* pMesh = 0;

AEGfxTexture* pDigipenLogoTex;
bool ShowGame;
double LogoTimer;
AEMtx33 m_TransformDigipenLogoData;

AEGfxTexture* pFloorTex;
AEGfxTexture* pSideLeftFloorTex;
AEGfxTexture* pSideRightFloorTex;
AEGfxTexture* pSkyTex;
AEGfxTexture* pSunOverlayTex;
AEGfxTexture* pFogTex;


struct Floor
{
	AEMtx33 m_TransformFloorData;
	AEMtx33 m_TransformFloorCurr;
	int m_currFloorNum = 0;
	double m_currFloorTimer = 0;
	double m_FloorSpeedTimer = 0.5;
	AEMtx33 m_currFloorSpeed = { 0 };
	bool m_IsRender = true;
};
Floor m_Floor[10];
Floor m_RightSideFloor[10], m_RightSecondSideFloor[10], m_RightThirdSideFloor[10], m_RightFourthSideFloor[10];
Floor m_LeftSideFloor[10], m_LeftSecondSideFloor[10], m_LeftThirdSideFloor[10], m_LeftFourthSideFloor[10];
AEMtx33 m_TransformSkyData, m_TransformSunData, m_TransformSunOverlayData, m_TransformFogData;
//Floor m_RightSideFloor[10];
//double m_FloorSpeedTimer = 0.5;


SceneSplashScreen::SceneSplashScreen()
{
}

SceneSplashScreen::SceneSplashScreen(SceneManager* _sceneMgr)
{
	_sceneMgr->AddScene("SceneSplashScreen", this);
}

SceneSplashScreen::~SceneSplashScreen()
{
}

void SceneSplashScreen::Load()
{
	//Still debating whether need this
}

void SceneSplashScreen::Init()
{
	std::cout << "Loading Scene SplashScreen" << std::endl;
	

	// Informing the library that we're about to start adding triangles
	AEGfxMeshStart();

	// This shape has 2 triangles that makes up a square
	// Color parameters represent colours as ARGB
	// UV coordinates to read from loaded textures
	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFFFF0000, 0.0f, 1.0f,  // bottom-left: red
		0.5f, -0.5f, 0xFF00FF00, 1.0f, 1.0f,   // bottom-right: green
		-0.5f, 0.5f, 0xFF0000FF, 0.0f, 0.0f);  // top-left: blue

	AEGfxTriAdd(
		0.5f, -0.5f, 0xFF00FF00, 1.0f, 1.0f,   // bottom-right: green
		0.5f, 0.5f, 0xFFFFFFFF, 1.0f, 0.0f,    // top-right: white
		-0.5f, 0.5f, 0xFF0000FF, 0.0f, 0.0f);  // top-left: blue

	//AEGfxTriAdd(
	//	-0.5f, -0.5f, 0xFFFF0000, 0.0f, 1.0f,  // bottom-left: red
	//	0.5f, -1.0f, 0xFF00FF00, 1.0f, 1.0f,   // bottom-right: green
	//	-0.5f, 0.5f, 0xFF0000FF, 0.0f, 0.0f);  // top-left: blue
	//AEGfxTriAdd(
	//	0.5f, -1.0f, 0xFF00FF00, 1.0f, 1.0f,   // bottom-right: green
	//	0.5f, 1.0f, 0xFFFFFFFF, 1.0f, 0.0f,    // top-right: white
	//	-0.5f, 0.5f, 0xFF0000FF, 0.0f, 0.0f);  // top-left: blue

	// Saving the mesh (list of triangles) in pMesh
	pMesh = AEGfxMeshEnd();

	pDigipenLogoTex = AEGfxTextureLoad("Assets/DigiPen_Singapore_WEB_RED.png");
	ShowGame = false;
	LogoTimer = 5.0;

	pFloorTex = AEGfxTextureLoad("Assets/Scene_Floor_Grass_3D.png");
	pSideRightFloorTex = AEGfxTextureLoad("Assets/Scene_FloorSideRight_Sand_3D.png");
	pSideLeftFloorTex = AEGfxTextureLoad("Assets/Scene_FloorSideLeft_Sand_3D.png");
	pSkyTex = AEGfxTextureLoad("Assets/Scene_Sky_Clear.png");
	pSunOverlayTex = AEGfxTextureLoad("Assets/Scene_Sun_Overlaylighting.png");
	pFogTex = AEGfxTextureLoad("Assets/Scene_Fog_Color.png");
	/*******************************************************************************/
	//MAIN FLOOR
	AEMtx33 scale = { 0 }, trans = { 0 };
	//Out of Screen Floor
	AEMtx33Scale(&scale, 8000.f, 1262.f);
	AEMtx33Trans(&trans, 0, -2829);
	AEMtx33Concat(&m_Floor[0].m_TransformFloorData, &trans, &scale);
	m_Floor[0].m_currFloorNum = 0;
	AEMtx33Scale(&scale, 7000.f, 1262.f);
	AEMtx33Trans(&trans, 0, -2229);
	AEMtx33Concat(&m_Floor[1].m_TransformFloorData, &trans, &scale);
	m_Floor[1].m_currFloorNum = 1;
	AEMtx33Scale(&scale, 6000.f, 1262.f);
	AEMtx33Trans(&trans, 0, -1629);
	AEMtx33Concat(&m_Floor[2].m_TransformFloorData, &trans, &scale);
	m_Floor[2].m_currFloorNum = 2;
	//First floor
	AEMtx33Scale(&scale, 2940.f, 616.f);
	AEMtx33Trans(&trans, 0, -696);
	AEMtx33Concat(&m_Floor[3].m_TransformFloorData, &trans, &scale);
	m_Floor[3].m_currFloorNum = 3;
	//Second floor
	AEMtx33Scale(&scale, 1593.0f, 339.f);
	AEMtx33Trans(&trans, 0, -282);
	AEMtx33Concat(&m_Floor[4].m_TransformFloorData, &trans, &scale);
	m_Floor[4].m_currFloorNum = 4;
	//Third floor
	AEMtx33Scale(&scale, 779.0f, 133.f);
	AEMtx33Trans(&trans, 0, -50);
	AEMtx33Concat(&m_Floor[5].m_TransformFloorData, &trans, &scale);
	m_Floor[5].m_currFloorNum = 5;
	//Fourth floor
	AEMtx33Scale(&scale, 381.0f, 47.f);
	AEMtx33Trans(&trans, 0, 39);
	AEMtx33Concat(&m_Floor[6].m_TransformFloorData, &trans, &scale);
	m_Floor[6].m_currFloorNum = 6;
	//Fifth floor
	AEMtx33Scale(&scale, 181.0f, 14.f);
	AEMtx33Trans(&trans, 0, 69);
	AEMtx33Concat(&m_Floor[7].m_TransformFloorData, &trans, &scale);
	m_Floor[7].m_currFloorNum = 7;
	//Sixth floor
	AEMtx33Scale(&scale, 85.0f, 4.f);
	AEMtx33Trans(&trans, 0, 78);
	AEMtx33Concat(&m_Floor[8].m_TransformFloorData, &trans, &scale);
	m_Floor[8].m_currFloorNum = 8;
	//Seventh floor
	AEMtx33Scale(&scale, 33.0f, 1.f);
	AEMtx33Trans(&trans, 0, 80);
	AEMtx33Concat(&m_Floor[9].m_TransformFloorData, &trans, &scale);
	m_Floor[9].m_currFloorNum = 9;

	for (int i = 0; i < 10; i++)
		m_Floor[i].m_TransformFloorCurr = m_Floor[i].m_TransformFloorData;

	/*******************************************************************************/
	//RIGHT SIDE FLOORS
	//Out of Screen Floor
	AEMtx33Scale(&scale, 8000.f, 1262.f);
	AEMtx33Trans(&trans, 16000, -2829); //DONT NEED TO CARE
	AEMtx33Concat(&m_RightSideFloor[0].m_TransformFloorData, &trans, &scale);
	m_RightSideFloor[0].m_currFloorNum = 0;
	AEMtx33Scale(&scale, 7000.f, 1262.f);
	AEMtx33Trans(&trans, 5750, -2229);
	AEMtx33Concat(&m_RightSideFloor[1].m_TransformFloorData, &trans, &scale);
	m_RightSideFloor[1].m_currFloorNum = 1;
	AEMtx33Scale(&scale, 6000.f, 1262.f);
	AEMtx33Trans(&trans, 4350, -1629);
	AEMtx33Concat(&m_RightSideFloor[2].m_TransformFloorData, &trans, &scale);
	m_RightSideFloor[2].m_currFloorNum = 2;
	//First floor
	AEMtx33Scale(&scale, 2940.f, 616.f);
	AEMtx33Trans(&trans, 2150, -696);
	AEMtx33Concat(&m_RightSideFloor[3].m_TransformFloorData, &trans, &scale);
	m_RightSideFloor[3].m_currFloorNum = 3;
	//Second floor
	AEMtx33Scale(&scale, 1593.0f, 339.f);
	AEMtx33Trans(&trans, 1158, -282);
	AEMtx33Concat(&m_RightSideFloor[4].m_TransformFloorData, &trans, &scale);
	m_RightSideFloor[4].m_currFloorNum = 4;
	//Third floor
	AEMtx33Scale(&scale, 779.0f, 133.f);
	AEMtx33Trans(&trans, 556, -50);
	AEMtx33Concat(&m_RightSideFloor[5].m_TransformFloorData, &trans, &scale);
	m_RightSideFloor[5].m_currFloorNum = 5;
	//Fourth floor
	AEMtx33Scale(&scale, 381.0f, 47.f);
	AEMtx33Trans(&trans, 276, 39);
	AEMtx33Concat(&m_RightSideFloor[6].m_TransformFloorData, &trans, &scale);
	m_RightSideFloor[6].m_currFloorNum = 6;
	//Fifth floor
	AEMtx33Scale(&scale, 181.0f, 14.f);
	AEMtx33Trans(&trans, 131, 69);
	AEMtx33Concat(&m_RightSideFloor[7].m_TransformFloorData, &trans, &scale);
	m_RightSideFloor[7].m_currFloorNum = 7;
	//Sixth floor
	AEMtx33Scale(&scale, 85.0f, 4.f);
	AEMtx33Trans(&trans, 62, 78);
	AEMtx33Concat(&m_RightSideFloor[8].m_TransformFloorData, &trans, &scale);
	m_RightSideFloor[8].m_currFloorNum = 8;
	//Seventh floor
	AEMtx33Scale(&scale, 33.0f, 1.f);
	AEMtx33Trans(&trans, 25, 80);
	AEMtx33Concat(&m_RightSideFloor[9].m_TransformFloorData, &trans, &scale);
	m_RightSideFloor[9].m_currFloorNum = 9;

	for (int i = 0; i < 10; i++)
		m_RightSideFloor[i].m_TransformFloorCurr = m_RightSideFloor[i].m_TransformFloorData;

	/*******************************************************************************/
	//RIGHT 2nd SIDE FLOORS
	//Out of Screen Floor
	AEMtx33Scale(&scale, 8000.f, 1262.f);
	AEMtx33Trans(&trans, 32000, -2829); //DONT NEED TO CARE
	AEMtx33Concat(&m_RightSecondSideFloor[0].m_TransformFloorData, &trans, &scale);
	m_RightSecondSideFloor[0].m_currFloorNum = 0;
	AEMtx33Scale(&scale, 7000.f, 1262.f);
	AEMtx33Trans(&trans, 11500, -2229);
	AEMtx33Concat(&m_RightSecondSideFloor[1].m_TransformFloorData, &trans, &scale);
	m_RightSecondSideFloor[1].m_currFloorNum = 1;
	AEMtx33Scale(&scale, 6000.f, 1262.f);
	AEMtx33Trans(&trans, 8700, -1629);
	AEMtx33Concat(&m_RightSecondSideFloor[2].m_TransformFloorData, &trans, &scale);
	m_RightSecondSideFloor[2].m_currFloorNum = 2;
	//First floor
	AEMtx33Scale(&scale, 2940.f, 616.f);
	AEMtx33Trans(&trans, 4300, -696);
	AEMtx33Concat(&m_RightSecondSideFloor[3].m_TransformFloorData, &trans, &scale);
	m_RightSecondSideFloor[3].m_currFloorNum = 3;
	//Second floor
	AEMtx33Scale(&scale, 1593.0f, 339.f);
	AEMtx33Trans(&trans, 2308, -282);
	AEMtx33Concat(&m_RightSecondSideFloor[4].m_TransformFloorData, &trans, &scale);
	m_RightSecondSideFloor[4].m_currFloorNum = 4;
	//Third floor
	AEMtx33Scale(&scale, 779.0f, 133.f);
	AEMtx33Trans(&trans, 1104, -50);
	AEMtx33Concat(&m_RightSecondSideFloor[5].m_TransformFloorData, &trans, &scale);
	m_RightSecondSideFloor[5].m_currFloorNum = 5;
	//Fourth floor
	AEMtx33Scale(&scale, 381.0f, 47.f);
	AEMtx33Trans(&trans, 546, 39);
	AEMtx33Concat(&m_RightSecondSideFloor[6].m_TransformFloorData, &trans, &scale);
	m_RightSecondSideFloor[6].m_currFloorNum = 6;
	//Fifth floor
	AEMtx33Scale(&scale, 181.0f, 14.f);
	AEMtx33Trans(&trans, 259, 69);
	AEMtx33Concat(&m_RightSecondSideFloor[7].m_TransformFloorData, &trans, &scale);
	m_RightSecondSideFloor[7].m_currFloorNum = 7;
	//Sixth floor
	AEMtx33Scale(&scale, 85.0f, 4.f);
	AEMtx33Trans(&trans, 121, 78);
	AEMtx33Concat(&m_RightSecondSideFloor[8].m_TransformFloorData, &trans, &scale);
	m_RightSecondSideFloor[8].m_currFloorNum = 8;
	//Seventh floor
	AEMtx33Scale(&scale, 33.0f, 1.f);
	AEMtx33Trans(&trans, 48, 80);
	AEMtx33Concat(&m_RightSecondSideFloor[9].m_TransformFloorData, &trans, &scale);
	m_RightSecondSideFloor[9].m_currFloorNum = 9;

	for (int i = 0; i < 10; i++)
		m_RightSecondSideFloor[i].m_TransformFloorCurr = m_RightSecondSideFloor[i].m_TransformFloorData;

	/*******************************************************************************/
	//RIGHT 3rd SIDE FLOORS
	//Out of Screen Floor
	AEMtx33Scale(&scale, 8000.f, 1262.f);
	AEMtx33Trans(&trans, 48000, -2829); //DONT NEED TO CARE
	AEMtx33Concat(&m_RightThirdSideFloor[0].m_TransformFloorData, &trans, &scale);
	m_RightThirdSideFloor[0].m_currFloorNum = 0;
	AEMtx33Scale(&scale, 7000.f, 1262.f);
	AEMtx33Trans(&trans, 17250, -2229);
	AEMtx33Concat(&m_RightThirdSideFloor[1].m_TransformFloorData, &trans, &scale);
	m_RightThirdSideFloor[1].m_currFloorNum = 1;
	AEMtx33Scale(&scale, 6000.f, 1262.f);
	AEMtx33Trans(&trans, 13050, -1629);
	AEMtx33Concat(&m_RightThirdSideFloor[2].m_TransformFloorData, &trans, &scale);
	m_RightThirdSideFloor[2].m_currFloorNum = 2;
	//First floor
	AEMtx33Scale(&scale, 2940.f, 616.f);
	AEMtx33Trans(&trans, 6450, -696);
	AEMtx33Concat(&m_RightThirdSideFloor[3].m_TransformFloorData, &trans, &scale);
	m_RightThirdSideFloor[3].m_currFloorNum = 3;
	//Second floor
	AEMtx33Scale(&scale, 1593.0f, 339.f);
	AEMtx33Trans(&trans, 3462, -282);
	AEMtx33Concat(&m_RightThirdSideFloor[4].m_TransformFloorData, &trans, &scale);
	m_RightThirdSideFloor[4].m_currFloorNum = 4;
	//Third floor
	AEMtx33Scale(&scale, 779.0f, 133.f);
	AEMtx33Trans(&trans, 1656, -50);
	AEMtx33Concat(&m_RightThirdSideFloor[5].m_TransformFloorData, &trans, &scale);
	m_RightThirdSideFloor[5].m_currFloorNum = 5;
	//Fourth floor
	AEMtx33Scale(&scale, 381.0f, 47.f);
	AEMtx33Trans(&trans, 815, 39);
	AEMtx33Concat(&m_RightThirdSideFloor[6].m_TransformFloorData, &trans, &scale);
	m_RightThirdSideFloor[6].m_currFloorNum = 6;
	//Fifth floor
	AEMtx33Scale(&scale, 181.0f, 14.f);
	AEMtx33Trans(&trans, 388, 69);
	AEMtx33Concat(&m_RightThirdSideFloor[7].m_TransformFloorData, &trans, &scale);
	m_RightThirdSideFloor[7].m_currFloorNum = 7;
	//Sixth floor
	AEMtx33Scale(&scale, 85.0f, 4.f);
	AEMtx33Trans(&trans, 181, 78);
	AEMtx33Concat(&m_RightThirdSideFloor[8].m_TransformFloorData, &trans, &scale);
	m_RightThirdSideFloor[8].m_currFloorNum = 8;
	//Seventh floor
	AEMtx33Scale(&scale, 33.0f, 1.f);
	AEMtx33Trans(&trans, 71, 80);
	AEMtx33Concat(&m_RightThirdSideFloor[9].m_TransformFloorData, &trans, &scale);
	m_RightThirdSideFloor[9].m_currFloorNum = 9;

	for (int i = 0; i < 10; i++)
		m_RightThirdSideFloor[i].m_TransformFloorCurr = m_RightThirdSideFloor[i].m_TransformFloorData;

	/*******************************************************************************/
//RIGHT 4th SIDE FLOORS
//Out of Screen Floor
	AEMtx33Scale(&scale, 8000.f, 1262.f);
	AEMtx33Trans(&trans, 64000, -2829); //DONT NEED TO CARE
	AEMtx33Concat(&m_RightFourthSideFloor[0].m_TransformFloorData, &trans, &scale);
	m_RightFourthSideFloor[0].m_currFloorNum = 0;
	AEMtx33Scale(&scale, 7000.f, 1262.f);
	AEMtx33Trans(&trans, 23000, -2229);
	AEMtx33Concat(&m_RightFourthSideFloor[1].m_TransformFloorData, &trans, &scale);
	m_RightFourthSideFloor[1].m_currFloorNum = 1;
	AEMtx33Scale(&scale, 6000.f, 1262.f);
	AEMtx33Trans(&trans, 17400, -1629);
	AEMtx33Concat(&m_RightFourthSideFloor[2].m_TransformFloorData, &trans, &scale);
	m_RightFourthSideFloor[2].m_currFloorNum = 2;
	//First floor
	AEMtx33Scale(&scale, 2940.f, 616.f);
	AEMtx33Trans(&trans, 8600, -696);
	AEMtx33Concat(&m_RightFourthSideFloor[3].m_TransformFloorData, &trans, &scale);
	m_RightFourthSideFloor[3].m_currFloorNum = 3;
	//Second floor
	AEMtx33Scale(&scale, 1593.0f, 339.f);
	AEMtx33Trans(&trans, 4610, -282);
	AEMtx33Concat(&m_RightFourthSideFloor[4].m_TransformFloorData, &trans, &scale);
	m_RightFourthSideFloor[4].m_currFloorNum = 4;
	//Third floor
	AEMtx33Scale(&scale, 779.0f, 133.f);
	AEMtx33Trans(&trans, 2204, -50);
	AEMtx33Concat(&m_RightFourthSideFloor[5].m_TransformFloorData, &trans, &scale);
	m_RightFourthSideFloor[5].m_currFloorNum = 5;
	//Fourth floor
	AEMtx33Scale(&scale, 381.0f, 47.f);
	AEMtx33Trans(&trans, 1078, 39);
	AEMtx33Concat(&m_RightFourthSideFloor[6].m_TransformFloorData, &trans, &scale);
	m_RightFourthSideFloor[6].m_currFloorNum = 6;
	//Fifth floor
	AEMtx33Scale(&scale, 181.0f, 14.f);
	AEMtx33Trans(&trans, 514, 69);
	AEMtx33Concat(&m_RightFourthSideFloor[7].m_TransformFloorData, &trans, &scale);
	m_RightFourthSideFloor[7].m_currFloorNum = 7;
	//Sixth floor
	AEMtx33Scale(&scale, 85.0f, 4.f);
	AEMtx33Trans(&trans, 240, 78);
	AEMtx33Concat(&m_RightFourthSideFloor[8].m_TransformFloorData, &trans, &scale);
	m_RightFourthSideFloor[8].m_currFloorNum = 8;
	//Seventh floor
	AEMtx33Scale(&scale, 33.0f, 1.f);
	AEMtx33Trans(&trans, 96, 80);
	AEMtx33Concat(&m_RightFourthSideFloor[9].m_TransformFloorData, &trans, &scale);
	m_RightFourthSideFloor[9].m_currFloorNum = 9;

	for (int i = 0; i < 10; i++)
		m_RightFourthSideFloor[i].m_TransformFloorCurr = m_RightFourthSideFloor[i].m_TransformFloorData;



	/*******************************************************************************/
	//LEFT SIDE FLOORS
	//Out of Screen Floor
	AEMtx33Scale(&scale, 8000.f, 1262.f);
	AEMtx33Trans(&trans, -16000, -2829);
	AEMtx33Concat(&m_LeftSideFloor[0].m_TransformFloorData, &trans, &scale);
	m_LeftSideFloor[0].m_currFloorNum = 0;
	AEMtx33Scale(&scale, 7000.f, 1262.f);
	AEMtx33Trans(&trans, -5750, -2229);
	AEMtx33Concat(&m_LeftSideFloor[1].m_TransformFloorData, &trans, &scale);
	m_LeftSideFloor[1].m_currFloorNum = 1;
	AEMtx33Scale(&scale, 6000.f, 1262.f);
	AEMtx33Trans(&trans, -4350, -1629);
	AEMtx33Concat(&m_LeftSideFloor[2].m_TransformFloorData, &trans, &scale);
	m_LeftSideFloor[2].m_currFloorNum = 2;
	//First floor
	AEMtx33Scale(&scale, 2940.f, 616.f);
	AEMtx33Trans(&trans, -2150, -696);
	AEMtx33Concat(&m_LeftSideFloor[3].m_TransformFloorData, &trans, &scale);
	m_LeftSideFloor[3].m_currFloorNum = 3;
	//Second floor
	AEMtx33Scale(&scale, 1593.0f, 339.f);
	AEMtx33Trans(&trans, -1158, -282);
	AEMtx33Concat(&m_LeftSideFloor[4].m_TransformFloorData, &trans, &scale);
	m_LeftSideFloor[4].m_currFloorNum = 4;
	//Third floor
	AEMtx33Scale(&scale, 779.0f, 133.f);
	AEMtx33Trans(&trans, -556, -50);
	AEMtx33Concat(&m_LeftSideFloor[5].m_TransformFloorData, &trans, &scale);
	m_LeftSideFloor[5].m_currFloorNum = 5;
	//Fourth floor
	AEMtx33Scale(&scale, 381.0f, 47.f);
	AEMtx33Trans(&trans, -276, 39);
	AEMtx33Concat(&m_LeftSideFloor[6].m_TransformFloorData, &trans, &scale);
	m_LeftSideFloor[6].m_currFloorNum = 6;
	//Fifth floor
	AEMtx33Scale(&scale, 181.0f, 14.f);
	AEMtx33Trans(&trans, -131, 69);
	AEMtx33Concat(&m_LeftSideFloor[7].m_TransformFloorData, &trans, &scale);
	m_LeftSideFloor[7].m_currFloorNum = 7;
	//Sixth floor
	AEMtx33Scale(&scale, 85.0f, 4.f);
	AEMtx33Trans(&trans, -62, 78);
	AEMtx33Concat(&m_LeftSideFloor[8].m_TransformFloorData, &trans, &scale);
	m_LeftSideFloor[8].m_currFloorNum = 8;
	//Seventh floor
	AEMtx33Scale(&scale, 33.0f, 1.f);
	AEMtx33Trans(&trans, -25, 80);
	AEMtx33Concat(&m_LeftSideFloor[9].m_TransformFloorData, &trans, &scale);
	m_LeftSideFloor[9].m_currFloorNum = 9;

	for (int i = 0; i < 10; i++)
		m_LeftSideFloor[i].m_TransformFloorCurr = m_LeftSideFloor[i].m_TransformFloorData;

	/*******************************************************************************/
//LEFT 2nd SIDE FLOORS
//Out of Screen Floor
	AEMtx33Scale(&scale, 8000.f, 1262.f);
	AEMtx33Trans(&trans, -32000, -2829); //DONT NEED TO CARE
	AEMtx33Concat(&m_LeftSecondSideFloor[0].m_TransformFloorData, &trans, &scale);
	m_LeftSecondSideFloor[0].m_currFloorNum = 0;
	AEMtx33Scale(&scale, 7000.f, 1262.f);
	AEMtx33Trans(&trans, -11500, -2229);
	AEMtx33Concat(&m_LeftSecondSideFloor[1].m_TransformFloorData, &trans, &scale);
	m_LeftSecondSideFloor[1].m_currFloorNum = 1;
	AEMtx33Scale(&scale, 6000.f, 1262.f);
	AEMtx33Trans(&trans, -8700, -1629);
	AEMtx33Concat(&m_LeftSecondSideFloor[2].m_TransformFloorData, &trans, &scale);
	m_LeftSecondSideFloor[2].m_currFloorNum = 2;
	//First floor
	AEMtx33Scale(&scale, 2940.f, 616.f);
	AEMtx33Trans(&trans, -4300, -696);
	AEMtx33Concat(&m_LeftSecondSideFloor[3].m_TransformFloorData, &trans, &scale);
	m_LeftSecondSideFloor[3].m_currFloorNum = 3;
	//Second floor
	AEMtx33Scale(&scale, 1593.0f, 339.f);
	AEMtx33Trans(&trans, -2308, -282);
	AEMtx33Concat(&m_LeftSecondSideFloor[4].m_TransformFloorData, &trans, &scale);
	m_LeftSecondSideFloor[4].m_currFloorNum = 4;
	//Third floor
	AEMtx33Scale(&scale, 779.0f, 133.f);
	AEMtx33Trans(&trans, -1104, -50);
	AEMtx33Concat(&m_LeftSecondSideFloor[5].m_TransformFloorData, &trans, &scale);
	m_LeftSecondSideFloor[5].m_currFloorNum = 5;
	//Fourth floor
	AEMtx33Scale(&scale, 381.0f, 47.f);
	AEMtx33Trans(&trans, -546, 39);
	AEMtx33Concat(&m_LeftSecondSideFloor[6].m_TransformFloorData, &trans, &scale);
	m_LeftSecondSideFloor[6].m_currFloorNum = 6;
	//Fifth floor
	AEMtx33Scale(&scale, 181.0f, 14.f);
	AEMtx33Trans(&trans, -259, 69);
	AEMtx33Concat(&m_LeftSecondSideFloor[7].m_TransformFloorData, &trans, &scale);
	m_LeftSecondSideFloor[7].m_currFloorNum = 7;
	//Sixth floor
	AEMtx33Scale(&scale, 85.0f, 4.f);
	AEMtx33Trans(&trans, -121, 78);
	AEMtx33Concat(&m_LeftSecondSideFloor[8].m_TransformFloorData, &trans, &scale);
	m_LeftSecondSideFloor[8].m_currFloorNum = 8;
	//Seventh floor
	AEMtx33Scale(&scale, 33.0f, 1.f);
	AEMtx33Trans(&trans, -48, 80);
	AEMtx33Concat(&m_LeftSecondSideFloor[9].m_TransformFloorData, &trans, &scale);
	m_LeftSecondSideFloor[9].m_currFloorNum = 9;

	for (int i = 0; i < 10; i++)
		m_LeftSecondSideFloor[i].m_TransformFloorCurr = m_LeftSecondSideFloor[i].m_TransformFloorData;


	/*******************************************************************************/
//LEFT 3rd SIDE FLOORS
//Out of Screen Floor
	AEMtx33Scale(&scale, 8000.f, 1262.f);
	AEMtx33Trans(&trans, -48000, -2829); //DONT NEED TO CARE
	AEMtx33Concat(&m_LeftThirdSideFloor[0].m_TransformFloorData, &trans, &scale);
	m_LeftThirdSideFloor[0].m_currFloorNum = 0;
	AEMtx33Scale(&scale, 7000.f, 1262.f);
	AEMtx33Trans(&trans, -17250, -2229);
	AEMtx33Concat(&m_LeftThirdSideFloor[1].m_TransformFloorData, &trans, &scale);
	m_LeftThirdSideFloor[1].m_currFloorNum = 1;
	AEMtx33Scale(&scale, 6000.f, 1262.f);
	AEMtx33Trans(&trans, -13050, -1629);
	AEMtx33Concat(&m_LeftThirdSideFloor[2].m_TransformFloorData, &trans, &scale);
	m_LeftThirdSideFloor[2].m_currFloorNum = 2;
	//First floor
	AEMtx33Scale(&scale, 2940.f, 616.f);
	AEMtx33Trans(&trans, -6450, -696);
	AEMtx33Concat(&m_LeftThirdSideFloor[3].m_TransformFloorData, &trans, &scale);
	m_LeftThirdSideFloor[3].m_currFloorNum = 3;
	//Second floor
	AEMtx33Scale(&scale, 1593.0f, 339.f);
	AEMtx33Trans(&trans, -3462, -282);
	AEMtx33Concat(&m_LeftThirdSideFloor[4].m_TransformFloorData, &trans, &scale);
	m_LeftThirdSideFloor[4].m_currFloorNum = 4;
	//Third floor
	AEMtx33Scale(&scale, 779.0f, 133.f);
	AEMtx33Trans(&trans, -1656, -50);
	AEMtx33Concat(&m_LeftThirdSideFloor[5].m_TransformFloorData, &trans, &scale);
	m_LeftThirdSideFloor[5].m_currFloorNum = 5;
	//Fourth floor
	AEMtx33Scale(&scale, 381.0f, 47.f);
	AEMtx33Trans(&trans, -815, 39);
	AEMtx33Concat(&m_LeftThirdSideFloor[6].m_TransformFloorData, &trans, &scale);
	m_LeftThirdSideFloor[6].m_currFloorNum = 6;
	//Fifth floor
	AEMtx33Scale(&scale, 181.0f, 14.f);
	AEMtx33Trans(&trans, -388, 69);
	AEMtx33Concat(&m_LeftThirdSideFloor[7].m_TransformFloorData, &trans, &scale);
	m_LeftThirdSideFloor[7].m_currFloorNum = 7;
	//Sixth floor
	AEMtx33Scale(&scale, 85.0f, 4.f);
	AEMtx33Trans(&trans, -181, 78);
	AEMtx33Concat(&m_LeftThirdSideFloor[8].m_TransformFloorData, &trans, &scale);
	m_LeftThirdSideFloor[8].m_currFloorNum = 8;
	//Seventh floor
	AEMtx33Scale(&scale, 33.0f, 1.f);
	AEMtx33Trans(&trans, -71, 80);
	AEMtx33Concat(&m_LeftThirdSideFloor[9].m_TransformFloorData, &trans, &scale);
	m_LeftThirdSideFloor[9].m_currFloorNum = 9;

	for (int i = 0; i < 10; i++)
		m_LeftThirdSideFloor[i].m_TransformFloorCurr = m_LeftThirdSideFloor[i].m_TransformFloorData;



	/*******************************************************************************/
//LEFT 4th SIDE FLOORS
//Out of Screen Floor
	AEMtx33Scale(&scale, 8000.f, 1262.f);
	AEMtx33Trans(&trans, -64000, -2829); //DONT NEED TO CARE
	AEMtx33Concat(&m_LeftFourthSideFloor[0].m_TransformFloorData, &trans, &scale);
	m_LeftFourthSideFloor[0].m_currFloorNum = 0;
	AEMtx33Scale(&scale, 7000.f, 1262.f);
	AEMtx33Trans(&trans, -23000, -2229);
	AEMtx33Concat(&m_LeftFourthSideFloor[1].m_TransformFloorData, &trans, &scale);
	m_LeftFourthSideFloor[1].m_currFloorNum = 1;
	AEMtx33Scale(&scale, 6000.f, 1262.f);
	AEMtx33Trans(&trans, -17400, -1629);
	AEMtx33Concat(&m_LeftFourthSideFloor[2].m_TransformFloorData, &trans, &scale);
	m_LeftFourthSideFloor[2].m_currFloorNum = 2;
	//First floor
	AEMtx33Scale(&scale, 2940.f, 616.f);
	AEMtx33Trans(&trans, -8600, -696);
	AEMtx33Concat(&m_LeftFourthSideFloor[3].m_TransformFloorData, &trans, &scale);
	m_LeftFourthSideFloor[3].m_currFloorNum = 3;
	//Second floor
	AEMtx33Scale(&scale, 1593.0f, 339.f);
	AEMtx33Trans(&trans, -4610, -282);
	AEMtx33Concat(&m_LeftFourthSideFloor[4].m_TransformFloorData, &trans, &scale);
	m_LeftFourthSideFloor[4].m_currFloorNum = 4;
	//Third floor
	AEMtx33Scale(&scale, 779.0f, 133.f);
	AEMtx33Trans(&trans, -2204, -50);
	AEMtx33Concat(&m_LeftFourthSideFloor[5].m_TransformFloorData, &trans, &scale);
	m_LeftFourthSideFloor[5].m_currFloorNum = 5;
	//Fourth floor
	AEMtx33Scale(&scale, 381.0f, 47.f);
	AEMtx33Trans(&trans, -1078, 39);
	AEMtx33Concat(&m_LeftFourthSideFloor[6].m_TransformFloorData, &trans, &scale);
	m_LeftFourthSideFloor[6].m_currFloorNum = 6;
	//Fifth floor
	AEMtx33Scale(&scale, 181.0f, 14.f);
	AEMtx33Trans(&trans, -514, 69);
	AEMtx33Concat(&m_LeftFourthSideFloor[7].m_TransformFloorData, &trans, &scale);
	m_LeftFourthSideFloor[7].m_currFloorNum = 7;
	//Sixth floor
	AEMtx33Scale(&scale, 85.0f, 4.f);
	AEMtx33Trans(&trans, -240, 78);
	AEMtx33Concat(&m_LeftFourthSideFloor[8].m_TransformFloorData, &trans, &scale);
	m_LeftFourthSideFloor[8].m_currFloorNum = 8;
	//Seventh floor
	AEMtx33Scale(&scale, 33.0f, 1.f);
	AEMtx33Trans(&trans, -96, 80);
	AEMtx33Concat(&m_LeftFourthSideFloor[9].m_TransformFloorData, &trans, &scale);
	m_LeftFourthSideFloor[9].m_currFloorNum = 9;

	for (int i = 0; i < 10; i++)
		m_LeftFourthSideFloor[i].m_TransformFloorCurr = m_LeftFourthSideFloor[i].m_TransformFloorData;

	//DO SKY DATA
	AEMtx33Scale(&scale, 2000.0f, 400.f);
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


	AEMtx33Scale(&scale, 1000.0f, 300.0f);
	AEMtx33Trans(&trans, 0, 30);
	AEMtx33Concat(&m_TransformDigipenLogoData, &trans, &scale);
}

void SceneSplashScreen::Update(double dt)
{
	std::cout << "Updating Scene SplashScreen" << std::endl;
	if (AEInputCheckTriggered(AEVK_3)) {
		SceneManager::GetInstance()->SetActiveScene("SceneBase");
	}
	if (AEInputCheckTriggered(AEVK_4)) {
		SceneManager::GetInstance()->SetActiveScene("TestScene");
	}
	if (AEInputCheckTriggered(AEVK_5)) {
		SceneManager::GetInstance()->SetActiveScene("CombatScene");
	}


	LogoTimer -= LogoTimer > 0.0f ? dt : 0;
	if (LogoTimer <= 0.0f)
		ShowGame = true;
	//	std::cout << "Updating Scene SplashScreen" << std::endl;

	//	static int x = 2000.f, y = 400.f;
	//if (AEInputCheckCurr(AEVK_W))
	//{
	//	y++;
	//}
	//if (AEInputCheckCurr(AEVK_S))
	//{
	//	y--;
	//}
	//if (AEInputCheckCurr(AEVK_A))
	//{
	//	x--;
	//}
	//if (AEInputCheckCurr(AEVK_D))
	//{
	//	x++;
	//}
	//static int mx = 0, my = 200;
	//if (AEInputCheckCurr(AEVK_UP))
	//{
	//	my++;
	//}
	//if (AEInputCheckCurr(AEVK_DOWN))
	//{
	//	my--;
	//}
	//AEMtx33 scale = { 0 }, trans = { 0 };
	//AEMtx33Scale(&scale, x, y);
	//AEMtx33Trans(&trans, mx, my);
	//AEMtx33Concat(&m_TransformFogData, &trans, &scale);
	//
	//cout << mx << endl;
		///////////////////////////////////////////////////////////////////////////
		//UPDATE FLOOR MOVEMENT
		//////////////////////////////////////////////////////////////////////////
	static bool start = false;
	start = AEInputCheckReleased(AEVK_SPACE) ? !start : start;

	if (LogoTimer > 0.0f || start)
	{
		AEMtx33 m_LastFloorData = m_Floor[8].m_TransformFloorData;
		for (int i = 9; i > -1; i--)
		{
			AEMtx33 m_NextFloorData = m_Floor[i].m_currFloorNum != 0 ? m_Floor[m_Floor[i].m_currFloorNum - 1].m_TransformFloorData : m_Floor[i].m_TransformFloorCurr = m_LastFloorData;
			AEMtx33 m_CurrFloorData = m_Floor[m_Floor[i].m_currFloorNum].m_TransformFloorData;

			//Minimum Speed of next floor
			AEMtx33 m_MinimumNextFloorSpeed = {
			(m_NextFloorData.m[0][0] - m_CurrFloorData.m[0][0]) / 80,//CHANGE THIS FOR PANNING CLOSER TO GROUND
			(m_NextFloorData.m[0][1] - m_CurrFloorData.m[0][1]) / 80,
			(m_NextFloorData.m[0][2] - m_CurrFloorData.m[0][2]) / 80,
			(m_NextFloorData.m[1][0] - m_CurrFloorData.m[1][0]) / 80,
			(m_NextFloorData.m[1][1] - m_CurrFloorData.m[1][1]) / 80,
			(m_NextFloorData.m[1][2] - m_CurrFloorData.m[1][2]) / 80,
			(m_NextFloorData.m[2][0] - m_CurrFloorData.m[2][0]) / 80,
			(m_NextFloorData.m[2][1] - m_CurrFloorData.m[2][1]) / 80,
			(m_NextFloorData.m[2][2] - m_CurrFloorData.m[2][2]) / 80
			};

			//Incrementing speed
			m_Floor[i].m_currFloorSpeed.m[0][0] += m_Floor[i].m_currFloorSpeed.m[0][0] < m_MinimumNextFloorSpeed.m[0][0] ? dt * m_MinimumNextFloorSpeed.m[0][0] : m_Floor[i].m_currFloorSpeed.m[0][0] > m_MinimumNextFloorSpeed.m[0][0] ? dt * m_MinimumNextFloorSpeed.m[0][0] : 0;
			m_Floor[i].m_currFloorSpeed.m[0][1] += m_Floor[i].m_currFloorSpeed.m[0][1] < m_MinimumNextFloorSpeed.m[0][1] ? dt * m_MinimumNextFloorSpeed.m[0][1] : m_Floor[i].m_currFloorSpeed.m[0][1] > m_MinimumNextFloorSpeed.m[0][1] ? dt * m_MinimumNextFloorSpeed.m[0][1] : 0;
			m_Floor[i].m_currFloorSpeed.m[0][2] += m_Floor[i].m_currFloorSpeed.m[0][2] < m_MinimumNextFloorSpeed.m[0][2] ? dt * m_MinimumNextFloorSpeed.m[0][2] : m_Floor[i].m_currFloorSpeed.m[0][2] > m_MinimumNextFloorSpeed.m[0][2] ? dt * m_MinimumNextFloorSpeed.m[0][2] : 0;
			m_Floor[i].m_currFloorSpeed.m[1][0] += m_Floor[i].m_currFloorSpeed.m[1][0] < m_MinimumNextFloorSpeed.m[1][0] ? dt * m_MinimumNextFloorSpeed.m[1][0] : m_Floor[i].m_currFloorSpeed.m[1][0] > m_MinimumNextFloorSpeed.m[1][0] ? dt * m_MinimumNextFloorSpeed.m[1][0] : 0;
			m_Floor[i].m_currFloorSpeed.m[1][1] += m_Floor[i].m_currFloorSpeed.m[1][1] < m_MinimumNextFloorSpeed.m[1][1] ? dt * m_MinimumNextFloorSpeed.m[1][1] : m_Floor[i].m_currFloorSpeed.m[1][1] > m_MinimumNextFloorSpeed.m[1][1] ? dt * m_MinimumNextFloorSpeed.m[1][1] : 0;
			m_Floor[i].m_currFloorSpeed.m[1][2] += m_Floor[i].m_currFloorSpeed.m[1][2] < m_MinimumNextFloorSpeed.m[1][2] ? dt * m_MinimumNextFloorSpeed.m[1][2] : m_Floor[i].m_currFloorSpeed.m[1][2] > m_MinimumNextFloorSpeed.m[1][2] ? dt * m_MinimumNextFloorSpeed.m[1][2] : 0;
			m_Floor[i].m_currFloorSpeed.m[2][0] += m_Floor[i].m_currFloorSpeed.m[2][0] < m_MinimumNextFloorSpeed.m[2][0] ? dt * m_MinimumNextFloorSpeed.m[2][0] : m_Floor[i].m_currFloorSpeed.m[2][0] > m_MinimumNextFloorSpeed.m[2][0] ? dt * m_MinimumNextFloorSpeed.m[2][0] : 0;
			m_Floor[i].m_currFloorSpeed.m[2][1] += m_Floor[i].m_currFloorSpeed.m[2][1] < m_MinimumNextFloorSpeed.m[2][1] ? dt * m_MinimumNextFloorSpeed.m[2][1] : m_Floor[i].m_currFloorSpeed.m[2][1] > m_MinimumNextFloorSpeed.m[2][1] ? dt * m_MinimumNextFloorSpeed.m[2][1] : 0;
			m_Floor[i].m_currFloorSpeed.m[2][2] += m_Floor[i].m_currFloorSpeed.m[2][2] < m_MinimumNextFloorSpeed.m[2][2] ? dt * m_MinimumNextFloorSpeed.m[2][2] : m_Floor[i].m_currFloorSpeed.m[2][2] > m_MinimumNextFloorSpeed.m[2][2] ? dt * m_MinimumNextFloorSpeed.m[2][2] : 0;
			//Adding to floor
			m_Floor[i].m_TransformFloorCurr.m[0][0] += m_Floor[i].m_currFloorSpeed.m[0][0];
			m_Floor[i].m_TransformFloorCurr.m[0][1] += m_Floor[i].m_currFloorSpeed.m[0][1];
			m_Floor[i].m_TransformFloorCurr.m[0][2] += m_Floor[i].m_currFloorSpeed.m[0][2];
			m_Floor[i].m_TransformFloorCurr.m[1][0] += m_Floor[i].m_currFloorSpeed.m[1][0];
			m_Floor[i].m_TransformFloorCurr.m[1][1] += m_Floor[i].m_currFloorSpeed.m[1][1];
			m_Floor[i].m_TransformFloorCurr.m[1][2] += m_Floor[i].m_currFloorSpeed.m[1][2];
			m_Floor[i].m_TransformFloorCurr.m[2][0] += m_Floor[i].m_currFloorSpeed.m[2][0];
			m_Floor[i].m_TransformFloorCurr.m[2][1] += m_Floor[i].m_currFloorSpeed.m[2][1];
			m_Floor[i].m_TransformFloorCurr.m[2][2] += m_Floor[i].m_currFloorSpeed.m[2][2];

			if (m_Floor[i].m_currFloorTimer > m_Floor[i].m_FloorSpeedTimer)
			{
				m_Floor[i].m_currFloorTimer = 0.0;
				if (m_Floor[i].m_currFloorNum > 1)
				{
					m_Floor[i].m_currFloorNum--;
					m_Floor[i].m_IsRender = true;
				}
				else
				{
					//Loop to the top
					m_Floor[i].m_currFloorNum = 8;
					m_Floor[i].m_currFloorSpeed = { 0 };
					m_Floor[i].m_TransformFloorCurr = m_LastFloorData;
					m_Floor[i].m_IsRender = false;
				}
			}
			else
				m_Floor[i].m_currFloorTimer += dt;
		}




		m_LastFloorData = m_RightSideFloor[8].m_TransformFloorData;
		for (int i = 9; i > -1; i--)
		{
			AEMtx33 m_NextFloorData = m_RightSideFloor[i].m_currFloorNum != 0 ? m_RightSideFloor[m_RightSideFloor[i].m_currFloorNum - 1].m_TransformFloorData : m_RightSideFloor[i].m_TransformFloorCurr = m_LastFloorData;
			AEMtx33 m_CurrFloorData = m_RightSideFloor[m_RightSideFloor[i].m_currFloorNum].m_TransformFloorData;

			//Minimum Speed of next floor
			AEMtx33 m_MinimumNextFloorSpeed = {
			(m_NextFloorData.m[0][0] - m_CurrFloorData.m[0][0]) / 80,
			(m_NextFloorData.m[0][1] - m_CurrFloorData.m[0][1]) / 80,
			(m_NextFloorData.m[0][2] - m_CurrFloorData.m[0][2]) / 80,
			(m_NextFloorData.m[1][0] - m_CurrFloorData.m[1][0]) / 80,
			(m_NextFloorData.m[1][1] - m_CurrFloorData.m[1][1]) / 80,
			(m_NextFloorData.m[1][2] - m_CurrFloorData.m[1][2]) / 80,
			(m_NextFloorData.m[2][0] - m_CurrFloorData.m[2][0]) / 80,
			(m_NextFloorData.m[2][1] - m_CurrFloorData.m[2][1]) / 80,
			(m_NextFloorData.m[2][2] - m_CurrFloorData.m[2][2]) / 80
			};

			//Incrementing speed
			m_RightSideFloor[i].m_currFloorSpeed.m[0][0] += m_RightSideFloor[i].m_currFloorSpeed.m[0][0] < m_MinimumNextFloorSpeed.m[0][0] ? dt * m_MinimumNextFloorSpeed.m[0][0] : m_RightSideFloor[i].m_currFloorSpeed.m[0][0] > m_MinimumNextFloorSpeed.m[0][0] ? dt * m_MinimumNextFloorSpeed.m[0][0] : 0;
			m_RightSideFloor[i].m_currFloorSpeed.m[0][1] += m_RightSideFloor[i].m_currFloorSpeed.m[0][1] < m_MinimumNextFloorSpeed.m[0][1] ? dt * m_MinimumNextFloorSpeed.m[0][1] : m_RightSideFloor[i].m_currFloorSpeed.m[0][1] > m_MinimumNextFloorSpeed.m[0][1] ? dt * m_MinimumNextFloorSpeed.m[0][1] : 0;
			m_RightSideFloor[i].m_currFloorSpeed.m[0][2] += m_RightSideFloor[i].m_currFloorSpeed.m[0][2] < m_MinimumNextFloorSpeed.m[0][2] ? dt * m_MinimumNextFloorSpeed.m[0][2] : m_RightSideFloor[i].m_currFloorSpeed.m[0][2] > m_MinimumNextFloorSpeed.m[0][2] ? dt * m_MinimumNextFloorSpeed.m[0][2] : 0;
			m_RightSideFloor[i].m_currFloorSpeed.m[1][0] += m_RightSideFloor[i].m_currFloorSpeed.m[1][0] < m_MinimumNextFloorSpeed.m[1][0] ? dt * m_MinimumNextFloorSpeed.m[1][0] : m_RightSideFloor[i].m_currFloorSpeed.m[1][0] > m_MinimumNextFloorSpeed.m[1][0] ? dt * m_MinimumNextFloorSpeed.m[1][0] : 0;
			m_RightSideFloor[i].m_currFloorSpeed.m[1][1] += m_RightSideFloor[i].m_currFloorSpeed.m[1][1] < m_MinimumNextFloorSpeed.m[1][1] ? dt * m_MinimumNextFloorSpeed.m[1][1] : m_RightSideFloor[i].m_currFloorSpeed.m[1][1] > m_MinimumNextFloorSpeed.m[1][1] ? dt * m_MinimumNextFloorSpeed.m[1][1] : 0;
			m_RightSideFloor[i].m_currFloorSpeed.m[1][2] += m_RightSideFloor[i].m_currFloorSpeed.m[1][2] < m_MinimumNextFloorSpeed.m[1][2] ? dt * m_MinimumNextFloorSpeed.m[1][2] : m_RightSideFloor[i].m_currFloorSpeed.m[1][2] > m_MinimumNextFloorSpeed.m[1][2] ? dt * m_MinimumNextFloorSpeed.m[1][2] : 0;
			m_RightSideFloor[i].m_currFloorSpeed.m[2][0] += m_RightSideFloor[i].m_currFloorSpeed.m[2][0] < m_MinimumNextFloorSpeed.m[2][0] ? dt * m_MinimumNextFloorSpeed.m[2][0] : m_RightSideFloor[i].m_currFloorSpeed.m[2][0] > m_MinimumNextFloorSpeed.m[2][0] ? dt * m_MinimumNextFloorSpeed.m[2][0] : 0;
			m_RightSideFloor[i].m_currFloorSpeed.m[2][1] += m_RightSideFloor[i].m_currFloorSpeed.m[2][1] < m_MinimumNextFloorSpeed.m[2][1] ? dt * m_MinimumNextFloorSpeed.m[2][1] : m_RightSideFloor[i].m_currFloorSpeed.m[2][1] > m_MinimumNextFloorSpeed.m[2][1] ? dt * m_MinimumNextFloorSpeed.m[2][1] : 0;
			m_RightSideFloor[i].m_currFloorSpeed.m[2][2] += m_RightSideFloor[i].m_currFloorSpeed.m[2][2] < m_MinimumNextFloorSpeed.m[2][2] ? dt * m_MinimumNextFloorSpeed.m[2][2] : m_RightSideFloor[i].m_currFloorSpeed.m[2][2] > m_MinimumNextFloorSpeed.m[2][2] ? dt * m_MinimumNextFloorSpeed.m[2][2] : 0;
			//Adding to floor
			m_RightSideFloor[i].m_TransformFloorCurr.m[0][0] += m_RightSideFloor[i].m_currFloorSpeed.m[0][0];
			m_RightSideFloor[i].m_TransformFloorCurr.m[0][1] += m_RightSideFloor[i].m_currFloorSpeed.m[0][1];
			m_RightSideFloor[i].m_TransformFloorCurr.m[0][2] += m_RightSideFloor[i].m_currFloorSpeed.m[0][2];
			m_RightSideFloor[i].m_TransformFloorCurr.m[1][0] += m_RightSideFloor[i].m_currFloorSpeed.m[1][0];
			m_RightSideFloor[i].m_TransformFloorCurr.m[1][1] += m_RightSideFloor[i].m_currFloorSpeed.m[1][1];
			m_RightSideFloor[i].m_TransformFloorCurr.m[1][2] += m_RightSideFloor[i].m_currFloorSpeed.m[1][2];
			m_RightSideFloor[i].m_TransformFloorCurr.m[2][0] += m_RightSideFloor[i].m_currFloorSpeed.m[2][0];
			m_RightSideFloor[i].m_TransformFloorCurr.m[2][1] += m_RightSideFloor[i].m_currFloorSpeed.m[2][1];
			m_RightSideFloor[i].m_TransformFloorCurr.m[2][2] += m_RightSideFloor[i].m_currFloorSpeed.m[2][2];

			if (m_RightSideFloor[i].m_currFloorTimer > m_RightSideFloor[i].m_FloorSpeedTimer)
			{
				m_RightSideFloor[i].m_currFloorTimer = 0.0;
				if (m_RightSideFloor[i].m_currFloorNum > 1)
				{
					m_RightSideFloor[i].m_currFloorNum--;
					m_RightSideFloor[i].m_IsRender = true;
				}
				else
				{
					//Loop to the top
					m_RightSideFloor[i].m_currFloorNum = 8;
					m_RightSideFloor[i].m_currFloorSpeed = { 0 };
					m_RightSideFloor[i].m_TransformFloorCurr = m_LastFloorData;
					m_RightSideFloor[i].m_IsRender = false;
				}
			}
			else
				m_RightSideFloor[i].m_currFloorTimer += dt;
		}

		m_LastFloorData = m_RightSecondSideFloor[8].m_TransformFloorData;
		for (int i = 9; i > -1; i--)
		{
			AEMtx33 m_NextFloorData = m_RightSecondSideFloor[i].m_currFloorNum != 0 ? m_RightSecondSideFloor[m_RightSecondSideFloor[i].m_currFloorNum - 1].m_TransformFloorData : m_RightSecondSideFloor[i].m_TransformFloorCurr = m_LastFloorData;
			AEMtx33 m_CurrFloorData = m_RightSecondSideFloor[m_RightSecondSideFloor[i].m_currFloorNum].m_TransformFloorData;

			//Minimum Speed of next floor
			AEMtx33 m_MinimumNextFloorSpeed = {
			(m_NextFloorData.m[0][0] - m_CurrFloorData.m[0][0]) / 80,
			(m_NextFloorData.m[0][1] - m_CurrFloorData.m[0][1]) / 80,
			(m_NextFloorData.m[0][2] - m_CurrFloorData.m[0][2]) / 80,
			(m_NextFloorData.m[1][0] - m_CurrFloorData.m[1][0]) / 80,
			(m_NextFloorData.m[1][1] - m_CurrFloorData.m[1][1]) / 80,
			(m_NextFloorData.m[1][2] - m_CurrFloorData.m[1][2]) / 80,
			(m_NextFloorData.m[2][0] - m_CurrFloorData.m[2][0]) / 80,
			(m_NextFloorData.m[2][1] - m_CurrFloorData.m[2][1]) / 80,
			(m_NextFloorData.m[2][2] - m_CurrFloorData.m[2][2]) / 80
			};

			//Incrementing speed
			m_RightSecondSideFloor[i].m_currFloorSpeed.m[0][0] += m_RightSecondSideFloor[i].m_currFloorSpeed.m[0][0] < m_MinimumNextFloorSpeed.m[0][0] ? dt * m_MinimumNextFloorSpeed.m[0][0] : m_RightSecondSideFloor[i].m_currFloorSpeed.m[0][0] > m_MinimumNextFloorSpeed.m[0][0] ? dt * m_MinimumNextFloorSpeed.m[0][0] : 0;
			m_RightSecondSideFloor[i].m_currFloorSpeed.m[0][1] += m_RightSecondSideFloor[i].m_currFloorSpeed.m[0][1] < m_MinimumNextFloorSpeed.m[0][1] ? dt * m_MinimumNextFloorSpeed.m[0][1] : m_RightSecondSideFloor[i].m_currFloorSpeed.m[0][1] > m_MinimumNextFloorSpeed.m[0][1] ? dt * m_MinimumNextFloorSpeed.m[0][1] : 0;
			m_RightSecondSideFloor[i].m_currFloorSpeed.m[0][2] += m_RightSecondSideFloor[i].m_currFloorSpeed.m[0][2] < m_MinimumNextFloorSpeed.m[0][2] ? dt * m_MinimumNextFloorSpeed.m[0][2] : m_RightSecondSideFloor[i].m_currFloorSpeed.m[0][2] > m_MinimumNextFloorSpeed.m[0][2] ? dt * m_MinimumNextFloorSpeed.m[0][2] : 0;
			m_RightSecondSideFloor[i].m_currFloorSpeed.m[1][0] += m_RightSecondSideFloor[i].m_currFloorSpeed.m[1][0] < m_MinimumNextFloorSpeed.m[1][0] ? dt * m_MinimumNextFloorSpeed.m[1][0] : m_RightSecondSideFloor[i].m_currFloorSpeed.m[1][0] > m_MinimumNextFloorSpeed.m[1][0] ? dt * m_MinimumNextFloorSpeed.m[1][0] : 0;
			m_RightSecondSideFloor[i].m_currFloorSpeed.m[1][1] += m_RightSecondSideFloor[i].m_currFloorSpeed.m[1][1] < m_MinimumNextFloorSpeed.m[1][1] ? dt * m_MinimumNextFloorSpeed.m[1][1] : m_RightSecondSideFloor[i].m_currFloorSpeed.m[1][1] > m_MinimumNextFloorSpeed.m[1][1] ? dt * m_MinimumNextFloorSpeed.m[1][1] : 0;
			m_RightSecondSideFloor[i].m_currFloorSpeed.m[1][2] += m_RightSecondSideFloor[i].m_currFloorSpeed.m[1][2] < m_MinimumNextFloorSpeed.m[1][2] ? dt * m_MinimumNextFloorSpeed.m[1][2] : m_RightSecondSideFloor[i].m_currFloorSpeed.m[1][2] > m_MinimumNextFloorSpeed.m[1][2] ? dt * m_MinimumNextFloorSpeed.m[1][2] : 0;
			m_RightSecondSideFloor[i].m_currFloorSpeed.m[2][0] += m_RightSecondSideFloor[i].m_currFloorSpeed.m[2][0] < m_MinimumNextFloorSpeed.m[2][0] ? dt * m_MinimumNextFloorSpeed.m[2][0] : m_RightSecondSideFloor[i].m_currFloorSpeed.m[2][0] > m_MinimumNextFloorSpeed.m[2][0] ? dt * m_MinimumNextFloorSpeed.m[2][0] : 0;
			m_RightSecondSideFloor[i].m_currFloorSpeed.m[2][1] += m_RightSecondSideFloor[i].m_currFloorSpeed.m[2][1] < m_MinimumNextFloorSpeed.m[2][1] ? dt * m_MinimumNextFloorSpeed.m[2][1] : m_RightSecondSideFloor[i].m_currFloorSpeed.m[2][1] > m_MinimumNextFloorSpeed.m[2][1] ? dt * m_MinimumNextFloorSpeed.m[2][1] : 0;
			m_RightSecondSideFloor[i].m_currFloorSpeed.m[2][2] += m_RightSecondSideFloor[i].m_currFloorSpeed.m[2][2] < m_MinimumNextFloorSpeed.m[2][2] ? dt * m_MinimumNextFloorSpeed.m[2][2] : m_RightSecondSideFloor[i].m_currFloorSpeed.m[2][2] > m_MinimumNextFloorSpeed.m[2][2] ? dt * m_MinimumNextFloorSpeed.m[2][2] : 0;
			//Adding to floor
			m_RightSecondSideFloor[i].m_TransformFloorCurr.m[0][0] += m_RightSecondSideFloor[i].m_currFloorSpeed.m[0][0];
			m_RightSecondSideFloor[i].m_TransformFloorCurr.m[0][1] += m_RightSecondSideFloor[i].m_currFloorSpeed.m[0][1];
			m_RightSecondSideFloor[i].m_TransformFloorCurr.m[0][2] += m_RightSecondSideFloor[i].m_currFloorSpeed.m[0][2];
			m_RightSecondSideFloor[i].m_TransformFloorCurr.m[1][0] += m_RightSecondSideFloor[i].m_currFloorSpeed.m[1][0];
			m_RightSecondSideFloor[i].m_TransformFloorCurr.m[1][1] += m_RightSecondSideFloor[i].m_currFloorSpeed.m[1][1];
			m_RightSecondSideFloor[i].m_TransformFloorCurr.m[1][2] += m_RightSecondSideFloor[i].m_currFloorSpeed.m[1][2];
			m_RightSecondSideFloor[i].m_TransformFloorCurr.m[2][0] += m_RightSecondSideFloor[i].m_currFloorSpeed.m[2][0];
			m_RightSecondSideFloor[i].m_TransformFloorCurr.m[2][1] += m_RightSecondSideFloor[i].m_currFloorSpeed.m[2][1];
			m_RightSecondSideFloor[i].m_TransformFloorCurr.m[2][2] += m_RightSecondSideFloor[i].m_currFloorSpeed.m[2][2];

			if (m_RightSecondSideFloor[i].m_currFloorTimer > m_RightSecondSideFloor[i].m_FloorSpeedTimer)
			{
				m_RightSecondSideFloor[i].m_currFloorTimer = 0.0;
				if (m_RightSecondSideFloor[i].m_currFloorNum > 1)
				{
					m_RightSecondSideFloor[i].m_currFloorNum--;
					m_RightSecondSideFloor[i].m_IsRender = true;
				}
				else
				{
					//Loop to the top
					m_RightSecondSideFloor[i].m_currFloorNum = 8;
					m_RightSecondSideFloor[i].m_currFloorSpeed = { 0 };
					m_RightSecondSideFloor[i].m_TransformFloorCurr = m_LastFloorData;
					m_RightSecondSideFloor[i].m_IsRender = false;
				}
			}
			else
				m_RightSecondSideFloor[i].m_currFloorTimer += dt;
		}

		m_LastFloorData = m_RightThirdSideFloor[8].m_TransformFloorData;
		for (int i = 9; i > -1; i--)
		{
			AEMtx33 m_NextFloorData = m_RightThirdSideFloor[i].m_currFloorNum != 0 ? m_RightThirdSideFloor[m_RightThirdSideFloor[i].m_currFloorNum - 1].m_TransformFloorData : m_RightThirdSideFloor[i].m_TransformFloorCurr = m_LastFloorData;
			AEMtx33 m_CurrFloorData = m_RightThirdSideFloor[m_RightThirdSideFloor[i].m_currFloorNum].m_TransformFloorData;

			//Minimum Speed of next floor
			AEMtx33 m_MinimumNextFloorSpeed = {
			(m_NextFloorData.m[0][0] - m_CurrFloorData.m[0][0]) / 80,
			(m_NextFloorData.m[0][1] - m_CurrFloorData.m[0][1]) / 80,
			(m_NextFloorData.m[0][2] - m_CurrFloorData.m[0][2]) / 80,
			(m_NextFloorData.m[1][0] - m_CurrFloorData.m[1][0]) / 80,
			(m_NextFloorData.m[1][1] - m_CurrFloorData.m[1][1]) / 80,
			(m_NextFloorData.m[1][2] - m_CurrFloorData.m[1][2]) / 80,
			(m_NextFloorData.m[2][0] - m_CurrFloorData.m[2][0]) / 80,
			(m_NextFloorData.m[2][1] - m_CurrFloorData.m[2][1]) / 80,
			(m_NextFloorData.m[2][2] - m_CurrFloorData.m[2][2]) / 80
			};

			//Incrementing speed
			m_RightThirdSideFloor[i].m_currFloorSpeed.m[0][0] += m_RightThirdSideFloor[i].m_currFloorSpeed.m[0][0] < m_MinimumNextFloorSpeed.m[0][0] ? dt * m_MinimumNextFloorSpeed.m[0][0] : m_RightThirdSideFloor[i].m_currFloorSpeed.m[0][0] > m_MinimumNextFloorSpeed.m[0][0] ? dt * m_MinimumNextFloorSpeed.m[0][0] : 0;
			m_RightThirdSideFloor[i].m_currFloorSpeed.m[0][1] += m_RightThirdSideFloor[i].m_currFloorSpeed.m[0][1] < m_MinimumNextFloorSpeed.m[0][1] ? dt * m_MinimumNextFloorSpeed.m[0][1] : m_RightThirdSideFloor[i].m_currFloorSpeed.m[0][1] > m_MinimumNextFloorSpeed.m[0][1] ? dt * m_MinimumNextFloorSpeed.m[0][1] : 0;
			m_RightThirdSideFloor[i].m_currFloorSpeed.m[0][2] += m_RightThirdSideFloor[i].m_currFloorSpeed.m[0][2] < m_MinimumNextFloorSpeed.m[0][2] ? dt * m_MinimumNextFloorSpeed.m[0][2] : m_RightThirdSideFloor[i].m_currFloorSpeed.m[0][2] > m_MinimumNextFloorSpeed.m[0][2] ? dt * m_MinimumNextFloorSpeed.m[0][2] : 0;
			m_RightThirdSideFloor[i].m_currFloorSpeed.m[1][0] += m_RightThirdSideFloor[i].m_currFloorSpeed.m[1][0] < m_MinimumNextFloorSpeed.m[1][0] ? dt * m_MinimumNextFloorSpeed.m[1][0] : m_RightThirdSideFloor[i].m_currFloorSpeed.m[1][0] > m_MinimumNextFloorSpeed.m[1][0] ? dt * m_MinimumNextFloorSpeed.m[1][0] : 0;
			m_RightThirdSideFloor[i].m_currFloorSpeed.m[1][1] += m_RightThirdSideFloor[i].m_currFloorSpeed.m[1][1] < m_MinimumNextFloorSpeed.m[1][1] ? dt * m_MinimumNextFloorSpeed.m[1][1] : m_RightThirdSideFloor[i].m_currFloorSpeed.m[1][1] > m_MinimumNextFloorSpeed.m[1][1] ? dt * m_MinimumNextFloorSpeed.m[1][1] : 0;
			m_RightThirdSideFloor[i].m_currFloorSpeed.m[1][2] += m_RightThirdSideFloor[i].m_currFloorSpeed.m[1][2] < m_MinimumNextFloorSpeed.m[1][2] ? dt * m_MinimumNextFloorSpeed.m[1][2] : m_RightThirdSideFloor[i].m_currFloorSpeed.m[1][2] > m_MinimumNextFloorSpeed.m[1][2] ? dt * m_MinimumNextFloorSpeed.m[1][2] : 0;
			m_RightThirdSideFloor[i].m_currFloorSpeed.m[2][0] += m_RightThirdSideFloor[i].m_currFloorSpeed.m[2][0] < m_MinimumNextFloorSpeed.m[2][0] ? dt * m_MinimumNextFloorSpeed.m[2][0] : m_RightThirdSideFloor[i].m_currFloorSpeed.m[2][0] > m_MinimumNextFloorSpeed.m[2][0] ? dt * m_MinimumNextFloorSpeed.m[2][0] : 0;
			m_RightThirdSideFloor[i].m_currFloorSpeed.m[2][1] += m_RightThirdSideFloor[i].m_currFloorSpeed.m[2][1] < m_MinimumNextFloorSpeed.m[2][1] ? dt * m_MinimumNextFloorSpeed.m[2][1] : m_RightThirdSideFloor[i].m_currFloorSpeed.m[2][1] > m_MinimumNextFloorSpeed.m[2][1] ? dt * m_MinimumNextFloorSpeed.m[2][1] : 0;
			m_RightThirdSideFloor[i].m_currFloorSpeed.m[2][2] += m_RightThirdSideFloor[i].m_currFloorSpeed.m[2][2] < m_MinimumNextFloorSpeed.m[2][2] ? dt * m_MinimumNextFloorSpeed.m[2][2] : m_RightThirdSideFloor[i].m_currFloorSpeed.m[2][2] > m_MinimumNextFloorSpeed.m[2][2] ? dt * m_MinimumNextFloorSpeed.m[2][2] : 0;
			//Adding to floor
			m_RightThirdSideFloor[i].m_TransformFloorCurr.m[0][0] += m_RightThirdSideFloor[i].m_currFloorSpeed.m[0][0];
			m_RightThirdSideFloor[i].m_TransformFloorCurr.m[0][1] += m_RightThirdSideFloor[i].m_currFloorSpeed.m[0][1];
			m_RightThirdSideFloor[i].m_TransformFloorCurr.m[0][2] += m_RightThirdSideFloor[i].m_currFloorSpeed.m[0][2];
			m_RightThirdSideFloor[i].m_TransformFloorCurr.m[1][0] += m_RightThirdSideFloor[i].m_currFloorSpeed.m[1][0];
			m_RightThirdSideFloor[i].m_TransformFloorCurr.m[1][1] += m_RightThirdSideFloor[i].m_currFloorSpeed.m[1][1];
			m_RightThirdSideFloor[i].m_TransformFloorCurr.m[1][2] += m_RightThirdSideFloor[i].m_currFloorSpeed.m[1][2];
			m_RightThirdSideFloor[i].m_TransformFloorCurr.m[2][0] += m_RightThirdSideFloor[i].m_currFloorSpeed.m[2][0];
			m_RightThirdSideFloor[i].m_TransformFloorCurr.m[2][1] += m_RightThirdSideFloor[i].m_currFloorSpeed.m[2][1];
			m_RightThirdSideFloor[i].m_TransformFloorCurr.m[2][2] += m_RightThirdSideFloor[i].m_currFloorSpeed.m[2][2];

			if (m_RightThirdSideFloor[i].m_currFloorTimer > m_RightThirdSideFloor[i].m_FloorSpeedTimer)
			{
				m_RightThirdSideFloor[i].m_currFloorTimer = 0.0;
				if (m_RightThirdSideFloor[i].m_currFloorNum > 1)
				{
					m_RightThirdSideFloor[i].m_currFloorNum--;
					m_RightThirdSideFloor[i].m_IsRender = true;
				}
				else
				{
					//Loop to the top
					m_RightThirdSideFloor[i].m_currFloorNum = 8;
					m_RightThirdSideFloor[i].m_currFloorSpeed = { 0 };
					m_RightThirdSideFloor[i].m_TransformFloorCurr = m_LastFloorData;
					m_RightThirdSideFloor[i].m_IsRender = false;
				}
			}
			else
				m_RightThirdSideFloor[i].m_currFloorTimer += dt;
		}

		m_LastFloorData = m_RightFourthSideFloor[8].m_TransformFloorData;
		for (int i = 9; i > -1; i--)
		{
			AEMtx33 m_NextFloorData = m_RightFourthSideFloor[i].m_currFloorNum != 0 ? m_RightFourthSideFloor[m_RightFourthSideFloor[i].m_currFloorNum - 1].m_TransformFloorData : m_RightFourthSideFloor[i].m_TransformFloorCurr = m_LastFloorData;
			AEMtx33 m_CurrFloorData = m_RightFourthSideFloor[m_RightFourthSideFloor[i].m_currFloorNum].m_TransformFloorData;

			//Minimum Speed of next floor
			AEMtx33 m_MinimumNextFloorSpeed = {
			(m_NextFloorData.m[0][0] - m_CurrFloorData.m[0][0]) / 80,
			(m_NextFloorData.m[0][1] - m_CurrFloorData.m[0][1]) / 80,
			(m_NextFloorData.m[0][2] - m_CurrFloorData.m[0][2]) / 80,
			(m_NextFloorData.m[1][0] - m_CurrFloorData.m[1][0]) / 80,
			(m_NextFloorData.m[1][1] - m_CurrFloorData.m[1][1]) / 80,
			(m_NextFloorData.m[1][2] - m_CurrFloorData.m[1][2]) / 80,
			(m_NextFloorData.m[2][0] - m_CurrFloorData.m[2][0]) / 80,
			(m_NextFloorData.m[2][1] - m_CurrFloorData.m[2][1]) / 80,
			(m_NextFloorData.m[2][2] - m_CurrFloorData.m[2][2]) / 80
			};

			//Incrementing speed
			m_RightFourthSideFloor[i].m_currFloorSpeed.m[0][0] += m_RightFourthSideFloor[i].m_currFloorSpeed.m[0][0] < m_MinimumNextFloorSpeed.m[0][0] ? dt * m_MinimumNextFloorSpeed.m[0][0] : m_RightFourthSideFloor[i].m_currFloorSpeed.m[0][0] > m_MinimumNextFloorSpeed.m[0][0] ? dt * m_MinimumNextFloorSpeed.m[0][0] : 0;
			m_RightFourthSideFloor[i].m_currFloorSpeed.m[0][1] += m_RightFourthSideFloor[i].m_currFloorSpeed.m[0][1] < m_MinimumNextFloorSpeed.m[0][1] ? dt * m_MinimumNextFloorSpeed.m[0][1] : m_RightFourthSideFloor[i].m_currFloorSpeed.m[0][1] > m_MinimumNextFloorSpeed.m[0][1] ? dt * m_MinimumNextFloorSpeed.m[0][1] : 0;
			m_RightFourthSideFloor[i].m_currFloorSpeed.m[0][2] += m_RightFourthSideFloor[i].m_currFloorSpeed.m[0][2] < m_MinimumNextFloorSpeed.m[0][2] ? dt * m_MinimumNextFloorSpeed.m[0][2] : m_RightFourthSideFloor[i].m_currFloorSpeed.m[0][2] > m_MinimumNextFloorSpeed.m[0][2] ? dt * m_MinimumNextFloorSpeed.m[0][2] : 0;
			m_RightFourthSideFloor[i].m_currFloorSpeed.m[1][0] += m_RightFourthSideFloor[i].m_currFloorSpeed.m[1][0] < m_MinimumNextFloorSpeed.m[1][0] ? dt * m_MinimumNextFloorSpeed.m[1][0] : m_RightFourthSideFloor[i].m_currFloorSpeed.m[1][0] > m_MinimumNextFloorSpeed.m[1][0] ? dt * m_MinimumNextFloorSpeed.m[1][0] : 0;
			m_RightFourthSideFloor[i].m_currFloorSpeed.m[1][1] += m_RightFourthSideFloor[i].m_currFloorSpeed.m[1][1] < m_MinimumNextFloorSpeed.m[1][1] ? dt * m_MinimumNextFloorSpeed.m[1][1] : m_RightFourthSideFloor[i].m_currFloorSpeed.m[1][1] > m_MinimumNextFloorSpeed.m[1][1] ? dt * m_MinimumNextFloorSpeed.m[1][1] : 0;
			m_RightFourthSideFloor[i].m_currFloorSpeed.m[1][2] += m_RightFourthSideFloor[i].m_currFloorSpeed.m[1][2] < m_MinimumNextFloorSpeed.m[1][2] ? dt * m_MinimumNextFloorSpeed.m[1][2] : m_RightFourthSideFloor[i].m_currFloorSpeed.m[1][2] > m_MinimumNextFloorSpeed.m[1][2] ? dt * m_MinimumNextFloorSpeed.m[1][2] : 0;
			m_RightFourthSideFloor[i].m_currFloorSpeed.m[2][0] += m_RightFourthSideFloor[i].m_currFloorSpeed.m[2][0] < m_MinimumNextFloorSpeed.m[2][0] ? dt * m_MinimumNextFloorSpeed.m[2][0] : m_RightFourthSideFloor[i].m_currFloorSpeed.m[2][0] > m_MinimumNextFloorSpeed.m[2][0] ? dt * m_MinimumNextFloorSpeed.m[2][0] : 0;
			m_RightFourthSideFloor[i].m_currFloorSpeed.m[2][1] += m_RightFourthSideFloor[i].m_currFloorSpeed.m[2][1] < m_MinimumNextFloorSpeed.m[2][1] ? dt * m_MinimumNextFloorSpeed.m[2][1] : m_RightFourthSideFloor[i].m_currFloorSpeed.m[2][1] > m_MinimumNextFloorSpeed.m[2][1] ? dt * m_MinimumNextFloorSpeed.m[2][1] : 0;
			m_RightFourthSideFloor[i].m_currFloorSpeed.m[2][2] += m_RightFourthSideFloor[i].m_currFloorSpeed.m[2][2] < m_MinimumNextFloorSpeed.m[2][2] ? dt * m_MinimumNextFloorSpeed.m[2][2] : m_RightFourthSideFloor[i].m_currFloorSpeed.m[2][2] > m_MinimumNextFloorSpeed.m[2][2] ? dt * m_MinimumNextFloorSpeed.m[2][2] : 0;
			//Adding to floor
			m_RightFourthSideFloor[i].m_TransformFloorCurr.m[0][0] += m_RightFourthSideFloor[i].m_currFloorSpeed.m[0][0];
			m_RightFourthSideFloor[i].m_TransformFloorCurr.m[0][1] += m_RightFourthSideFloor[i].m_currFloorSpeed.m[0][1];
			m_RightFourthSideFloor[i].m_TransformFloorCurr.m[0][2] += m_RightFourthSideFloor[i].m_currFloorSpeed.m[0][2];
			m_RightFourthSideFloor[i].m_TransformFloorCurr.m[1][0] += m_RightFourthSideFloor[i].m_currFloorSpeed.m[1][0];
			m_RightFourthSideFloor[i].m_TransformFloorCurr.m[1][1] += m_RightFourthSideFloor[i].m_currFloorSpeed.m[1][1];
			m_RightFourthSideFloor[i].m_TransformFloorCurr.m[1][2] += m_RightFourthSideFloor[i].m_currFloorSpeed.m[1][2];
			m_RightFourthSideFloor[i].m_TransformFloorCurr.m[2][0] += m_RightFourthSideFloor[i].m_currFloorSpeed.m[2][0];
			m_RightFourthSideFloor[i].m_TransformFloorCurr.m[2][1] += m_RightFourthSideFloor[i].m_currFloorSpeed.m[2][1];
			m_RightFourthSideFloor[i].m_TransformFloorCurr.m[2][2] += m_RightFourthSideFloor[i].m_currFloorSpeed.m[2][2];

			if (m_RightFourthSideFloor[i].m_currFloorTimer > m_RightFourthSideFloor[i].m_FloorSpeedTimer)
			{
				m_RightFourthSideFloor[i].m_currFloorTimer = 0.0;
				if (m_RightFourthSideFloor[i].m_currFloorNum > 1)
				{
					m_RightFourthSideFloor[i].m_currFloorNum--;
					m_RightFourthSideFloor[i].m_IsRender = true;
				}
				else
				{
					//Loop to the top
					m_RightFourthSideFloor[i].m_currFloorNum = 8;
					m_RightFourthSideFloor[i].m_currFloorSpeed = { 0 };
					m_RightFourthSideFloor[i].m_TransformFloorCurr = m_LastFloorData;
					m_RightFourthSideFloor[i].m_IsRender = false;
				}
			}
			else
				m_RightFourthSideFloor[i].m_currFloorTimer += dt;
		}

		m_LastFloorData = m_LeftSideFloor[8].m_TransformFloorData;
		for (int i = 9; i > -1; i--)
		{
			AEMtx33 m_NextFloorData = m_LeftSideFloor[i].m_currFloorNum != 0 ? m_LeftSideFloor[m_LeftSideFloor[i].m_currFloorNum - 1].m_TransformFloorData : m_LeftSideFloor[i].m_TransformFloorCurr = m_LastFloorData;
			AEMtx33 m_CurrFloorData = m_LeftSideFloor[m_LeftSideFloor[i].m_currFloorNum].m_TransformFloorData;

			//Minimum Speed of next floor
			AEMtx33 m_MinimumNextFloorSpeed = {
			(m_NextFloorData.m[0][0] - m_CurrFloorData.m[0][0]) / 80,
			(m_NextFloorData.m[0][1] - m_CurrFloorData.m[0][1]) / 80,
			(m_NextFloorData.m[0][2] - m_CurrFloorData.m[0][2]) / 80,
			(m_NextFloorData.m[1][0] - m_CurrFloorData.m[1][0]) / 80,
			(m_NextFloorData.m[1][1] - m_CurrFloorData.m[1][1]) / 80,
			(m_NextFloorData.m[1][2] - m_CurrFloorData.m[1][2]) / 80,
			(m_NextFloorData.m[2][0] - m_CurrFloorData.m[2][0]) / 80,
			(m_NextFloorData.m[2][1] - m_CurrFloorData.m[2][1]) / 80,
			(m_NextFloorData.m[2][2] - m_CurrFloorData.m[2][2]) / 80
			};

			//Incrementing speed
			m_LeftSideFloor[i].m_currFloorSpeed.m[0][0] += m_LeftSideFloor[i].m_currFloorSpeed.m[0][0] < m_MinimumNextFloorSpeed.m[0][0] ? dt * m_MinimumNextFloorSpeed.m[0][0] : m_LeftSideFloor[i].m_currFloorSpeed.m[0][0] > m_MinimumNextFloorSpeed.m[0][0] ? dt * m_MinimumNextFloorSpeed.m[0][0] : 0;
			m_LeftSideFloor[i].m_currFloorSpeed.m[0][1] += m_LeftSideFloor[i].m_currFloorSpeed.m[0][1] < m_MinimumNextFloorSpeed.m[0][1] ? dt * m_MinimumNextFloorSpeed.m[0][1] : m_LeftSideFloor[i].m_currFloorSpeed.m[0][1] > m_MinimumNextFloorSpeed.m[0][1] ? dt * m_MinimumNextFloorSpeed.m[0][1] : 0;
			m_LeftSideFloor[i].m_currFloorSpeed.m[0][2] += m_LeftSideFloor[i].m_currFloorSpeed.m[0][2] < m_MinimumNextFloorSpeed.m[0][2] ? dt * m_MinimumNextFloorSpeed.m[0][2] : m_LeftSideFloor[i].m_currFloorSpeed.m[0][2] > m_MinimumNextFloorSpeed.m[0][2] ? dt * m_MinimumNextFloorSpeed.m[0][2] : 0;
			m_LeftSideFloor[i].m_currFloorSpeed.m[1][0] += m_LeftSideFloor[i].m_currFloorSpeed.m[1][0] < m_MinimumNextFloorSpeed.m[1][0] ? dt * m_MinimumNextFloorSpeed.m[1][0] : m_LeftSideFloor[i].m_currFloorSpeed.m[1][0] > m_MinimumNextFloorSpeed.m[1][0] ? dt * m_MinimumNextFloorSpeed.m[1][0] : 0;
			m_LeftSideFloor[i].m_currFloorSpeed.m[1][1] += m_LeftSideFloor[i].m_currFloorSpeed.m[1][1] < m_MinimumNextFloorSpeed.m[1][1] ? dt * m_MinimumNextFloorSpeed.m[1][1] : m_LeftSideFloor[i].m_currFloorSpeed.m[1][1] > m_MinimumNextFloorSpeed.m[1][1] ? dt * m_MinimumNextFloorSpeed.m[1][1] : 0;
			m_LeftSideFloor[i].m_currFloorSpeed.m[1][2] += m_LeftSideFloor[i].m_currFloorSpeed.m[1][2] < m_MinimumNextFloorSpeed.m[1][2] ? dt * m_MinimumNextFloorSpeed.m[1][2] : m_LeftSideFloor[i].m_currFloorSpeed.m[1][2] > m_MinimumNextFloorSpeed.m[1][2] ? dt * m_MinimumNextFloorSpeed.m[1][2] : 0;
			m_LeftSideFloor[i].m_currFloorSpeed.m[2][0] += m_LeftSideFloor[i].m_currFloorSpeed.m[2][0] < m_MinimumNextFloorSpeed.m[2][0] ? dt * m_MinimumNextFloorSpeed.m[2][0] : m_LeftSideFloor[i].m_currFloorSpeed.m[2][0] > m_MinimumNextFloorSpeed.m[2][0] ? dt * m_MinimumNextFloorSpeed.m[2][0] : 0;
			m_LeftSideFloor[i].m_currFloorSpeed.m[2][1] += m_LeftSideFloor[i].m_currFloorSpeed.m[2][1] < m_MinimumNextFloorSpeed.m[2][1] ? dt * m_MinimumNextFloorSpeed.m[2][1] : m_LeftSideFloor[i].m_currFloorSpeed.m[2][1] > m_MinimumNextFloorSpeed.m[2][1] ? dt * m_MinimumNextFloorSpeed.m[2][1] : 0;
			m_LeftSideFloor[i].m_currFloorSpeed.m[2][2] += m_LeftSideFloor[i].m_currFloorSpeed.m[2][2] < m_MinimumNextFloorSpeed.m[2][2] ? dt * m_MinimumNextFloorSpeed.m[2][2] : m_LeftSideFloor[i].m_currFloorSpeed.m[2][2] > m_MinimumNextFloorSpeed.m[2][2] ? dt * m_MinimumNextFloorSpeed.m[2][2] : 0;
			//Adding to floor
			m_LeftSideFloor[i].m_TransformFloorCurr.m[0][0] += m_LeftSideFloor[i].m_currFloorSpeed.m[0][0];
			m_LeftSideFloor[i].m_TransformFloorCurr.m[0][1] += m_LeftSideFloor[i].m_currFloorSpeed.m[0][1];
			m_LeftSideFloor[i].m_TransformFloorCurr.m[0][2] += m_LeftSideFloor[i].m_currFloorSpeed.m[0][2];
			m_LeftSideFloor[i].m_TransformFloorCurr.m[1][0] += m_LeftSideFloor[i].m_currFloorSpeed.m[1][0];
			m_LeftSideFloor[i].m_TransformFloorCurr.m[1][1] += m_LeftSideFloor[i].m_currFloorSpeed.m[1][1];
			m_LeftSideFloor[i].m_TransformFloorCurr.m[1][2] += m_LeftSideFloor[i].m_currFloorSpeed.m[1][2];
			m_LeftSideFloor[i].m_TransformFloorCurr.m[2][0] += m_LeftSideFloor[i].m_currFloorSpeed.m[2][0];
			m_LeftSideFloor[i].m_TransformFloorCurr.m[2][1] += m_LeftSideFloor[i].m_currFloorSpeed.m[2][1];
			m_LeftSideFloor[i].m_TransformFloorCurr.m[2][2] += m_LeftSideFloor[i].m_currFloorSpeed.m[2][2];

			if (m_LeftSideFloor[i].m_currFloorTimer > m_LeftSideFloor[i].m_FloorSpeedTimer)
			{
				m_LeftSideFloor[i].m_currFloorTimer = 0.0;
				if (m_LeftSideFloor[i].m_currFloorNum > 1)
				{
					m_LeftSideFloor[i].m_currFloorNum--;
					m_LeftSideFloor[i].m_IsRender = true;
				}
				else
				{
					//Loop to the top
					m_LeftSideFloor[i].m_currFloorNum = 8;
					m_LeftSideFloor[i].m_currFloorSpeed = { 0 };
					m_LeftSideFloor[i].m_TransformFloorCurr = m_LastFloorData;
					m_LeftSideFloor[i].m_IsRender = false;
				}
			}
			else
				m_LeftSideFloor[i].m_currFloorTimer += dt;
		}


		m_LastFloorData = m_LeftSecondSideFloor[8].m_TransformFloorData;
		for (int i = 9; i > -1; i--)
		{
			AEMtx33 m_NextFloorData = m_LeftSecondSideFloor[i].m_currFloorNum != 0 ? m_LeftSecondSideFloor[m_LeftSecondSideFloor[i].m_currFloorNum - 1].m_TransformFloorData : m_LeftSecondSideFloor[i].m_TransformFloorCurr = m_LastFloorData;
			AEMtx33 m_CurrFloorData = m_LeftSecondSideFloor[m_LeftSecondSideFloor[i].m_currFloorNum].m_TransformFloorData;

			//Minimum Speed of next floor
			AEMtx33 m_MinimumNextFloorSpeed = {
			(m_NextFloorData.m[0][0] - m_CurrFloorData.m[0][0]) / 80,
			(m_NextFloorData.m[0][1] - m_CurrFloorData.m[0][1]) / 80,
			(m_NextFloorData.m[0][2] - m_CurrFloorData.m[0][2]) / 80,
			(m_NextFloorData.m[1][0] - m_CurrFloorData.m[1][0]) / 80,
			(m_NextFloorData.m[1][1] - m_CurrFloorData.m[1][1]) / 80,
			(m_NextFloorData.m[1][2] - m_CurrFloorData.m[1][2]) / 80,
			(m_NextFloorData.m[2][0] - m_CurrFloorData.m[2][0]) / 80,
			(m_NextFloorData.m[2][1] - m_CurrFloorData.m[2][1]) / 80,
			(m_NextFloorData.m[2][2] - m_CurrFloorData.m[2][2]) / 80
			};

			//Incrementing speed
			m_LeftSecondSideFloor[i].m_currFloorSpeed.m[0][0] += m_LeftSecondSideFloor[i].m_currFloorSpeed.m[0][0] < m_MinimumNextFloorSpeed.m[0][0] ? dt * m_MinimumNextFloorSpeed.m[0][0] : m_LeftSecondSideFloor[i].m_currFloorSpeed.m[0][0] > m_MinimumNextFloorSpeed.m[0][0] ? dt * m_MinimumNextFloorSpeed.m[0][0] : 0;
			m_LeftSecondSideFloor[i].m_currFloorSpeed.m[0][1] += m_LeftSecondSideFloor[i].m_currFloorSpeed.m[0][1] < m_MinimumNextFloorSpeed.m[0][1] ? dt * m_MinimumNextFloorSpeed.m[0][1] : m_LeftSecondSideFloor[i].m_currFloorSpeed.m[0][1] > m_MinimumNextFloorSpeed.m[0][1] ? dt * m_MinimumNextFloorSpeed.m[0][1] : 0;
			m_LeftSecondSideFloor[i].m_currFloorSpeed.m[0][2] += m_LeftSecondSideFloor[i].m_currFloorSpeed.m[0][2] < m_MinimumNextFloorSpeed.m[0][2] ? dt * m_MinimumNextFloorSpeed.m[0][2] : m_LeftSecondSideFloor[i].m_currFloorSpeed.m[0][2] > m_MinimumNextFloorSpeed.m[0][2] ? dt * m_MinimumNextFloorSpeed.m[0][2] : 0;
			m_LeftSecondSideFloor[i].m_currFloorSpeed.m[1][0] += m_LeftSecondSideFloor[i].m_currFloorSpeed.m[1][0] < m_MinimumNextFloorSpeed.m[1][0] ? dt * m_MinimumNextFloorSpeed.m[1][0] : m_LeftSecondSideFloor[i].m_currFloorSpeed.m[1][0] > m_MinimumNextFloorSpeed.m[1][0] ? dt * m_MinimumNextFloorSpeed.m[1][0] : 0;
			m_LeftSecondSideFloor[i].m_currFloorSpeed.m[1][1] += m_LeftSecondSideFloor[i].m_currFloorSpeed.m[1][1] < m_MinimumNextFloorSpeed.m[1][1] ? dt * m_MinimumNextFloorSpeed.m[1][1] : m_LeftSecondSideFloor[i].m_currFloorSpeed.m[1][1] > m_MinimumNextFloorSpeed.m[1][1] ? dt * m_MinimumNextFloorSpeed.m[1][1] : 0;
			m_LeftSecondSideFloor[i].m_currFloorSpeed.m[1][2] += m_LeftSecondSideFloor[i].m_currFloorSpeed.m[1][2] < m_MinimumNextFloorSpeed.m[1][2] ? dt * m_MinimumNextFloorSpeed.m[1][2] : m_LeftSecondSideFloor[i].m_currFloorSpeed.m[1][2] > m_MinimumNextFloorSpeed.m[1][2] ? dt * m_MinimumNextFloorSpeed.m[1][2] : 0;
			m_LeftSecondSideFloor[i].m_currFloorSpeed.m[2][0] += m_LeftSecondSideFloor[i].m_currFloorSpeed.m[2][0] < m_MinimumNextFloorSpeed.m[2][0] ? dt * m_MinimumNextFloorSpeed.m[2][0] : m_LeftSecondSideFloor[i].m_currFloorSpeed.m[2][0] > m_MinimumNextFloorSpeed.m[2][0] ? dt * m_MinimumNextFloorSpeed.m[2][0] : 0;
			m_LeftSecondSideFloor[i].m_currFloorSpeed.m[2][1] += m_LeftSecondSideFloor[i].m_currFloorSpeed.m[2][1] < m_MinimumNextFloorSpeed.m[2][1] ? dt * m_MinimumNextFloorSpeed.m[2][1] : m_LeftSecondSideFloor[i].m_currFloorSpeed.m[2][1] > m_MinimumNextFloorSpeed.m[2][1] ? dt * m_MinimumNextFloorSpeed.m[2][1] : 0;
			m_LeftSecondSideFloor[i].m_currFloorSpeed.m[2][2] += m_LeftSecondSideFloor[i].m_currFloorSpeed.m[2][2] < m_MinimumNextFloorSpeed.m[2][2] ? dt * m_MinimumNextFloorSpeed.m[2][2] : m_LeftSecondSideFloor[i].m_currFloorSpeed.m[2][2] > m_MinimumNextFloorSpeed.m[2][2] ? dt * m_MinimumNextFloorSpeed.m[2][2] : 0;
			//Adding to floor
			m_LeftSecondSideFloor[i].m_TransformFloorCurr.m[0][0] += m_LeftSecondSideFloor[i].m_currFloorSpeed.m[0][0];
			m_LeftSecondSideFloor[i].m_TransformFloorCurr.m[0][1] += m_LeftSecondSideFloor[i].m_currFloorSpeed.m[0][1];
			m_LeftSecondSideFloor[i].m_TransformFloorCurr.m[0][2] += m_LeftSecondSideFloor[i].m_currFloorSpeed.m[0][2];
			m_LeftSecondSideFloor[i].m_TransformFloorCurr.m[1][0] += m_LeftSecondSideFloor[i].m_currFloorSpeed.m[1][0];
			m_LeftSecondSideFloor[i].m_TransformFloorCurr.m[1][1] += m_LeftSecondSideFloor[i].m_currFloorSpeed.m[1][1];
			m_LeftSecondSideFloor[i].m_TransformFloorCurr.m[1][2] += m_LeftSecondSideFloor[i].m_currFloorSpeed.m[1][2];
			m_LeftSecondSideFloor[i].m_TransformFloorCurr.m[2][0] += m_LeftSecondSideFloor[i].m_currFloorSpeed.m[2][0];
			m_LeftSecondSideFloor[i].m_TransformFloorCurr.m[2][1] += m_LeftSecondSideFloor[i].m_currFloorSpeed.m[2][1];
			m_LeftSecondSideFloor[i].m_TransformFloorCurr.m[2][2] += m_LeftSecondSideFloor[i].m_currFloorSpeed.m[2][2];

			if (m_LeftSecondSideFloor[i].m_currFloorTimer > m_LeftSecondSideFloor[i].m_FloorSpeedTimer)
			{
				m_LeftSecondSideFloor[i].m_currFloorTimer = 0.0;
				if (m_LeftSecondSideFloor[i].m_currFloorNum > 1)
				{
					m_LeftSecondSideFloor[i].m_currFloorNum--;
					m_LeftSecondSideFloor[i].m_IsRender = true;
				}
				else
				{
					//Loop to the top
					m_LeftSecondSideFloor[i].m_currFloorNum = 8;
					m_LeftSecondSideFloor[i].m_currFloorSpeed = { 0 };
					m_LeftSecondSideFloor[i].m_TransformFloorCurr = m_LastFloorData;
					m_LeftSecondSideFloor[i].m_IsRender = false;
				}
			}
			else
				m_LeftSecondSideFloor[i].m_currFloorTimer += dt;
		}

		m_LastFloorData = m_LeftThirdSideFloor[8].m_TransformFloorData;
		for (int i = 9; i > -1; i--)
		{
			AEMtx33 m_NextFloorData = m_LeftThirdSideFloor[i].m_currFloorNum != 0 ? m_LeftThirdSideFloor[m_LeftThirdSideFloor[i].m_currFloorNum - 1].m_TransformFloorData : m_LeftThirdSideFloor[i].m_TransformFloorCurr = m_LastFloorData;
			AEMtx33 m_CurrFloorData = m_LeftThirdSideFloor[m_LeftThirdSideFloor[i].m_currFloorNum].m_TransformFloorData;

			//Minimum Speed of next floor
			AEMtx33 m_MinimumNextFloorSpeed = {
			(m_NextFloorData.m[0][0] - m_CurrFloorData.m[0][0]) / 80,
			(m_NextFloorData.m[0][1] - m_CurrFloorData.m[0][1]) / 80,
			(m_NextFloorData.m[0][2] - m_CurrFloorData.m[0][2]) / 80,
			(m_NextFloorData.m[1][0] - m_CurrFloorData.m[1][0]) / 80,
			(m_NextFloorData.m[1][1] - m_CurrFloorData.m[1][1]) / 80,
			(m_NextFloorData.m[1][2] - m_CurrFloorData.m[1][2]) / 80,
			(m_NextFloorData.m[2][0] - m_CurrFloorData.m[2][0]) / 80,
			(m_NextFloorData.m[2][1] - m_CurrFloorData.m[2][1]) / 80,
			(m_NextFloorData.m[2][2] - m_CurrFloorData.m[2][2]) / 80
			};

			//Incrementing speed
			m_LeftThirdSideFloor[i].m_currFloorSpeed.m[0][0] += m_LeftThirdSideFloor[i].m_currFloorSpeed.m[0][0] < m_MinimumNextFloorSpeed.m[0][0] ? dt * m_MinimumNextFloorSpeed.m[0][0] : m_LeftThirdSideFloor[i].m_currFloorSpeed.m[0][0] > m_MinimumNextFloorSpeed.m[0][0] ? dt * m_MinimumNextFloorSpeed.m[0][0] : 0;
			m_LeftThirdSideFloor[i].m_currFloorSpeed.m[0][1] += m_LeftThirdSideFloor[i].m_currFloorSpeed.m[0][1] < m_MinimumNextFloorSpeed.m[0][1] ? dt * m_MinimumNextFloorSpeed.m[0][1] : m_LeftThirdSideFloor[i].m_currFloorSpeed.m[0][1] > m_MinimumNextFloorSpeed.m[0][1] ? dt * m_MinimumNextFloorSpeed.m[0][1] : 0;
			m_LeftThirdSideFloor[i].m_currFloorSpeed.m[0][2] += m_LeftThirdSideFloor[i].m_currFloorSpeed.m[0][2] < m_MinimumNextFloorSpeed.m[0][2] ? dt * m_MinimumNextFloorSpeed.m[0][2] : m_LeftThirdSideFloor[i].m_currFloorSpeed.m[0][2] > m_MinimumNextFloorSpeed.m[0][2] ? dt * m_MinimumNextFloorSpeed.m[0][2] : 0;
			m_LeftThirdSideFloor[i].m_currFloorSpeed.m[1][0] += m_LeftThirdSideFloor[i].m_currFloorSpeed.m[1][0] < m_MinimumNextFloorSpeed.m[1][0] ? dt * m_MinimumNextFloorSpeed.m[1][0] : m_LeftThirdSideFloor[i].m_currFloorSpeed.m[1][0] > m_MinimumNextFloorSpeed.m[1][0] ? dt * m_MinimumNextFloorSpeed.m[1][0] : 0;
			m_LeftThirdSideFloor[i].m_currFloorSpeed.m[1][1] += m_LeftThirdSideFloor[i].m_currFloorSpeed.m[1][1] < m_MinimumNextFloorSpeed.m[1][1] ? dt * m_MinimumNextFloorSpeed.m[1][1] : m_LeftThirdSideFloor[i].m_currFloorSpeed.m[1][1] > m_MinimumNextFloorSpeed.m[1][1] ? dt * m_MinimumNextFloorSpeed.m[1][1] : 0;
			m_LeftThirdSideFloor[i].m_currFloorSpeed.m[1][2] += m_LeftThirdSideFloor[i].m_currFloorSpeed.m[1][2] < m_MinimumNextFloorSpeed.m[1][2] ? dt * m_MinimumNextFloorSpeed.m[1][2] : m_LeftThirdSideFloor[i].m_currFloorSpeed.m[1][2] > m_MinimumNextFloorSpeed.m[1][2] ? dt * m_MinimumNextFloorSpeed.m[1][2] : 0;
			m_LeftThirdSideFloor[i].m_currFloorSpeed.m[2][0] += m_LeftThirdSideFloor[i].m_currFloorSpeed.m[2][0] < m_MinimumNextFloorSpeed.m[2][0] ? dt * m_MinimumNextFloorSpeed.m[2][0] : m_LeftThirdSideFloor[i].m_currFloorSpeed.m[2][0] > m_MinimumNextFloorSpeed.m[2][0] ? dt * m_MinimumNextFloorSpeed.m[2][0] : 0;
			m_LeftThirdSideFloor[i].m_currFloorSpeed.m[2][1] += m_LeftThirdSideFloor[i].m_currFloorSpeed.m[2][1] < m_MinimumNextFloorSpeed.m[2][1] ? dt * m_MinimumNextFloorSpeed.m[2][1] : m_LeftThirdSideFloor[i].m_currFloorSpeed.m[2][1] > m_MinimumNextFloorSpeed.m[2][1] ? dt * m_MinimumNextFloorSpeed.m[2][1] : 0;
			m_LeftThirdSideFloor[i].m_currFloorSpeed.m[2][2] += m_LeftThirdSideFloor[i].m_currFloorSpeed.m[2][2] < m_MinimumNextFloorSpeed.m[2][2] ? dt * m_MinimumNextFloorSpeed.m[2][2] : m_LeftThirdSideFloor[i].m_currFloorSpeed.m[2][2] > m_MinimumNextFloorSpeed.m[2][2] ? dt * m_MinimumNextFloorSpeed.m[2][2] : 0;
			//Adding to floor
			m_LeftThirdSideFloor[i].m_TransformFloorCurr.m[0][0] += m_LeftThirdSideFloor[i].m_currFloorSpeed.m[0][0];
			m_LeftThirdSideFloor[i].m_TransformFloorCurr.m[0][1] += m_LeftThirdSideFloor[i].m_currFloorSpeed.m[0][1];
			m_LeftThirdSideFloor[i].m_TransformFloorCurr.m[0][2] += m_LeftThirdSideFloor[i].m_currFloorSpeed.m[0][2];
			m_LeftThirdSideFloor[i].m_TransformFloorCurr.m[1][0] += m_LeftThirdSideFloor[i].m_currFloorSpeed.m[1][0];
			m_LeftThirdSideFloor[i].m_TransformFloorCurr.m[1][1] += m_LeftThirdSideFloor[i].m_currFloorSpeed.m[1][1];
			m_LeftThirdSideFloor[i].m_TransformFloorCurr.m[1][2] += m_LeftThirdSideFloor[i].m_currFloorSpeed.m[1][2];
			m_LeftThirdSideFloor[i].m_TransformFloorCurr.m[2][0] += m_LeftThirdSideFloor[i].m_currFloorSpeed.m[2][0];
			m_LeftThirdSideFloor[i].m_TransformFloorCurr.m[2][1] += m_LeftThirdSideFloor[i].m_currFloorSpeed.m[2][1];
			m_LeftThirdSideFloor[i].m_TransformFloorCurr.m[2][2] += m_LeftThirdSideFloor[i].m_currFloorSpeed.m[2][2];

			if (m_LeftThirdSideFloor[i].m_currFloorTimer > m_LeftThirdSideFloor[i].m_FloorSpeedTimer)
			{
				m_LeftThirdSideFloor[i].m_currFloorTimer = 0.0;
				if (m_LeftThirdSideFloor[i].m_currFloorNum > 1)
				{
					m_LeftThirdSideFloor[i].m_currFloorNum--;
					m_LeftThirdSideFloor[i].m_IsRender = true;
				}
				else
				{
					//Loop to the top
					m_LeftThirdSideFloor[i].m_currFloorNum = 8;
					m_LeftThirdSideFloor[i].m_currFloorSpeed = { 0 };
					m_LeftThirdSideFloor[i].m_TransformFloorCurr = m_LastFloorData;
					m_LeftThirdSideFloor[i].m_IsRender = false;
				}
			}
			else
				m_LeftThirdSideFloor[i].m_currFloorTimer += dt;
		}

		m_LastFloorData = m_LeftFourthSideFloor[8].m_TransformFloorData;
		for (int i = 9; i > -1; i--)
		{
			AEMtx33 m_NextFloorData = m_LeftFourthSideFloor[i].m_currFloorNum != 0 ? m_LeftFourthSideFloor[m_LeftFourthSideFloor[i].m_currFloorNum - 1].m_TransformFloorData : m_LeftFourthSideFloor[i].m_TransformFloorCurr = m_LastFloorData;
			AEMtx33 m_CurrFloorData = m_LeftFourthSideFloor[m_LeftFourthSideFloor[i].m_currFloorNum].m_TransformFloorData;

			//Minimum Speed of next floor
			AEMtx33 m_MinimumNextFloorSpeed = {
			(m_NextFloorData.m[0][0] - m_CurrFloorData.m[0][0]) / 80,
			(m_NextFloorData.m[0][1] - m_CurrFloorData.m[0][1]) / 80,
			(m_NextFloorData.m[0][2] - m_CurrFloorData.m[0][2]) / 80,
			(m_NextFloorData.m[1][0] - m_CurrFloorData.m[1][0]) / 80,
			(m_NextFloorData.m[1][1] - m_CurrFloorData.m[1][1]) / 80,
			(m_NextFloorData.m[1][2] - m_CurrFloorData.m[1][2]) / 80,
			(m_NextFloorData.m[2][0] - m_CurrFloorData.m[2][0]) / 80,
			(m_NextFloorData.m[2][1] - m_CurrFloorData.m[2][1]) / 80,
			(m_NextFloorData.m[2][2] - m_CurrFloorData.m[2][2]) / 80
			};

			//Incrementing speed
			m_LeftFourthSideFloor[i].m_currFloorSpeed.m[0][0] += m_LeftFourthSideFloor[i].m_currFloorSpeed.m[0][0] < m_MinimumNextFloorSpeed.m[0][0] ? dt * m_MinimumNextFloorSpeed.m[0][0] : m_LeftFourthSideFloor[i].m_currFloorSpeed.m[0][0] > m_MinimumNextFloorSpeed.m[0][0] ? dt * m_MinimumNextFloorSpeed.m[0][0] : 0;
			m_LeftFourthSideFloor[i].m_currFloorSpeed.m[0][1] += m_LeftFourthSideFloor[i].m_currFloorSpeed.m[0][1] < m_MinimumNextFloorSpeed.m[0][1] ? dt * m_MinimumNextFloorSpeed.m[0][1] : m_LeftFourthSideFloor[i].m_currFloorSpeed.m[0][1] > m_MinimumNextFloorSpeed.m[0][1] ? dt * m_MinimumNextFloorSpeed.m[0][1] : 0;
			m_LeftFourthSideFloor[i].m_currFloorSpeed.m[0][2] += m_LeftFourthSideFloor[i].m_currFloorSpeed.m[0][2] < m_MinimumNextFloorSpeed.m[0][2] ? dt * m_MinimumNextFloorSpeed.m[0][2] : m_LeftFourthSideFloor[i].m_currFloorSpeed.m[0][2] > m_MinimumNextFloorSpeed.m[0][2] ? dt * m_MinimumNextFloorSpeed.m[0][2] : 0;
			m_LeftFourthSideFloor[i].m_currFloorSpeed.m[1][0] += m_LeftFourthSideFloor[i].m_currFloorSpeed.m[1][0] < m_MinimumNextFloorSpeed.m[1][0] ? dt * m_MinimumNextFloorSpeed.m[1][0] : m_LeftFourthSideFloor[i].m_currFloorSpeed.m[1][0] > m_MinimumNextFloorSpeed.m[1][0] ? dt * m_MinimumNextFloorSpeed.m[1][0] : 0;
			m_LeftFourthSideFloor[i].m_currFloorSpeed.m[1][1] += m_LeftFourthSideFloor[i].m_currFloorSpeed.m[1][1] < m_MinimumNextFloorSpeed.m[1][1] ? dt * m_MinimumNextFloorSpeed.m[1][1] : m_LeftFourthSideFloor[i].m_currFloorSpeed.m[1][1] > m_MinimumNextFloorSpeed.m[1][1] ? dt * m_MinimumNextFloorSpeed.m[1][1] : 0;
			m_LeftFourthSideFloor[i].m_currFloorSpeed.m[1][2] += m_LeftFourthSideFloor[i].m_currFloorSpeed.m[1][2] < m_MinimumNextFloorSpeed.m[1][2] ? dt * m_MinimumNextFloorSpeed.m[1][2] : m_LeftFourthSideFloor[i].m_currFloorSpeed.m[1][2] > m_MinimumNextFloorSpeed.m[1][2] ? dt * m_MinimumNextFloorSpeed.m[1][2] : 0;
			m_LeftFourthSideFloor[i].m_currFloorSpeed.m[2][0] += m_LeftFourthSideFloor[i].m_currFloorSpeed.m[2][0] < m_MinimumNextFloorSpeed.m[2][0] ? dt * m_MinimumNextFloorSpeed.m[2][0] : m_LeftFourthSideFloor[i].m_currFloorSpeed.m[2][0] > m_MinimumNextFloorSpeed.m[2][0] ? dt * m_MinimumNextFloorSpeed.m[2][0] : 0;
			m_LeftFourthSideFloor[i].m_currFloorSpeed.m[2][1] += m_LeftFourthSideFloor[i].m_currFloorSpeed.m[2][1] < m_MinimumNextFloorSpeed.m[2][1] ? dt * m_MinimumNextFloorSpeed.m[2][1] : m_LeftFourthSideFloor[i].m_currFloorSpeed.m[2][1] > m_MinimumNextFloorSpeed.m[2][1] ? dt * m_MinimumNextFloorSpeed.m[2][1] : 0;
			m_LeftFourthSideFloor[i].m_currFloorSpeed.m[2][2] += m_LeftFourthSideFloor[i].m_currFloorSpeed.m[2][2] < m_MinimumNextFloorSpeed.m[2][2] ? dt * m_MinimumNextFloorSpeed.m[2][2] : m_LeftFourthSideFloor[i].m_currFloorSpeed.m[2][2] > m_MinimumNextFloorSpeed.m[2][2] ? dt * m_MinimumNextFloorSpeed.m[2][2] : 0;
			//Adding to floor
			m_LeftFourthSideFloor[i].m_TransformFloorCurr.m[0][0] += m_LeftFourthSideFloor[i].m_currFloorSpeed.m[0][0];
			m_LeftFourthSideFloor[i].m_TransformFloorCurr.m[0][1] += m_LeftFourthSideFloor[i].m_currFloorSpeed.m[0][1];
			m_LeftFourthSideFloor[i].m_TransformFloorCurr.m[0][2] += m_LeftFourthSideFloor[i].m_currFloorSpeed.m[0][2];
			m_LeftFourthSideFloor[i].m_TransformFloorCurr.m[1][0] += m_LeftFourthSideFloor[i].m_currFloorSpeed.m[1][0];
			m_LeftFourthSideFloor[i].m_TransformFloorCurr.m[1][1] += m_LeftFourthSideFloor[i].m_currFloorSpeed.m[1][1];
			m_LeftFourthSideFloor[i].m_TransformFloorCurr.m[1][2] += m_LeftFourthSideFloor[i].m_currFloorSpeed.m[1][2];
			m_LeftFourthSideFloor[i].m_TransformFloorCurr.m[2][0] += m_LeftFourthSideFloor[i].m_currFloorSpeed.m[2][0];
			m_LeftFourthSideFloor[i].m_TransformFloorCurr.m[2][1] += m_LeftFourthSideFloor[i].m_currFloorSpeed.m[2][1];
			m_LeftFourthSideFloor[i].m_TransformFloorCurr.m[2][2] += m_LeftFourthSideFloor[i].m_currFloorSpeed.m[2][2];

			if (m_LeftFourthSideFloor[i].m_currFloorTimer > m_LeftFourthSideFloor[i].m_FloorSpeedTimer)
			{
				m_LeftFourthSideFloor[i].m_currFloorTimer = 0.0;
				if (m_LeftFourthSideFloor[i].m_currFloorNum > 1)
				{
					m_LeftFourthSideFloor[i].m_currFloorNum--;
					m_LeftFourthSideFloor[i].m_IsRender = true;
				}
				else
				{
					//Loop to the top
					m_LeftFourthSideFloor[i].m_currFloorNum = 8;
					m_LeftFourthSideFloor[i].m_currFloorSpeed = { 0 };
					m_LeftFourthSideFloor[i].m_TransformFloorCurr = m_LastFloorData;
					m_LeftFourthSideFloor[i].m_IsRender = false;
				}
			}
			else
				m_LeftFourthSideFloor[i].m_currFloorTimer += dt;
		}
	}
}


void SceneSplashScreen::Render()
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

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Render Digipen Logo
	// ///////////////////////////////////////////////////////////////////////////////////////////////////////
	if (!ShowGame)
	{
		AEGfxSetTransparency(3.0f - 5/ LogoTimer);
		AEGfxTextureSet(pDigipenLogoTex, 0, 0);
		AEGfxSetTransform(m_TransformDigipenLogoData.m);
		AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
	}
	else
	{
		AEGfxSetTransparency(1.0f);
		//Sky
		AEGfxTextureSet(pSkyTex, 0, 0);
		AEGfxSetTransform(m_TransformSkyData.m);
		AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);

		//Sun
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		AEGfxSetColorToAdd(1.0f, 1.0f, 1.0f, 1.0f);
		AEGfxSetTransparency(0.0f);
		AEGfxSetTransform(m_TransformSunData.m);
		AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxSetTransparency(1.0f);
		AEGfxTextureSet(pSunOverlayTex, 0, 0);
		AEGfxSetTransform(m_TransformSunOverlayData.m);
		AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);

		////////////////////////////////////////////////////////////////////////////////////////
		// Floors
		// //////////////////////////////////////////////////////////////////////////////////
		// Tell the engine to get ready to draw something with texture.
		// Set the color to add to nothing, so that we don't alter the sprite's color
		AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 1.0f);
		//Main Floor
		AEGfxTextureSet(pFloorTex, 0, 0);
		for (int i = 0; i < 10; i++)
		{
			if (m_Floor[i].m_IsRender)
			{
				AEGfxSetTransform(m_Floor[i].m_TransformFloorCurr.m);
				AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
			}
		}

		//Right Side Floor
		AEGfxTextureSet(pSideRightFloorTex, 0, 0);
		for (int i = 0; i < 10; i++)
		{
			if (m_RightSideFloor[i].m_IsRender)
			{
				AEGfxSetTransform(m_RightSideFloor[i].m_TransformFloorCurr.m);
				AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
			}
		}

		//Right Second Side Floor
		AEGfxTextureSet(pSideRightFloorTex, 0, 0);
		for (int i = 0; i < 10; i++)
		{
			if (m_RightSecondSideFloor[i].m_IsRender)
			{
				AEGfxSetTransform(m_RightSecondSideFloor[i].m_TransformFloorCurr.m);
				AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
			}
		}

		//Right Third Side Floor
		AEGfxTextureSet(pSideRightFloorTex, 0, 0);
		for (int i = 0; i < 10; i++)
		{
			if (m_RightThirdSideFloor[i].m_IsRender)
			{
				AEGfxSetTransform(m_RightThirdSideFloor[i].m_TransformFloorCurr.m);
				AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
			}
		}

		//Right Fourth Side Floor
		AEGfxTextureSet(pSideRightFloorTex, 0, 0);
		for (int i = 0; i < 10; i++)
		{
			if (m_RightFourthSideFloor[i].m_IsRender)
			{
				AEGfxSetTransform(m_RightFourthSideFloor[i].m_TransformFloorCurr.m);
				AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
			}
		}

		//Left Side Floor
		AEGfxTextureSet(pSideLeftFloorTex, 0, 0);
		for (int i = 0; i < 10; i++)
		{
			if (m_LeftSideFloor[i].m_IsRender)
			{
				AEGfxSetTransform(m_LeftSideFloor[i].m_TransformFloorCurr.m);
				AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
			}
		}


		//Left Second Side Floor
		AEGfxTextureSet(pSideLeftFloorTex, 0, 0);
		for (int i = 0; i < 10; i++)
		{
			if (m_LeftSecondSideFloor[i].m_IsRender)
			{
				AEGfxSetTransform(m_LeftSecondSideFloor[i].m_TransformFloorCurr.m);
				AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
			}
		}

		//Left Third Side Floor
		AEGfxTextureSet(pSideLeftFloorTex, 0, 0);
		for (int i = 0; i < 10; i++)
		{
			if (m_LeftThirdSideFloor[i].m_IsRender)
			{
				AEGfxSetTransform(m_LeftThirdSideFloor[i].m_TransformFloorCurr.m);
				AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
			}
		}

		//Left Fourth Side Floor
		AEGfxTextureSet(pSideLeftFloorTex, 0, 0);
		for (int i = 0; i < 10; i++)
		{
			if (m_LeftFourthSideFloor[i].m_IsRender)
			{
				AEGfxSetTransform(m_LeftFourthSideFloor[i].m_TransformFloorCurr.m);
				AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
			}
		}


		AEGfxTextureSet(pFogTex, 0, 0);
		AEGfxSetTransform(m_TransformFogData.m);
		AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);

	}



	
	//AEGfxMeshDraw(pMesh, AE_GFX_MDM_LINES_STRIP);
}

void SceneSplashScreen::Exit()
{
	std::cout << "Exiting Scene SplashScreen" << std::endl;

	AEGfxMeshFree(pMesh);
	AEGfxTextureUnload(pFloorTex);
	AEGfxTextureUnload(pSideRightFloorTex);
	AEGfxTextureUnload(pSideLeftFloorTex);
	AEGfxTextureUnload(pSkyTex);
	AEGfxTextureUnload(pSunOverlayTex);
	AEGfxTextureUnload(pFogTex);
	AEGfxTextureUnload(pDigipenLogoTex);
}

//2D Camera Movement - Screen Shake
//AEGfxSetColorToMultiply(0.0f, 0.0f, 0.0f, 0.0f);
//static float x = 0, y = 0;
//x += 0.3f;
//y += 0.3f;
//AEGfxSetCamPosition(x, y);

/**********************************************************/
//Calculation Profiler
//static int x = 580.0f, y = 194.f;
//if (AEInputCheckCurr(AEVK_W))
//{
//	y++;
//}
//if (AEInputCheckCurr(AEVK_S))
//{
//	y--;
//}
//if (AEInputCheckCurr(AEVK_A))
//{
//	x--;
//}
//if (AEInputCheckCurr(AEVK_D))
//{
//	x++;
//}
//static int mx = 0, my = 47;
//if (AEInputCheckCurr(AEVK_UP))
//{
//	my++;
//}
//if (AEInputCheckCurr(AEVK_DOWN))
//{
//	my--;
//}

//for (int i = 1; i < 9; i++)
//{
//	if (m_Floor[i].m_currFloorNum != 0)
//	{
//		m_Floor[i].m_TransformFloorCurr.m[0][0] = (m_Floor[m_Floor[i].m_currFloorNum - 1].m_TransformFloorData.m[0][0] - m_Floor[m_Floor[i].m_currFloorNum].m_TransformFloorData.m[0][0]) * (m_Floor[i].m_currFloorTimer / m_FloorSpeedTimer) + m_Floor[m_Floor[i].m_currFloorNum].m_TransformFloorData.m[0][0];
//		m_Floor[i].m_TransformFloorCurr.m[0][1] = (m_Floor[m_Floor[i].m_currFloorNum - 1].m_TransformFloorData.m[0][1] - m_Floor[m_Floor[i].m_currFloorNum].m_TransformFloorData.m[0][1]) * (m_Floor[i].m_currFloorTimer / m_FloorSpeedTimer) + m_Floor[m_Floor[i].m_currFloorNum].m_TransformFloorData.m[0][1];
//		m_Floor[i].m_TransformFloorCurr.m[0][2] = (m_Floor[m_Floor[i].m_currFloorNum - 1].m_TransformFloorData.m[0][2] - m_Floor[m_Floor[i].m_currFloorNum].m_TransformFloorData.m[0][2]) * (m_Floor[i].m_currFloorTimer / m_FloorSpeedTimer) + m_Floor[m_Floor[i].m_currFloorNum].m_TransformFloorData.m[0][2];
//		m_Floor[i].m_TransformFloorCurr.m[1][0] = (m_Floor[m_Floor[i].m_currFloorNum - 1].m_TransformFloorData.m[1][0] - m_Floor[m_Floor[i].m_currFloorNum].m_TransformFloorData.m[1][0]) * (m_Floor[i].m_currFloorTimer / m_FloorSpeedTimer) + m_Floor[m_Floor[i].m_currFloorNum].m_TransformFloorData.m[1][0];
//		m_Floor[i].m_TransformFloorCurr.m[1][1] = (m_Floor[m_Floor[i].m_currFloorNum - 1].m_TransformFloorData.m[1][1] - m_Floor[m_Floor[i].m_currFloorNum].m_TransformFloorData.m[1][1]) * (m_Floor[i].m_currFloorTimer / m_FloorSpeedTimer) + m_Floor[m_Floor[i].m_currFloorNum].m_TransformFloorData.m[1][1];
//		m_Floor[i].m_TransformFloorCurr.m[1][2] = (m_Floor[m_Floor[i].m_currFloorNum - 1].m_TransformFloorData.m[1][2] - m_Floor[m_Floor[i].m_currFloorNum].m_TransformFloorData.m[1][2]) * (m_Floor[i].m_currFloorTimer / m_FloorSpeedTimer) + m_Floor[m_Floor[i].m_currFloorNum].m_TransformFloorData.m[1][2];
//		m_Floor[i].m_TransformFloorCurr.m[2][0] = (m_Floor[m_Floor[i].m_currFloorNum - 1].m_TransformFloorData.m[2][0] - m_Floor[m_Floor[i].m_currFloorNum].m_TransformFloorData.m[2][0]) * (m_Floor[i].m_currFloorTimer / m_FloorSpeedTimer) + m_Floor[m_Floor[i].m_currFloorNum].m_TransformFloorData.m[2][0];
//		m_Floor[i].m_TransformFloorCurr.m[2][1] = (m_Floor[m_Floor[i].m_currFloorNum - 1].m_TransformFloorData.m[2][1] - m_Floor[m_Floor[i].m_currFloorNum].m_TransformFloorData.m[2][1]) * (m_Floor[i].m_currFloorTimer / m_FloorSpeedTimer) + m_Floor[m_Floor[i].m_currFloorNum].m_TransformFloorData.m[2][1];
//		m_Floor[i].m_TransformFloorCurr.m[2][2] = (m_Floor[m_Floor[i].m_currFloorNum - 1].m_TransformFloorData.m[2][2] - m_Floor[m_Floor[i].m_currFloorNum].m_TransformFloorData.m[2][2]) * (m_Floor[i].m_currFloorTimer / m_FloorSpeedTimer) + m_Floor[m_Floor[i].m_currFloorNum].m_TransformFloorData.m[2][2];
//	}
//	else
//	{
//		m_Floor[i].m_TransformFloorCurr.m[0][0] = m_Floor[6].m_TransformFloorData.m[0][0];
//		m_Floor[i].m_TransformFloorCurr.m[0][1] = m_Floor[6].m_TransformFloorData.m[0][1];
//		m_Floor[i].m_TransformFloorCurr.m[0][2] = m_Floor[6].m_TransformFloorData.m[0][2];
//		m_Floor[i].m_TransformFloorCurr.m[1][0] = m_Floor[6].m_TransformFloorData.m[1][0];
//		m_Floor[i].m_TransformFloorCurr.m[1][1] = m_Floor[6].m_TransformFloorData.m[1][1];
//		m_Floor[i].m_TransformFloorCurr.m[1][2] = m_Floor[6].m_TransformFloorData.m[1][2];
//		m_Floor[i].m_TransformFloorCurr.m[2][0] = m_Floor[6].m_TransformFloorData.m[2][0];
//		m_Floor[i].m_TransformFloorCurr.m[2][1] = m_Floor[6].m_TransformFloorData.m[2][1];
//		m_Floor[i].m_TransformFloorCurr.m[2][2] = m_Floor[6].m_TransformFloorData.m[2][2];
//
//	}
//	//Change CurrFloorNum
//	if (m_Floor[i].m_currFloorTimer > m_FloorSpeedTimer)
//	{
//		//m_Floor[i].m_currFloorTimer = 0.0;
//		m_Floor[i].m_currFloorTimer = m_FloorSpeedTimer;
//		//if (m_Floor[i].m_currFloorNum != 0)
//		//	m_Floor[i].m_currFloorNum--;
//		//else
//		//	m_Floor[i].m_currFloorNum = 6; //Loop to the top
//	}
//	else
//		m_Floor[i].m_currFloorTimer += dt;
//
//	//cout << (m_Floor[1].m_TransformFloorData.m[0][0] - m_Floor[2].m_TransformFloorData.m[0][0]) / (m_FloorSpeedTimer / dt) << endl;
//}




//AEMtx33 m_NextNextFloorData = m_Floor[m_Floor[i].m_currFloorNum - 2].m_TransformFloorData;
			//AEMtx33 m_NextFloorData = m_Floor[m_Floor[i].m_currFloorNum - 1].m_TransformFloorData;
			//AEMtx33 m_CurrFloorData = m_Floor[m_Floor[i].m_currFloorNum].m_TransformFloorData;
			////Add its speed towards the next floor transition speed
			//m_Floor[i].m_currFloorSpeed += m_Floor[i].m_currFloorSpeed - (m_NextFloorData.m[0][0] - m_CurrFloorData.m[0][0]) / 100 ? ;
			//if (!((m_NextFloorData.m[0][0] - m_Floor[i].m_TransformFloorCurr.m[0][0]) < 1.0f &&
			//	(m_NextFloorData.m[1][0] - m_Floor[i].m_TransformFloorCurr.m[1][0]) < 1.0f &&
			//	(m_NextFloorData.m[2][0] - m_Floor[i].m_TransformFloorCurr.m[2][0]) < 1.0f &&
			//	(m_NextFloorData.m[0][1] - m_Floor[i].m_TransformFloorCurr.m[0][1]) < 1.0f &&
			//	(m_NextFloorData.m[1][1] - m_Floor[i].m_TransformFloorCurr.m[1][1]) < 1.0f &&
			//	(m_NextFloorData.m[2][1] - m_Floor[i].m_TransformFloorCurr.m[2][1]) < 1.0f &&
			//	(m_NextFloorData.m[0][2] - m_Floor[i].m_TransformFloorCurr.m[0][2]) < 1.0f &&
			//	(m_NextFloorData.m[1][2] - m_Floor[i].m_TransformFloorCurr.m[1][2]) < 1.0f &&
			//	(m_NextFloorData.m[2][2] - m_Floor[i].m_TransformFloorCurr.m[2][2]) < 1.0f))
			//{

			//	/*if (m_Floor[i].m_currFloorNum != 0)
			//	{
			//		m_Floor[i].m_TransformFloorCurr.m[0][0] += (m_NextFloorData.m[0][0] - m_CurrFloorData.m[0][0]) / 100;
			//		m_Floor[i].m_TransformFloorCurr.m[0][1] += (m_NextFloorData.m[0][1] - m_CurrFloorData.m[0][1]) / 100;
			//		m_Floor[i].m_TransformFloorCurr.m[0][2] += (m_NextFloorData.m[0][2] - m_CurrFloorData.m[0][2]) / 100;
			//		m_Floor[i].m_TransformFloorCurr.m[1][0] += (m_NextFloorData.m[1][0] - m_CurrFloorData.m[1][0]) / 100;
			//		m_Floor[i].m_TransformFloorCurr.m[1][1] += (m_NextFloorData.m[1][1] - m_CurrFloorData.m[1][1]) / 100;
			//		m_Floor[i].m_TransformFloorCurr.m[1][2] += (m_NextFloorData.m[1][2] - m_CurrFloorData.m[1][2]) / 100;
			//		m_Floor[i].m_TransformFloorCurr.m[2][0] += (m_NextFloorData.m[2][0] - m_CurrFloorData.m[2][0]) / 100;
			//		m_Floor[i].m_TransformFloorCurr.m[2][1] += (m_NextFloorData.m[2][1] - m_CurrFloorData.m[2][1]) / 100;
			//		m_Floor[i].m_TransformFloorCurr.m[2][2] += (m_NextFloorData.m[2][2] - m_CurrFloorData.m[2][2]) / 100;
			//	}*/
			//	if (m_Floor[i].m_currFloorNum != 0)
			//	{
			//		m_Floor[i].m_TransformFloorCurr.m[0][0] += (m_NextFloorData.m[0][0] - m_CurrFloorData.m[0][0]) / 100;
			//		m_Floor[i].m_TransformFloorCurr.m[0][1] += (m_NextFloorData.m[0][1] - m_CurrFloorData.m[0][1]) / 100;
			//		m_Floor[i].m_TransformFloorCurr.m[0][2] += (m_NextFloorData.m[0][2] - m_CurrFloorData.m[0][2]) / 100;
			//		m_Floor[i].m_TransformFloorCurr.m[1][0] += (m_NextFloorData.m[1][0] - m_CurrFloorData.m[1][0]) / 100;
			//		m_Floor[i].m_TransformFloorCurr.m[1][1] += (m_NextFloorData.m[1][1] - m_CurrFloorData.m[1][1]) / 100;
			//		m_Floor[i].m_TransformFloorCurr.m[1][2] += (m_NextFloorData.m[1][2] - m_CurrFloorData.m[1][2]) / 100;
			//		m_Floor[i].m_TransformFloorCurr.m[2][0] += (m_NextFloorData.m[2][0] - m_CurrFloorData.m[2][0]) / 100;
			//		m_Floor[i].m_TransformFloorCurr.m[2][1] += (m_NextFloorData.m[2][1] - m_CurrFloorData.m[2][1]) / 100;
			//		m_Floor[i].m_TransformFloorCurr.m[2][2] += (m_NextFloorData.m[2][2] - m_CurrFloorData.m[2][2]) / 100;
			//	}
			//	else
			//	{
			//		m_Floor[i].m_TransformFloorCurr.m[0][0] = m_LastFloorData.m[0][0];
			//		m_Floor[i].m_TransformFloorCurr.m[0][1] = m_LastFloorData.m[0][1];
			//		m_Floor[i].m_TransformFloorCurr.m[0][2] = m_LastFloorData.m[0][2];
			//		m_Floor[i].m_TransformFloorCurr.m[1][0] = m_LastFloorData.m[1][0];
			//		m_Floor[i].m_TransformFloorCurr.m[1][1] = m_LastFloorData.m[1][1];
			//		m_Floor[i].m_TransformFloorCurr.m[1][2] = m_LastFloorData.m[1][2];
			//		m_Floor[i].m_TransformFloorCurr.m[2][0] = m_LastFloorData.m[2][0];
			//		m_Floor[i].m_TransformFloorCurr.m[2][1] = m_LastFloorData.m[2][1];
			//		m_Floor[i].m_TransformFloorCurr.m[2][2] = m_LastFloorData.m[2][2];

			//	}