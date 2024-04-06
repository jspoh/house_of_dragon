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

Event* Event::m_instance = nullptr;

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

	for (std::pair<EVENT_KEYS, std::string> map : eKeyToStr) {
		bool success1 = RenderHelper::getInstance()->registerTexture("key_" + map.second, "./Assets/Combat_UI/keyboard_" + map.second + ".png");
		bool success2 = RenderHelper::getInstance()->registerTexture("keyoutline_" + map.second, "./Assets/Combat_UI/keyboard_" + map.second + "_outline.png");
		if (!success1 || !success2) {
			cerr << "ERROR: " << "Failed to load asset in Event constructor\n";
			//throw std::exception();
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
	this->m_barWidth = AEGfxGetWindowWidth() / 2.f;
	this->m_barHeight = this->m_barWidth / 30.f;
	this->m_barX = AEGfxGetWindowWidth() / 2.f;
	this->m_barY = AEGfxGetWindowHeight() / 6.f;

	// pi -> power indicator
	this->m_piWidth = this->m_barWidth / 200.f;
	this->m_piHeight = this->m_barHeight * 1.5f;
	this->m_piX = this->m_barX - this->m_barWidth / 2.f;
	this->m_piY = this->m_barY;

	/**
	 * distance based formula for acceleration:
	 *
	 * acceleration = (final velocity^2 - initial velocity^2) / (2 * distance)
	 *
	 */
	m_piAcc = (m_piMaxVelocity * m_piMaxVelocity) / (2.f * (m_barWidth / 2.f));

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
	//for (const std::string& ref : meshReferences) {
	//	RenderHelper::getInstance()->removeMeshByRef(ref);
	//}

	//for (const std::string& ref : textureReferences) {
	//	RenderHelper::getInstance()->removeTextureByRef(ref);
	//}
}

Event* Event::getInstance() {
	if (m_instance == nullptr) {
		m_instance = new Event();
	}
	return m_instance;
}

void Event::startRandomEvent() {
	// start a random quicktime event
	EVENT_TYPES e = static_cast<EVENT_TYPES>((rand() % NUM_EVENT_TYPES));
	//e = EVENT_TYPES::SPAM_KEY;  // hardcoded for testing
	//e = EVENT_TYPES::OSCILLATING_TIMER;  // hardcoded for testing
	//e = EVENT_TYPES::MULTI_CLICK;  // hardcoded for testing
	//e = EVENT_TYPES::TYPING;  // hardcoded for testing
	//e = EVENT_TYPES::ORANGE_THROWING;  // hardcoded for testing
	cout << "Random event: " << e << "\n";
	Event::getInstance()->setActiveEvent(e);
}

bool Event::setActiveEvent(EVENT_TYPES e) {
	if (m__activeEvent == EVENT_TYPES::NONE_EVENT_TYPE) {
		m__activeEvent = e;

		return true;
	}
	cerr << "ERROR: " << "An event is already running!\n";
	return false;
}


EVENT_TYPES Event::getActiveEvent() {
	return m__activeEvent;
}

void Event::init() {

	m_spamkeyX = AEGfxGetWindowWidth() / 2.f;
	m_spamkeyY = AEGfxGetWindowHeight() / 2.f;

	/* difficulty settings */

	const float timerMultiplier = DIFFICULTY_TIME_MULTIPLIER.at(difficulty);
	const float sizeMultiplier = DIFFICULTY_SIZE_MULTIPLIER.at(difficulty);

	// spamkey
	_spamkeyTimeoutMs = static_cast<int>(nSPAM_KEY_TIMEOUT_MS * timerMultiplier);
	m_proc = m_PROC * sizeMultiplier;

	// otimer
	m_oTimerTimeoutMs = static_cast<int>(m_OTIMER_TIMEOUT_MS * timerMultiplier);

	// mco
	m_multiClickTimeoutMs = static_cast<int>(m_MULTICLICK_TIMEOUT_MS * timerMultiplier);
	m__mcoRadius = m_MCO_RADIUS * sizeMultiplier;

	// typing  
	m_typingTimeoutMs = static_cast<int>(m_TYPING_TIMEOUT_MS * timerMultiplier);

	// orange/demon throwing
	m_orangeEventTimeoutMs = static_cast<int>(m_ORANGE_TIMEOUT_MS * timerMultiplier);
	m_orangeRadius = m_ORANGE_RADIUS * sizeMultiplier;
}

void Event::update(EVENT_RESULTS& result, double dt, EVENT_KEYS spamkey, EVENT_KEYS oTimerKey) {
	AEInputGetCursorPosition(&m_mouseX, &m_mouseY);
	if (m_prevMouseX == 0 || m_prevMouseY == 0) {
		m_prevMouseX = m_mouseX;
		m_prevMouseY = m_mouseY;
	}

	switch (m__activeEvent) {
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
		cerr << "ERROR: " << "Event::update reached end of switch case\n";
		break;
	}

	m_prevMouseX = m_mouseX;
	m_prevMouseY = m_mouseY;
}

void Event::render() {

	switch (m__activeEvent) {
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
		cerr << "ERROR: " << "Event::render reached end of switch case\n";
		break;
	}
}


/*private*/
void Event::_renderTimer(int elapsedTimeMs, int timeoutMs) {
	float x = AEGfxGetWindowWidth() * 0.95f;
	float y = AEGfxGetWindowHeight() * 0.8f;
	AEVec2 world = stow(x, y);

	std::array<int, 5> thresholds = { 100,75,50,25,0 };

	int timeLeftPctg = static_cast<int>(ceil((static_cast<f32>(elapsedTimeMs) / timeoutMs) * 100));		// time left percentage

	for (const int t : thresholds) {
		if (timeLeftPctg >= t) {
			RenderHelper::getInstance()->texture("timer_" + std::to_string(100 - t), world.x + camOffset.x, world.y + camOffset.y);
			break;
		}
	}
}

void Event::_resetState() {
	m__activeEvent = EVENT_TYPES::NONE_EVENT_TYPE;
	_resetTime();
	m_useOutline = true;
	m_size = m_minSize;
	m_isRenderingEventResult = false;

	// oscillating timer
	this->m_piX = this->m_barX - this->m_barWidth / 2.f;
	this->m_piVelocity = 0.f;
	this->m_oTimerOpacity = 1.f;
	this->m_piMoving = true;
	moTimerEventState = INNER_STATES::ON_ENTER;

	// multiclick
	m_mcoHits = 0;
	m_mcoMisses = 0;
	m_mcoDisplayHits = 0;
	m_multiClickObjectsts.clear();
	m_mcoIsTransitioningOut = false;

	// typing event
	m_currentWord.clear();
	m_typed.clear();
	m_typingState = INNER_STATES::ON_ENTER;
	m_wordsCompleted = 0;

	// orange event
	m_orangeState = INNER_STATES::ON_ENTER;
}

void Event::_resetTime() {
	m_elapsedTimeMs = 0;
	m_totalElapsedMs = 0;
}

void Event::_updateTime(double dt) {
	// integer delta time
	int idt = static_cast<int>(dt * 1000);
	m_elapsedTimeMs += idt;
	m_totalElapsedMs += idt;
}

void Event::_showEventSpamKeyResult(float screenX, float screenY) {
	if (m_eventResult == EVENT_RESULTS::SUCCESS) {
		// draw success
		RenderHelper::getInstance()->texture("circle", screenX, screenY, m_minSize, m_minSize, 1.0f, Color{ 0,1,0,0 });
	}
	else if (m_eventResult == EVENT_RESULTS::FAILURE) {
		// draw failure
		RenderHelper::getInstance()->texture("fail", screenX, screenY, m_minSize, m_minSize);
	}
}

void Event::_spamKeyEventUpdate(EVENT_RESULTS& result, double dt, EVENT_KEYS key) {
	_updateTime(dt);
	m_spamKeyChoice = key;

	//cout << m_totalElapsedMs << "\n";

	AEVec2 worldPos = stow(m_spamkeyX, m_spamkeyY);
	//float worldX = worldPos.x;
	//float worldY = worldPos.y;

	/*logic*/

	// if event is over, is rendering event result
	if (m_isRenderingEventResult) {
		/*event result duration over*/
		if (m_elapsedTimeMs >= m_EVENT_RESULT_DURATION * 1000) {
			result = m_eventResult;  // update result passed by caller so that they know event is over
			_resetState();
		}
		return;
	}

	// check if user succeeded on spamming key
	if (m_size >= m_targetSize) {
		m_eventResult = EVENT_RESULTS::SUCCESS;
		m_elapsedTimeMs = 0;
		m_isRenderingEventResult = true;
		return;
	}
	else if (m_totalElapsedMs >= _spamkeyTimeoutMs) {
		m_eventResult = EVENT_RESULTS::FAILURE;
		m_elapsedTimeMs = 0;
		m_isRenderingEventResult = true;
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
		cerr << "ERROR: " << "Key was not registered in EVENT_KEYS!\n";
		//throw std::exception();
		break;
	}

	if (AEInputCheckTriggered(aevk)) {
		m_size += m_proc;
	}
	m_size -= static_cast<f32>(m_nroc * dt);
	m_size = m_size < m_minSize ? m_minSize : m_size;

	/*rendering*/
	//cout << m_elapsedTimeMs << "\n";

	if (m_elapsedTimeMs > m_changeMs) {
		m_useOutline = !m_useOutline;
		m_elapsedTimeMs = 0;
	}
}

void Event::_spamKeyEventRender() {

	_renderTimer(m_totalElapsedMs, _spamkeyTimeoutMs);

	AEVec2 worldPos = stow(m_spamkeyX, m_spamkeyY);
	float worldX = worldPos.x;
	float worldY = worldPos.y;

	// if event is over, is rendering event result
	if (m_isRenderingEventResult) {
		_showEventSpamKeyResult(worldX + camOffset.x, worldY + camOffset.y);
		return;
	}

	// key in string format
	std::string skey = eKeyToStr.find(m_spamKeyChoice)->second;

	if (m_useOutline) {
		RenderHelper::getInstance()->texture("keyoutline_" + skey, worldX, worldY, m_size, m_size);
	}
	else {
		RenderHelper::getInstance()->texture("key_" + skey, worldX, worldY, m_size, m_size);
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
		cerr << "ERROR: " << "Key was not registered in EVENT_KEYS!\n";
		//exit(3);
		break;
	}

	/*logic*/
	switch (moTimerEventState) {
	case INNER_STATES::ON_ENTER:
		moTimerEventState = INNER_STATES::ON_UPDATE;
		break;
	case INNER_STATES::ON_UPDATE:
		// check if timeout
		if (m_totalElapsedMs >= m_oTimerTimeoutMs) {
			moTimerEventState = INNER_STATES::ON_EXIT;
			m_eventMultiplier = 0;
			m_elapsedTimeMs = 0;
			break;
		}

		// check if event is over
		if (AEInputCheckTriggered(aevk)) {
			m_piMoving = false;
			m_elapsedTimeMs = 0;	// using time to fade out

			// calculate multiplier
			const float piDistanceToCenter = abs(m_barX - m_piX);
			const float percentageMultiplier = ((m_barWidth / 2.f) - piDistanceToCenter) / (m_barWidth / 2.f);
			m_eventMultiplier = percentageMultiplier * m_maxMultiplier;
			m_eventMultiplier = precisionRound(m_eventMultiplier, m_eventMultiplierPrecision);
			moTimerEventState = INNER_STATES::ON_EXIT;
		}

		// power indicator movement logic. accerlerates until center of bar, then decelerates
		// pi is left of or on the center of bar
		if (m_piX <= m_barX) {
			m_piVelocity += static_cast<f32>(m_piAcc * dt);
		}
		// pi is right of the center of bar
		else {
			m_piVelocity -= static_cast<f32>(m_piAcc * dt);
		}

		// cap velocity
		m_piVelocity = m_piVelocity > m_piMaxVelocity ? m_piMaxVelocity : m_piVelocity;
		m_piVelocity = m_piVelocity < -m_piMaxVelocity ? -m_piMaxVelocity : m_piVelocity;

		//cout << "Power indicator speed: " << m_piVelocity << "\n";
		m_piX += static_cast<f32>(m_piVelocity * dt);

		// guards to ensure that pi does not go out of bar
		m_piX = m_piX < m_barX - m_barWidth / 2.f ? m_barX - m_barWidth / 2.f : m_piX;
		m_piX = m_piX > m_barX + m_barWidth / 2.f ? m_barX + m_barWidth / 2.f : m_piX;

		break;
	case INNER_STATES::ON_EXIT:
		// fade out event
		if (m_elapsedTimeMs >= m_oTimerTimeBeforeFadeOut * 1000) {
			// if invisible, reset state as fade out completed
			if (m_oTimerOpacity <= 0.f) {
				result = EVENT_RESULTS::CUSTOM_MULTIPLIER;
				_resetState();
				return;
			}

			// start fading out
			float change = 1.f / m_oTimerFadeOutDuration;
			m_oTimerOpacity -= static_cast<f32>(change * dt);
		}
		break;
	}
}

void Event::_oscillatingTimerEventRender() {
	switch (moTimerEventState) {
	case INNER_STATES::ON_ENTER:
		break;
	case INNER_STATES::ON_UPDATE: {
		/*rendering*/
		const std::string oTimerMesh = meshReferences[0];
		const AEVec2 barTranslation = stow(m_barX, m_barY);

		// power bar
		RenderHelper::getInstance()->rect(oTimerMesh, barTranslation.x, barTranslation.y, m_barWidth, m_barHeight, 0.f, Color{ 0,0,0,m_oTimerOpacity }, m_oTimerOpacity);

		AEVec2 piTranslation = stow(m_piX, m_piY);
		// power indicator
		RenderHelper::getInstance()->rect(piTranslation.x, piTranslation.y, m_piWidth, m_piHeight, 0.f, Color{ 0.95f,0.95f,0.95f,m_oTimerOpacity }, m_oTimerOpacity);
		_renderTimer(m_totalElapsedMs, m_oTimerTimeoutMs);

		break;
	}
	case INNER_STATES::ON_EXIT: {
		if (m_oTimerOpacity <= 0.f) {
			break;
		}
		// power bar
		const std::string oTimerMesh = meshReferences[0];
		const AEVec2 barTranslation = stow(m_barX, m_barY);

		RenderHelper::getInstance()->rect(oTimerMesh, barTranslation.x, barTranslation.y, m_barWidth, m_barHeight, 0.f, Color{ 0,0,0,m_oTimerOpacity }, m_oTimerOpacity);

		AEVec2 piTranslation = stow(m_piX, m_piY);
		// power indicator
		RenderHelper::getInstance()->rect(piTranslation.x, piTranslation.y, m_piWidth, m_piHeight, 0.f, Color{ 0.95f,0.95f,0.95f,m_oTimerOpacity }, m_oTimerOpacity);
		//_renderTimer(m_totalElapsedMs, m_oTimerTimeoutMs);

		std::ostringstream oss;
		oss.precision(m_eventMultiplierPrecision);
		oss << std::fixed << m_eventMultiplier << "x";
		RenderHelper::getInstance()->text(oss.str(), m_barX, m_barY + 50.f);
		break;
	}
	}


}

void Event::_multiClickEventUpdate(EVENT_RESULTS& result, double dt) {
	_updateTime(dt);

	// multiclick is based on duration and if event is completed
	if ((m_totalElapsedMs >= m_multiClickTimeoutMs && !m_mcoIsTransitioningOut) || (m_mcoHits - m_mcoMisses == m_maxMcoHits && !m_mcoIsTransitioningOut)) {
		cout << "multiclick event over\n";
		m_elapsedTimeMs = 0;
		m_mcoIsTransitioningOut = true;
		int score = (m_mcoHits - m_mcoMisses);
		score = score < 0 ? 0 : score;
		m_eventMultiplier = (score / static_cast<f32>(m_maxMcoHits)) * m_maxMultiplier;
	}
	if (m_mcoIsTransitioningOut) {
		if (m_elapsedTimeMs <= m_mcoTransitionTime * 1000) {	// transition out of state in this time
			m_mcoIsTransitioningOut = true;
		}
		else {		// end event
			result = EVENT_RESULTS::CUSTOM_MULTIPLIER;
			_resetState();
			return;
		}
	}

	/*logic*/
	if (AEInputCheckTriggered(AEVK_LBUTTON) && !m_mcoIsTransitioningOut) {

		// check if user clicked on any of the multi click objects
		bool hit = false;
		int i = 0;
		for (MultiClickObject& mco : m_multiClickObjectsts) {
			AEVec2 worldPos = stow(mco.x, mco.y);
			AEVec2 collisionPos = wtos(worldPos.x - camOffset.x, worldPos.y - camOffset.y);

			hit = CollisionChecker::isMouseInCircle(collisionPos.x, collisionPos.y, mco.radius, static_cast<f32>(mouseX), static_cast<f32>(mouseY));
			if (hit) {
				cout << "mco hit\n";
				m_mcoHits++;
				m_mcoDisplayHits++;
				mco.alive = false;
				break;
			}
			i++;
		}

		if (hit) {
			m_multiClickObjectsts.erasm_multiClickObjectsects.begin() + i);
		}
		else if (!hit) {
			cout << "mco missed\n";
			m_mcoDisplayHits--;
			m_mcoMisses++;
		}
		m_mcoDisplayHits = m_mcoDisplayHits < 0 ? 0 : m_mcoDisplayHits;
	}
	// ensure that there are always mcoCount objects on screen
	while (m_multiClickObjectsts.size() < m_mcoCount && !m_mcoIsTransitioningOut) {
		m_multiClickObjectsts.push_back(MultiClickObject{
			static_cast<f32>(rand() % static_cast<int>(AEGfxGetWindowWidth() - m_spawnOffsetX * 2) + m_spawnOffsetX),
			static_cast<f32>(rand() % static_cast<int>(AEGfxGetWindowHeight() - m_SPAWN_OFFSET_Y * 2) + m_SPAWN_OFFSET_Y),
			m__mcoRadius,
			true,
			0.f
			});
	}

	for (MultiClickObject& mco : m_multiClickObjectsts) {
		mco.timeSinceChange += static_cast<f32>(dt);	// no real game logic here. just for rendering the blinking effect
	}
}

