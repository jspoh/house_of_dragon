#include "AEEngine.h"
#include "Event.h"
#include "../utils/utils.h"
#include <unordered_map>
#include <string>
#include <iostream>
#include <vector>

Event* Event::_instance = nullptr;

static std::unordered_map<EVENT_KEYS, std::string> eKeyToStr = {
	{E, "E"},
	{Q, "Q"},
	{SPACE, "SPACE"}
};


Event::Event() {
	for (std::pair<EVENT_KEYS, std::string> map : eKeyToStr) {
		bool success1 = Draw::getInstance()->registerTexture("key_" + map.second, "./Assets/keys/keyboard_" + map.second + ".png");
		bool success2 = Draw::getInstance()->registerTexture("keyoutline_" + map.second, "./Assets/keys/keyboard_" + map.second + "_outline.png");
		if (!success1 || !success2) {
			std::cerr << "Failed to load asset in Event constructor\n";
			//exit(2);
		}
	}
	Draw::getInstance()->registerTexture("pass", "./Assets/flairs/flair_circle_red_8.png");
	Draw::getInstance()->registerTexture("fail", "./Assets/flairs/flair_disabled_cross.png");
}

Event::~Event() {

}

Event* Event::getInstance() {
	if (_instance == nullptr) {
		_instance = new Event();
	}
	return _instance;
}

void Event::startRandomEvent() {
	// start a random quicktime event
	double time;
	AEGetTime(&time);
	srand(static_cast<unsigned int>(time));
	EVENT_TYPES e = static_cast<EVENT_TYPES>(rand() % NUM_EVENT_TYPES);
	e = EVENT_TYPES::SPAM_KEY;  // hardcoded for now as we dont have multiple quicktime events yet
	Event::getInstance()->setActiveEvent(e);
}

bool Event::setActiveEvent(EVENT_TYPES e) {
	if (_activeEvent == EVENT_TYPES::NONE_EVENT_TYPES) {
		_activeEvent = e;
		return true;
	}
	std::cerr << "An event is already running!\n";
	return false;
}

void Event::updateLoop(EVENT_RESULTS& result, double dt, float screenX, float screenY, EVENT_KEYS key, double timeout) {
	switch (_activeEvent) {
	case EVENT_TYPES::NONE_EVENT_TYPES:
		break;
	case EVENT_TYPES::SPAM_KEY:
		_spamKey(result, dt, screenX, screenY, key, timeout);
		break;
	case EVENT_TYPES::OSCILLATING_TIMER:
		_oscillatingTimer(result, dt, key, timeout);
		break;
	case EVENT_TYPES::CLICK_TIMER:
		_clickTimer(result, dt, key, timeout);
		break;
	default:
		std::cerr << "Event::updateLoop reached end of switch case\n";
		break;
	}
}



/*private*/

void Event::_resetState() {
	_activeEvent = EVENT_TYPES::NONE_EVENT_TYPES;
	_elapsedTimeMs = 0;
	_totalElapsedMs = 0;
	_useOutline = true;
	_size = _minSize;
	_isRenderingEventResult = false;
}

void Event::_updateTime(double dt) {
	// integer delta time
	int idt = static_cast<int>(dt * 1000);
	_elapsedTimeMs += idt;
	_totalElapsedMs += idt;
}

void Event::_showEventResult(EVENT_RESULTS& result, double dt, float screenX, float screenY, double timeout) {
	/*event result duration over*/
	if (_elapsedTimeMs >= _eventResultDuration * 1000) {
		result = _eventResult;  // update result passed by caller so that they know event is over
		_resetState();
		return;
	}

	if (_eventResult == EVENT_RESULTS::SUCCESS) {
		// draw success
		Draw::getInstance()->texture("pass", screenX, screenY, _minSize, _minSize, 1.0f, Color{ 0,1,0,0 });
	}
	else if (_eventResult == EVENT_RESULTS::FAILURE) {
		// draw failure
		Draw::getInstance()->texture("fail", screenX, screenY, _minSize, _minSize);
	}
}

void Event::_spamKey(EVENT_RESULTS& result, double dt, float screenX, float screenY, EVENT_KEYS key, double timeout) {
	_updateTime(dt);
	std::cout << _totalElapsedMs << "\n";

	/*logic*/
	// if event is over, is rendering event result
	if (_isRenderingEventResult) {
		_showEventResult(result, dt, screenX, screenY, timeout);
		return;
	}

	// check if user succeeded on spamming key
	if (_size >= _targetSize) {
		_eventResult = EVENT_RESULTS::SUCCESS;
		_elapsedTimeMs = 0;
		_isRenderingEventResult = true;
		return;
	}
	else if (_totalElapsedMs >= timeout * 1000) {
		_eventResult = EVENT_RESULTS::FAILURE;
		_elapsedTimeMs = 0;
		_isRenderingEventResult = true;
		return;
	}

	u8 aevk = AEVK_E;
	switch (key) {
	case E:
		aevk = AEVK_E;
		break;
	case Q:
		aevk = AEVK_Q;
		break;
	case SPACE:
		aevk = AEVK_SPACE;
		break;
	default:
		std::cerr << "Key was not registered in EVENT_KEYS!\n";
		//exit(3);
		break;
	}

	if (AEInputCheckTriggered(aevk)) {
		_size += proc;
	}
	_size -= static_cast<float>(nroc * dt);
	_size = _size < _minSize ? _minSize : _size;

	/*rendering*/
	//std::cout << _elapsedTimeMs << "\n";

	// key in string format
	std::string skey = eKeyToStr.find(key)->second;

	if (_elapsedTimeMs > _changeMs) {
		_useOutline = !_useOutline;
		_elapsedTimeMs = 0;
	}

	//Draw::getInstance()->texture("keyoutline_" + skey, screenX, screenY, _targetSize, _targetSize);
	if (_useOutline) {
		Draw::getInstance()->texture("keyoutline_" + skey, screenX, screenY, _size, _size);
	}
	else {
		Draw::getInstance()->texture("key_" + skey, screenX, screenY, _size, _size);
	}
}

void Event::_oscillatingTimer(EVENT_RESULTS& result, double dt, EVENT_KEYS key, double timeout) {
	_updateTime(dt);
}

void Event::_clickTimer(EVENT_RESULTS& result, double dt, EVENT_KEYS key, double timeout) {
	_updateTime(dt);
}
