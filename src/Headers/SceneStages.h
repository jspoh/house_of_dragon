#ifndef SCENE_STAGES_H
#define SCENE_STAGES_H

#include "Scene.h"

#include "Pch.h"
#include "SceneManager.h"
#include "SceneLevelBuilder.h"

#define MOUSE_SENSITIVITY 5

class SceneManager;
class SceneStages : public Scene
{
public:
	SceneStages();
	~SceneStages();

	virtual void Load();
	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

private:
	SceneStages(SceneManager* _sceneMgr); // This is used to register to SceneManager

	static SceneStages* sInstance; // The pointer to the object that gets registered

	SceneLevelBuilder* m_LevelBuilder;
};

#endif