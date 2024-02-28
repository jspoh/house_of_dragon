/* Start Header ************************************************************************/
/*!
\file event.cpp
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



#include "AEEngine.h"
#include "Event.h"
#include "utils.h"
#include <unordered_map>
#include <string>
#include <iostream>
#include <vector>
#include <sstream>

Event* Event::_instance = nullptr;

namespace {
	static std::unordered_map<EVENT_KEYS, std::string> eKeyToStr = {
		{E, "E"},
		{Q, "Q"},
		{SPACE, "SPACE"}
	};

	std::vector<std::string> meshReferences = {
		"oTimerMesh",
	};
}


Event::Event() {
	for (std::pair<EVENT_KEYS, std::string> map : eKeyToStr) {
		bool success1 = RenderHelper::getInstance()->registerTexture("key_" + map.second, "./Assets/keys/keyboard_" + map.second + ".png");
		bool success2 = RenderHelper::getInstance()->registerTexture("keyoutline_" + map.second, "./Assets/keys/keyboard_" + map.second + "_outline.png");
		if (!success1 || !success2) {
			std::cerr << "Failed to load asset in Event constructor\n";
			//exit(2);
		}
	}
	RenderHelper::getInstance()->registerTexture("pass", "./Assets/flairs/flair_circle_red_8.png");
	RenderHelper::getInstance()->registerTexture("fail", "./Assets/flairs/flair_disabled_cross.png");

	// register custom mesh for otimer event
	AEGfxMeshStart();
	AEGfxTriAdd(
		-0.5f, -0.5f, 0x00FF0000, 0.0f, 0.0f,	// bottom left
		0.f, -0.5f, 0x0000FF00, 1.0f, 0.0f,		// bottom mid
		-0.5f, 0.5f, 0x00FF0000, 0.5f, 1.0f		// top left
	);
	AEGfxTriAdd(
		0.f, -0.5f, 0x0000FF00, 1.0f, 0.0f,		// bottom mid
		0.f, 0.5f, 0x0000FF00, 1.0f, 1.0f,		// top mid
		-0.5f, 0.5f, 0x00FF0000, 0.5f, 1.0f		// top left
	);

	AEGfxTriAdd(
		-0.f, -0.5f, 0x0000FF00, 0.0f, 0.0f,	// bottom mid
		0.5f, -0.5f, 0x00FF0000, 1.0f, 0.0f,	// bottom right
		-0.f, 0.5f, 0x0000FF00, 0.5f, 1.0f		// top mid
	);
	AEGfxTriAdd(
		0.5f, -0.5f, 0x00FF0000, 1.0f, 0.0f,	// bottom right
		0.5f, 0.5f, 0x00FF0000, 1.0f, 1.0f,		// top right
		-0.f, 0.5f, 0x0000FF00, 0.5f, 1.0f		// top mid
	);
	AEGfxVertexList* oTimerMesh = AEGfxMeshEnd();

	RenderHelper::getInstance()->registerMeshByRef(meshReferences[0], oTimerMesh);

	/*init oTimer variables*/
	this->_barWidth = AEGfxGetWindowWidth() / 2.f;
	this->_barHeight = this->_barWidth / 30.f;
	this->_barX = AEGfxGetWindowWidth() / 2.f;
	this->_barY = 50.f;

	// pi -> power indicator
	this->_piWidth = this->_barWidth / 200.f;
	this->_piHeight = this->_barHeight * 1.5f;
	this->_piX = this->_barX - this->_barWidth / 2.f;
	this->_piY = this->_barY;

	/**
	 * distance based formula for acceleration:
	 * 
	 * acceleration = (final velocity^2 - initial velocity^2) / (2 * distance)
	 * 
	 */
	_piAcc = (_piMaxVelocity * _piMaxVelocity) / (2.f * (_barWidth / 2.f));
}

