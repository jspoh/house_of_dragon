/* Start Header ************************************************************************/
/*!
\file SoundPlayer.h
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

#pragma once


#include "Pch.h"
#include "Elements.hpp"


/**
 * Contains app segments for different audio groups.
 *
 * Segmented into things like menu audio, gameplay audio, combat audio
 *
 * call play loops on state init, stop loops on state exit
 */
class SoundPlayer {
private:
	SoundPlayer();
	~SoundPlayer();

	const std::string m_BASE_PATH = "./Assets/Audio/";
	const std::string m_SFX_PATH = m_BASE_PATH + "SFX/";
	const std::string m_MUSIC_PATH = m_BASE_PATH + "Music/";

public:
	static SoundPlayer instance;

	static void stopAll();

	/**
	*
	* @param m_subpath
	 * @param sfxPath [out]
	 * @param musicPath [out].
	 */
	friend void __getPath(std::string subpath, std::string& sfxPath, std::string& musicPath);

	friend void __loadAudio(const std::string& subpath);

	/**
	 * Used for stuff like button click sound.
	 * 
	 * should not have music
	 */
	class GlobalAudio {
	private:
		GlobalAudio();
		~GlobalAudio();

		const std::string m_subpath = "ui/";

	public:
		static GlobalAudio& getInstance();

		void playSfxClick();
	};

	/**
	 * All menu audio goes here.
	 */
	class MenuAudio {
	private:
		MenuAudio();
		~MenuAudio();

		const std::string m_subpath = "menu/";

		std::vector<std::string> menuLoopRefs;
		std::vector<std::string> levelSelectLoopRefs;

	public:
		static MenuAudio& getInstance();

		void playLoopMenu();

		//void playLoopLevelSelect();
	};


	class GameAudio {
	private:
		GameAudio();
		~GameAudio();

		const std::string m_subpath = "gameplay/";

	public:
		static GameAudio& getInstance();

		void playLoop();
	};


	class CombatAudio {
	private:
		CombatAudio();
		~CombatAudio();

		const std::string m_subpath = "combat/";

		static constexpr int m_NUM_BATTLE_LOOPS = 3;
		std::vector<std::string> m_battleLoopRefs{};

		static constexpr int m_NUM_HURT_SFX = 13;
		std::vector<std::string> m_hurtSfxRefs{};

		static constexpr int m_NUM_KNIFE_SFX = 2;
		std::vector<std::string> m_knifeSfxRefs{};

		static constexpr int m_NUM_PUNCH_SFX = 4;
		std::vector<std::string> m_punchSfxRefs{};

		static constexpr int m_NUM_ANIMALS = 13;		// 12 zodiac animals + nian
		std::unordered_map<std::string, std::vector<std::string>> m_animalSfxRef{};

	public:
		static CombatAudio& getInstance();

		void playLoop();

		//void playSfxVictory();

		//void playSfxDeath();

		void playSfxAnimal(const std::string& animal);

		void playSfxHealth();

		//void playSfxHeartbeat();

		void playSfxHurt();

		void playSfxKnife();

		void playSfxPowerup();

		void playSfxPunch();

		void playSfxInvalid();

		/**
		 * !TODO: get more assets. currently only supports water.
		 *
		 * \param element
		 */
		//void playSfxElement(Element element);
	};
};

