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

	const std::string BASE_PATH = "./Assets/Audio/";
	const std::string SFX_PATH = BASE_PATH + "SFX/";
	const std::string MUSIC_PATH = BASE_PATH + "Music/";

public:
	static SoundPlayer instance;

	static void stopAll();

	/**
	*
	* @param subpath
	 * @param sfxPath [out]
	 * @param musicPath [out].
	 */
	friend void __getPath(std::string subpath, std::string& sfxPath, std::string& musicPath);

	/**
	 * Used for stuff like button click sound.
	 * 
	 * should not have music
	 */
	class GlobalAudio {
	private:
		GlobalAudio();
		~GlobalAudio();

		const std::string subpath = "ui/";

	public:
		static GlobalAudio& getInstance();

		void playSfxClick();
	};

	/**
	 * All menu audio goes here.
	 */
	//class MenuAudio {
	//private:
	//	MenuAudio();
	//	~MenuAudio();

	//	const std::string subpath = "menu/";

	//public:
	//	static MenuAudio instance;

	//	void playLoopMenu();

	//	void stopLoopMenu();

	//	void playLoopLevelSelect();

	//	void stopLoopLevelSelect();
	//};


	//class GameAudio {
	//private:
	//	GameAudio();
	//	~GameAudio();

	//	const std::string subpath = "gameplay/";

	//public:
	//	static GameAudio instance;

	//	void playLoop();

	//	void stopLoop();
	//};


	//class CombatAudio {
	//private:
	//	CombatAudio();
	//	~CombatAudio();

	//	const std::string subpath = "combat/";

	//public:
	//	static CombatAudio instance;

	//	void playLoop();

	//	void stopLoop();

	//	void playSfxDeathSfx();

	//	void playSfxAngryDemonicScream();

	//	void playSfxAnimal(std::string ref);

	//	void playSfxGrowl();

	//	void playSfxHealth();

	//	void playSfxHeartbeat();

	//	void playSfxHurt();

	//	void playSfxKnife();

	//	void playSfxPowerup();

	//	void playSfxPunch();

	//	void playSfxInvalid();

	//	void playSfxScifiGunshot();

	//	/**
	//	 * !TODO: get more assets. currently only supports water.
	//	 *
	//	 * \param element
	//	 */
	//	void playSfxElement(Element element);
	//};
};

