/* Start Header ************************************************************************/
/*!
\file SceneCredit.cpp
\author Yang yujie, yujie.yang, 2301383
\par yujie.yang\@digipen.edu
\date 01 Apr 2024
\brief credits scene
/*
Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/


#include "Pch.h"
#include "SceneCredit.h"

// should put in namespace to avoid cluttering
static char strBuffer[1024];
static char strBuffer1[1024];
static float speed = 8;
static AEVec2 text;
static float scrolling;
static float timef;




SceneCredits* SceneCredits::sInstance = new SceneCredits(SceneManager::getInstance());

SceneCredits::SceneCredits()
{
	pFontL = pFontM = pFontS = pFontxS = 0;
	textPosX = textPosY = 0.0f;
}

SceneCredits::SceneCredits(SceneManager* _sceneMgr)
{
	_sceneMgr->AddScene("SceneCredits", this);
	pFontL = pFontM = pFontS = pFontxS = 0;
	textPosX = textPosY = 0.0f;
}

SceneCredits::~SceneCredits()
{
}



void SceneCredits::load()
{
	credits.bg = AEGfxTextureLoad("Assets/Menu/bg.jpg");
	//credits.mesh = 0; 
	AEGfxMeshStart();
	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFFFF00FF, 0.0f, 1.0f,
		0.5f, -0.5f, 0xFFFFFF00, 1.0f, 1.0f,
		-0.5f, 0.5f, 0xFF00FFFF, 0.0f, 0.0f);
	AEGfxTriAdd(
		0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f,
		0.5f, 0.5f, 0xFFFFFFFF, 1.0f, 0.0f,
		-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f);
	credits.mesh = AEGfxMeshEnd();



}

/**
 * @brief Loads the credit scene assets.
 *
 * This method loads the necessary assets for the credit scene, including
 * the background texture and sets up the mesh for rendering.
 */

 /**
  * @brief Initializes the credit scene.
  *
  * Sets up the fonts with different sizes and initializes the positions for
  * text rendering in the credit scene.
  */

void SceneCredits::init()
{	pFontL = AEGfxCreateFont("Assets/Fonts/TokyoMidnight.otf", 50);
	pFontM = AEGfxCreateFont("Assets/Fonts/liberation-mono.ttf", 40);
	pFontS = AEGfxCreateFont("Assets/Fonts/MangaBold.otf", 30);
	pFontxS = AEGfxCreateFont("Assets/Fonts/MangaBold.otf", 20);

	f32 TextWidth = 0, TextHeight = 0;
	textPosX = -TextWidth / 2;
	textPosY = -TextHeight / 2;

	m_elapsedTime = 0.0f;
}

/**
 * @brief Updates the credit scene.
 *
 * Handles the logic for the credit scene's state transitions and text
 * scrolling effect based on frame time.
 * @param dt Delta time since the last frame update.
 */

void SceneCredits::update(double dt)
{
	UNREFERENCED_PARAMETER(dt);
	//if "Escape" button triggered, go to menu state
	if (AEInputCheckTriggered(AEVK_Q))
		SceneManager::getInstance()->SetActiveScene("SceneMenu");

	m_elapsedTime += static_cast<float>(dt);
	if (m_elapsedTime >= 20.0f)
	{
		// Close the application
		SceneManager::getInstance()->SetActiveScene("SceneMenu");
	}

}

/**
 * @brief Renders the credit scene.
 *
 * Draws the scene elements to the screen, including the scrolling text
 * and background image.
 */
