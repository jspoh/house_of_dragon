#include "SceneSplashScreen.h"

#include "../Backend/GameManager.h" //? Still thinking
#include "SceneManager.h"
#include <iostream> //Remove upon test finish

SceneSplashScreen* SceneSplashScreen::sInstance = new SceneSplashScreen(SceneManager::GetInstance());
AEGfxVertexList* pMesh = 0;
AEGfxTexture* pFloorTex;
AEGfxTexture* pSideLeftFloorTex;
AEGfxTexture* pSideRightFloorTex;

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
Floor m_Floor[10], m_RightSideFloor[10], m_LeftSideFloor[10];
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
	AEMtx33Trans(&trans, 0, -2829);
	AEMtx33Concat(&m_RightSideFloor[0].m_TransformFloorData, &trans, &scale);
	m_RightSideFloor[0].m_currFloorNum = 0;
	AEMtx33Scale(&scale, 7000.f, 1262.f);
	AEMtx33Trans(&trans, 0, -2229);
	AEMtx33Concat(&m_RightSideFloor[1].m_TransformFloorData, &trans, &scale);
	m_RightSideFloor[1].m_currFloorNum = 1;
	AEMtx33Scale(&scale, 6000.f, 1262.f);
	AEMtx33Trans(&trans, 0, -1629);
	AEMtx33Concat(&m_RightSideFloor[2].m_TransformFloorData, &trans, &scale);
	m_RightSideFloor[2].m_currFloorNum = 2;
	//First floor
	AEMtx33Scale(&scale, 2940.f, 616.f);
	AEMtx33Trans(&trans, 0, -696);
	AEMtx33Concat(&m_RightSideFloor[3].m_TransformFloorData, &trans, &scale);
	m_RightSideFloor[3].m_currFloorNum = 3;
	//Second floor
	AEMtx33Scale(&scale, 1593.0f, 339.f);
	AEMtx33Trans(&trans, 0, -282);
	AEMtx33Concat(&m_RightSideFloor[4].m_TransformFloorData, &trans, &scale);
	m_RightSideFloor[4].m_currFloorNum = 4;
	//Third floor
	AEMtx33Scale(&scale, 779.0f, 133.f);
	AEMtx33Trans(&trans, 0, -50);
	AEMtx33Concat(&m_RightSideFloor[5].m_TransformFloorData, &trans, &scale);
	m_RightSideFloor[5].m_currFloorNum = 5;
	//Fourth floor
	AEMtx33Scale(&scale, 381.0f, 47.f);
	AEMtx33Trans(&trans, 0, 39);
	AEMtx33Concat(&m_RightSideFloor[6].m_TransformFloorData, &trans, &scale);
	m_RightSideFloor[6].m_currFloorNum = 6;
	//Fifth floor
	AEMtx33Scale(&scale, 181.0f, 14.f);
	AEMtx33Trans(&trans, 0, 69);
	AEMtx33Concat(&m_RightSideFloor[7].m_TransformFloorData, &trans, &scale);
	m_RightSideFloor[7].m_currFloorNum = 7;
	//Sixth floor
	AEMtx33Scale(&scale, 85.0f, 4.f);
	AEMtx33Trans(&trans, 0, 78);
	AEMtx33Concat(&m_RightSideFloor[8].m_TransformFloorData, &trans, &scale);
	m_RightSideFloor[8].m_currFloorNum = 8;
	//Seventh floor
	AEMtx33Scale(&scale, 33.0f, 1.f);
	AEMtx33Trans(&trans, 0, 80);
	AEMtx33Concat(&m_RightSideFloor[9].m_TransformFloorData, &trans, &scale);
	m_RightSideFloor[9].m_currFloorNum = 9;
	
	for (int i = 0; i < 10; i++)
		m_RightSideFloor[i].m_TransformFloorCurr = m_RightSideFloor[i].m_TransformFloorData;

	/*******************************************************************************/
	//LEFT SIDE FLOORS
	//Out of Screen Floor
	AEMtx33Scale(&scale, 8000.f, 1262.f);
	AEMtx33Trans(&trans, 0, -2829);
	AEMtx33Concat(&m_LeftSideFloor[0].m_TransformFloorData, &trans, &scale);
	m_LeftSideFloor[0].m_currFloorNum = 0;
	AEMtx33Scale(&scale, 7000.f, 1262.f);
	AEMtx33Trans(&trans, 0, -2229);
	AEMtx33Concat(&m_LeftSideFloor[1].m_TransformFloorData, &trans, &scale);
	m_LeftSideFloor[1].m_currFloorNum = 1;
	AEMtx33Scale(&scale, 6000.f, 1262.f);
	AEMtx33Trans(&trans, 0, -1629);
	AEMtx33Concat(&m_LeftSideFloor[2].m_TransformFloorData, &trans, &scale);
	m_LeftSideFloor[2].m_currFloorNum = 2;
	//First floor
	AEMtx33Scale(&scale, 2940.f, 616.f);
	AEMtx33Trans(&trans, 0, -696);
	AEMtx33Concat(&m_LeftSideFloor[3].m_TransformFloorData, &trans, &scale);
	m_LeftSideFloor[3].m_currFloorNum = 3;
	//Second floor
	AEMtx33Scale(&scale, 1593.0f, 339.f);
	AEMtx33Trans(&trans, 0, -282);
	AEMtx33Concat(&m_LeftSideFloor[4].m_TransformFloorData, &trans, &scale);
	m_LeftSideFloor[4].m_currFloorNum = 4;
	//Third floor
	AEMtx33Scale(&scale, 779.0f, 133.f);
	AEMtx33Trans(&trans, 0, -50);
	AEMtx33Concat(&m_LeftSideFloor[5].m_TransformFloorData, &trans, &scale);
	m_LeftSideFloor[5].m_currFloorNum = 5;
	//Fourth floor
	AEMtx33Scale(&scale, 381.0f, 47.f);
	AEMtx33Trans(&trans, 0, 39);
	AEMtx33Concat(&m_LeftSideFloor[6].m_TransformFloorData, &trans, &scale);
	m_LeftSideFloor[6].m_currFloorNum = 6;
	//Fifth floor
	AEMtx33Scale(&scale, 181.0f, 14.f);
	AEMtx33Trans(&trans, 0, 69);
	AEMtx33Concat(&m_LeftSideFloor[7].m_TransformFloorData, &trans, &scale);
	m_LeftSideFloor[7].m_currFloorNum = 7;
	//Sixth floor
	AEMtx33Scale(&scale, 85.0f, 4.f);
	AEMtx33Trans(&trans, 0, 78);
	AEMtx33Concat(&m_LeftSideFloor[8].m_TransformFloorData, &trans, &scale);
	m_LeftSideFloor[8].m_currFloorNum = 8;
	//Seventh floor
	AEMtx33Scale(&scale, 33.0f, 1.f);
	AEMtx33Trans(&trans, 0, 80);
	AEMtx33Concat(&m_LeftSideFloor[9].m_TransformFloorData, &trans, &scale);
	m_LeftSideFloor[9].m_currFloorNum = 9;

	for (int i = 0; i < 10; i++)
		m_LeftSideFloor[i].m_TransformFloorCurr = m_LeftSideFloor[i].m_TransformFloorData;
}

