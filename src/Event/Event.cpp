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

#include "Pch.h"
#include "Event.h"

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

	std::vector<std::string> textureReferences;
}

// constructor
Event::Event() {
	//double time;
	//AEGetTime(&time);
	//srand(static_cast<unsigned int>(time));

	for (std::pair<EVENT_KEYS, std::string> map : eKeyToStr) {
		bool success1 = RenderHelper::getInstance()->registerTexture("key_" + map.second, "./Assets/Combat_UI/keyboard_" + map.second + ".png");
		bool success2 = RenderHelper::getInstance()->registerTexture("keyoutline_" + map.second, "./Assets/Combat_UI/keyboard_" + map.second + "_outline.png");
		if (!success1 || !success2) {
			std::cerr << "Failed to load asset in Event constructor\n";
			//exit(2);
		}
	}
	RenderHelper::getInstance()->registerTexture("circle", "./Assets/Combat_UI/flair_circle_red_8.png");
	textureReferences.push_back("circle");
	RenderHelper::getInstance()->registerTexture("fail", "./Assets/Combat_UI/flair_disabled_cross.png");
	textureReferences.push_back("fail");

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
	this->_barY = AEGfxGetWindowHeight() / 6.f;

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

	/*init multiclick variables and textures*/
	// multiclick targets
	RenderHelper::getInstance()->registerTexture("clickme_light", "./Assets/Combat_UI/clickme_light.png");
	textureReferences.push_back("clickme_light");
	RenderHelper::getInstance()->registerTexture("clickme_dark", "./Assets/Combat_UI/clickme_dark.png");
	textureReferences.push_back("clickme_dark");
	RenderHelper::getInstance()->registerTexture("noclick_light", "./Assets/Combat_UI/noclick_light.png");
	textureReferences.push_back("noclick_light");
	RenderHelper::getInstance()->registerTexture("noclick_dark", "./Assets/Combat_UI/noclick_dark.png");
	textureReferences.push_back("noclick_dark");
	// timers
	RenderHelper::getInstance()->registerTexture("timer_0", "./Assets/Combat_UI/timer/timer_0.png");
	textureReferences.push_back("timer_0");
	RenderHelper::getInstance()->registerTexture("timer_25", "./Assets/Combat_UI/timer/timer_25.png");
	textureReferences.push_back("timer_25");
	RenderHelper::getInstance()->registerTexture("timer_50", "./Assets/Combat_UI/timer/timer_50.png");
	textureReferences.push_back("timer_50");
	RenderHelper::getInstance()->registerTexture("timer_75", "./Assets/Combat_UI/timer/timer_75.png");
	textureReferences.push_back("timer_75");
	RenderHelper::getInstance()->registerTexture("timer_100", "./Assets/Combat_UI/timer/timer_100.png");
	textureReferences.push_back("timer_100");
	RenderHelper::getInstance()->registerTexture("ball", "./Assets/Combat_UI/ball.png");
	textureReferences.push_back("ball");
	RenderHelper::getInstance()->registerTexture("nian", "./Assets/Combat_UI/nian.png");
	textureReferences.push_back("nian");
}

