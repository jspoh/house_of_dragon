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

#include "AEEngine.h"
#include "utils.h"
#include <unordered_map>
#include <string>
#include <iostream>
#include <vector>
#include <sstream>

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
	NUM_EVENT_TYPES,
	NONE_EVENT_TYPES,
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

struct MultiClickObject {
	float x;
	float y;
	float radius;
	float opacity;	// 0 to 1
};


class Event {
private:
	static Event* _instance;
	EVENT_TYPES _activeEvent = EVENT_TYPES::NONE_EVENT_TYPES;
	EVENT_RESULTS _eventResult = NONE_EVENT_RESULTS;
	bool _isRenderingEventResult = false;  // is render success/failure animation
	double _eventResultDuration = 1;  // event success/failure animation duration (seconds)

	int _elapsedTimeMs = 0;
	int _totalElapsedMs = 0;
	bool _useOutline = true;
	/*how many milliseconds before key type changes*/
	const int _changeMs = 100;

	/*spam key vars*/
	const float _spamkeyTimeout = 5.f;
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
	const float _oTimerTimeout = 10.f;
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
	const float _multiClickDuration = 5.f;
	int _multiClickHits = 0;
	int _multiClickMisses = 0;
	std::vector<MultiClickObject> _multiClickObjects;
	// mco -> multi click object
	int _mcoCount = 1;
	const float _mcoRadius = 25.f;

	Event();

	void _updateTime(double dt);
	void _resetTime();
	void _resetState();

	void _showEventSpamKeyResult(EVENT_RESULTS& result, double dt, float screenX, float screenY, double timeout);

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
	void _spamKey(EVENT_RESULTS& result, double dt, EVENT_KEYS key = EVENT_KEYS::E);

	/**
	 * renders an oscillating timer event.
	 * aka swing meter, timing bar, power guage
	 * 
	 * \param key key to use
	 * \param timeout
	 */
	void _oscillatingTimer(EVENT_RESULTS& result, double dt, EVENT_KEYS key = EVENT_KEYS::SPACE);

	/**
	 * .
	 * 
	 * \param key key to use
	 * \param timeout time in seconds for user to click
	 */
	void _multiClick(EVENT_RESULTS& result, double dt);

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
	 */
	void updateRenderLoop(EVENT_RESULTS& result, double dt, EVENT_KEYS spamkey = EVENT_KEYS::E, EVENT_KEYS oTimerKey = EVENT_KEYS::SPACE);
};
