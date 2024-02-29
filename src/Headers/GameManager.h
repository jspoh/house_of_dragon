#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include <stdio.h>
#include <stdlib.h>
#include "Pch.h"
#include "SceneManager.h"
////#include "../GameObject/GameObjectManager.h"
#include "SoundManager.h"
#include "Event.h"
#include "db.h"

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
