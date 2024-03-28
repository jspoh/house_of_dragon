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

SoundPlayer::SoundPlayer() {

}

SoundPlayer::~SoundPlayer() {
	// assets cleaned up by SoundManager
}

void SoundPlayer::stopAll() {
	SoundManager::GetInstance()->stopAll();
}

/* global audio */

SoundPlayer::GlobalAudio::GlobalAudio() {
	std::string sfxPath, musicPath;
	__getPath(subpath, sfxPath, musicPath);

	for (const auto& item : fs::directory_iterator(sfxPath)) {
		std::string filename = item.path().filename().string();
		SoundManager::GetInstance()->registerAudio(filename, sfxPath + filename);
	}
}

SoundPlayer::GlobalAudio::~GlobalAudio() {

}

SoundPlayer::GlobalAudio& SoundPlayer::GlobalAudio::getInstance() {
	static GlobalAudio instance;
	return instance;
}

void SoundPlayer::GlobalAudio::playSfxClick() {
	SoundManager::GetInstance()->playAudio("click_0.wav");
}


/* game audio */