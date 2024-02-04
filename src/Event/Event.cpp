#include "AEEngine.h"
#include "Event.h"


Event::Event() {

}

Event::~Event() {

}

Event* Event::getInstance() {
	if (_instance == nullptr) {
		_instance = new Event();
	}
	return _instance;
}

void Event::spamKey(float screenX, float screenY, EVENT_KEYS key1, bool useMultipleKeys, EVENT_KEYS key2) {

}

void oscillatingTimer(EVENT_KEYS key, int cycles) {

}

void clickTimer(EVENT_KEYS key, double time) {

}
