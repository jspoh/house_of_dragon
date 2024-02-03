#include "SceneSplashScreen.h"

#include "../Backend/GameManager.h" //? Still thinking
#include "SceneManager.h"
#include <iostream> //Remove upon test finish


SceneSplashScreen* SceneSplashScreen::sInstance = new SceneSplashScreen(SceneManager::GetInstance());
AEGfxVertexList* pMesh = 0;
AEGfxTexture* pTex;
AEMtx33 m_TransformFloor[10];

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
	//First floor
	AEMtx33Scale(&scale, 1200.f, 400.f);
	AEMtx33Trans(&trans, 0, -250);
	AEMtx33Concat(&m_TransformFloor[0], &trans, &scale);
	//Second floor
	AEMtx33Scale(&scale, 580.f, 194.f);
	AEMtx33Trans(&trans, 0, 47);
	AEMtx33Concat(&m_TransformFloor[1], &trans, &scale);
	////Third floor
	//AEMtx33Scale(&scale, 1200.f, 400.f);
	//AEMtx33Trans(&trans, 0, -250);
	//AEMtx33Concat(&m_TransformFloor[2], &trans, &scale);
	
}

void SceneSplashScreen::Update(double dt)
{
	std::cout << "Updating Scene SplashScreen" << std::endl;
	if (AEInputCheckTriggered(AEVK_3))
		SceneManager::GetInstance()->SetActiveScene("SceneBase");
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

	static AEMtx33 TESTtransform = m_TransformFloor[1];
	for (int i = 0; i < 2; i++)
	{
		AEGfxSetTransform(m_TransformFloor[i].m);
		AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
	}

	m_TransformFloor[1].m[0][0] += (m_TransformFloor[0].m[0][0] - m_TransformFloor[1].m[0][0]) / 100;
	m_TransformFloor[1].m[0][1] += (m_TransformFloor[0].m[0][1] - m_TransformFloor[1].m[0][1]) / 100;
	m_TransformFloor[1].m[0][2] += (m_TransformFloor[0].m[0][2] - m_TransformFloor[1].m[0][2]) / 100;
	m_TransformFloor[1].m[1][0] += (m_TransformFloor[0].m[1][0] - m_TransformFloor[1].m[1][0]) / 100;
	m_TransformFloor[1].m[1][1] += (m_TransformFloor[0].m[1][1] - m_TransformFloor[1].m[1][1]) / 100;
	m_TransformFloor[1].m[1][2] += (m_TransformFloor[0].m[1][2] - m_TransformFloor[1].m[1][2]) / 100;
	m_TransformFloor[1].m[2][0] += (m_TransformFloor[0].m[2][0] - m_TransformFloor[1].m[2][0]) / 100;
	m_TransformFloor[1].m[2][1] += (m_TransformFloor[0].m[2][1] - m_TransformFloor[1].m[2][1]) / 100;
	m_TransformFloor[1].m[2][2] += (m_TransformFloor[0].m[2][2] - m_TransformFloor[1].m[2][2]) / 100;


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