void Event::_multiClickEventRender() {
	_renderTimer(m_totalElapsedMs, m_multiClickTimeoutMs);

	/*render*/
	RenderHelper::getInstance()->text("Hits: " + std::to_string(m_mcoDisplayHits) + "/" + std::to_string(m_maxMcoHits), AEGfxGetWindowWidth() / 2.f, AEGfxGetWindowHeight() / 6.f);

	if (m_mcoIsTransitioningOut) {
		return;
	}

	for (MultiClickObject& mco : m_multiClickObjectsts) {
		if (mco.timeSinceChange >= m_mcoBlinkDuration) {
			mco.blink = !mco.blink;
			mco.timeSinceChange = 0.f;
		}

		AEVec2 translate = stow(mco.x, mco.y);
		if (mco.blink) {
			RenderHelper::getInstance()->texture("clickme_light", translate.x, translate.y, mco.radius * 2, mco.radius * 2, 1, Color{ 0,0,0,1 }, 0.f);
		}
		else {
			RenderHelper::getInstance()->texture("clickme_dark", translate.x, translate.y, mco.radius * 2, mco.radius * 2, 1, Color{ 0,0,0,1 }, 0.f);
		}
	}
}

void Event::_typingEventUpdate(EVENT_RESULTS& result, double dt) {
	/*update*/
	_updateTime(dt);

	switch (m_typingState) {
	case INNER_STATES::ON_ENTER:
		// on enter state
		m_currentWord = m__wordlist[rand() % m__wordlist.size()];

		m_typed.clear();
		// init map on which letters are typed
		for (const char c : m_currentWord) {
			m_typed.push_back({ c, false });
		}

		m_typingState = INNER_STATES::ON_UPDATE;
		break;

	case INNER_STATES::ON_UPDATE:
		// on update state
		if (m_elapsedTimeMs >= m_typingTimeoutMs || m_wordsCompleted >= m_typingMaxScore) {
			m_elapsedTimeMs = 0;
			m_eventMultiplier = static_cast<f32>(m_wordsCompleted) / m_typingMaxScore * m_maxMultiplier;
			m_typingState = INNER_STATES::ON_EXIT;
		}

		for (const std::pair<int, char> map : keyMappings) {
			if (AEInputCheckTriggered(static_cast<u8>(map.first))) {

				// set next to first iterator of vector
				auto next = &m_typed[0];
				// find the first letter that hasnt been typed
				while (next->second) {
					// word is fully typed. use guard to prevent out of range
					if (next == &m_typed.back()) {
						break;
					}
					next++;
				}

				if (m_typingState != INNER_STATES::ON_UPDATE) {
					break;
				}

				// mark letter as typed if correct key is triggered
				if (map.first + AEVK_OFFSET == next->first) {
					next->second = true;

					// finished typing. move on to next state
					if (next == &m_typed.back()) {
						m_wordsCompleted++;
						m_typingState = INNER_STATES::ON_NEXT;
						break;
					}
				}
			}
		}
		break;

	case INNER_STATES::ON_NEXT:
		m_typingState = INNER_STATES::ON_ENTER;
		break;

	case INNER_STATES::ON_EXIT:
		if (m_elapsedTimeMs >= m_typingTransitionTime * 1000) {
			result = EVENT_RESULTS::CUSTOM_MULTIPLIER;
			//_resetState();
		}
		break;
	}
}

