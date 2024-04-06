/* Start Header ************************************************************************/
/*!
\file SoundPlayer.cpp
\author Poh Jing Seng, jingseng.poh, 2301363
\par jingseng.poh\@digipen.edu
\date 3 Feb 2024
\brief wrapper on top of SoundManager to abstract away dynamic loading and random picking of sounds
/*
Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/


#include "Pch.h"
#include "SoundPlayer.h"
#include "SoundManager.h"

#include <filesystem>

namespace fs = std::filesystem;

SoundPlayer SoundPlayer::instance;

void __getPath(std::string subpath, std::string& sfxPath, std::string& musicPath) {
	sfxPath = SoundPlayer::instance.SFX_PATH + subpath;
	musicPath = SoundPlayer::instance.MUSIC_PATH + subpath;
}

void __loadAudio(const std::string& subpath) {
	std::string sfxPath, musicPath;
	__getPath(subpath, sfxPath, musicPath);

	if (fs::exists(sfxPath)) {
		for (const auto& item : fs::directory_iterator(sfxPath)) {
			std::string filename = item.path().filename().string();
			SoundManager::getInstance()->registerAudio(filename, sfxPath + filename);
		}
	}

	if (fs::exists(musicPath)) {
		for (const auto& item : fs::directory_iterator(musicPath)) {
			std::string filename = item.path().filename().string();
			SoundManager::getInstance()->registerAudio(filename, musicPath + filename, true);
		}
	}
}


SoundPlayer::SoundPlayer() {

}

SoundPlayer::~SoundPlayer() {
	// assets cleaned up by SoundManager
}

void SoundPlayer::stopAll() {
	SoundManager::getInstance()->stopAll();
}

/* global audio */

SoundPlayer::GlobalAudio::GlobalAudio() {
	__loadAudio(subpath);
}

SoundPlayer::GlobalAudio::~GlobalAudio() {

}

SoundPlayer::GlobalAudio& SoundPlayer::GlobalAudio::getInstance() {
	static GlobalAudio _instance;
	return _instance;
}

void SoundPlayer::GlobalAudio::playSfxClick() {
	SoundManager::getInstance()->playAudio("click_0.wav");
}


/* menu audio */

SoundPlayer::MenuAudio::MenuAudio() {
	__loadAudio(subpath);
}

SoundPlayer::MenuAudio::~MenuAudio() {

}

SoundPlayer::MenuAudio& SoundPlayer::MenuAudio::getInstance() {
	static MenuAudio _instance;
	return _instance;
}

void SoundPlayer::MenuAudio::playLoopMenu() {
	SoundManager::getInstance()->playAudio("menu_0.wav", 1, -1, true);
}

void SoundPlayer::MenuAudio::playLoopLevelSelect() {
	SoundManager::getInstance()->playAudio("level_select_0.wav", 1, -1, true);
}


/* game audio */

SoundPlayer::GameAudio::GameAudio() {
	__loadAudio(subpath);
}

SoundPlayer::GameAudio::~GameAudio() {

}

SoundPlayer::GameAudio& SoundPlayer::GameAudio::getInstance() {
	static GameAudio _instance;
	return _instance;
}

void SoundPlayer::GameAudio::playLoop() {
	SoundManager::getInstance()->playAudio("footsteps_fast_0.wav", 1, -1, false);
	SoundManager::getInstance()->playAudio("movement_0.wav", 0.1f, -1, true);
}


/* combat audio */


