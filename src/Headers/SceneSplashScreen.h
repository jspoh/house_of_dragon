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

	virtual void Load();
	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

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