void Event::_typingEventRender() {
	_renderTimer(m_totalElapsedMs, m_typingTimeoutMs);

	/*render*/
	float wordWidth = m_currentWord.size() * RenderHelper::getInstance()->getFontSize() + (m_currentWord.size() - 1) * m_charGap;
	const float start = AEGfxGetWindowWidth() / 2.f - wordWidth / 2.f;
	float currXOffset = start;
	float currYOffset = AEGfxGetWindowHeight() * 0.85f;
	int i{};

	switch (m_typingState) {
	case INNER_STATES::ON_ENTER:
	case INNER_STATES::ON_UPDATE:
	case INNER_STATES::ON_NEXT:
		for (const char c : m_currentWord) {
			Color col;

			// if has been typed, set color
			if (m_typed[i].second) {
				col = { 0, 1, 0, 1 };	// green
			}
			else {
				col = { 1,1,1, 1 };	// white
			}

			RenderHelper::getInstance()->text(std::string{ static_cast<char>(toupper(c)) }, currXOffset, currYOffset, col.r, col.g, col.b, col.a);
			currXOffset += RenderHelper::getInstance()->getFontSize() + m_charGap;

			i++;
		}
		break;


	case INNER_STATES::ON_EXIT:
		std::ostringstream oss;
		oss.precision(m_eventMultiplierPrecision);
		oss << std::fixed << m_eventMultiplier << "x";
		RenderHelper::getInstance()->text(oss.str(), AEGfxGetWindowWidth() / 2.f, currYOffset);

		if (m_elapsedTimeMs >= m_typingTransitionTime * 1000) {
			//result = EVENT_RESULTS::CUSTOM_MULTIPLIER
			_resetState();
		}
		break;
	}
}