void SceneCredits::render()
{

	scrolling += timef * speed;

	texture(credits.bg, 0.f, static_cast<float>(AEGfxGetWindowWidth()), static_cast<float>(AEGfxGetWindowHeight()), 0.f, 0.f, credits.mesh, 1.f);
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxTextureSet(NULL, 0, 0);

	f32 TextWidth = 0, TextHeight = 0;

	sprintf_s(strBuffer, "Press 'Q' to exit");
	AEGfxGetPrintSize(pFontM, strBuffer, 1.0f, &TextWidth, &TextHeight);
	AEGfxPrint(pFontM, strBuffer, 0 - TextWidth / 2, textPosY + 0.20f, 1.f, 1.f, 1.f, 1.f, 1.0f);

	sprintf_s(strBuffer, "HOUSE OF DRAGON");
	AEGfxGetPrintSize(pFontL, strBuffer, 1.0f, &TextWidth, &TextHeight);
	AEGfxPrint(pFontL, strBuffer, 0 - TextWidth / 2, textPosY + 0.01f, 1.f, 1.f, 1.f, 1.f, 1.0f);

	sprintf_s(strBuffer, "Created By");
	AEGfxGetPrintSize(pFontM, strBuffer, 1.0f, &TextWidth, &TextHeight);
	AEGfxPrint(pFontM, strBuffer, 0 - TextWidth / 2, textPosY - 0.2f, 1.f, 1.f, 1.f, 1.f, 1.0f);
	textPosY += speed * (f32)AEFrameRateControllerGetFrameTime() / AEGfxGetWindowHeight();

	sprintf_s(strBuffer, "SOH WEI JIE");
	AEGfxGetPrintSize(pFontS, strBuffer, 1.0f, &TextWidth, &TextHeight);
	AEGfxPrint(pFontS, strBuffer, 0 - TextWidth / 2, textPosY - 0.3f, 1.f, 1.f, 1.f, 1.f, 1.0f);
	textPosY += speed * (f32)AEFrameRateControllerGetFrameTime() / AEGfxGetWindowHeight();

	sprintf_s(strBuffer, "POH JING SENG");
	AEGfxGetPrintSize(pFontS, strBuffer, 1.0f, &TextWidth, &TextHeight);
	AEGfxPrint(pFontS, strBuffer, 0 - TextWidth / 2, textPosY - 0.4f, 1.f, 1.f, 1.f, 1.f, 1.0f);
	textPosY += speed * (f32)AEFrameRateControllerGetFrameTime() / AEGfxGetWindowHeight();

	sprintf_s(strBuffer, "YANG YUJIE");
	AEGfxGetPrintSize(pFontS, strBuffer, 1.0f, &TextWidth, &TextHeight);
	AEGfxPrint(pFontS, strBuffer, 0 - TextWidth / 2, textPosY - 0.5f, 1.f, 1.f, 1.f, 1.f, 1.0f);
	textPosY += speed * (f32)AEFrameRateControllerGetFrameTime() / AEGfxGetWindowHeight();

	sprintf_s(strBuffer, "KUEK WEI JIE");
	AEGfxGetPrintSize(pFontS, strBuffer, 1.0f, &TextWidth, &TextHeight);
	AEGfxPrint(pFontS, strBuffer, 0 - TextWidth / 2, textPosY - 0.6f, 1.f, 1.f, 1.f, 1.f, 1.0f);
	textPosY += speed * (f32)AEFrameRateControllerGetFrameTime() / AEGfxGetWindowHeight();

	sprintf_s(strBuffer, "Faculty and Advisors");
	AEGfxGetPrintSize(pFontM, strBuffer, 1.0f, &TextWidth, &TextHeight);
	AEGfxPrint(pFontM, strBuffer, 0 - TextWidth / 2, textPosY - 0.8f, 1.f, 1.f, 1.f, 1.f, 1.0f);
	textPosY += speed * (f32)AEFrameRateControllerGetFrameTime() / AEGfxGetWindowHeight();

	sprintf_s(strBuffer, "Programming");
	AEGfxGetPrintSize(pFontS, strBuffer, 1.0f, &TextWidth, &TextHeight);
	AEGfxPrint(pFontS, strBuffer, 0 - TextWidth / 2, textPosY - 0.9f, 1.f, 1.f, 1.f, 1.f, 1.0f);
	textPosY += speed * (f32)AEFrameRateControllerGetFrameTime() / AEGfxGetWindowHeight();

	sprintf_s(strBuffer, "CHENG DING XIANG");
	AEGfxGetPrintSize(pFontS, strBuffer, 1.0f, &TextWidth, &TextHeight);
	AEGfxPrint(pFontS, strBuffer, 0 - TextWidth / 2, textPosY - 1.0f, 1.f, 1.f, 1.f, 1.f, 1.0f);
	textPosY += speed * (f32)AEFrameRateControllerGetFrameTime() / AEGfxGetWindowHeight();

	sprintf_s(strBuffer, "GERALD WONG");
	AEGfxGetPrintSize(pFontS, strBuffer, 1.0f, &TextWidth, &TextHeight);
	AEGfxPrint(pFontS, strBuffer, 0 - TextWidth / 2, textPosY - 1.1f, 1.f, 1.f, 1.f, 1.f, 1.0f);
	textPosY += speed * (f32)AEFrameRateControllerGetFrameTime() / AEGfxGetWindowHeight();

	sprintf_s(strBuffer, "Created at");
	AEGfxGetPrintSize(pFontM, strBuffer, 1.0f, &TextWidth, &TextHeight);
	AEGfxPrint(pFontM, strBuffer, 0 - TextWidth / 2, textPosY - 1.3f, 1.f, 1.f, 1.f, 1.f, 1.0f);
	textPosY += speed * (f32)AEFrameRateControllerGetFrameTime() / AEGfxGetWindowHeight();

	sprintf_s(strBuffer, "DigiPen Institute of Technology Singapore");
	AEGfxGetPrintSize(pFontM, strBuffer, 1.0f, &TextWidth, &TextHeight);
	AEGfxPrint(pFontM, strBuffer, 0 - TextWidth / 2, textPosY - 1.40f, 1.f, 1.f, 1.f, 1.f, 1.0f);
	textPosY += speed * (f32)AEFrameRateControllerGetFrameTime() / AEGfxGetWindowHeight();

	sprintf_s(strBuffer, "PRESIDENT");
	AEGfxGetPrintSize(pFontM, strBuffer, 1.0f, &TextWidth, &TextHeight);
	AEGfxPrint(pFontM, strBuffer, 0 - TextWidth / 2, textPosY - 1.5f, 1.f, 1.f, 1.f, 1.f, 1.0f);
	textPosY += speed * (f32)AEFrameRateControllerGetFrameTime() / AEGfxGetWindowHeight();

	sprintf_s(strBuffer, "CLAUDE COMAIR");
	AEGfxGetPrintSize(pFontS, strBuffer, 1.0f, &TextWidth, &TextHeight);
	AEGfxPrint(pFontS, strBuffer, 0 - TextWidth / 2, textPosY - 1.6f, 1.f, 1.f, 1.f, 1.f, 1.0f);
	textPosY += speed * (f32)AEFrameRateControllerGetFrameTime() / AEGfxGetWindowHeight();

	sprintf_s(strBuffer, "EXECUTIVES");
	AEGfxGetPrintSize(pFontM, strBuffer, 1.0f, &TextWidth, &TextHeight);
	AEGfxPrint(pFontM, strBuffer, 0 - TextWidth / 2, textPosY - 1.7f, 1.f, 1.f, 1.f, 1.f, 1.0f);
	textPosY += speed * (f32)AEFrameRateControllerGetFrameTime() / AEGfxGetWindowHeight();

	sprintf_s(strBuffer, "JASON CHU  SAMIR ABOU SAMRA  MICHELE COMAIR");
	AEGfxGetPrintSize(pFontS, strBuffer, 1.0f, &TextWidth, &TextHeight);
	AEGfxPrint(pFontS, strBuffer, 0 - TextWidth / 2, textPosY - 1.8f, 1.f, 1.f, 1.f, 1.f, 1.0f);
	textPosY += speed * (f32)AEFrameRateControllerGetFrameTime() / AEGfxGetWindowHeight();

	sprintf_s(strBuffer, "ANGELA KUGLER  ERIK MOHRMANN");
	AEGfxGetPrintSize(pFontS, strBuffer, 1.0f, &TextWidth, &TextHeight);
	AEGfxPrint(pFontS, strBuffer, 0 - TextWidth / 2, textPosY - 1.9f, 1.f, 1.f, 1.f, 1.f, 1.0f);
	textPosY += speed * (f32)AEFrameRateControllerGetFrameTime() / AEGfxGetWindowHeight();

	sprintf_s(strBuffer, "BENJAMIN ELLINGER  MELVIN GONSALVEZ");
	AEGfxGetPrintSize(pFontS, strBuffer, 1.0f, &TextWidth, &TextHeight);
	AEGfxPrint(pFontS, strBuffer, 0 - TextWidth / 2, textPosY - 2.0f, 1.f, 1.f, 1.f, 1.f, 1.0f);
	textPosY += speed * (f32)AEFrameRateControllerGetFrameTime() / AEGfxGetWindowHeight();

	sprintf_s(strBuffer, "WWW.DIGIPEN.EDU");
	AEGfxGetPrintSize(pFontxS, strBuffer, 1.0f, &TextWidth, &TextHeight);
	AEGfxPrint(pFontxS, strBuffer, 0 - TextWidth / 2, textPosY - 2.1f, 1.f, 1.f, 1.f, 1.f, 1.0f);
	textPosY += speed * (f32)AEFrameRateControllerGetFrameTime() / AEGfxGetWindowHeight();


	sprintf_s(strBuffer, "All content © 2024 DigiPen Institute of Technology Singapore. All Rights Reserved");
	AEGfxGetPrintSize(pFontxS, strBuffer, 1.0f, &TextWidth, &TextHeight);
	AEGfxPrint(pFontxS, strBuffer, 0 - TextWidth / 2, textPosY - 2.2f, 1.f, 1.f, 1.f, 1.f, 1.0f);
	textPosY += speed * (f32)AEFrameRateControllerGetFrameTime() / AEGfxGetWindowHeight();

	sprintf_s(strBuffer, "FMOD Studio © FireLight Technologies Pty Ltd ");
	AEGfxGetPrintSize(pFontxS, strBuffer, 1.0f, &TextWidth, &TextHeight);
	AEGfxPrint(pFontxS, strBuffer, 0 - TextWidth / 2, textPosY - 2.4f, 1.f, 1.f, 1.f, 1.f, 1.0f);
	textPosY += speed * (f32)AEFrameRateControllerGetFrameTime() / AEGfxGetWindowHeight();

	sprintf_s(strBuffer, " www.kenney.nl ");
	AEGfxGetPrintSize(pFontxS, strBuffer, 1.0f, &TextWidth, &TextHeight);
	AEGfxPrint(pFontxS, strBuffer, 0 - TextWidth / 2, textPosY - 2.6f, 1.f, 1.f, 1.f, 1.f, 1.0f);
	textPosY += speed * (f32)AEFrameRateControllerGetFrameTime() / AEGfxGetWindowHeight();



}
/**
 * @brief Cleans up resources used by the credit scene.
 *
 * Frees up the meshes and textures used by the credit scene and destroys
 * the fonts that were created.
 */
