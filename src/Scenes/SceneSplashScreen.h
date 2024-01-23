#ifndef SCENE_SPLASHSCREEN_H
#define SCENE_SPLASHSCREEN_H

#include "Scene.h"

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
};

#endif