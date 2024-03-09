/*!************************************************************************
\file SoundManager.cpp
\author Soh Wei Jie (weijie.soh)
\par DP email: weijie.soh\@digipen.edu
\par Course: csd1451
\par House 0F Dragons
\date 23-1-2024
\brief This source file manages sounds from AEAudio to play sounds from Fmod.
**************************************************************************/

#include "SoundManager.h"
#include <iostream>

using namespace std;

// Constructor
SoundManager::SoundManager()
{
}

// Destructor
SoundManager::~SoundManager()
{
	// Clear out the sound map
	soundMap.clear();
}