SoundPlayer::CombatAudio::CombatAudio() {
	__loadAudio(subpath);

	battleLoopRefs.reserve(NUM_BATTLE_LOOPS);
	hurtSfxRefs.reserve(NUM_HURT_SFX);
	knifeSfxRefs.reserve(NUM_KNIFE_SFX);
	punchSfxRefs.reserve(NUM_PUNCH_SFX);

	animalSfxRef.reserve(NUM_ANIMALS);

	std::string sfxPath, musicPath;
	__getPath(subpath, sfxPath, musicPath);

	for (const auto& item : fs::directory_iterator(sfxPath)) {
		std::string filename = item.path().filename().string();

		// add animal sounds to map, based on animal type
		if (filename.find("animal") == 0) {
			const std::vector<std::string> tokens = split(filename, '_');
			animalSfxRef[tokens[1]].push_back(filename);
		}
		// add sounds to relevant vector, based on sound type
		else if (filename.find("hurt") == 0) {
			hurtSfxRefs.push_back(filename);
		}
		else if (filename.find("knife") == 0) {
			knifeSfxRefs.push_back(filename);
		}
		else if (filename.find("punch") == 0) {
			punchSfxRefs.push_back(filename);
		}
	}

	for (const auto& item : fs::directory_iterator(musicPath)) {
		std::string filename = item.path().filename().string();

		if (filename.find("battle") == 0) {
			battleLoopRefs.push_back(filename);
		}
	}
}

SoundPlayer::CombatAudio::~CombatAudio() {

}

SoundPlayer::CombatAudio& SoundPlayer::CombatAudio::getInstance() {
	static CombatAudio _instance;
	return _instance;
}

void SoundPlayer::CombatAudio::playLoop() {
	int randNum = rand() % NUM_BATTLE_LOOPS;
	SoundManager::getInstance()->playAudio(battleLoopRefs[randNum], 1, -1, true);
}

void SoundPlayer::CombatAudio::playSfxVictory() {
	SoundManager::getInstance()->playAudio("victory_0.wav");
}

void SoundPlayer::CombatAudio::playSfxDeath() {
	SoundManager::getInstance()->playAudio("death_0.wav");
}

void SoundPlayer::CombatAudio::playSfxAnimal(const std::string& animal) {
	int availSounds{};
	const std::string prefix = animal;
	for (const auto& [mapKey, refs] : animalSfxRef) {
		if (mapKey.substr(0, prefix.size()) == prefix) {
			availSounds = static_cast<int>(refs.size());
		}
	}

	if (availSounds == 0) {
		cerr << "Combat animal sound for " << animal << " does not exist!\n";
		throw std::exception();
	}

	const int randSoundIdx = rand() % availSounds;
	std::stringstream soundRef;
	soundRef << "animal_" << prefix << "_" << randSoundIdx << ".wav";

	SoundManager::getInstance()->playAudio(soundRef.str());
}

void SoundPlayer::CombatAudio::playSfxHealth() {
	SoundManager::getInstance()->playAudio("health_restore_0.wav");
}

void SoundPlayer::CombatAudio::playSfxHeartbeat() {
	SoundManager::getInstance()->playAudio("heartbeat_0.wav");
}

void SoundPlayer::CombatAudio::playSfxHurt() {
	int randNum = rand() % NUM_HURT_SFX;
	SoundManager::getInstance()->playAudio(hurtSfxRefs[randNum]);
}

void SoundPlayer::CombatAudio::playSfxKnife() {
	int randNum = rand() % NUM_KNIFE_SFX;
	SoundManager::getInstance()->playAudio(knifeSfxRefs[randNum]);
}

void SoundPlayer::CombatAudio::playSfxPowerup() {
	SoundManager::getInstance()->playAudio("powerup_0.wav");
}

void SoundPlayer::CombatAudio::playSfxPunch() {
	int randNum = rand() % NUM_PUNCH_SFX;
	SoundManager::getInstance()->playAudio(punchSfxRefs[randNum]);
}

void SoundPlayer::CombatAudio::playSfxInvalid() {
	SoundManager::getInstance()->playAudio("invalid_0.wav");
}

void SoundPlayer::CombatAudio::playSfxElement(Element element) {
	std::string el = ElementProperties::getElementName(element);
	std::transform(el.begin(), el.end(), el.begin(), [](char c) { return std::tolower(c); });
	SoundManager::getInstance()->playAudio(el + "_0.wav");
}
