#include "SceneSplashScreen.h"

#include "../Backend/GameManager.h" //? Still thinking
#include "SceneManager.h"
#include <iostream> //Remove upon test finish

SceneSplashScreen* SceneSplashScreen::sInstance = new SceneSplashScreen(SceneManager::GetInstance());
AEGfxVertexList* pMesh = 0;
AEGfxTexture* pTex;
struct Floor
{
	AEMtx33 m_TransformFloorData;
	AEMtx33 m_TransformFloorCurr;
	int m_currFloorNum = 0;
	double m_currFloorTimer = 0.0;
};
Floor m_Floor[8];
double m_FloorSpeedTimer = 2.0;

float Linear(float start, float end, float value)
{
	return (1.f - value) * start + value * end;
}

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
	pTex = AEGfxTextureLoad("Assets/Scene_Floor_Grass_3D.png");

	/*******************************************************************************/
	//Setting the trasnform for the floor
	AEMtx33 scale = { 0 }, trans = { 0 };
	//Out of Screen Floor
	AEMtx33Scale(&scale, 4362.f, 1200.f);
	AEMtx33Trans(&trans, 0, -1300);
	AEMtx33Concat(&m_Floor[0].m_TransformFloorData, &trans, &scale);
	m_Floor[0].m_currFloorNum = 0;
	//First floor
	AEMtx33Scale(&scale, 2940.f, 616.f);
	AEMtx33Trans(&trans, 0, -696);
	AEMtx33Concat(&m_Floor[1].m_TransformFloorData, &trans, &scale);
	m_Floor[1].m_currFloorNum = 1;
	//Second floor
	AEMtx33Scale(&scale, 1593.0f, 339.f);
	AEMtx33Trans(&trans, 0, -282);
	AEMtx33Concat(&m_Floor[2].m_TransformFloorData, &trans, &scale);
	m_Floor[2].m_currFloorNum = 2;
	//Third floor
	AEMtx33Scale(&scale, 779.0f, 133.f);
	AEMtx33Trans(&trans, 0, -50);
	AEMtx33Concat(&m_Floor[3].m_TransformFloorData, &trans, &scale);
	m_Floor[3].m_currFloorNum = 3;
	//Fourth floor
	AEMtx33Scale(&scale, 381.0f, 47.f);
	AEMtx33Trans(&trans, 0, 39);
	AEMtx33Concat(&m_Floor[4].m_TransformFloorData, &trans, &scale);
	m_Floor[4].m_currFloorNum = 4;
	//Fifth floor
	AEMtx33Scale(&scale, 181.0f, 14.f);
	AEMtx33Trans(&trans, 0, 69);
	AEMtx33Concat(&m_Floor[5].m_TransformFloorData, &trans, &scale);
	m_Floor[5].m_currFloorNum = 5;
	//Sixth floor
	AEMtx33Scale(&scale, 85.0f, 4.f);
	AEMtx33Trans(&trans, 0, 78);
	AEMtx33Concat(&m_Floor[6].m_TransformFloorData, &trans, &scale);
	m_Floor[6].m_currFloorNum = 6;
	//Seventh floor
	AEMtx33Scale(&scale, 33.0f, 1.f);
	AEMtx33Trans(&trans, 0, 80);
	AEMtx33Concat(&m_Floor[7].m_TransformFloorData, &trans, &scale);
	m_Floor[7].m_currFloorNum = 7;

	for (int i = 0; i < 8; i++)
		m_Floor[i].m_TransformFloorCurr = m_Floor[i].m_TransformFloorData;
	
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