Event::~Event() {
	for (const std::string& ref : meshReferences) {
		RenderHelper::getInstance()->removeMeshByRef(ref);
	}
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
	EVENT_TYPES e = static_cast<EVENT_TYPES>((rand() % NUM_EVENT_TYPES));
	//e = EVENT_TYPES::SPAM_KEY;  // hardcoded for now as we dont have multiple quicktime events yet
	std::cout << "Random event: " << e << "\n";
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

void Event::updateRenderLoop(EVENT_RESULTS& result, double dt, EVENT_KEYS spamkey, EVENT_KEYS oTimerKey) {
	switch (_activeEvent) {
	case EVENT_TYPES::NONE_EVENT_TYPES:
		break;
	case EVENT_TYPES::SPAM_KEY:
		_spamKey(result, dt, spamkey);
		break;
	case EVENT_TYPES::OSCILLATING_TIMER:
		_oscillatingTimer(result, dt, oTimerKey);
		break;
	//case EVENT_TYPES::CLICK_TIMER:
	//	_multiClick(result, dt, spamkey);
	//	break;
	default:
		std::cerr << "Event::updateRenderLoop reached end of switch case\n";
		break;
	}
}



/*private*/

void Event::_resetState() {
	_activeEvent = EVENT_TYPES::NONE_EVENT_TYPES;
	_resetTime();
	_useOutline = true;
	_size = _minSize;
	_isRenderingEventResult = false;

	// oscillating timer
	this->_piX = this->_barX - this->_barWidth / 2.f;
	this->_piVelocity = 0.f;
	this->_oTimerOpacity = 1.f;
	this->_piMoving = true;
}

void Event::_resetTime() {
	_elapsedTimeMs = 0;
	_totalElapsedMs = 0;
}

void Event::_updateTime(double dt) {
	// integer delta time
	int idt = static_cast<int>(dt * 1000);
	_elapsedTimeMs += idt;
	_totalElapsedMs += idt;
}

void Event::_showEventSpamKeyResult(EVENT_RESULTS& result, double dt, float screenX, float screenY, double timeout) {
	/*event result duration over*/
	if (_elapsedTimeMs >= _eventResultDuration * 1000) {
		result = _eventResult;  // update result passed by caller so that they know event is over
		_resetState();
		return;
	}

	if (_eventResult == EVENT_RESULTS::SUCCESS) {
		// draw success
		RenderHelper::getInstance()->texture("pass", screenX, screenY, _minSize, _minSize, 1.0f, Color{ 0,1,0,0 });
	}
	else if (_eventResult == EVENT_RESULTS::FAILURE) {
		// draw failure
		RenderHelper::getInstance()->texture("fail", screenX, screenY, _minSize, _minSize);
	}
}

void Event::_spamKey(EVENT_RESULTS& result, double dt, EVENT_KEYS key) {
	_updateTime(dt);
	//std::cout << _totalElapsedMs << "\n";

	Point worldPos = stow(_spamkeyX, _spamkeyY);
	float worldX = worldPos.x;
	float worldY = worldPos.y;

	/*logic*/
	// if event is over, is rendering event result
	if (_isRenderingEventResult) {
		_showEventSpamKeyResult(result, dt, worldX, worldY, _spamkeyTimeout);
		return;
	}

	// check if user succeeded on spamming key
	if (_size >= _targetSize) {
		_eventResult = EVENT_RESULTS::SUCCESS;
		_elapsedTimeMs = 0;
		_isRenderingEventResult = true;
		return;
	}
	else if (_totalElapsedMs >= _spamkeyTimeout * 1000) {
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

	//RenderHelper::getInstance()->texture("keyoutline_" + skey, worldX, worldY, _targetSize, _targetSize);
	if (_useOutline) {
		RenderHelper::getInstance()->texture("keyoutline_" + skey, worldX, worldY, _size, _size);
	}
	else {
		RenderHelper::getInstance()->texture("key_" + skey, worldX, worldY, _size, _size);
	}
}

void Event::_oscillatingTimer(EVENT_RESULTS& result, double dt, EVENT_KEYS key) {
	_updateTime(dt);

	/*logic*/
	// check if timeout
	if (_totalElapsedMs >= _oTimerTimeout * 1000) {
		result = EVENT_RESULTS::FAILURE;
		_showEventSpamKeyResult(result, dt, _barX, _barY, _oTimerTimeout);
		_resetState();
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

	// check if event is over
	if (_piMoving) {
		if (AEInputCheckTriggered(aevk)) {
			_piMoving = false;
			_resetTime();	// using time to fade out

			// calculate multiplier
			const float piDistanceToCenter = abs(_barX - _piX);
			const float percentageMultiplier = ((_barWidth / 2.f) - piDistanceToCenter) / (_barWidth / 2.f);
			eventMultiplier = percentageMultiplier * maxMultiplier;
			eventMultiplier = precisionRound(eventMultiplier, eventMultiplierPrecision);
		}
	}
	
	/*rendering*/
	const std::string oTimerMesh = meshReferences[0];
	const Point barTranslation = stow(_barX, _barY);

	// power bar
	RenderHelper::getInstance()->rect(oTimerMesh,barTranslation.x, barTranslation.y, _barWidth, _barHeight, 0.f, Color{ 0,0,0,_oTimerOpacity }, _oTimerOpacity);

	Point piTranslation = stow(_piX, _piY);

	if (_piMoving) {
		// power indicator movement logic. accerlerates until center of bar, then decelerates
		// pi is left of or on the center of bar
		if (_piX <= _barX) {
			_piVelocity += static_cast<float>(_piAcc * dt);
		}
		// pi is right of the center of bar
		else {
			_piVelocity -= static_cast<float>(_piAcc * dt);
		}

		// cap velocity
		_piVelocity = _piVelocity > _piMaxVelocity ? _piMaxVelocity : _piVelocity;
		_piVelocity = _piVelocity < -_piMaxVelocity ? -_piMaxVelocity : _piVelocity;

		//std::cout << "Power indicator speed: " << _piVelocity << std::endl;
		_piX += static_cast<float>(_piVelocity * dt);

		// guards to ensure that pi does not go out of bar
		_piX = _piX < _barX - _barWidth / 2.f ? _barX - _barWidth / 2.f : _piX;
		_piX = _piX > _barX + _barWidth / 2.f ? _barX + _barWidth / 2.f : _piX;
	}

	// power indicator
	RenderHelper::getInstance()->rect(piTranslation.x, piTranslation.y, _piWidth, _piHeight, 0.f, Color{ 0.95f,0.95f,0.95f,_oTimerOpacity }, _oTimerOpacity);

	// multiplier result
	if (!_piMoving) {
		std::ostringstream oss;
		oss.precision(eventMultiplierPrecision);
		oss << std::fixed << eventMultiplier << "x";
		RenderHelper::getInstance()->text(oss.str(), _barX, _barY + 50.f);

		// fade out event
		if (_totalElapsedMs >= _oTimerTimeBeforeFadeOut * 1000) {
			// if invisible, reset state
			if (_oTimerOpacity <= 0.f) {
				result = EVENT_RESULTS::CUSTOM_MULTIPLIER;
				_resetState();
				return;
			}

			// start fading out
			float change = 1.f / _oTimerFadeOutDuration;
			_oTimerOpacity -= static_cast<float>(change * dt);
		}
	}
}

void Event::_multiClick(EVENT_RESULTS& result, double dt, EVENT_KEYS key) {
	_updateTime(dt);
}
