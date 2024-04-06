/* Start Header ************************************************************************/
/*!
\file SceneSplashScreen.h
\author Soh Wei Jie, weijie.soh, 2301289
\par weijie.soh\@digipen.edu
\date 01 Apr 2024
\brief splash screen implementation
/*
Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#pragma once

#ifndef SCENE_SPLASHSCREEN_H
#define SCENE_SPLASHSCREEN_H

#include "Scene.h"
#include "SceneManager.h"
#include "Pch.h"

#define LogoMaxTime 3.0f
#define NUMOFSPLASHES 2 //MULTIPLY BY 2 FOR FADEIN AND FADEOUT IN UPDATE AND RENDER

class SceneManager;
class SceneSplashScreen : public Scene
{
public:
	SceneSplashScreen();
	~SceneSplashScreen();

	virtual void load();
	virtual void init();
	virtual void update(double dt);
	virtual void render();
	virtual void exit();

private:
	SceneSplashScreen(SceneManager* _sceneMgr); // This is used to register to SceneManager

	static SceneSplashScreen* sInstance; // The pointer to the object that gets registered

	AEGfxVertexList* m_Mesh;

	AEGfxTexture* m_DigipenLogoTex;
	AEGfxTexture* m_TeamLogoTex;
	int m_LogoIndex;
	double m_LogoTimer;
	AEMtx33 m_TransformDigipenLogoData;
	AEMtx33 m_TransformTeamLogoData;
};

#endif