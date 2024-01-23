#ifndef SCENE_STAGE1_H
#define SCENE_STAGE1_H

#include "Scene.h"

class SceneManager;
class SceneStage1 : public Scene
{
public:
	SceneStage1();
	~SceneStage1();

	virtual void Load();
	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

private:
	SceneStage1(SceneManager* _sceneMgr); // This is used to register to SceneManager

	static SceneStage1* sInstance; // The pointer to the object that gets registered
};

#endif