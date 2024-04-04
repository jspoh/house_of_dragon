/* Start Header ************************************************************************/
/*!
\file SceneSetting.cpp
\author Yang yujie, yujie.yang, 2301383
\par yujie.yang\@digipen.edu
\author Poh Jing Seng, jingseng.poh, 2301363
\par jingseng.poh\@digipen.edu
\date 01 Apr 2024
\brief settings scene implementation, changes of difficulty and sound settings
/*
Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/


#include "Pch.h"
#include "SceneSetting.h"
#include "ParticleManager.h"

/**
 * @enum MOD_STATES
 * @brief Enumeration to identify the modification state in the settings scene.
 *
 * This enum is used to manage the state of settings modifications, such as
 * adjusting audio volume for sound effects and music within the setting's scene.
 */
enum class MOD_STATES {
	NONE,
	SFX_AUDIO_VOLUME,
	MUSIC_AUDIO_VOLUME,
	NUM_MOD_STATES
};


SceneSetting* SceneSetting::sInstance = new SceneSetting(SceneManager::GetInstance());

SceneSetting::SceneSetting(SceneManager* _sceneMgr)
{
	_sceneMgr->AddScene("SceneSetting", this);
}

SceneSetting::~SceneSetting()
{

}

/**
 * @brief Loads the necessary textures for the settings scene.
 *
 * Textures such as the background and music note icons are loaded to be
 * used in the rendering of the scene.
 */
void SceneSetting::Load()
{

	RenderHelper::getInstance()->registerTexture("settingbg", "./Assets/Menu/setting.png");
	RenderHelper::getInstance()->registerTexture("musicnote", "./Assets/Menu/musicnote.png");

}

/**
 * @brief Initializes the settings scene.
 *
 * The settings scene is initialized with the default settings for the
 * difficulty level, sound effects volume, and music volume. The initial
 * positions for the difficulty selection and audio sliders are also set.
 */
void SceneSetting::Init()
{
	ParticleManager::GetInstance()->init();

	const int btnIndex = static_cast<int>(difficulty);

	selectionPos.x = btnStartPos.x + btnIndex * (DIFFICULTY_BUTTON_WIDTH + DIFFICULTY_BUTTON_GAP);
	selectionTargetPos.y = selectionPos.y;
}

/**
 * @brief Updates the settings scene based on user interactions.
 *
 * The settings scene is updated based on the user's interactions with the
 * mouse cursor and keyboard inputs. The user can change the difficulty level
 * and adjust the audio volume for sound effects and music. The scene will
 * transition to the menu scene if the user presses the "Escape" button.
 *
 * @param dt the delta time for the update
 */
void SceneSetting::Update(double dt)
{
	//if "Escape" button triggered, go to menu state
	if (AEInputCheckTriggered(AEVK_Q))
		SceneManager::GetInstance()->SetActiveScene("SceneMenu");

	int mX, mY;
	AEInputGetCursorPosition(&mX, &mY);

	ParticleManager::GetInstance()->setParticlePos(static_cast<float>(mX), static_cast<float>(mY));
	ParticleManager::GetInstance()->update(dt);

	/* update slider position */

	// get updated volume values
	SoundManager::GetInstance()->getVolume(sfxVolume, musicVolume);

	const float sliderRadius = max(sliderScale.x, sliderScale.y);

	const float minX = soundBarPos.x - soundBarScale.x / 2.f;
	const float maxX = soundBarPos.x + soundBarScale.x / 2.f;

	static MOD_STATES modState = MOD_STATES::NONE;
	if (AEInputCheckCurr(AEVK_LBUTTON)) {

		Point sPos = wtos(soundSliderPos.x, soundSliderPos.y);
		Point mPos = wtos(musicSliderPos.x, musicSliderPos.y);
		if (modState == MOD_STATES::SFX_AUDIO_VOLUME || CollisionChecker::isMouseInCircle(sPos.x, sPos.y, sliderRadius, static_cast<float>(mouseX), static_cast<float>(mouseY))) {
			modState = MOD_STATES::SFX_AUDIO_VOLUME;
			const float sliderX = AEClamp(static_cast<float>(wMouseX), minX, maxX);
			sfxVolume = (sliderX - minX) / (maxX - minX);
		}
		else if (modState == MOD_STATES::MUSIC_AUDIO_VOLUME || CollisionChecker::isMouseInCircle(mPos.x, mPos.y, sliderRadius, static_cast<float>(mouseX), static_cast<float>(mouseY))) {
			modState = MOD_STATES::MUSIC_AUDIO_VOLUME;
			const float sliderX = AEClamp(static_cast<float>(wMouseX), minX, maxX);
			musicVolume = (sliderX - minX) / (maxX - minX);
		}
	}
	else {
		modState = MOD_STATES::NONE;
	}

	soundSliderPos.x = minX + sfxVolume * soundBarScale.x;
	musicSliderPos.x = minX + musicVolume * soundBarScale.x;

	SoundManager::GetInstance()->setVolume(sfxVolume, false);
	SoundManager::GetInstance()->setVolume(musicVolume, true);

	soundSliderPos.x = AEClamp(soundSliderPos.x, minX, maxX);

	/* difficulty */

	AEVec2 btnPos = btnStartPos;
	int i{};
	for (const auto& [setting, str] : DIFFICULTY_OPTIONS) {
		Point rectScreenPos = wtos(btnPos.x, btnPos.y);

		if (CollisionChecker::isMouseInRect(rectScreenPos.x, rectScreenPos.y, DIFFICULTY_BUTTON_WIDTH, DIFFICULTY_BUTTON_HEIGHT, static_cast<float>(mouseX), static_cast<float>(mouseY))) {
			// hover state
			if (!AEInputCheckTriggered(AEVK_LBUTTON)) {
				continue;
			}
			else {
				difficulty = static_cast<DIFFICULTY_SETTINGS>(i);
				lerpElapsedTime = 0;
			}
		}

		btnPos.x += DIFFICULTY_BUTTON_WIDTH + DIFFICULTY_BUTTON_GAP;
		i++;
	}

	selectionTargetPos.x = btnStartPos.x + static_cast<int>(difficulty) * (DIFFICULTY_BUTTON_WIDTH + DIFFICULTY_BUTTON_GAP);

	if (selectionPos.x != selectionTargetPos.x) {
		lerpElapsedTime += static_cast<float>(dt);
		AEVec2Lerp(&selectionPos, &selectionPos, &selectionTargetPos, lerpElapsedTime/DIFFICULTY_LERP_TIME);
	}
	else {
		lerpElapsedTime = 0;
	}
}

