/* Start Header ************************************************************************/
/*!
\file SoundManager.h
\author Poh Jing Seng, jingseng.poh, 2301363
\par jingseng.poh\@digipen.edu
\date 28 feb 2024
\brief SoundManager wrapper for AEAudio
/*
Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#pragma once

#include "SingletonTemplate.h"
#include "AEAudio.h"
#include <unordered_map>
#include <string>

class SoundManager : public Singleton<SoundManager>
{
private:
	AEAudioGroup m_sfxGroup;
	AEAudioGroup m_musicGroup;

	float m_sfxVolume;
	float m_musicVolume;

protected:
	std::unordered_map<std::string, AEAudio> m_soundMap;
	std::unordered_map<std::string, AEAudio> m_musicMap;

public:
	// Constructor
	SoundManager();
	~SoundManager();

	float getSfxVolume() const;
	float getMusicVolume() const;

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

