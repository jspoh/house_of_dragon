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
protected:
	std::unordered_map<std::string, std::string> soundMap;

public:
	// Constructor
	SoundManager();
	~SoundManager();
};