/**
 * @brief Renders the settings scene.
 *
 * The settings scene is rendered with the background image, difficulty
 * selection buttons, and audio sliders. The scene also renders the particle
 * effects for the user's interactions with the scene.
 */
void SceneSetting::Render()
{
	// render background
	RenderHelper::getInstance()->texture("settingbg", 0, 0, static_cast<float>(AEGfxGetWindowWidth()), static_cast<float>(AEGfxGetWindowHeight()));

	/* difficulty */
	RenderHelper::getInstance()->text("DIFFICULTY	:", AEGfxGetWindowWidth() / 3.0f, AEGfxGetWindowHeight() / 3.0f, 1.0f, 1.0f, 1.0f, 1.0f);
	
	// draw buttons
	AEVec2 btnPos = btnStartPos;
	for (const auto& [setting, str] : DIFFICULTY_OPTIONS) {
		RenderHelper::getInstance()->rect(btnPos.x, btnPos.y, DIFFICULTY_BUTTON_WIDTH, DIFFICULTY_BUTTON_HEIGHT, 0, Color{ 0.1f, 0.1f, 0.5f, 1 }, 1.f);
		Point screenPos = wtos(btnPos.x, btnPos.y);
		RenderHelper::getInstance()->text(str, screenPos.x, screenPos.y, 1, 1, 1, 1);

		btnPos.x += DIFFICULTY_BUTTON_WIDTH + DIFFICULTY_BUTTON_GAP;
	}

	// draw selected
	RenderHelper::getInstance()->rect("invis", selectionPos.x, selectionPos.y, DIFFICULTY_BUTTON_WIDTH, DIFFICULTY_BUTTON_HEIGHT, 0.f, Color{ 0.7f, 0.7f, 1, 0.5f }, 0.5f);

	/* audio */
	RenderHelper::getInstance()->text("SOUND	:", AEGfxGetWindowWidth() / 3.0f, AEGfxGetWindowHeight() / 2.0f, 1.0f, 1.0f, 1.0f, 1.0f);
	RenderHelper::getInstance()->text("MUSIC	:", AEGfxGetWindowWidth() / 3.0f, AEGfxGetWindowHeight() / 1.6f, 1.0f, 1.0f, 1.0f, 1.0f);

	RenderHelper::getInstance()->rect(soundBarPos.x, soundBarPos.y, soundBarScale.x, soundBarScale.y, 0, soundBarColor);
	RenderHelper::getInstance()->rect(musicBarPos.x, musicBarPos.y, soundBarScale.x, soundBarScale.y, 0, soundBarColor);
	RenderHelper::getInstance()->texture("musicnote", soundSliderPos.x, soundSliderPos.y, sliderScale.x, sliderScale.y);
	RenderHelper::getInstance()->texture("musicnote", musicSliderPos.x, musicSliderPos.y, sliderScale.x, sliderScale.y);


	ParticleManager::GetInstance()->render();

}

/**
 * @brief Exits the settings scene.
 *
 * The settings scene is exited, and the user's settings for the difficulty
 * level and audio volume are saved to the database.
 */
void SceneSetting::Exit()
{
	Database::getInstance()->data["game"]["difficulty"] = static_cast<int>(difficulty);

	//Image unloading was handle in the render helper
}



