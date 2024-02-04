#pragma once

#include "../../Scenes/Scene.h"

#include <vector>
//Testing SpriteAnimation
class SceneManager;
class CombatScene : public Scene
{
public:
	CombatScene();
	~CombatScene();

	virtual void Load();
	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

private:
	CombatScene(SceneManager* _sceneMgr); // This is used to register to SceneManager

	static CombatScene* sInstance; // The pointer to the object that gets registered
};