void SceneCredits::exit()
{
	AEGfxMeshFree(credits.mesh);
	AEGfxTextureUnload(credits.bg);
	AEGfxDestroyFont(pFontL);
	AEGfxDestroyFont(pFontM);
	AEGfxDestroyFont(pFontS);
	AEGfxDestroyFont(pFontxS);
}

/**
 * @brief Renders a texture to the screen.
 *
 * Draws a texture to the screen with the specified parameters.
 * @param texture The texture to render.
 * @param scaleX The scale factor for the texture on the x-axis.
 * @param scaleY The scale factor for the texture on the y-axis.
 * @param rotation The rotation angle for the texture.
 * @param positionX The x-coordinate for the texture's position.
 * @param positionY The y-coordinate for the texture's position.
 * @param mesh The mesh to render the texture on.
 * @param transparency The transparency value for the texture.
 */

void SceneCredits::texture(AEGfxTexture* texture, f32 scaleX, f32 scaleY, f32 rotation, f32 positionX, f32 positionY, AEGfxVertexList* mesh, f32 transparency)
{
	UNREFERENCED_PARAMETER(transparency);
	UNREFERENCED_PARAMETER(texture);
	UNREFERENCED_PARAMETER(scaleY);
	UNREFERENCED_PARAMETER(scaleX);
	UNREFERENCED_PARAMETER(rotation);
	UNREFERENCED_PARAMETER(positionX);
	UNREFERENCED_PARAMETER(positionY);
	UNREFERENCED_PARAMETER(mesh);
	AEGfxSetBackgroundColor(0.0f, 0.2f, 1.0f);
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 1.0f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);


	AEMtx33 scale = { 0 };
	AEMtx33Scale(&scale, 5670, 3780);

	AEMtx33 transform;
	AEMtx33Trans(&transform, 0, 0);

	AEMtx33 model = { 0 };
	AEMtx33Concat(&model, &scale, &transform);

	// prepare to draw
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetColorToAdd(0, 0, 0, 1);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(1);
	AEGfxSetTransform(model.m);
	AEGfxTextureSet(credits.bg, 0, 0);
	AEGfxMeshDraw(credits.mesh, AE_GFX_MDM_TRIANGLES);




}