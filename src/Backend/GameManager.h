#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include <stdio.h>
#include <stdlib.h>
#include "../Backend/Pch.h"
#include "../Scenes/SceneManager.h"
#include "../Backend/SoundManager.h"

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