void SceneSplashScreen::Update(double dt)
{
	std::cout << "Updating Scene SplashScreen" << std::endl;
	if (AEInputCheckTriggered(AEVK_3))
		SceneManager::GetInstance()->SetActiveScene("SceneBase");

//	static int x = 4362.f, y = 1200.f;
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
//static int mx = 0, my = -300;
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
//AEMtx33Concat(&m_Floor[0].m_TransformFloorCurr, &trans, &scale);
//AEMtx33Concat(&m_Floor[0].m_TransformFloorData, &trans, &scale);
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

	//Main Floor
	AEGfxTextureSet(pFloorTex, 0, 0);
	for (int i = 0; i < 9; i++)
	{
		if (m_Floor[i].m_IsRender)
		{
			AEGfxSetTransform(m_Floor[i].m_TransformFloorCurr.m);
			AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
		}
	}

	//Right Side Floor
	AEGfxTextureSet(pSideRightFloorTex, 0, 0);
	for (int i = 0; i < 9; i++)
	{
		if (m_RightSideFloor[i].m_IsRender)
		{
			AEGfxSetTransform(m_RightSideFloor[i].m_TransformFloorCurr.m);
			AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
		}
	}

	//Left Side Floor
	AEGfxTextureSet(pSideLeftFloorTex, 0, 0);
	for (int i = 0; i < 9; i++)
	{
		if (m_LeftSideFloor[i].m_IsRender)
		{
			AEGfxSetTransform(m_LeftSideFloor[i].m_TransformFloorCurr.m);
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