void Event::_orangeEventUpdate(EVENT_RESULTS& result, double dt) {
	_updateTime(dt);

	switch (m_orangeState) {
	case INNER_STATES::ON_ENTER: {
		m_orangeObj = Orange{
			AEGfxGetWindowWidth() / 2.f, AEGfxGetWindowHeight() / 2.f,			// pos
			{0,0},										// velocity
			m_orangeRadius,							// radius
			false
		};

		for (Demon& d : m_demons) {
			const float demonSpeed = rand() % static_cast<int>(m_DEMON_MAX_SPEED - m_DEMON_MIN_SPEED) + m_DEMON_MIN_SPEED;

			d.x = rand() % static_cast<int>(AEGfxGetWindowWidth() - m_DEMON_RADIUS * 2) + m_DEMON_RADIUS;
			d.y = AEGfxGetWindowHeight() * 0.15f;
			AEVec2Set(&d.vel, rand() % 2 == 0 ? demonSpeed : -demonSpeed, 0);
			d.radius = m_DEMON_RADIUS;
			d.isActive = true;
		}

		m_orangeState = INNER_STATES::ON_UPDATE;
		break;
	}

	case INNER_STATES::ON_UPDATE: {
		int activeDemons{};
		for (const Demon& d : m_demons) {
			if (!d.isActive) {
				continue;
			}
			activeDemons++;
		}


		if (m_elapsedTimeMs >= m_orangeEventTimeoutMs || activeDemons == 0) {
			int hits{};
			for (const Demon& d : m_demons) {
				if (!d.isActive) {
					hits++;
				}
			}
			m_eventMultiplier = static_cast<f32>(hits) / m_NUM_DEMONS * m_maxMultiplier;

			m_orangeState = INNER_STATES::ON_EXIT;
			m_elapsedTimeMs = 0;
			break;
		}

		/*new impl*/
		if (AEInputCheckTriggered(AEVK_R)) {
			AEVec2Set(&m_orangeObj.vel, 0, 0);
			m_orangeObj.x = 0;
			m_orangeObj.y = 0;
		}

		if (AEInputCheckCurr(AEVK_LBUTTON) && CollisionChecker::isMouseInCircle(m_orangeObj.x, m_orangeObj.y, m_orangeObj.radius, static_cast<f32>(m_mouseX), static_cast<f32>(m_mouseY))) {
			m_orangeObj.isHeld = true;
		}
		// check to ensure that the user cannot hold the ball past 75% of the screen
		static constexpr float DEADZONE = 0.25f;
		if (m_orangeObj.isHeld && (m_orangeObj.y <= AEGfxGetWindowHeight() * DEADZONE || m_orangeObj.y >= static_cast<f32>(AEGfxGetWindowHeight()))) {
			m_orangeObj.isHeld = false;	// force user to let go
			AEVec2Set(&m_orangeObj.vel, 0, 0);			// reset velocity to 0
		}

		if (m_orangeObj.isHeld && AEInputCheckCurr(AEVK_LBUTTON)) {
			m_orangeObj.x = static_cast<f32>(m_mouseX);
			m_orangeObj.y = static_cast<f32>(m_mouseY);

			// get vector between last frame and this frame
			m_orangeObj.vel.x = static_cast<f32>(m_mouseX) - m_prevMouseX;
			m_orangeObj.vel.y = static_cast<f32>(m_mouseY) - m_prevMouseY;
		}
		else {
			m_orangeObj.isHeld = false;
		}


		//cout << (m_orangeObj.y <= AEGfxGetWindowHeight() * DEADZONE) << "\n";

		//cout << "obj vel: " << m_orangeObj.vel.x << ", " << m_orangeObj.vel.y << "\n";

		// obj not held, apply normal physics to object
		if (!m_orangeObj.isHeld) {
			// gravity
			if (m_orangeObj.y + m_orangeObj.radius + m_ORANGE_BORDER_PADDING <= AEGfxGetWindowHeight()) {
				m_orangeObj.vel.y += static_cast<f32>(m_ORANGE_GRAVITY * dt);
			}

			// resistance
			if (m_orangeObj.vel.x > 0) {

				m_orangeObj.vel.x -= m_orangeObj.vel.x - m_X_RESISTANCE * dt > 0 ? static_cast<f32>(m_X_RESISTANCE * dt) : m_orangeObj.vel.x;

				// set value to 0 when below 1 as float arithmetic is not precise
				if (m_orangeObj.vel.x < 1) {
					m_orangeObj.vel.x = 0;
				}
			}
			else if (m_orangeObj.vel.x < 0) {

				m_orangeObj.vel.x += m_orangeObj.vel.x + m_X_RESISTANCE * dt < 0 ? static_cast<f32>(m_X_RESISTANCE * dt) : m_orangeObj.vel.x;

				// set value to 0 when above -1 as float arithmetic is not precise
				if (m_orangeObj.vel.x > -1) {
					m_orangeObj.vel.x = 0;
				}
			}
			if (m_orangeObj.vel.y > 0) {

				m_orangeObj.vel.y -= m_orangeObj.vel.y - m_X_RESISTANCE * dt > 0 ? static_cast<f32>(m_X_RESISTANCE * dt) : m_orangeObj.vel.y;

				// set value to 0 when below 1 as float arithmetic is not precise
				// for y axis, must also check if object is near ground
				if (m_orangeObj.vel.y < 1 && m_orangeObj.y + m_orangeObj.radius >= AEGfxGetWindowHeight()) {
					m_orangeObj.vel.y = 0;
				}
			}
			else if (m_orangeObj.vel.y < 0) {

				m_orangeObj.vel.y += m_orangeObj.vel.y + m_X_RESISTANCE * dt < 0 ? static_cast<f32>(m_X_RESISTANCE * dt) : m_orangeObj.vel.y;

				// set value to 0 when above -1 as float arithmetic is not precise
				// for y axis, must also check if object is near ground
				if (m_orangeObj.vel.y > -1 && m_orangeObj.y + m_orangeObj.radius >= AEGfxGetWindowHeight()) {
					m_orangeObj.vel.y = 0;
				}
			}

			// clamp velocity
			m_orangeObj.vel.x = AEClamp(m_orangeObj.vel.x, -m_SPEED_LIMIT, m_SPEED_LIMIT);
			m_orangeObj.vel.y = AEClamp(m_orangeObj.vel.y, -m_SPEED_LIMIT, m_SPEED_LIMIT);

			// invert x/y vector when collide w wall to provide illusion of bouncing
			if (CollisionChecker::isRectTouchingScreenXBorder(m_orangeObj.x, m_orangeObj.y, m_orangeObj.radius * 2, m_orangeObj.radius * 2, m_ORANGE_BORDER_PADDING)) {
				m_orangeObj.vel.x = -m_orangeObj.vel.x * m_ENERGY_KEPT_BOUNCING;
			}
			if (CollisionChecker::isRectTouchingScreenYBorder(m_orangeObj.x, m_orangeObj.y, m_orangeObj.radius * 2, m_orangeObj.radius * 2, m_ORANGE_BORDER_PADDING)) {
				m_orangeObj.vel.y = -m_orangeObj.vel.y * m_ENERGY_KEPT_BOUNCING;
			}

			m_orangeObj.x += static_cast<f32>(m_orangeObj.vel.x * AEFrameRateControllerGetFrameRate() * dt);
			m_orangeObj.y += static_cast<f32>(m_orangeObj.vel.y * AEFrameRateControllerGetFrameRate() * dt);

			// clamp positions
			m_orangeObj.x = AEClamp(m_orangeObj.x, m_orangeObj.radius + m_ORANGE_BORDER_PADDING, AEGfxGetWindowWidth() - m_orangeObj.radius - m_ORANGE_BORDER_PADDING);
			m_orangeObj.y = AEClamp(m_orangeObj.y, m_orangeObj.radius + m_ORANGE_BORDER_PADDING, AEGfxGetWindowHeight() - m_orangeObj.radius - m_ORANGE_BORDER_PADDING);
		}

		// update demon
		for (Demon& d : m_demons) {
			if (!d.isActive) {
				continue;
			}

			if (CollisionChecker::isRectTouchingScreenXBorder(d.x, d.y, d.radius, d.radius)) {
				d.vel.x = -d.vel.x;
			}

			d.x += static_cast<f32>(d.vel.x * dt);
			d.y += static_cast<f32>(d.vel.y * dt);

			// check for collision with ball (orange?)
			if (CollisionChecker::areCirclesIntersecting(d.x, d.y, d.radius, m_orangeObj.x, m_orangeObj.y, m_orangeObj.radius)) {
				//cout << "collided\n";
				d.isActive = false;
			}
		}


		break;
	}

	case INNER_STATES::ON_NEXT:
		break;

	case INNER_STATES::ON_EXIT:
		if (m_elapsedTimeMs >= m_afterEventDisplayTimeoutMs) {
			result = EVENT_RESULTS::CUSTOM_MULTIPLIER;
			_resetState();
		}
		break;
	}
}

