/* Start Header ************************************************************************/
/*!
\file event.h
\author Poh Jing Seng, jingseng.poh, 2301363
\par jingseng.poh\@digipen.edu
\date 28 feb 2024
\brief event manager for the quicktime events during combat
/*
Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/



#pragma once

#include "Pch.h"

/**
 * update `eKeyToStr` too and ensure that the image assets follow this format:
 * "./Assets/keys/keyboard_<key>.png"
 * "./Assets/keys/keyboard_<key>_outline.png"
 *
 * other places to udpate:
 * Event::spamKey -> switch case
 */
enum EVENT_KEYS {
	E,
	Q,
	SPACE,
	NUM_EVENT_KEYS
};

enum EVENT_TYPES {
	SPAM_KEY,
	OSCILLATING_TIMER,
	MULTI_CLICK,
	TYPING,
	ORANGE_THROWING,
	NUM_EVENT_TYPES,
	NONE_EVENT_TYPE,
};

/**
 * event is over only when event results is not `NONE_EVENT_RESULTS`.
 */
enum EVENT_RESULTS {
	NONE_EVENT_RESULTS,
	SUCCESS,
	FAILURE,
	CUSTOM_MULTIPLIER,  // used for timed events
	NUM_EVENT_RESULTS
};

enum class INNER_STATES {
	ON_ENTER,
	ON_UPDATE,
	ON_NEXT,	// redo action in update
	ON_EXIT
};


class Event {
private:
	static Event* m_instance;
	EVENT_TYPES m_activeEvent = EVENT_TYPES::NONE_EVENT_TYPE;
	EVENT_RESULTS m_eventResult = NONE_EVENT_RESULTS;
	bool m_isRenderingEventResult = false;  // is render success/failure animation
	static constexpr double m_EVENT_RESULT_DURATION = 1;  // event success/failure animation duration (seconds)

	int m_elapsedTimeMs = 0;
	int m_totalElapsedMs = 0;
	bool m_useOutline = true;
	/*how many milliseconds before key type changes*/
	static constexpr int m_changeMs = 100;

	int m_mouseX = 0, m_mouseY = 0, m_prevMouseX = 0, m_prevMouseY = 0;

	/*spam key vars*/
	EVENT_KEYS m_spamKeyChoice = EVENT_KEYS::E;
	static constexpr int m_SPAM_KEY_TIMEOUT_MS = 5000;
	int _spamkeyTimeoutMs = m_SPAM_KEY_TIMEOUT_MS;
	static constexpr float m_minSize = 100;
	static constexpr float m_targetSize = 200;
	float m_size = m_minSize;
	float m_spamkeyX;
	float m_spamkeyY;
	// positive rate of change per click
	static constexpr float m_PROC = 10;
	float m_proc = m_PROC;
	// negative rate of change per second
	const float m_nroc = 50;

	/*oTimer vars*/
	INNER_STATES m_oTimerEventState = INNER_STATES::ON_ENTER;
	static constexpr int m_OTIMER_TIMEOUT_MS = 5000;
	int m_oTimerTimeoutMs = m_OTIMER_TIMEOUT_MS;
	float m_oTimerOpacity = 1.f;		// percentage
	float m_oTimerTimeBeforeFadeOut = 0.5f;	// seconds
	float m_oTimerFadeOutDuration = 0.5f;	// seconds

	float m_barWidth = 0.f;
	float m_barHeight = 0.f;
	float m_barX = 0.f;
	float m_barY = 0.f;

	// pi -> power indicator
	bool m_piMoving = true;
	float m_piWidth = 0.f;
	float m_piHeight = 0.f;
	float m_piX = 0.f;
	float m_piY = 0.f;
	float m_piVelocity = 0.f;
	float m_piMaxVelocity = 1000.f;
	//float __secondsToReachMaxVelocity = 0.5f;
	// note: we are not using time based but distance based. see cpp file in constructor
	// time based formula:
	// 
	// acceleration = (final velocity - initial velocity) / final time(seconds) - initial time(seconds)
	// 
	float m_piAcc;// = (m_piMaxVelocity - m_piVelocity) / __secondsToReachMaxVelocity;

	/*multi click vars*/
	struct MultiClickObject {
		float x;
		float y;
		float radius;
		bool alive;
		float timeSinceChange;	// for blinking effect. changing assets light and dark
		bool blink;
	};

	static constexpr int m_MULTICLICK_TIMEOUT_MS = 5000;
	int m_multiClickTimeoutMs = m_MULTICLICK_TIMEOUT_MS;
	int m_mcoHits = 0;
	int m_mcoMisses = 0;
	int m_mcoDisplayHits = 0;
	int m_maxMcoHits = 10;
	static constexpr int m_SPAWN_OFFSET_X = 200;		// dont spawn within 200 pixels of border
	static constexpr int m_SPAWN_OFFSET_Y = 200;
	std::vector<MultiClickObject> m_multiClickObjectsts;
	// mco -> multi click object
	static constexpr int m_MCO_COUNT = 3;
	static constexpr float m_MCO_RADIUS = 25.F;
	float m_mcoRadius = m_MCO_RADIUS;
	// how long before transitioning to another display state (light vs dark) for blinking effect
	static constexpr float m_mcoBlinkDuration = 0.25f;
	static constexpr float m_mcoTransitionTime = 1.f;
	bool m_mcoIsTransitioningOut = false;

