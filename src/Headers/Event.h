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


// !TODO: jspoh add difficulty config

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
	static Event* _instance;
	EVENT_TYPES _activeEvent = EVENT_TYPES::NONE_EVENT_TYPE;
	EVENT_RESULTS _eventResult = NONE_EVENT_RESULTS;
	bool _isRenderingEventResult = false;  // is render success/failure animation
	double _eventResultDuration = 1;  // event success/failure animation duration (seconds)

	int _elapsedTimeMs = 0;
	int _totalElapsedMs = 0;
	bool _useOutline = true;
	/*how many milliseconds before key type changes*/
	const int _changeMs = 100;

	int _mouseX = 0, _mouseY = 0, _prevMouseX = 0, _prevMouseY = 0;

	/*spam key vars*/
	EVENT_KEYS _spamKeyChoice = EVENT_KEYS::E;
	const int _spamkeyTimeoutMs = 5000;
	const float _minSize = 100;
	const float _targetSize = 200;
	float _size = _minSize;
	float _spamkeyX = 100.f;
	float _spamkeyY = 100.f;
	// positive rate of change per click
	const float proc = 10;
	// negative rate of change per second
	const float nroc = 50;

	/*oTimer vars*/
	INNER_STATES oTimerEventState = INNER_STATES::ON_ENTER;
	const int _oTimerTimeoutMs = 5000;
	float _oTimerOpacity = 1.f;		// percentage
	float _oTimerTimeBeforeFadeOut = 0.5f;	// seconds
	float _oTimerFadeOutDuration = 0.5f;	// seconds

	float _barWidth = 0.f;
	float _barHeight = 0.f;
	float _barX = 0.f;
	float _barY = 0.f;

	// pi -> power indicator
	bool _piMoving = true;
	float _piWidth = 0.f;
	float _piHeight = 0.f;
	float _piX = 0.f;
	float _piY = 0.f;
	float _piVelocity = 0.f;
	float _piMaxVelocity = 1000.f;
	//float __secondsToReachMaxVelocity = 0.5f;
	// note: we are not using time based but distance based. see cpp file in constructor
	// time based formula:
	// 
	// acceleration = (final velocity - initial velocity) / final time(seconds) - initial time(seconds)
	// 
	float _piAcc;// = (_piMaxVelocity - _piVelocity) / __secondsToReachMaxVelocity;

	/*multi click vars*/
	struct MultiClickObject {
		float x;
		float y;
		float radius;
		bool alive;
		float timeSinceChange;	// for blinking effect. changing assets light and dark
		bool blink;
	};

	const int _multiClickTimeoutMs = 5000;
	int _mcoHits = 0;
	int _mcoMisses = 0;
	int _mcoDisplayHits = 0;
	int _maxMcoHits = 10;
	static constexpr int spawnOffsetX = 200;		// dont spawn within 200 pixels of border
	static constexpr int spawnOffsetY = 200;
	std::vector<MultiClickObject> _multiClickObjects;
	// mco -> multi click object
	const int _mcoCount = 3;
	const float _mcoRadius = 25.f;
	// how long before transitioning to another display state (light vs dark) for blinking effect
	const float _mcoBlinkDuration = 0.25f;
	const float _mcoTransitionTime = 1.f;
	bool _mcoIsTransitioningOut = false;

	/*typing event vars*/
	const std::array<std::string, 5> _wordlist{
		"nian",
		"dragon",
		"angpao",
		"oranges",
		"caishenye"
	};
	// determines whether to get a new word.
	// when user is done typing current word, then set this to true
	std::string _currentWord;	// current word used
	const float _charGap = 10.f;		// gap between characters, in screen pos
	std::vector<std::pair<char, bool>> _typed;
	const float _typingTransitionTime = 1.f;		// time taken in seconds to transition out
	INNER_STATES _typingState = INNER_STATES::ON_ENTER;
	const int _typingTimeoutMs = 5000;			// time before typing event ends
	int _wordsCompleted = 0;			// words player managed to type before timeends
	const int _typingMaxScore = 5;

	/*demon event vars*/
	static constexpr int _orangeEventTimeoutMs = 5000;
	static constexpr int _afterEventDisplayTimeoutMs = 1000;
	INNER_STATES _orangeState = INNER_STATES::ON_ENTER;
	static constexpr int spawnIntervalMs = 1000;
	static constexpr float _orangeRadius = 30.f;
	static constexpr float _orangeSpeed = 500.f;
	static constexpr float _orangeGravity = 15.f;
	static constexpr float _xResistance = 1.f;		// how much of speed lost per second
	static constexpr float _speedLimit = 20.f;
	static constexpr float _energyKeptBouncing = 0.7f;

	static constexpr float _demonMinSpeed = 25.f;
	static constexpr float _demonMaxSpeed = 200.f;
	static constexpr float _demonRadius = 50.f;

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
		bool isActive;
	};

	Orange _orangeObj{ 0 };
	static constexpr int NUM_DEMONS = 7;
	std::array<Demon, NUM_DEMONS> demons{ 0 };


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
	float eventMultiplier = 1.f;
	float maxMultiplier = 1.5f;
	float minMultiplier = 0.2f;
	int eventMultiplierPrecision = 2;

	~Event();

	static Event* getInstance();

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

	/**
	 * put this in update loop. use `setActiveEvent` to trigger events
	 *
	 * warning: deprecated. to be removed
	 *
	 */
	void updateRenderLoop(EVENT_RESULTS& result, double dt, EVENT_KEYS spamkey = EVENT_KEYS::E, EVENT_KEYS oTimerKey = EVENT_KEYS::SPACE);

	void update(EVENT_RESULTS& result, double dt, EVENT_KEYS spamkey = EVENT_KEYS::E, EVENT_KEYS oTimerKey = EVENT_KEYS::SPACE);

	void render();
};
