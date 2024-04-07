/* Start Header ************************************************************************/
/*!
\file SoundManager.cpp
\author Poh Jing Seng, jingseng.poh, 2301363
\par jingseng.poh\@digipen.edu
\date 01 Apr 2024
\brief SoundManager wrapper for AEAudio
/*
Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#include "Pch.h"
#include "SoundManager.h"

// Constructor
SoundManager::SoundManager()
{
	cout << "Initializing SoundManager..\n";
	m_sfxGroup = AEAudioCreateGroup();
	m_musicGroup = AEAudioCreateGroup();

	m_sfxVolume = Database::getInstance().data["game"]["audio"]["sfx"];
	m_musicVolume = Database::getInstance().data["game"]["audio"]["music"];

	setVolume(m_sfxVolume, false);
	setVolume(m_musicVolume, true);
}

// Destructor
SoundManager::~SoundManager()
{
	for (const auto& map : m_soundMap) {
		AEAudioUnloadAudio(map.second);
	}
	for (const auto& map : m_musicMap) {
		AEAudioUnloadAudio(map.second);
	}

	AEAudioUnloadAudioGroup(m_sfxGroup);
	AEAudioUnloadAudioGroup(m_musicGroup);

	// Clear out the maps
	m_soundMap.clear();
	m_musicMap.clear();

	// save volume
	Database::getInstance().data["game"]["audio"]["sfx"] = m_sfxVolume;
	Database::getInstance().data["game"]["audio"]["music"] = m_musicVolume;
}

bool SoundManager::removeAudio(std::string ref, bool isMusic) {
	if (m_soundMap.find(ref) == m_soundMap.end() && !isMusic || m_musicMap.find(ref) == m_musicMap.end() && isMusic) {
		cerr << "Audio reference " << ref << " does not exist!\n";
		return false;
	}

	if (!isMusic) {
		AEAudioUnloadAudio(m_soundMap.find(ref)->second);
		m_soundMap.erase(ref);
	}
	else {
		AEAudioUnloadAudio(m_musicMap.find(ref)->second);
		m_musicMap.erase(ref);
	}

	return true;
}

bool SoundManager::registerAudio(std::string ref, std::string path, bool isMusic) {
	// check if ref is already used
	if (m_soundMap.find(ref) != m_soundMap.end() || m_musicMap.find(ref) != m_musicMap.end()) {
		cerr << "Audio reference " << ref << " already exists!\n";
		//throw std::exception("Audio reference already exists");
		return false;
	}

	AEAudio audio = isMusic ? AEAudioLoadMusic(path.c_str()) : AEAudioLoadSound(path.c_str());

	if (!AEAudioIsValidAudio(audio)) {
		cerr << "SoundManager failed to load " << (isMusic ? "music" : "sound") << " with ref " << ref << " using path " << path << "\n";
		//throw std::exception("Audio failed to load");
		return false;
	}

	if (isMusic) {
		m_musicMap[ref] = audio;
	}
	else {
		m_soundMap[ref] = audio;
	}

	cout << "SoundManager loaded " << (isMusic ? "music" : "sound") << " with ref " << ref << " using path " << path << "\n";


	return true;
}

void SoundManager::playAudio(std::string ref, float volume, int loop, bool isMusic) {
	AEAudio audio;

	if (m_soundMap.find(ref) != m_soundMap.end()) {
		audio = m_soundMap[ref];
	}
	else if (m_musicMap.find(ref) != m_musicMap.end()) {
		audio = m_musicMap[ref];
	}
	else {
		cerr << "Audio with ref " << ref << " does not exist!\n";
		//throw std::exception("Audio does not exist!");
		return;
	}

	//volume *= isMusic ? m_musicVolume : m_sfxVolume;

	AEAudioPlay(audio, isMusic ? m_musicGroup : m_sfxGroup, volume, 1.f, loop);
}


float SoundManager::getSfxVolume() const {
	return m_sfxVolume;
}

float SoundManager::getMusicVolume() const {
	return m_musicVolume;
}


void SoundManager::setVolume(float volume, bool setMusic) {
	if (setMusic) {
		m_musicVolume = volume;
	}
	else {
		m_sfxVolume = volume;
	}

	AEAudioSetGroupVolume(setMusic ? m_musicGroup : m_sfxGroup, volume);
}

void SoundManager::stopAll() {
	AEAudioStopGroup(m_sfxGroup);
	AEAudioStopGroup(m_musicGroup);
}

void SoundManager::getVolume(float& sfx, float& music) {
	sfx = m_sfxVolume;
	music = m_musicVolume;
}

void SoundManager::pauseGroup(bool isMusic) {
	if (isMusic) {
		AEAudioPauseGroup(m_musicGroup);
	}
	else {
		AEAudioPauseGroup(m_sfxGroup);
	}
}

void SoundManager::resumeGroup(bool isMusic) {
	if (isMusic) {
		AEAudioResumeGroup(m_musicGroup);
	}
	else {
		AEAudioResumeGroup(m_sfxGroup);
	}
}
