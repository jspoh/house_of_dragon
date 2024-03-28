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
		return false;
	}

	AEAudio audio = isMusic ? AEAudioLoadMusic(path.c_str()) : AEAudioLoadSound(path.c_str());

	if (!AEAudioIsValidAudio(audio)) {
		std::cerr << "SoundManager failed to load " << (isMusic ? "music" : "sound") << " with ref " << ref << " using path " << path << "\n";
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
	else if (musicMap.find(ref) != soundMap.end()) {
		audio = musicMap[ref];
	}
	else {
		std::cerr << "Audio with ref " << ref << " does not exist!\n";
		return;
	}

	AEAudioPlay(audio, isMusic ? musicGroup : sfxGroup, volume, 1.f, loop);
}


void SoundManager::setVolume(float volume, bool setMusic) {
	AEAudioSetGroupVolume(setMusic ? musicGroup : sfxGroup, volume);
}
