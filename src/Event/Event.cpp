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


Event::Event() {
	double time;
	AEGetTime(&time);
	srand(static_cast<unsigned int>(time));

	for (std::pair<EVENT_KEYS, std::string> map : eKeyToStr) {
		bool success1 = RenderHelper::getInstance()->registerTexture("key_" + map.second, "./Assets/keys/keyboard_" + map.second + ".png");
		bool success2 = RenderHelper::getInstance()->registerTexture("keyoutline_" + map.second, "./Assets/keys/keyboard_" + map.second + "_outline.png");
		if (!success1 || !success2) {
			std::cerr << "Failed to load asset in Event constructor\n";
			//exit(2);
		}
	}
	RenderHelper::getInstance()->registerTexture("circle", "./Assets/flairs/flair_circle_red_8.png");
	textureReferences.push_back("circle");
	RenderHelper::getInstance()->registerTexture("fail", "./Assets/flairs/flair_disabled_cross.png");
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
	RenderHelper::getInstance()->registerTexture("clickme_light", "./Assets/keys/clickme_light.png");
	textureReferences.push_back("clickme_light");
	RenderHelper::getInstance()->registerTexture("clickme_dark", "./Assets/keys/clickme_dark.png");
	textureReferences.push_back("clickme_dark");
	RenderHelper::getInstance()->registerTexture("noclick_light", "./Assets/keys/noclick_light.png");
	textureReferences.push_back("noclick_light");
	RenderHelper::getInstance()->registerTexture("noclick_dark", "./Assets/keys/noclick_dark.png");
	textureReferences.push_back("noclick_dark");
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
	//e = EVENT_TYPES::TYPING;  // hardcoded for testing
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
	switch (_activeEvent) {
	case EVENT_TYPES::NONE_EVENT_TYPE:
		break;
	case EVENT_TYPES::SPAM_KEY:
		_spamKey(result, dt, spamkey);
		break;
	case EVENT_TYPES::OSCILLATING_TIMER:
		_oscillatingTimer(result, dt, oTimerKey);
		break;
	case EVENT_TYPES::MULTI_CLICK:
		_multiClick(result, dt);
		break;
	case EVENT_TYPES::TYPING:
		_typingEvent(result, dt);
		break;
	default:
		std::cerr << "Event::updateRenderLoop reached end of switch case\n";
		break;
	}
}



/*private*/
void Event::_drawTimer(float elapsedTime, float timeout) {

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

void Event::_showEventSpamKeyResult(EVENT_RESULTS& result, float screenX, float screenY) {
	/*event result duration over*/
	if (_elapsedTimeMs >= _eventResultDuration * 1000) {
		result = _eventResult;  // update result passed by caller so that they know event is over
		_resetState();
		return;
	}

	if (_eventResult == EVENT_RESULTS::SUCCESS) {
		// draw success
		RenderHelper::getInstance()->texture("circle", screenX, screenY, _minSize, _minSize, 1.0f, Color{ 0,1,0,0 });
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
		_showEventSpamKeyResult(result, worldX, worldY);
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
		_showEventSpamKeyResult(result, _barX, _barY);
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
	RenderHelper::getInstance()->rect(oTimerMesh, barTranslation.x, barTranslation.y, _barWidth, _barHeight, 0.f, Color{ 0,0,0,_oTimerOpacity }, _oTimerOpacity);

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
			// if invisible, reset state as fade out completed
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

// !TODO: consider changing cursor to crosshair when multiclick event is active
void Event::_multiClick(EVENT_RESULTS& result, double dt) {
	_updateTime(dt);

	// multiclick is based on duration only
	if (_totalElapsedMs >= _multiClickDuration * 1000 && !_mcoIsTransitioningOut) {
		std::cout << "multiclick event over\n";
		_elapsedTimeMs = 0;
		_mcoIsTransitioningOut = true;
		int score = (_mcoHits - _mcoMisses);
		score = score < 0 ? 0 : score;
		eventMultiplier = (score / static_cast<float>(_maxMcoHits)) * maxMultiplier;
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
		int mouseX, mouseY;
		AEInputGetCursorPosition(&mouseX, &mouseY);

		// check if user clicked on any of the multi click objects
		bool hit = false;
		int i = 0;
		for (MultiClickObject& mco : _multiClickObjects) {
			hit = CollisionChecker::isMouseInCircle(mco.x, mco.y, mco.radius, static_cast<float>(mouseX), static_cast<float>(mouseY));
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

	/*rendering*/
	// ensure that there are always mcoCount objects on screen
	while (_multiClickObjects.size() < _mcoCount && !_mcoIsTransitioningOut) {
		_multiClickObjects.push_back(MultiClickObject{
			static_cast<float>(rand() % static_cast<int>(AEGfxGetWindowWidth())),
			static_cast<float>(rand() % static_cast<int>(AEGfxGetWindowHeight())),
			_mcoRadius,
			true,
			0.f
			});
	}

	/*render*/
	RenderHelper::getInstance()->text("Hits: " + std::to_string(_mcoDisplayHits) + "/" + std::to_string(_maxMcoHits), AEGfxGetWindowWidth() / 2.f, AEGfxGetWindowHeight() / 6.f);

	if (_mcoIsTransitioningOut) {
		return;
	}

	for (MultiClickObject& mco : _multiClickObjects) {
		mco.timeSinceChange += static_cast<float>(dt);	// no real game logic here. just for rendering the blinking effect
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
void Event::_typingEvent(EVENT_RESULTS& result, double dt) {
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
		if (_elapsedTimeMs >= _typingTimeout * 1000) {
			_elapsedTimeMs = 0;
			eventMultiplier = static_cast<float>(_wordsCompleted) / _typingMaxScore * maxMultiplier;
			_typingState = INNER_STATES::ON_EXIT;
		}

		for (const std::pair<int, char> map : keyMappings) {
			if (AEInputCheckTriggered(map.first)) {

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
			_resetState();
		}
		break;
	}

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
				col = { 0.5f, 0.5f, 0.5f, 1 };	// grey
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
		break;
	}
}
