#include "SceneSplashScreen.h"

#include "../Backend/GameManager.h" //? Still thinking
#include "SceneManager.h"
#include <iostream> //Remove upon test finish

SceneSplashScreen* SceneSplashScreen::sInstance = new SceneSplashScreen(SceneManager::GetInstance());
AEGfxVertexList* pMesh = 0;
AEGfxTexture* pFloorTex;
AEGfxTexture* pSideLeftFloorTex;
AEGfxTexture* pSideRightFloorTex;
AEGfxTexture* pSkyTex;
AEGfxTexture* pSunOverlayTex;

struct Floor
{
	AEMtx33 m_TransformFloorData;
	AEMtx33 m_TransformFloorCurr;
	int m_currFloorNum = 0;
	double m_currFloorTimer = 0;
	double m_FloorSpeedTimer = 0.5;
	AEMtx33 m_currFloorSpeed={0};
	bool m_IsRender = true;
};
Floor m_Floor[10], m_RightSideFloor[10], m_RightSecondSideFloor[10], m_LeftSideFloor[10], m_LeftSecondSideFloor[10];
AEMtx33 m_TransformSkyData, m_TransformSunData, m_TransformSunOverlayData;
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


	pFloorTex = AEGfxTextureLoad("Assets/Scene_Floor_Grass_3D.png");
	pSideRightFloorTex = AEGfxTextureLoad("Assets/Scene_FloorSideRight_Sand_3D.png");
	pSideLeftFloorTex = AEGfxTextureLoad("Assets/Scene_FloorSideLeft_Sand_3D.png");
	pSkyTex = AEGfxTextureLoad("Assets/Scene_Sky_Clear.png");
	pSunOverlayTex = AEGfxTextureLoad("Assets/Scene_Sun_Overlaylighting.png");
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
	AEMtx33Trans(&trans, 2312, -282);
	AEMtx33Concat(&m_RightSecondSideFloor[4].m_TransformFloorData, &trans, &scale);
	m_RightSecondSideFloor[4].m_currFloorNum = 4;
	//Third floor
	AEMtx33Scale(&scale, 779.0f, 133.f);
	AEMtx33Trans(&trans, 1109, -50);
	AEMtx33Concat(&m_RightSecondSideFloor[5].m_TransformFloorData, &trans, &scale);
	m_RightSecondSideFloor[5].m_currFloorNum = 5;
	//Fourth floor
	AEMtx33Scale(&scale, 381.0f, 47.f);
	AEMtx33Trans(&trans, 549, 39);
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
	AEMtx33Trans(&trans, -16000, -2829);
	AEMtx33Concat(&m_LeftSecondSideFloor[0].m_TransformFloorData, &trans, &scale);
	m_LeftSecondSideFloor[0].m_currFloorNum = 0;
	AEMtx33Scale(&scale, 7000.f, 1262.f);
	AEMtx33Trans(&trans, -5750, -2229);
	AEMtx33Concat(&m_LeftSecondSideFloor[1].m_TransformFloorData, &trans, &scale);
	m_LeftSecondSideFloor[1].m_currFloorNum = 1;
	AEMtx33Scale(&scale, 6000.f, 1262.f);
	AEMtx33Trans(&trans, -4350, -1629);
	AEMtx33Concat(&m_LeftSecondSideFloor[2].m_TransformFloorData, &trans, &scale);
	m_LeftSecondSideFloor[2].m_currFloorNum = 2;
	//First floor
	AEMtx33Scale(&scale, 2940.f, 616.f);
	AEMtx33Trans(&trans, -2150, -696);
	AEMtx33Concat(&m_LeftSecondSideFloor[3].m_TransformFloorData, &trans, &scale);
	m_LeftSecondSideFloor[3].m_currFloorNum = 3;
	//Second floor
	AEMtx33Scale(&scale, 1593.0f, 339.f);
	AEMtx33Trans(&trans, -1158, -282);
	AEMtx33Concat(&m_LeftSecondSideFloor[4].m_TransformFloorData, &trans, &scale);
	m_LeftSecondSideFloor[4].m_currFloorNum = 4;
	//Third floor
	AEMtx33Scale(&scale, 779.0f, 133.f);
	AEMtx33Trans(&trans, -556, -50);
	AEMtx33Concat(&m_LeftSecondSideFloor[5].m_TransformFloorData, &trans, &scale);
	m_LeftSecondSideFloor[5].m_currFloorNum = 5;
	//Fourth floor
	AEMtx33Scale(&scale, 381.0f, 47.f);
	AEMtx33Trans(&trans, -276, 39);
	AEMtx33Concat(&m_LeftSecondSideFloor[6].m_TransformFloorData, &trans, &scale);
	m_LeftSecondSideFloor[6].m_currFloorNum = 6;
	//Fifth floor
	AEMtx33Scale(&scale, 181.0f, 14.f);
	AEMtx33Trans(&trans, -131, 69);
	AEMtx33Concat(&m_LeftSecondSideFloor[7].m_TransformFloorData, &trans, &scale);
	m_LeftSecondSideFloor[7].m_currFloorNum = 7;
	//Sixth floor
	AEMtx33Scale(&scale, 85.0f, 4.f);
	AEMtx33Trans(&trans, -62, 78);
	AEMtx33Concat(&m_LeftSecondSideFloor[8].m_TransformFloorData, &trans, &scale);
	m_LeftSecondSideFloor[8].m_currFloorNum = 8;
	//Seventh floor
	AEMtx33Scale(&scale, 33.0f, 1.f);
	AEMtx33Trans(&trans, -25, 80);
	AEMtx33Concat(&m_LeftSecondSideFloor[9].m_TransformFloorData, &trans, &scale);
	m_LeftSecondSideFloor[9].m_currFloorNum = 9;

	for (int i = 0; i < 10; i++)
		m_LeftSecondSideFloor[i].m_TransformFloorCurr = m_LeftSecondSideFloor[i].m_TransformFloorData;

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
	
}