	/*typing event vars*/
	static constexpr int m_NUM_TYPING_EVENT_WORDS = 41;
	// ^[a-z]+$ only (regex)
	const std::array<std::string, m_NUM_TYPING_EVENT_WORDS> m_wordlist{
		"nian",
		"dragon",
		"angpao",
		"oranges",
		"caishenye",
		"abundance",
		"ancestors",
		"auspicious",
		"bamboo",
		"blessings",
		"bokchoy",
		"bainian",
		"dimsum",
		"dumplings",
		"emperor",
		"evilspirit",
		"family",
		"fortune",
		"fireworks",
		"firecrackers",
		"fullmoon",
		"gold",
		"kungfu",
		"panda",
		"goodluck",
		"gratitude",
		"fatchoy",
		"incense",
		"jade",
		"lantern",
		"festival",
		"longevity",
		"lunar",
		"panda",
		"poetry",
		"qipao",
		"reunion",
		"spirits",
		"season",
		"tangyuan",
		"zhongzi"
	};

	// determines whether to get a new word.
	// when user is done typing current word, then set this to true
	std::string m_currentWord;	// current word used
	static constexpr float m_charGap = 10.f;		// gap between characters, in screen pos
	std::vector<std::pair<char, bool>> m_typed;
	static constexpr float m_typingTransitionTime = 1.f;		// time taken in seconds to transition out
	INNER_STATES m_typingState = INNER_STATES::ON_ENTER;
	static constexpr int m_TYPING_TIMEOUT_MS = 5000;
	int m_typingTimeoutMs = m_TYPING_TIMEOUT_MS;			// time before typing event ends
	int m_wordsCompleted = 0;			// words player managed to type before timeends
	static constexpr int m_typingMaxScore = 5;

	/*demon event vars*/
	static constexpr int m_ORANGE_TIMEOUT_MS = 5000;
	int m_orangeEventTimeoutMs = m_ORANGE_TIMEOUT_MS;
	static constexpr int m_afterEventDisplayTimeoutMs = 1000;
	INNER_STATES m_orangeState = INNER_STATES::ON_ENTER;
	static constexpr int m_SPAWN_INTERVAL_MS = 1000;
	static constexpr float m_ORANGE_RADIUS = 30.f;
	float m_orangeRadius = m_ORANGE_RADIUS;
	static constexpr float m_ORANGE_SPEED = 500.f;
	static constexpr float m_ORANGE_GRAVITY = 15.f;
	static constexpr float m_X_RESISTANCE = 1.f;		// how much of speed lost per second
	static constexpr float m_SPEED_LIMIT = 20.f;
	static constexpr float m_ENERGY_KEPT_BOUNCING = 0.7f;
	static constexpr float m_ORANGE_BORDER_PADDING = 50.f;

	static constexpr float m_DEMON_MIN_SPEED = 25.f;
	static constexpr float m_DEMON_MAX_SPEED = 200.f;
	static constexpr float m_DEMON_RADIUS = 50.f;

	struct Orange {
		float x;				// pos in screen coords
		float y;				// pos in screen coords
		AEVec2 vel;
		float radius;

		bool isHeld;
	};

	struct Demon {
		float x;
		float y;
		AEVec2 vel;
		float radius;
		bool m_isActive;
	};

	Orange m_orangeObj{ 0 };
	static constexpr int m_NUM_DEMONS = 7;
	std::array<Demon, m_NUM_DEMONS> m_demons{ 0 };


	/*ctor and methods*/
	Event();

	void _updateTime(double dt);
	void _resetTime();
	void _resetState();

	/**
	 * used for timed events.
	 * eg. multiClick,
	 *
	 * use milliseconds as args
	 *
	 */
	void _renderTimer(int elapsedTimeMs, int timeoutMs);

	void _showEventSpamKeyResult(float screenX, float screenY);

	/*events*/

	/**
	 * renders spamkey event.
	 *
	 * \param screenX screen x position to render
	 * \param screenY screen y position to render
	 * \param key1
	 * \param useMultipleKeys
	 * \param key2
	 */
	void _spamKeyEventUpdate(EVENT_RESULTS& result, double dt, EVENT_KEYS key = EVENT_KEYS::E);
	void _spamKeyEventRender();

	/**
	 * renders an oscillating timer event.
	 * aka swing meter, timing bar, power guage
	 *
	 */
	void _oscillatingTimerEventUpdate(EVENT_RESULTS& result, double dt, EVENT_KEYS key = EVENT_KEYS::SPACE);
	void _oscillatingTimerEventRender();

	/**
	 * .
	 *
	 * \param
	 * \param timeout time in seconds for user to click
	 */
	void _multiClickEventUpdate(EVENT_RESULTS& result, double dt);
	void _multiClickEventRender();

	/**
	 * @brief	typing event.
	 *
	 * IMPORTANT: update and render must both be called!
	 * update function does not reset state. only render does
	 *
	 * if you want to use update without render, you must add reset state in exit
	 *
	 * this is to prevent rendering the wrong thing for even 1 frame
	 */
	void _typingEventUpdate(EVENT_RESULTS& result, double dt);
	void _typingEventRender();

	void _orangeEventUpdate(EVENT_RESULTS& result, double dt);
	void _orangeEventRender();

public:
	// output variable for event multiplier
	float m_eventMultiplier = 1.f;
	float m_maxMultiplier = 1.5f;
	float m_minMultiplier = 0.2f;
	int m_eventMultiplierPrecision = 2;

	~Event();

	static Event* getInstance();
	EVENT_TYPES getActiveEvent();
	/**
	 * used to trigger event.
	 *
	 * \param event
	 * \param dt
	 * \param screenX
	 * \param screenY
	 * \param key
	 * \param timeout
	 * \returns if setting active event is successful
	 */
	bool setActiveEvent(EVENT_TYPES event);

	/**
	 * Used to start a random event.
	 *
	 */
	void startRandomEvent();

	void init();

	void update(EVENT_RESULTS& result, double dt, EVENT_KEYS spamkey = EVENT_KEYS::E, EVENT_KEYS oTimerKey = EVENT_KEYS::SPACE);

	void render();
};
