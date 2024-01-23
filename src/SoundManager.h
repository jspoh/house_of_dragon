#pragma once
/*!************************************************************************
\file SoundManager.h
\author Soh Wei Jie (weijie.soh)
\par DP email: weijie.soh\@digipen.edu
\par Course: csd1171
\par House 0F Dragons
\date 18-1-2024
\brief This source file manages sounds from AEAudio to play sounds from Fmod.
**************************************************************************/

#include "SingletonTemplate.h"
#include <map>
#include <string>

class CSoundEngine : public Singleton<CSoundEngine>
{
protected:
	// Destructor
	// ///////////////////////////////////////////////////////////
	// Change To Fmod
	// ///////////////////////////////////////////////////////////
	//ISoundEngine* theSoundEngine; 

	// The library map of all the sounds created
	std::map<std::string, std::string> soundMap;

public:
	// Constructor
	CSoundEngine();
	virtual ~CSoundEngine();

	// Init this class and it will create the Sound Engine
	bool Init(void);

	// Get the handler to the sound engine
	// 	// ///////////////////////////////////////////////////////////
	// Change To Fmod
	// ///////////////////////////////////////////////////////////
	//ISoundEngine* GetSoundEngine(void);

	// Add a sound to this library
	void AddSound(const std::string& _soundIndex, const std::string& _soundSource);
	// Get a sound from this map 
	std::string GetSound(const std::string& _soundIndex);
	// Remove a sound from this map
	bool RemoveSound(const std::string& _soundIndex);
	// Play a sound from this map
	void PlayASound(const std::string& _soundIndex);
};