//	static int x = 2940.f, y = 616.f;
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
//static int mx = 0, my = -696;
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
	if (AEInputCheckReleased(AEVK_SPACE)) {
		start = true;
	}

	if (start)
	{
		AEMtx33 m_LastFloorData = m_Floor[6].m_TransformFloorData;
		for (int i = 1; i < 9; i++)
		{
			AEMtx33 m_NextFloorData = m_Floor[m_Floor[i].m_currFloorNum - 1].m_TransformFloorData;
			AEMtx33 m_CurrFloorData = m_Floor[m_Floor[i].m_currFloorNum].m_TransformFloorData;

			if (!((m_NextFloorData.m[0][0] - m_Floor[i].m_TransformFloorCurr.m[0][0]) < 5.0f &&
				(m_NextFloorData.m[1][0] - m_Floor[i].m_TransformFloorCurr.m[1][0]) < 5.0f &&
				(m_NextFloorData.m[2][0] - m_Floor[i].m_TransformFloorCurr.m[2][0]) < 5.0f &&
				(m_NextFloorData.m[0][1] - m_Floor[i].m_TransformFloorCurr.m[0][1]) < 5.0f &&
				(m_NextFloorData.m[1][1] - m_Floor[i].m_TransformFloorCurr.m[1][1]) < 5.0f &&
				(m_NextFloorData.m[2][1] - m_Floor[i].m_TransformFloorCurr.m[2][1]) < 5.0f &&
				(m_NextFloorData.m[0][2] - m_Floor[i].m_TransformFloorCurr.m[0][2]) < 5.0f &&
				(m_NextFloorData.m[1][2] - m_Floor[i].m_TransformFloorCurr.m[1][2]) < 5.0f &&
				(m_NextFloorData.m[2][2] - m_Floor[i].m_TransformFloorCurr.m[2][2]) < 5.0f))
			{

				if (m_Floor[i].m_currFloorNum != 0)
				{
					m_Floor[i].m_TransformFloorCurr.m[0][0] += (m_NextFloorData.m[0][0] - m_CurrFloorData.m[0][0]) / 100;
					m_Floor[i].m_TransformFloorCurr.m[0][1] += (m_NextFloorData.m[0][1] - m_CurrFloorData.m[0][1]) / 100;
					m_Floor[i].m_TransformFloorCurr.m[0][2] += (m_NextFloorData.m[0][2] - m_CurrFloorData.m[0][2]) / 100;
					m_Floor[i].m_TransformFloorCurr.m[1][0] += (m_NextFloorData.m[1][0] - m_CurrFloorData.m[1][0]) / 100;
					m_Floor[i].m_TransformFloorCurr.m[1][1] += (m_NextFloorData.m[1][1] - m_CurrFloorData.m[1][1]) / 100;
					m_Floor[i].m_TransformFloorCurr.m[1][2] += (m_NextFloorData.m[1][2] - m_CurrFloorData.m[1][2]) / 100;
					m_Floor[i].m_TransformFloorCurr.m[2][0] += (m_NextFloorData.m[2][0] - m_CurrFloorData.m[2][0]) / 100;
					m_Floor[i].m_TransformFloorCurr.m[2][1] += (m_NextFloorData.m[2][1] - m_CurrFloorData.m[2][1]) / 100;
					m_Floor[i].m_TransformFloorCurr.m[2][2] += (m_NextFloorData.m[2][2] - m_CurrFloorData.m[2][2]) / 100;
				}
				else
				{
					m_Floor[i].m_TransformFloorCurr.m[0][0] = m_LastFloorData.m[0][0];
					m_Floor[i].m_TransformFloorCurr.m[0][1] = m_LastFloorData.m[0][1];
					m_Floor[i].m_TransformFloorCurr.m[0][2] = m_LastFloorData.m[0][2];
					m_Floor[i].m_TransformFloorCurr.m[1][0] = m_LastFloorData.m[1][0];
					m_Floor[i].m_TransformFloorCurr.m[1][1] = m_LastFloorData.m[1][1];
					m_Floor[i].m_TransformFloorCurr.m[1][2] = m_LastFloorData.m[1][2];
					m_Floor[i].m_TransformFloorCurr.m[2][0] = m_LastFloorData.m[2][0];
					m_Floor[i].m_TransformFloorCurr.m[2][1] = m_LastFloorData.m[2][1];
					m_Floor[i].m_TransformFloorCurr.m[2][2] = m_LastFloorData.m[2][2];

				}

			}
			else
			{
				if (m_Floor[i].m_currFloorNum != 0)
		        	m_Floor[i].m_currFloorNum--;
		        else
		        	m_Floor[i].m_currFloorNum = 6; //Loop to the top
			}
			//Change CurrFloorNum
			//Based On Dist

			//cout << (m_Floor[1].m_TransformFloorData.m[0][0] - m_Floor[2].m_TransformFloorData.m[0][0]) / (m_FloorSpeedTimer / dt) << endl;
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

	AEGfxTextureSet(pTex, 0, 0);

	for (int i = 1; i < 8; i++)
	{
		AEGfxSetTransform(m_Floor[i].m_TransformFloorCurr.m);
		AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
	}


	//Working
	//m_Floor[1].m[0][0] += (m_Floor[0].m[0][0] - m_Floor[1].m[0][0]) / 100;
	//m_Floor[1].m[0][1] += (m_Floor[0].m[0][1] - m_Floor[1].m[0][1]) / 100;
	//m_Floor[1].m[0][2] += (m_Floor[0].m[0][2] - m_Floor[1].m[0][2]) / 100;
	//m_Floor[1].m[1][0] += (m_Floor[0].m[1][0] - m_Floor[1].m[1][0]) / 100;
	//m_Floor[1].m[1][1] += (m_Floor[0].m[1][1] - m_Floor[1].m[1][1]) / 100;
	//m_Floor[1].m[1][2] += (m_Floor[0].m[1][2] - m_Floor[1].m[1][2]) / 100;
	//m_Floor[1].m[2][0] += (m_Floor[0].m[2][0] - m_Floor[1].m[2][0]) / 100;
	//m_Floor[1].m[2][1] += (m_Floor[0].m[2][1] - m_Floor[1].m[2][1]) / 100;
	//m_Floor[1].m[2][2] += (m_Floor[0].m[2][2] - m_Floor[1].m[2][2]) / 100;


	//AEGfxMeshDraw(pMesh, AE_GFX_MDM_LINES_STRIP);
}

void SceneSplashScreen::Exit()
{
	std::cout << "Exiting Scene SplashScreen" << std::endl;

	AEGfxMeshFree(pMesh);
	AEGfxTextureUnload(pTex);
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