Event::~Event() {
	for (const std::string& ref : meshReferences) {
		RenderHelper::getInstance()->removeMeshByRef(ref);
	}

	for (const std::string& ref : textureReferences) {
		RenderHelper::getInstance()->removeTextureByRef(ref);
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
	EVENT_TYPES e = static_cast<EVENT_TYPES>((rand() % NUM_EVENT_TYPES));
	//e = EVENT_TYPES::SPAM_KEY;  // hardcoded for testing
	//e = EVENT_TYPES::OSCILLATING_TIMER;  // hardcoded for testing
	//e = EVENT_TYPES::MULTI_CLICK;  // hardcoded for testing
	//e = EVENT_TYPES::TYPING;  // hardcoded for testing
	//e = EVENT_TYPES::ORANGE_THROWING;  // hardcoded for testing
	std::cout << "Random event: " << e << "\n";
	Event::getInstance()->setActiveEvent(e);
}

bool Event::setActiveEvent(EVENT_TYPES e) {
	if (_activeEvent == EVENT_TYPES::NONE_EVENT_TYPE) {
		_activeEvent = e;
		return true;
	}
	std::cerr << "An event is already running!\n";
	return false;
}

void Event::updateRenderLoop(EVENT_RESULTS& result, double dt, EVENT_KEYS spamkey, EVENT_KEYS oTimerKey) {
	AEInputGetCursorPosition(&_mouseX, &_mouseY);
	if (_prevMouseX == 0 || _prevMouseY == 0) {
		_prevMouseX = _mouseX;
		_prevMouseY = _mouseY;
	}

	switch (_activeEvent) {
	case EVENT_TYPES::NONE_EVENT_TYPE:
		break;
	case EVENT_TYPES::SPAM_KEY:
		_spamKeyEventUpdate(result, dt, spamkey);
		_spamKeyEventRender();
		break;
	case EVENT_TYPES::OSCILLATING_TIMER:
		_oscillatingTimerEventUpdate(result, dt, oTimerKey);
		_oscillatingTimerEventRender();
		break;
	case EVENT_TYPES::MULTI_CLICK:
		_multiClickEventUpdate(result, dt);
		_multiClickEventRender();
		break;
	case EVENT_TYPES::TYPING:
		_typingEventUpdate(result, dt);
		_typingEventRender();
		break;
	case EVENT_TYPES::ORANGE_THROWING:
		_orangeEventUpdate(result, dt);
		_orangeEventRender();
		break;
	default:
		std::cerr << "Event::updateRenderLoop reached end of switch case\n";
		break;
	}

	_prevMouseX = _mouseX;
	_prevMouseY = _mouseY;
}


void Event::update(EVENT_RESULTS& result, double dt, EVENT_KEYS spamkey, EVENT_KEYS oTimerKey) {
	AEInputGetCursorPosition(&_mouseX, &_mouseY);
	if (_prevMouseX == 0 || _prevMouseY == 0) {
		_prevMouseX = _mouseX;
		_prevMouseY = _mouseY;
	}

	switch (_activeEvent) {
	case EVENT_TYPES::NONE_EVENT_TYPE:
		break;
	case EVENT_TYPES::SPAM_KEY:
		_spamKeyEventUpdate(result, dt, spamkey);
		break;
	case EVENT_TYPES::OSCILLATING_TIMER:
		_oscillatingTimerEventUpdate(result, dt, oTimerKey);
		break;
	case EVENT_TYPES::MULTI_CLICK:
		_multiClickEventUpdate(result, dt);
		break;
	case EVENT_TYPES::TYPING:
		_typingEventUpdate(result, dt);
		break;
	case EVENT_TYPES::ORANGE_THROWING:
		_orangeEventUpdate(result, dt);
		break;
	default:
		std::cerr << "Event::update reached end of switch case\n";
		break;
	}

	_prevMouseX = _mouseX;
	_prevMouseY = _mouseY;
}

void Event::render() {

	switch (_activeEvent) {
	case EVENT_TYPES::NONE_EVENT_TYPE:
		break;
	case EVENT_TYPES::SPAM_KEY:
		_spamKeyEventRender();
		break;
	case EVENT_TYPES::OSCILLATING_TIMER:
		_oscillatingTimerEventRender();
		break;
	case EVENT_TYPES::MULTI_CLICK:
		_multiClickEventRender();
		break;
	case EVENT_TYPES::TYPING:
		_typingEventRender();
		break;
	case EVENT_TYPES::ORANGE_THROWING:
		_orangeEventRender();
		break;
	default:
		std::cerr << "Event::render reached end of switch case\n";
		break;
	}
}


/*private*/
void Event::_renderTimer(int elapsedTimeMs, int timeoutMs) {
	float x = AEGfxGetWindowWidth() * 0.925f;
	float y = AEGfxGetWindowHeight() * 0.11125f;
	Point world = stow(x, y);

	std::array<int, 5> thresholds = { 100,75,50,25,0 };

	int timeLeftPctg = static_cast<int>(ceil((static_cast<f32>(elapsedTimeMs) / timeoutMs) * 100));		// time left percentage

	for (const int t : thresholds) {
		if (timeLeftPctg >= t) {
			RenderHelper::getInstance()->texture("timer_" + std::to_string(100 - t), world.x, world.y);
			break;
		}
	}
}

void Event::_resetState() {
	_activeEvent = EVENT_TYPES::NONE_EVENT_TYPE;
	_resetTime();
	_useOutline = true;
	_size = _minSize;
	_isRenderingEventResult = false;

	// oscillating timer
	this->_piX = this->_barX - this->_barWidth / 2.f;
	this->_piVelocity = 0.f;
	this->_oTimerOpacity = 1.f;
	this->_piMoving = true;
	oTimerEventState = INNER_STATES::ON_ENTER;

	// multiclick
	_mcoHits = 0;
	_mcoMisses = 0;
	_mcoDisplayHits = 0;
	_multiClickObjects.clear();
	_mcoIsTransitioningOut = false;

	// typing event
	_currentWord.clear();
	_typed.clear();
	_typingState = INNER_STATES::ON_ENTER;
	_wordsCompleted = 0;

	// orange event
	_orangeState = INNER_STATES::ON_ENTER;
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

void Event::_showEventSpamKeyResult(float screenX, float screenY) {
	if (_eventResult == EVENT_RESULTS::SUCCESS) {
		// draw success
		RenderHelper::getInstance()->texture("circle", screenX, screenY, _minSize, _minSize, 1.0f, Color{ 0,1,0,0 });
	}
	else if (_eventResult == EVENT_RESULTS::FAILURE) {
		// draw failure
		RenderHelper::getInstance()->texture("fail", screenX, screenY, _minSize, _minSize);
	}
}

void Event::_spamKeyEventUpdate(EVENT_RESULTS& result, double dt, EVENT_KEYS key) {
	_updateTime(dt);
	_spamKeyChoice = key;

	//std::cout << _totalElapsedMs << "\n";

	Point worldPos = stow(_spamkeyX, _spamkeyY);
	//float worldX = worldPos.x;
	//float worldY = worldPos.y;

	/*logic*/

	// if event is over, is rendering event result
	if (_isRenderingEventResult) {
		/*event result duration over*/
		if (_elapsedTimeMs >= _eventResultDuration * 1000) {
			result = _eventResult;  // update result passed by caller so that they know event is over
			_resetState();
		}
		return;
	}

	// check if user succeeded on spamming key
	if (_size >= _targetSize) {
		_eventResult = EVENT_RESULTS::SUCCESS;
		_elapsedTimeMs = 0;
		_isRenderingEventResult = true;
		return;
	}
	else if (_totalElapsedMs >= _spamkeyTimeoutMs) {
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
	_size -= static_cast<f32>(nroc * dt);
	_size = _size < _minSize ? _minSize : _size;

	/*rendering*/
	//std::cout << _elapsedTimeMs << "\n";

	if (_elapsedTimeMs > _changeMs) {
		_useOutline = !_useOutline;
		_elapsedTimeMs = 0;
	}
}

void Event::_spamKeyEventRender() {

	_renderTimer(_totalElapsedMs, _spamkeyTimeoutMs);

	Point worldPos = stow(_spamkeyX, _spamkeyY);
	float worldX = worldPos.x;
	float worldY = worldPos.y;

	// if event is over, is rendering event result
	if (_isRenderingEventResult) {
		_showEventSpamKeyResult(worldX + camOffset.x, worldY + camOffset.y);
		return;
	}

	// key in string format
	std::string skey = eKeyToStr.find(_spamKeyChoice)->second;

	//RenderHelper::getInstance()->texture("keyoutline_" + skey, worldX, worldY, _targetSize, _targetSize);
	if (_useOutline) {
		RenderHelper::getInstance()->texture("keyoutline_" + skey, worldX, worldY, _size, _size);
	}
	else {
		RenderHelper::getInstance()->texture("key_" + skey, worldX, worldY, _size, _size);
	}
}

void Event::_oscillatingTimerEventUpdate(EVENT_RESULTS& result, double dt, EVENT_KEYS key) {
	_updateTime(dt);

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

	/*logic*/
	switch (oTimerEventState) {
	case INNER_STATES::ON_ENTER:
		oTimerEventState = INNER_STATES::ON_UPDATE;
		break;
	case INNER_STATES::ON_UPDATE:
		// check if timeout
		if (_totalElapsedMs >= _oTimerTimeoutMs) {
			oTimerEventState = INNER_STATES::ON_EXIT;
			eventMultiplier = 0;
			_elapsedTimeMs = 0;
			break;
		}

		// check if event is over
		if (AEInputCheckTriggered(aevk)) {
			_piMoving = false;
			_elapsedTimeMs = 0;	// using time to fade out

			// calculate multiplier
			const float piDistanceToCenter = abs(_barX - _piX);
			const float percentageMultiplier = ((_barWidth / 2.f) - piDistanceToCenter) / (_barWidth / 2.f);
			eventMultiplier = percentageMultiplier * maxMultiplier;
			eventMultiplier = precisionRound(eventMultiplier, eventMultiplierPrecision);
			oTimerEventState = INNER_STATES::ON_EXIT;
		}

		// power indicator movement logic. accerlerates until center of bar, then decelerates
		// pi is left of or on the center of bar
		if (_piX <= _barX) {
			_piVelocity += static_cast<f32>(_piAcc * dt);
		}
		// pi is right of the center of bar
		else {
			_piVelocity -= static_cast<f32>(_piAcc * dt);
		}

		// cap velocity
		_piVelocity = _piVelocity > _piMaxVelocity ? _piMaxVelocity : _piVelocity;
		_piVelocity = _piVelocity < -_piMaxVelocity ? -_piMaxVelocity : _piVelocity;

		//std::cout << "Power indicator speed: " << _piVelocity << std::endl;
		_piX += static_cast<f32>(_piVelocity * dt);

		// guards to ensure that pi does not go out of bar
		_piX = _piX < _barX - _barWidth / 2.f ? _barX - _barWidth / 2.f : _piX;
		_piX = _piX > _barX + _barWidth / 2.f ? _barX + _barWidth / 2.f : _piX;

		break;
	case INNER_STATES::ON_EXIT:
		// fade out event
		if (_elapsedTimeMs >= _oTimerTimeBeforeFadeOut * 1000) {
			// if invisible, reset state as fade out completed
			if (_oTimerOpacity <= 0.f) {
				result = EVENT_RESULTS::CUSTOM_MULTIPLIER;
				_resetState();
				return;
			}

			// start fading out
			float change = 1.f / _oTimerFadeOutDuration;
			_oTimerOpacity -= static_cast<f32>(change * dt);
		}
		break;
	}
}

void Event::_oscillatingTimerEventRender() {
	switch (oTimerEventState) {
	case INNER_STATES::ON_ENTER:
		break;
	case INNER_STATES::ON_UPDATE: {
		/*rendering*/
		const std::string oTimerMesh = meshReferences[0];
		const Point barTranslation = stow(_barX, _barY);

		// power bar
		RenderHelper::getInstance()->rect(oTimerMesh, barTranslation.x, barTranslation.y, _barWidth, _barHeight, 0.f, Color{ 0,0,0,_oTimerOpacity }, _oTimerOpacity);

		Point piTranslation = stow(_piX, _piY);
		// power indicator
		RenderHelper::getInstance()->rect(piTranslation.x, piTranslation.y, _piWidth, _piHeight, 0.f, Color{ 0.95f,0.95f,0.95f,_oTimerOpacity }, _oTimerOpacity);
		_renderTimer(_totalElapsedMs, _oTimerTimeoutMs);

		break;
	}
	case INNER_STATES::ON_EXIT: {
		if (_oTimerOpacity <= 0.f) {
			break;
		}
		// power bar
		const std::string oTimerMesh = meshReferences[0];
		const Point barTranslation = stow(_barX, _barY);

		RenderHelper::getInstance()->rect(oTimerMesh, barTranslation.x, barTranslation.y, _barWidth, _barHeight, 0.f, Color{ 0,0,0,_oTimerOpacity }, _oTimerOpacity);

		Point piTranslation = stow(_piX, _piY);
		// power indicator
		RenderHelper::getInstance()->rect(piTranslation.x, piTranslation.y, _piWidth, _piHeight, 0.f, Color{ 0.95f,0.95f,0.95f,_oTimerOpacity }, _oTimerOpacity);
		//_renderTimer(_totalElapsedMs, _oTimerTimeoutMs);

		std::ostringstream oss;
		oss.precision(eventMultiplierPrecision);
		oss << std::fixed << eventMultiplier << "x";
		RenderHelper::getInstance()->text(oss.str(), _barX, _barY + 50.f);
		break;
	}
	}


}

// !TODO: consider changing cursor to crosshair when multiclick event is active
void Event::_multiClickEventUpdate(EVENT_RESULTS& result, double dt) {
	_updateTime(dt);

	// multiclick is based on duration only
	if (_totalElapsedMs >= _multiClickTimeoutMs && !_mcoIsTransitioningOut) {
		std::cout << "multiclick event over\n";
		_elapsedTimeMs = 0;
		_mcoIsTransitioningOut = true;
		int score = (_mcoHits - _mcoMisses);
		score = score < 0 ? 0 : score;
		eventMultiplier = (score / static_cast<f32>(_maxMcoHits)) * maxMultiplier;
	}
	if (_mcoIsTransitioningOut) {
		if (_elapsedTimeMs <= _mcoTransitionTime * 1000) {	// transition out of state in this time
			_mcoIsTransitioningOut = true;
		}
		else {		// end event
			result = EVENT_RESULTS::CUSTOM_MULTIPLIER;
			_resetState();
			return;
		}
	}

	/*logic*/
	if (AEInputCheckTriggered(AEVK_LBUTTON) && !_mcoIsTransitioningOut) {

		// check if user clicked on any of the multi click objects
		bool hit = false;
		int i = 0;
		for (MultiClickObject& mco : _multiClickObjects) {
			Point worldPos = stow(mco.x, mco.y);
			Point collisionPos = wtos(worldPos.x - camOffset.x, worldPos.y - camOffset.y);

			hit = CollisionChecker::isMouseInCircle(collisionPos.x, collisionPos.y, mco.radius, static_cast<f32>(mouseX), static_cast<f32>(mouseY));
			if (hit) {
				std::cout << "mco hit\n";
				_mcoHits++;
				_mcoDisplayHits++;
				mco.alive = false;
				break;
			}
			i++;
		}

		// !TODO: consider adding animations
		if (hit) {
			_multiClickObjects.erase(_multiClickObjects.begin() + i);
		}
		else if (!hit) {
			std::cout << "mco missed\n";
			_mcoDisplayHits--;
			_mcoMisses++;
		}
		_mcoDisplayHits = _mcoDisplayHits < 0 ? 0 : _mcoDisplayHits;
	}
	// ensure that there are always mcoCount objects on screen
	while (_multiClickObjects.size() < _mcoCount && !_mcoIsTransitioningOut) {
		_multiClickObjects.push_back(MultiClickObject{
			static_cast<f32>(rand() % static_cast<int>(AEGfxGetWindowWidth())),
			static_cast<f32>(rand() % static_cast<int>(AEGfxGetWindowHeight())),
			_mcoRadius,
			true,
			0.f
			});
	}

	for (MultiClickObject& mco : _multiClickObjects) {
		mco.timeSinceChange += static_cast<f32>(dt);	// no real game logic here. just for rendering the blinking effect
	}
}

void Event::_multiClickEventRender() {
	_renderTimer(_totalElapsedMs, _multiClickTimeoutMs);

	/*render*/
	RenderHelper::getInstance()->text("Hits: " + std::to_string(_mcoDisplayHits) + "/" + std::to_string(_maxMcoHits), AEGfxGetWindowWidth() / 2.f, AEGfxGetWindowHeight() / 6.f);

	if (_mcoIsTransitioningOut) {
		return;
	}

	for (MultiClickObject& mco : _multiClickObjects) {
		if (mco.timeSinceChange >= _mcoBlinkDuration) {
			mco.blink = !mco.blink;
			mco.timeSinceChange = 0.f;
		}

		Point translate = stow(mco.x, mco.y);
		if (mco.blink) {
			RenderHelper::getInstance()->texture("clickme_light", translate.x, translate.y, mco.radius * 2, mco.radius * 2, 1, Color{ 0,0,0,1 }, 0.f);
		}
		else {
			RenderHelper::getInstance()->texture("clickme_dark", translate.x, translate.y, mco.radius * 2, mco.radius * 2, 1, Color{ 0,0,0,1 }, 0.f);
		}
	}
}

// !TODO: change to sprite
void Event::_typingEventUpdate(EVENT_RESULTS& result, double dt) {
	/*update*/
	_updateTime(dt);

	switch (_typingState) {
	case INNER_STATES::ON_ENTER:
		// on enter state
		_currentWord = _wordlist[rand() % _wordlist.size()];

		_typed.clear();
		// init map on which letters are typed
		for (const char c : _currentWord) {
			_typed.push_back({ c, false });
		}

		_typingState = INNER_STATES::ON_UPDATE;
		break;

	case INNER_STATES::ON_UPDATE:
		// on update state
		if (_elapsedTimeMs >= _typingTimeoutMs) {
			_elapsedTimeMs = 0;
			eventMultiplier = static_cast<f32>(_wordsCompleted) / _typingMaxScore * maxMultiplier;
			_typingState = INNER_STATES::ON_EXIT;
		}

		for (const std::pair<int, char> map : keyMappings) {
			if (AEInputCheckTriggered(static_cast<u8>(map.first))) {

				// set next to first iterator of vector
				auto next = &_typed[0];
				// find the first letter that hasnt been typed
				while (next->second) {
					// word is fully typed. use guard to prevent out of range
					if (next == &_typed.back()) {
						break;
					}
					next++;
				}

				if (_typingState != INNER_STATES::ON_UPDATE) {
					break;
				}

				// mark letter as typed if correct key is triggered
				if (map.first + AEVK_OFFSET == next->first) {
					next->second = true;

					// finished typing. move on to next state
					if (next + 1 == &_typed.back()) {
						_wordsCompleted++;
						_typingState = INNER_STATES::ON_NEXT;
						break;
					}
				}
			}
		}
		break;

	case INNER_STATES::ON_NEXT:
		_typingState = INNER_STATES::ON_ENTER;
		break;

	case INNER_STATES::ON_EXIT:
		if (_elapsedTimeMs >= _typingTransitionTime * 1000) {
			result = EVENT_RESULTS::CUSTOM_MULTIPLIER;
			//_resetState();
		}
		break;
	}
}

void Event::_typingEventRender() {
	_renderTimer(_totalElapsedMs, _typingTimeoutMs);

	/*render*/
	float wordWidth = _currentWord.size() * RenderHelper::getInstance()->getFontSize() + (_currentWord.size() - 1) * _charGap;
	const float start = AEGfxGetWindowWidth() / 2.f - wordWidth / 2.f;
	float currXOffset = start;
	float currYOffset = AEGfxGetWindowHeight() * 0.85f;
	int i{};

	switch (_typingState) {
	case INNER_STATES::ON_ENTER:
	case INNER_STATES::ON_UPDATE:
	case INNER_STATES::ON_NEXT:
		for (const char c : _currentWord) {
			Color col;

			// if has been typed, set color
			if (_typed[i].second) {
				col = { 0, 1, 0, 1 };	// green
			}
			else {
				col = { 0.9f, 0.9f, 0.9f, 1 };	// grey
			}

			RenderHelper::getInstance()->text(std::string{ static_cast<char>(toupper(c)) }, currXOffset, currYOffset, col.r, col.g, col.b, col.a);
			currXOffset += RenderHelper::getInstance()->getFontSize() + _charGap;

			i++;
		}
		break;


	case INNER_STATES::ON_EXIT:
		std::ostringstream oss;
		oss.precision(eventMultiplierPrecision);
		oss << std::fixed << eventMultiplier << "x";
		RenderHelper::getInstance()->text(oss.str(), AEGfxGetWindowWidth() / 2.f, currYOffset);

		if (_elapsedTimeMs >= _typingTransitionTime * 1000) {
			//result = EVENT_RESULTS::CUSTOM_MULTIPLIER
			_resetState();
		}
		break;
	}
}


void Event::_orangeEventUpdate(EVENT_RESULTS& result, double dt) {
	_updateTime(dt);

	switch (_orangeState) {
	case INNER_STATES::ON_ENTER: {
		//const int dirDeg = rand() % 360;
		//const float dirRad = degToRad(static_cast<const float>(dirDeg));
		_orangeObj = Orange{
			AEGfxGetWindowWidth() / 2.f, AEGfxGetWindowHeight() / 2.f,			// pos
			//{cosf(dirRad), sinf(dirRad)},				// velocity
			{0,0},										// velocity
			_orangeRadius,							// radius
			false
		};

		for (Demon& d : demons) {
			const float demonSpeed = rand() % static_cast<int>(_demonMaxSpeed - _demonMinSpeed) + _demonMinSpeed;

			d.x = rand() % static_cast<int>(AEGfxGetWindowWidth() - _demonRadius * 2) + _demonRadius;
			d.y = AEGfxGetWindowHeight() * 0.1f;
			AEVec2Set(&d.vel, rand() % 2 == 0 ? demonSpeed : -demonSpeed, 0);
			d.radius = _demonRadius;
			d.isActive = true;
		}

		//AEVec2Normalize(&_orangeObj.vel, &_orangeObj.vel);
		//AEVec2Scale(&_orangeObj.vel, &_orangeObj.vel, _orangeSpeed);
		_orangeState = INNER_STATES::ON_UPDATE;
		break;
	}

	case INNER_STATES::ON_UPDATE: {
		if (_elapsedTimeMs >= _orangeEventTimeoutMs) {
			int hits{};
			for (const Demon& d : demons) {
				if (!d.isActive) {
					hits++;
				}
			}
			eventMultiplier = static_cast<f32>(hits) / NUM_DEMONS * maxMultiplier;

			_orangeState = INNER_STATES::ON_EXIT;
			_elapsedTimeMs = 0;
			break;
		}

		/*new impl*/
		if (AEInputCheckTriggered(AEVK_R)) {
			AEVec2Set(&_orangeObj.vel, 0, 0);
			_orangeObj.x = 0;
			_orangeObj.y = 0;
		}

		if (AEInputCheckCurr(AEVK_LBUTTON) && CollisionChecker::isMouseInCircle(_orangeObj.x, _orangeObj.y, _orangeObj.radius, static_cast<f32>(_mouseX), static_cast<f32>(_mouseY))) {
			_orangeObj.isHeld = true;
		}
		// check to ensure that the user cannot hold the ball past 75% of the screen
		if (_orangeObj.isHeld && (_orangeObj.y <= AEGfxGetWindowHeight() * 0.25f || _orangeObj.y >= static_cast<f32>(AEGfxGetWindowHeight()))) {
			_orangeObj.isHeld = false;	// force user to let go
			AEVec2Set(&_orangeObj.vel, 0, 0);			// reset velocity to 0
		}

		if (_orangeObj.isHeld && AEInputCheckCurr(AEVK_LBUTTON)) {
			_orangeObj.x = static_cast<f32>(_mouseX);
			_orangeObj.y = static_cast<f32>(_mouseY);

			// get vector between last frame and this frame
			_orangeObj.vel.x = static_cast<f32>(_mouseX) - _prevMouseX;
			_orangeObj.vel.y = static_cast<f32>(_mouseY) - _prevMouseY;
		}
		else {
			_orangeObj.isHeld = false;
		}


		//std::cout << (_orangeObj.y <= AEGfxGetWindowHeight() * 0.25f) << "\n";

		//std::cout << "obj vel: " << _orangeObj.vel.x << ", " << _orangeObj.vel.y << "\n";

		// !TODO: jspoh dont allow user to bring ball past 75% of screen from bottom
		// obj not held, apply normal physics to object
		if (!_orangeObj.isHeld) {
			// gravity
			if (_orangeObj.y + _orangeObj.radius < AEGfxGetWindowHeight()) {
				_orangeObj.vel.y += static_cast<f32>(_orangeGravity * dt);
			}

			// resistance
			if (_orangeObj.vel.x > 0) {

				_orangeObj.vel.x -= _orangeObj.vel.x - _xResistance * dt > 0 ? static_cast<f32>(_xResistance * dt) : _orangeObj.vel.x;

				// set value to 0 when below 1 as float arithmetic is not precise
				if (_orangeObj.vel.x < 1) {
					_orangeObj.vel.x = 0;
				}
			}
			else if (_orangeObj.vel.x < 0) {

				_orangeObj.vel.x += _orangeObj.vel.x + _xResistance * dt < 0 ? static_cast<f32>(_xResistance * dt) : _orangeObj.vel.x;

				// set value to 0 when above -1 as float arithmetic is not precise
				if (_orangeObj.vel.x > -1) {
					_orangeObj.vel.x = 0;
				}
			}
			if (_orangeObj.vel.y > 0) {

				_orangeObj.vel.y -= _orangeObj.vel.y - _xResistance * dt > 0 ? static_cast<f32>(_xResistance * dt) : _orangeObj.vel.y;

				// set value to 0 when below 1 as float arithmetic is not precise
				// for y axis, must also check if object is near ground
				if (_orangeObj.vel.y < 1 && _orangeObj.y + _orangeObj.radius >= AEGfxGetWindowHeight()) {
					_orangeObj.vel.y = 0;
				}
			}
			else if (_orangeObj.vel.y < 0) {

				_orangeObj.vel.y += _orangeObj.vel.y + _xResistance * dt < 0 ? static_cast<f32>(_xResistance * dt) : _orangeObj.vel.y;

				// set value to 0 when above -1 as float arithmetic is not precise
				// for y axis, must also check if object is near ground
				if (_orangeObj.vel.y > -1 && _orangeObj.y + _orangeObj.radius >= AEGfxGetWindowHeight()) {
					_orangeObj.vel.y = 0;
				}
			}

			// clamp velocity
			_orangeObj.vel.x = AEClamp(_orangeObj.vel.x, -_speedLimit, _speedLimit);
			_orangeObj.vel.y = AEClamp(_orangeObj.vel.y, -_speedLimit, _speedLimit);

			// invert x/y vector when collide w wall to provide illusion of bouncing
			if (CollisionChecker::isRectTouchingScreenXBorder(_orangeObj.x, _orangeObj.y, _orangeObj.radius * 2, _orangeObj.radius * 2)) {
				_orangeObj.vel.x = -_orangeObj.vel.x * _energyKeptBouncing;
			}
			if (CollisionChecker::isRectTouchingScreenYBorder(_orangeObj.x, _orangeObj.y, _orangeObj.radius * 2, _orangeObj.radius * 2)) {
				_orangeObj.vel.y = -_orangeObj.vel.y * _energyKeptBouncing;
			}

			_orangeObj.x += static_cast<f32>(_orangeObj.vel.x * AEFrameRateControllerGetFrameRate() * dt);
			_orangeObj.y += static_cast<f32>(_orangeObj.vel.y * AEFrameRateControllerGetFrameRate() * dt);

			// clamp positions
			_orangeObj.x = AEClamp(_orangeObj.x, _orangeObj.radius, AEGfxGetWindowWidth() - _orangeObj.radius);
			_orangeObj.y = AEClamp(_orangeObj.y, _orangeObj.radius, AEGfxGetWindowHeight() - _orangeObj.radius);
		}

		// update demon
		for (Demon& d : demons) {
			if (!d.isActive) {
				continue;
			}

			if (CollisionChecker::isRectTouchingScreenXBorder(d.x, d.y, d.radius, d.radius)) {
				d.vel.x = -d.vel.x;
			}

			d.x += static_cast<f32>(d.vel.x * dt);
			d.y += static_cast<f32>(d.vel.y * dt);

			// check for collision with ball (orange?)
			if (CollisionChecker::areCirclesIntersecting(d.x, d.y, d.radius, _orangeObj.x, _orangeObj.y, _orangeObj.radius)) {
				//std::cout << "collided\n";
				d.isActive = false;
			}
		}


		break;
	}

	case INNER_STATES::ON_NEXT:
		break;

	case INNER_STATES::ON_EXIT:
		if (_elapsedTimeMs >= _afterEventDisplayTimeoutMs) {
			result = EVENT_RESULTS::CUSTOM_MULTIPLIER;
			_resetState();
		}
		break;
	}
}

void Event::_orangeEventRender() {
	switch (_orangeState) {
	case INNER_STATES::ON_ENTER:
		break;

	case INNER_STATES::ON_UPDATE:
		Point pos = stow(_orangeObj.x, _orangeObj.y);
		RenderHelper::getInstance()->texture("ball", pos.x + camOffset.x, pos.y + camOffset.y, _orangeObj.radius, _orangeObj.radius, 1.f, Color{ 1.f, 0, 0, 1.f }, 0);
		for (const Demon& d : demons) {
			if (!d.isActive) {
				continue;
			}

			pos = stow(d.x, d.y);
			RenderHelper::getInstance()->texture("nian", pos.x + camOffset.x, pos.y + camOffset.y, d.radius, d.radius, 1, Color{ 1,0,0,1 }, degToRad(0));
		}
		_renderTimer(_elapsedTimeMs, _orangeEventTimeoutMs);
		break;

	case INNER_STATES::ON_NEXT:
		break;

	case INNER_STATES::ON_EXIT:
		std::ostringstream oss;
		oss.precision(eventMultiplierPrecision);
		oss << std::fixed << eventMultiplier << "x";
		RenderHelper::getInstance()->text(oss.str(), AEGfxGetWindowWidth() / 2.f, AEGfxGetWindowHeight() / 2.f);
		break;
	}
}
