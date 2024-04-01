/* Start Header ************************************************************************/
/*!
\file SceneSplashScreen.cpp
\author 
\par 
\date 01 Apr 2024
\brief splash screen implementation
/*
Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/


#include "Pch.h"
#include "SceneSplashScreen.h"

SceneSplashScreen* SceneSplashScreen::sInstance = new SceneSplashScreen(SceneManager::GetInstance());

SceneSplashScreen::SceneSplashScreen() 
	: m_Mesh(nullptr),
	m_DigipenLogoTex(nullptr),
	m_TeamLogoTex(nullptr), 
	m_LogoIndex(1), 
	m_LogoTimer(LogoMaxTime)
{
	AEMtx33Identity(&m_TransformDigipenLogoData);
	AEMtx33Identity(&m_TransformTeamLogoData);
}

SceneSplashScreen::SceneSplashScreen(SceneManager* _sceneMgr)
	: m_Mesh(nullptr),
    m_DigipenLogoTex(nullptr),
	m_TeamLogoTex(nullptr),
	m_LogoIndex(1),
	m_LogoTimer(LogoMaxTime)
{
	_sceneMgr->AddScene("SceneSplashScreen", this);
	AEMtx33Identity(&m_TransformDigipenLogoData);
	AEMtx33Identity(&m_TransformTeamLogoData);
}

SceneSplashScreen::~SceneSplashScreen()
{
}

void SceneSplashScreen::Load()
{
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

	// Saving the mesh (list of triangles) in pMesh
	m_Mesh = AEGfxMeshEnd();

	m_DigipenLogoTex = AEGfxTextureLoad("Assets/SplashScreen/DigiPen_Singapore_WEB_RED.png");
	m_TeamLogoTex = AEGfxTextureLoad("Assets/SplashScreen/Team_Logo.png");
}

void SceneSplashScreen::Init()
{
	cout << "Loading Scene SplashScreen" << "\n";

	AEMtx33 scale{}, trans{};
	AEMtx33Scale(&scale, 1000.0f, 300.0f);
	AEMtx33Trans(&trans, 0, 30);
	AEMtx33Concat(&m_TransformDigipenLogoData, &trans, &scale);

	AEMtx33Scale(&scale, 800.0f, 800.0f);
	AEMtx33Trans(&trans, 0, -30);
	AEMtx33Concat(&m_TransformTeamLogoData, &trans, &scale);
}

void SceneSplashScreen::Update(double dt)
{
	m_LogoTimer -= m_LogoTimer > 0.0f ? dt : 0;

	if (m_LogoTimer <= 1.1f || AEInputCheckTriggered(AEVK_SPACE))
	{
		if (m_LogoIndex >= NUMOFSPLASHES * 2 - 1)
		{
			SceneManager::GetInstance()->SetActiveScene("SceneMenu");
			return;
		}
		m_LogoTimer = LogoMaxTime;
		
		if (AEInputCheckCurr(AEVK_SPACE))
			m_LogoIndex += (m_LogoIndex % 2)? 4 : 1;
		else
			m_LogoIndex++;


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
	if(!(m_LogoIndex % 2))
		AEGfxSetTransparency(static_cast<f32>(-3.0 + 4.0 / m_LogoTimer));
	else
		AEGfxSetTransparency(static_cast<f32>(3.0 - 5.0/ m_LogoTimer));

	switch(m_LogoIndex/2)
	{
	case 0:
		AEGfxTextureSet(m_DigipenLogoTex, 0, 0);
		AEGfxSetTransform(m_TransformDigipenLogoData.m);
		break;
	case 1:
	default:
		AEGfxTextureSet(m_TeamLogoTex, 0, 0); 
		AEGfxSetTransform(m_TransformTeamLogoData.m);
		break;
	}
	AEGfxMeshDraw(m_Mesh, AE_GFX_MDM_TRIANGLES);
}

void SceneSplashScreen::Exit()
{
	cout << "Exiting Scene SplashScreen" << "\n";

	AEGfxMeshFree(m_Mesh);

	AEGfxTextureUnload(m_DigipenLogoTex);
	AEGfxTextureUnload(m_TeamLogoTex);
}