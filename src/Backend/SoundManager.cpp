/*!************************************************************************
\file SoundManager.cpp
\author Soh Wei Jie (weijie.soh), Poh Jing Seng (jingseng.poh)
\par DP email: weijie.soh\@digipen.edu, jingseng.poh\@digipen.edu
\par Course: csd1451
\par House 0F Dragons
\date 23-1-2024
\brief This source file manages sounds from AEAudio to play sounds from Fmod.
**************************************************************************/

#include "Pch.h"
#include "SoundManager.h"

using namespace std;

// Constructor
SoundManager::SoundManager()
{
	std::cout << "Initializing SoundManager..\n";
	sfxGroup = AEAudioCreateGroup();
	musicGroup = AEAudioCreateGroup();

	SFX_VOLUME = Database::getInstance()->data["game"]["audio"]["sfx"];
	MUSIC_VOLUME = Database::getInstance()->data["game"]["audio"]["music"];

	setVolume(SFX_VOLUME, false);
	setVolume(MUSIC_VOLUME, true);
}

// Destructor
SoundManager::~SoundManager()
{
	for (const auto& map : soundMap) {
		AEAudioUnloadAudio(map.second);
	}
	for (const auto& map : musicMap) {
		AEAudioUnloadAudio(map.second);
	}

	AEAudioUnloadAudioGroup(sfxGroup);
	AEAudioUnloadAudioGroup(musicGroup);

	// Clear out the maps
	soundMap.clear();
	musicMap.clear();

	// save volume
	Database::getInstance()->data["game"]["audio"]["sfx"] = SFX_VOLUME;
	Database::getInstance()->data["game"]["audio"]["music"] = MUSIC_VOLUME;
}

bool SoundManager::removeAudio(std::string ref, bool isMusic) {
	if (soundMap.find(ref) == soundMap.end() && !isMusic || musicMap.find(ref) == musicMap.end() && isMusic) {
		std::cerr << "Audio reference " << ref << " does not exist!\n";
		return false;
	}

	if (!isMusic) {
		AEAudioUnloadAudio(soundMap.find(ref)->second);
		soundMap.erase(ref);
	}
	else {
		AEAudioUnloadAudio(musicMap.find(ref)->second);
		musicMap.erase(ref);
	}

	return true;
}

bool SoundManager::registerAudio(std::string ref, std::string path, bool isMusic) {
	// check if ref is already used
	if (soundMap.find(ref) != soundMap.end() || musicMap.find(ref) != musicMap.end()) {
		std::cerr << "Audio reference " << ref << " already exists!\n";
		throw std::exception("Audio reference already exists");
		return false;
	}

	AEAudio audio = isMusic ? AEAudioLoadMusic(path.c_str()) : AEAudioLoadSound(path.c_str());

	if (!AEAudioIsValidAudio(audio)) {
		std::cerr << "SoundManager failed to load " << (isMusic ? "music" : "sound") << " with ref " << ref << " using path " << path << "\n";
		throw std::exception("Audio failed to load");
		return false;
	}

	if (isMusic) {
		musicMap[ref] = audio;
	}
	else {
		soundMap[ref] = audio;
	}

	std::cout << "SoundManager loaded " << (isMusic ? "music" : "sound") << " with ref " << ref << " using path " << path << "\n";


	return true;
}

void SoundManager::playAudio(std::string ref, float volume, int loop, bool isMusic) {
	AEAudio audio;

	if (soundMap.find(ref) != soundMap.end()) {
		audio = soundMap[ref];
	}
	else if (musicMap.find(ref) != musicMap.end()) {
		audio = musicMap[ref];
	}
	else {
		std::cerr << "Audio with ref " << ref << " does not exist!\n";
		throw std::exception("Audio does not exist!");
		return;
	}

	//volume *= isMusic ? MUSIC_VOLUME : SFX_VOLUME;

	AEAudioPlay(audio, isMusic ? musicGroup : sfxGroup, volume, 1.f, loop);
}


void SoundManager::setVolume(float volume, bool setMusic) {
	if (setMusic) {
		MUSIC_VOLUME = volume;
	}
	else {
		SFX_VOLUME = volume;
	}

	AEAudioSetGroupVolume(setMusic ? musicGroup : sfxGroup, volume);
}

void SoundManager::stopAll() {
	AEAudioStopGroup(sfxGroup);
	AEAudioStopGroup(musicGroup);
}

void SoundManager::getVolume(float& sfx, float& music) {
	sfx = SFX_VOLUME;
	music = MUSIC_VOLUME;
}

void SoundManager::pauseGroup(bool isMusic) {
	if (isMusic) {
		AEAudioPauseGroup(musicGroup);
	}
	else {
		AEAudioPauseGroup(sfxGroup);
	}
}

void SoundManager::resumeGroup(bool isMusic) {
	if (isMusic) {
		AEAudioResumeGroup(musicGroup);
	}
	else {
		AEAudioResumeGroup(sfxGroup);
	}
}
