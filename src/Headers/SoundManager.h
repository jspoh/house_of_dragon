#pragma once
/*!************************************************************************
\file SoundManager.h
\author Soh Wei Jie (weijie.soh), Poh Jing Seng (jingseng.poh)
\par DP email: weijie.soh\@digipen.edu, jingseng.poh\@digipen.edu
\par Course: csd1451
\par House 0F Dragons
\date 23-1-2024
\brief This source file manages sounds from AEAudio to play sounds from Fmod.
**************************************************************************/

#include "SingletonTemplate.h"
#include "AEAudio.h"
#include <unordered_map>
#include <string>

class SoundManager : public Singleton<SoundManager>
{
private:
	AEAudioGroup sfxGroup;
	AEAudioGroup musicGroup;

	float SFX_VOLUME;
	float MUSIC_VOLUME;

protected:
	std::unordered_map<std::string, AEAudio> soundMap;
	std::unordered_map<std::string, AEAudio> musicMap;

public:
	// Constructor
	SoundManager();
	~SoundManager();

	bool registerAudio(std::string ref, std::string path, bool isMusic=false);

	bool removeAudio(std::string ref, bool isMusic = false);

	/**
	 * .
	 * 
	 * \param ref
	 * \param volume between 0 and 1, in percentage
	 * \param loop whether to loop the audio or not.
	 *			0	-> play once
	 *			1	-> play twice
	 *			-1	-> play forever
	 */
	void playAudio(std::string ref, float volume=1.f, int loop=0, bool isMusic=false);

	void setVolume(float volume, bool setMusic=false);

	void stopAll();

	void getVolume(float& sfx, float& music);

	void pauseGroup(bool isMusic = false);

	void resumeGroup(bool isMusic = false);
};

