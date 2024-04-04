/* Start Header ************************************************************************/
/*!
\file SceneSetting.h
\author Yang yujie, yujie.yang, 2301383
\par yujie.yang\@digipen.edu
\date 01 Apr 2024
\brief settings scene implementation, changes of difficulty and sound settings
/*
Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#pragma once

//#ifndef SCENE_MENU_H
//#define SCENE_MENU_H

#include "Scene.h"
#include "SceneManager.h"
#include "Pch.h"

/**
 * @class SceneSetting
 * @brief Manages the user settings within the game.
 *
 * This class is responsible for handling the operations required for the
 * settings scene, which includes loading resources, initializing settings,
 * updating based on user interactions, and rendering the scene.
 */

class SceneManager;
class SceneSetting : public Scene {
public:
	SceneSetting(SceneManager* _sceneMgr);
	~SceneSetting();



	virtual void Load();
	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();



private:



	static SceneSetting* sInstance; // The pointer to the object that gets registered

	/* audio */

	AEVec2 soundBarPos{ 170, -2 };
	const AEVec2 soundBarScale{ 600, 15 };
	AEVec2 musicBarPos{ 170, -95 };
	const Color soundBarColor{ 1, 0.82f, 0.62f, 1.f };

	const AEVec2 sliderScale{ 20, 30 };
	AEVec2 soundSliderPos{ 0, -2 };
	AEVec2 musicSliderPos{ 0, -95 };

	float sfxVolume{};
	float musicVolume{};

	/* difficulty */

	const std::array<std::pair<DIFFICULTY_SETTINGS, std::string>, 3> DIFFICULTY_OPTIONS{
		std::pair{DIFFICULTY_SETTINGS::EASY, "EASY"},
		{DIFFICULTY_SETTINGS::EASY, "MEDIUM"},
		{DIFFICULTY_SETTINGS::EASY, "HARD"}
	};

	static constexpr int DIFFICULTY_BUTTON_GAP = 1;
	static constexpr int DIFFICULTY_BUTTON_WIDTH = 185;
	static constexpr int DIFFICULTY_BUTTON_HEIGHT = 40;
	const AEVec2 btnStartPos{ 0, 125 };
	AEVec2 selectionPos{-999, btnStartPos.y};	// update x in init
	AEVec2 selectionTargetPos{};			// on changing difficulty, the pos for the selection highlight to go to
	static constexpr float DIFFICULTY_LERP_TIME = 1;
	float lerpElapsedTime = 0;
};

//#endif