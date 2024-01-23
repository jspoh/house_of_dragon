#ifndef SCENE_MENU_H
#define SCENE_MENU_H

#include "Scene.h"

class SceneManager;
class SceneMenu : public Scene
{
public:
	SceneMenu();
	~SceneMenu();

	virtual void Load();
	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

private:
	SceneMenu(SceneManager* _sceneMgr); // This is used to register to SceneManager

	static SceneMenu* sInstance; // The pointer to the object that gets registered
};

#endif