void Event::_orangeEventRender() {
	switch (m_orangeState) {
	case INNER_STATES::ON_ENTER:
		break;

	case INNER_STATES::ON_UPDATE:
		AEVec2 pos = stow(m_orangeObj.x, m_orangeObj.y);
		RenderHelper::getInstance()->texture("ball", pos.x + camOffset.x, pos.y + camOffset.y, m_orangeObj.radius, m_orangeObj.radius, 1.f, Color{ 1.f, 0, 0, 1.f }, 0);
		for (const Demon& d : m_demons) {
			if (!d.isActive) {
				continue;
			}

			pos = stow(d.x, d.y);
			RenderHelper::getInstance()->texture("nian", pos.x + camOffset.x, pos.y + camOffset.y, d.radius, d.radius, 1, Color{ 1,0,0,1 }, degToRad(0));
		}
		_renderTimer(m_elapsedTimeMs, m_orangeEventTimeoutMs);
		break;

	case INNER_STATES::ON_NEXT:
		break;

	case INNER_STATES::ON_EXIT:
		std::ostringstream oss;
		oss.precision(m_eventMultiplierPrecision);
		oss << std::fixed << m_eventMultiplier << "x";
		RenderHelper::getInstance()->text(oss.str(), AEGfxGetWindowWidth() / 2.f, AEGfxGetWindowHeight() / 2.f);
		break;
	}
}
