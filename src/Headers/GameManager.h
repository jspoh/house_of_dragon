#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include <stdio.h>
#include <stdlib.h>
#include "Pch.h"
#include "SceneManager.h"
////#include "../GameObject/GameObjectManager.h"
#include "CombatManager.h"
#include "ParticleManager.h"

class GameManager
{
public:
	static GameManager& GetInstance()
	{
		static GameManager game;
		return game;
	}
	void Init();
	void Run();
	void Exit();
private:
	GameManager();
	~GameManager();
};
#endif
