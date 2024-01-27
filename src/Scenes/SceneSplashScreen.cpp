#include "SceneSplashScreen.h"

#include "../Backend/GameManager.h" //? Still thinking
#include "SceneManager.h"
#include <iostream> //Remove upon test finish

SceneSplashScreen* SceneSplashScreen::sInstance = new SceneSplashScreen(SceneManager::GetInstance());
AEGfxVertexList* pMesh = 0;
//AEGfxTexture* pTex;

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
	//AEGfxTriAdd(
	//	-0.5f, -0.5f, 0xFFFF0000, 0.0f, 1.0f,  // bottom-left: red
	//	0.5f, -0.5f, 0xFF00FF00, 1.0f, 1.0f,   // bottom-right: green
	//	-0.5f, 0.5f, 0xFF0000FF, 0.0f, 0.0f);  // top-left: blue

	//AEGfxTriAdd(
	//	0.5f, -0.5f, 0xFF00FF00, 1.0f, 1.0f,   // bottom-right: green
	//	0.5f, 0.5f, 0xFFFFFFFF, 1.0f, 0.0f,    // top-right: white
	//	-0.5f, 0.5f, 0xFF0000FF, 0.0f, 0.0f);  // top-left: blue

	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFFFF0000, 0.0f, 1.0f,  // bottom-left: red
		0.5f, -1.0f, 0xFF00FF00, 1.0f, 1.0f,   // bottom-right: green
		-0.5f, 0.5f, 0xFF0000FF, 0.0f, 0.0f);  // top-left: blue

	AEGfxTriAdd(
		0.5f, -1.0f, 0xFF00FF00, 1.0f, 1.0f,   // bottom-right: green
		0.5f, 1.0f, 0xFFFFFFFF, 1.0f, 0.0f,    // top-right: white
		-0.5f, 0.5f, 0xFF0000FF, 0.0f, 0.0f);  // top-left: blue

	// Saving the mesh (list of triangles) in pMesh
	pMesh = AEGfxMeshEnd();
	//pTex = AEGfxTextureLoad("Assets/PlanetTexture.png");
}

void SceneSplashScreen::Update(double dt)
{
	std::cout << "Updating Scene SplashScreen" << std::endl;
	if (AEInputCheckTriggered(AEVK_3))
		SceneManager::GetInstance()->SetActiveScene("SceneBase");
}

void SceneSplashScreen::Render()
{
	std::cout << "Rendering Scene SplashScreen" << std::endl;
	////////////////////////////////////////////////////////////////////////////////////////
	// FIRST FLOOR
	// ///////////////////////////////////////////////////////////////////////////////////
	// Create a scale matrix that scales by 500 x and y
	AEMtx33 scale = { 0 };
	AEMtx33Scale(&scale, 300.f, 900.f);

	// Create a rotation matrix that rotates by 90 degrees
	// Note that PI in radians is 180 degrees.
	// Since 90 degrees is 180/2, 90 degrees in radians is PI/2
	AEMtx33 rotate = { 0 };
	AEMtx33Rot(&rotate, 3*PI/2);

	// Create a translation matrix that translates by
	// 200 in the x-axis and 100 in the y-axis
	AEMtx33 translate = { 0 };
	AEMtx33Trans(&translate, 0, -250);

	// Concatenate the matrices into the 'transform' variable.
	// We concatenate in the order of translation * rotation * scale
	// i.e. this means we scale, then rotate, then translate.
	AEMtx33 transform = { 0 };
	AEMtx33Concat(&transform, &rotate, &scale);
	AEMtx33Concat(&transform, &translate, &transform);


	// Your own rendering logic goes here
		// Set the background to black.
	AEGfxSetBackgroundColor(0.2f, 0.2f, 1.0f);

	// Tell the engine to get ready to draw something with texture.
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);

	// Set the the color to multiply to white, so that the sprite can 
	// display the full range of colors (default is black).
	AEGfxSetColorToMultiply(0.0f, 1.0f, 0.0f, 1.0f);

	// Set the color to add to nothing, so that we don't alter the sprite's color
	AEGfxSetColorToAdd(0.0f, 0.2f, 0.0f, 1.0f);

	// Set blend mode to AE_GFX_BM_BLEND
	// This will allow transparency.
	//AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	//AEGfxSetTransparency(1.0f);

	// Set the texture to pTex
	//AEGfxTextureSet(pTex, 0, 0);


	// Tell Alpha Engine to use the matrix in 'transform' to apply onto all
	// the vertices of the mesh that we are about to choose to draw in the next line.
	AEGfxSetTransform(transform.m);

	// Tell Alpha Engine to draw the mesh with the above settings.
	AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);


	////////////////////////////////////////////////////////////////////////////////////////
	// SECOND FLOOR
	// ///////////////////////////////////////////////////////////////////////////////////
	// Create a scale matrix that scales by 500 x and y
	scale = { 0 };
	AEMtx33Scale(&scale, 160.f, 450.f);

	// Create a rotation matrix that rotates by 90 degrees
	// Note that PI in radians is 180 degrees.
	// Since 90 degrees is 180/2, 90 degrees in radians is PI/2
	rotate = { 0 };
	AEMtx33Rot(&rotate, 3 * PI / 2);

	// Create a translation matrix that translates by
	// 200 in the x-axis and 100 in the y-axis
	translate = { 0 };
	AEMtx33Trans(&translate, 0, -20);

	// Concatenate the matrices into the 'transform' variable.
	// We concatenate in the order of translation * rotation * scale
	// i.e. this means we scale, then rotate, then translate.
	transform = { 0 };
	AEMtx33Concat(&transform, &rotate, &scale);
	AEMtx33Concat(&transform, &translate, &transform);

	AEGfxSetTransform(transform.m);

	// Tell Alpha Engine to draw the mesh with the above settings.
	AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
	//AEGfxMeshDraw(pMesh, AE_GFX_MDM_LINES_STRIP);
}

void SceneSplashScreen::Exit()
{
	std::cout << "Exiting Scene SplashScreen" << std::endl;

	AEGfxMeshFree(pMesh);
	//AEGfxTextureUnload(pTex);
}


//2D Camera Movement - Screen Shake
//AEGfxSetColorToMultiply(0.0f, 0.0f, 0.0f, 0.0f);
//static float x = 0, y = 0;
//x += 0.3f;
//y += 0.3f;
//AEGfxSetCamPosition(x, y);