void SceneSplashScreen::Update(double dt)
{
	std::cout << "Updating Scene SplashScreen" << std::endl;
	if (AEInputCheckTriggered(AEVK_3))
		SceneManager::GetInstance()->SetActiveScene("SceneBase");

//	static int x = 181.0f, y = 14.f;
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
//static int mx = 131, my = 69;
//if (AEInputCheckCurr(AEVK_UP))
//{
//	mx++;
//}
//if (AEInputCheckCurr(AEVK_DOWN))
//{
//	mx--;
//}
//AEMtx33 scale = { 0 }, trans = { 0 };
//AEMtx33Scale(&scale, x, y);
//AEMtx33Trans(&trans, mx, my);
//AEMtx33Concat(&m_RightSecondSideFloor[8].m_TransformFloorCurr, &trans, &scale);
//AEMtx33Concat(&m_RightSecondSideFloor[8].m_TransformFloorData, &trans, &scale);
//cout << mx << endl;
	///////////////////////////////////////////////////////////////////////////
	//UPDATE FLOOR MOVEMENT
	//////////////////////////////////////////////////////////////////////////
	static bool start = false;
	start = AEInputCheckReleased(AEVK_SPACE)? !start: start;
	
	if (start)
	{
		AEMtx33 m_LastFloorData = m_Floor[8].m_TransformFloorData;
		for (int i = 9; i > -1; i--)
		{
			AEMtx33 m_NextFloorData = m_Floor[i].m_currFloorNum != 0 ? m_Floor[m_Floor[i].m_currFloorNum - 1].m_TransformFloorData : m_Floor[i].m_TransformFloorCurr = m_LastFloorData;
			AEMtx33 m_CurrFloorData = m_Floor[m_Floor[i].m_currFloorNum].m_TransformFloorData;

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
	}
}


void SceneSplashScreen::Render()
{
    // Set the background to black.
	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);

	////////////////////////////////////////////////////////////////////////////////////////
	// Floors
	// //////////////////////////////////////////////////////////////////////////////////
	// Tell the engine to get ready to draw something with texture.
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);

	// Set the the color to multiply to white, so that the sprite can 
	// display the full range of colors (default is black).
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);

	// Set the color to add to nothing, so that we don't alter the sprite's color
	AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 1.0f);

	// Set blend mode to AE_GFX_BM_BLEND
	// This will allow transparency.
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
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