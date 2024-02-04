#pragma once

enum EVENT_KEYS {
	E,
	Q,
	SPACE,
	MAX_EVENT_KEYS
};


class Event {
private:
	Event* _instance;

	Event();
	~Event();

public:
	Event* getInstance();

	/**
	 * renders spamkey event.
	 * 
	 * \param screenX screen x position to render
	 * \param screenY screen y position to render
	 * \param key1
	 * \param useMultipleKeys
	 * \param key2
	 */
	void spamKey(float screenX, float screenY, EVENT_KEYS key1 = EVENT_KEYS::E, bool useMultipleKeys = true, EVENT_KEYS key2 = EVENT_KEYS::Q);

	/**
	 * renders an oscillating timer event.
	 * 
	 * \param key key to use
	 * \param cycles oscillation cycles before timer is invalid
	 */
	void oscillatingTimer(EVENT_KEYS key = EVENT_KEYS::SPACE, int cycles = 5);

	/**
	 * .
	 * 
	 * \param key key to use
	 * \param time time in seconds for user to click
	 */
	void clickTimer(EVENT_KEYS key = EVENT_KEYS::E, double time = 1);
};
