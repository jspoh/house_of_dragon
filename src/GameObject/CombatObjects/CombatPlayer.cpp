/* Start Header ************************************************************************/
/*!
\file CombatPlayer.cpp
\author Poh Jing Seng, jingseng.poh, 2301363 (60%, base infra and player logic)
\par jingseng.poh\@digipen.edu
\author Kuek wei jie, weijie.kuek, 2301325 (30%, rendering)
\par weijie.kuek\@digipen.edu
\author Soh Wei Jie, weijie.soh (10%, hands rendering)
\par weijie.soh\@digipen.edu
\date 28 feb 2024
\brief handles player in combat
/*
Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#include "Pch.h"
#include "CombatPlayer.h"
#include "CombatManager.h"

namespace {
	//First data is left, second data is right
	std::pair<AEMtx33, AEMtx33> Hand1PosData{};
	std::pair<AEMtx33, AEMtx33> Hand2PosData{};
	std::pair<AEMtx33, AEMtx33> Hand3PosData{};
	std::pair<AEMtx33, AEMtx33> Hand4PosData{};

	static int t_AnimationFrame = 0;
	static double t_AnimationDuration = 0.0;
	static bool LeftSide = false;
	double LerpSpeed = 10.0;
	AEVec2 targetPos{};
	f32 camX, camY;
}


Player::Player(float _health, float _dmg, Element element) : Mob(element, _health, _dmg* DIFFICULTY_PLAYER_DAMAGE_MULTIPLIER.at(difficulty)) {
	//RenderHelper::getInstance()->registerTexture("m_shield", "./Assets/Combat_UI/m_shield.png");
	//float StartHealth = m_health;		// what is this for?
	// set m_shield properties
	AEVec2Set(&m_shield.pos, -AEGfxGetWindowWidth() / 2.f, -AEGfxGetWindowHeight() / 2.f * 2.f);
	AEVec2Set(&m_shield.size, AEGfxGetWindowWidth() * 0.75f, (AEGfxGetWindowWidth() / 2.f) * 2.f);

	AEVec2Set(&m_shieldInitialPos, m_shield.pos.x, m_shield.pos.y);
	AEVec2Set(&m_shieldBlockingPos, 0, -AEGfxGetWindowHeight() * 0.8f);

	// set vector from m_shield inital to final position
	AEVec2Set(&m_shieldInitialToShieldBlockingVector, m_shieldBlockingPos.x - m_shieldInitialPos.x, m_shieldBlockingPos.y - m_shieldInitialPos.y);
	AEVec2Normalize(&m_shieldInitialToShieldBlockingVector, &m_shieldInitialToShieldBlockingVector);

	// get transition speed
	float initialFinalDistance = AEVec2Distance(&m_shieldInitialPos, &m_shieldBlockingPos);
	m_transitionUpSpeed = initialFinalDistance / (m_shieldUpTransitionTimeMs / 1000.f);
	m_transitionDownSpeed = initialFinalDistance / (m_shieldDownTransitionTimeMs / 1000.f);

	// init level
	m_playerLevel = Database::getInstance().data["player"]["level"];

	// increase player damage by percentages
	//m_health += m_health * ((m_playerLevel - 1) * levelHealthIncPercentage);
	m_dmg += m_dmg * ((m_playerLevel - 1) * m_LEVEL_DAMAGE_INC_PERCENTAGE);

	m_hasAttacked = false;
	m_attackedRenderX = m_health / 80;

	cout << "Player initialized with " << m_health << " health and " << m_dmg << " damage\n";

	// init inventory
	for (const auto& [itemName, qtyHolding] : Database::getInstance().data["player"]["inventory"].items()) {
		m_inventory[itemName] = qtyHolding;
		cout << "Created item in inventory with type " << itemName << " with qty " << Database::getInstance().data["player"]["inventory"][itemName] << "\n";
	}


}

Player::~Player() {
	//RenderHelper::getInstance()->removeTextureByRef("m_shield");	// let renderhelper manage

	for (const auto& [itemName, itemQty] : m_inventory) {
		Database::getInstance().data["player"]["inventory"][itemName] = itemQty;
	}

	Database::getInstance().data["player"]["level"] = m_playerLevel;

}

void Player::playerAttacked() {
	this->m_hasAttacked = true;
	this->m_attackedRenderXprev = m_attackedRenderX;
	m_healthRenderTime = 0.f;

}


void Player::healthGain(float healthIncrease) {
	float newhealth = this->m_health + healthIncrease;
	if (newhealth < 100.f) {
		this->m_health = newhealth;
	}
	else {
		this->m_health = 100.f;
	}

}

void Player::_drawHealth(float screenX, float screenY) {
	std::string name = "Player";
	std::string level = "  Lv:" + std::to_string(m_playerLevel);
	float paddingY = 50;
	float paddingX = 90;
	AEVec2 panelPos = stow(screenX, screenY);
	//panel rendering
	f32 truex, truey;
	AEGfxGetCamPosition(&truex, &truey);
	RenderHelper::getInstance()->texture("panel", panelPos.x + truex, panelPos.y + truey + paddingY, 270, 100);
	RenderHelper::getInstance()->text(name + level, screenX, screenY - 65);
	if (this->m_health > 66) {
		RenderHelper::getInstance()->texture("greenbar1", panelPos.x - paddingX + truex, panelPos.y + truey + paddingY - 20, 10, 15); //start point, but coordinates is centralised so need to take account of the widthw
		RenderHelper::getInstance()->texture("greenbar3", panelPos.x + truex - paddingX + 5 + m_attackedRenderX * 50, panelPos.y + truey + paddingY - 20, m_attackedRenderX * 100, 15);
		RenderHelper::getInstance()->texture("greenbar2", panelPos.x + truex + m_attackedRenderX * 100 - paddingX + 10, panelPos.y + truey + paddingY - 20, 10, 15);
	}
	else if (this->m_health > 33) {
		RenderHelper::getInstance()->texture("yellowbar1", panelPos.x - paddingX + truex, panelPos.y + truey + paddingY - 20, 10, 15); //start point, but coordinates is centralised so need to take account of the widthw
		RenderHelper::getInstance()->texture("yellowbar3", panelPos.x + truex - paddingX + 5 + m_attackedRenderX * 50, panelPos.y + truey + paddingY - 20, m_attackedRenderX * 100, 15);
		RenderHelper::getInstance()->texture("yellowbar2", panelPos.x + truex + m_attackedRenderX * 100 - paddingX + 10, panelPos.y + truey + paddingY - 20, 10, 15);
	}
	else {
		RenderHelper::getInstance()->texture("redbar1", panelPos.x - paddingX + truex, panelPos.y + truey + paddingY - 20, 10, 15); //start point, but coordinates is centralised so need to take account of the widthw
		RenderHelper::getInstance()->texture("redbar3", panelPos.x + truex - paddingX + 5 + m_attackedRenderX * 50, panelPos.y + truey + paddingY - 20, m_attackedRenderX * 100, 15);
		RenderHelper::getInstance()->texture("redbar2", panelPos.x + truex + m_attackedRenderX * 100 - paddingX + 10, panelPos.y + truey + paddingY - 20, 10, 15);

	}
}

void Player::update(double dt) {
	m_elapsedTimeMs += static_cast<int>(dt * 1000);

	updateHands(static_cast<float>(dt));

	//cout << mouseX << " | " << mouseY << "\n";
	//cout << camOffset.x << " | " << camOffset.y << "\n";

	/* blocking stuff */
	//HandStateAnimationType;
	if (this->m_hasAttacked && this->m_healthRenderTime < this->m_healthRenderTimeMax) {
		m_healthRenderTime += static_cast<float>(dt);
		float percenttime = static_cast<float>(m_healthRenderTime / m_healthRenderTimeMax);
		float t = percenttime;
		if (t > m_healthRenderTimeMax) {
			t = m_healthRenderTimeMax;
		}
		this->m_attackedRenderX = lerp(this->m_attackedRenderXprev, m_health / 80, t);
	}
	else {
		this->m_hasAttacked = false;
		this->m_healthRenderTime = 0.f;
	}

	if (
		AEInputCheckTriggered(AEVK_SPACE)
		&& m_blockingState == PLAYER_BLOCKING_STATES::NOT_BLOCKING
		&& CombatManager::getInstance().m_turn == CombatManager::TURN::ENEMY
		) {
		setHandStateAnimationType(HandAnimationType::Block);
	}
	//else if (!AEInputCheckCurr(AEVK_SPACE) && m_blockingState != PLAYER_BLOCKING_STATES::NOT_BLOCKING && m_blockingState != PLAYER_BLOCKING_STATES::ON_COOLDOWN) {
	//	m_elapsedTimeMs = 0;
	//	m_blockingState = PLAYER_BLOCKING_STATES::ON_EXIT;
	//}

	//cout << static_cast<int>(m_blockingState) << "\n";

	switch (m_blockingState) {
	case PLAYER_BLOCKING_STATES::NOT_BLOCKING:
		//cout << "Player blocking state: NOT_BLOCKING\n";
		m_elapsedTimeMs = 0;
		break;
	case PLAYER_BLOCKING_STATES::ON_ENTER:
		//cout << "Player blocking state: ON_ENTER\n";
		if (m_elapsedTimeMs >= m_shieldUpTransitionTimeMs || AEVec2Distance(&m_shield.pos, &m_shieldBlockingPos) <= m_snapThreshold) {
			m_blockingState = PLAYER_BLOCKING_STATES::ON_UPDATE;
			m_elapsedTimeMs = 0;
		}
		break;
	case PLAYER_BLOCKING_STATES::ON_UPDATE:
		//cout << "Player blocking state: ON_UPDATE\n";
		if (m_elapsedTimeMs >= m_shieldUpTimeMs) {
			m_blockingState = PLAYER_BLOCKING_STATES::ON_EXIT;
			m_elapsedTimeMs = 0;
		}
		break;
	case PLAYER_BLOCKING_STATES::ON_EXIT:
		//cout << "Player blocking state: ON_EXIT\n";
		if (m_elapsedTimeMs >= m_shieldDownTransitionTimeMs || AEVec2Distance(&m_shield.pos, &m_shieldInitialPos) <= m_snapThreshold) {
			m_blockingState = PLAYER_BLOCKING_STATES::ON_COOLDOWN;
			m_elapsedTimeMs = 0;
		}
		break;
	case PLAYER_BLOCKING_STATES::ON_COOLDOWN:
		//cout << "Player blocking state: ON_COOLDOWN\n";
		if (m_elapsedTimeMs >= m_timeBeforeNextBlockMs) {
			m_elapsedTimeMs = 0;
			m_HandStateAnimationType = HandAnimationType::None;
			m_blockingState = PLAYER_BLOCKING_STATES::NOT_BLOCKING;
		}
		break;
	}

	//_updateShield(dt);
	_updateBlockingHands();
}

void Player::render() {
	//_renderShield();
	_renderHands();
}
void Player::renderHealth(double x, double y) {
	this->_drawHealth(static_cast<float>(x), static_cast<float>(y));
}

void Player::setNextAttackDmgMul(float mul) {
	m_dmgMul = mul;
}

float Player::attack(Mob& target, Element attackEl, float qtMultiplier) {



	DamageMultiplier dm = ElementProperties::getEffectiveDamage(attackEl, target.element);
	float elementMul = 1;
	cout << "attackEl enum: " << attackEl << "\n";
	cout << "targetEl enum: " << target.element << "\n";
	cout << "Damage multiplier enum: " << dm << "\n";
	switch (dm) {
	case Weak:
		elementMul = 0.5;
		break;
	case Strong:
		elementMul = 2;
		break;
	}

	const float effectiveDmgMul = elementMul * qtMultiplier * m_dmgMul;
	const float damage = this->m_dmg * effectiveDmgMul;
	m_dmgMul = m_DEFAULT_DMG_MUL;
	target.m_health -= damage;
	cout << "Attacking enemy with a total dmg multiplier of " << effectiveDmgMul << " (not inclusive of difficulty)\n";
	return damage;
}


void Player::setHandStateAnimationType(HandAnimationType t) {
	m_HandStateAnimationType = t;

	switch (m_HandStateAnimationType) {
	case HandAnimationType::Block:
		if (m_blockingState != PLAYER_BLOCKING_STATES::NOT_BLOCKING) {
			break;		// dont allow player to try to trigger block while not not blocking (yes its not not blocking and not just blocking)
		}
		m_elapsedTimeMs = 0;
		m_blockingState = PLAYER_BLOCKING_STATES::ON_ENTER;
		break;
	}
}

void Player::updateHands(float t_dt)
{
	LerpSpeed = 10.0;
	updateGlobals();

	static int mX{}, mY{};
	switch (m_HandStateAnimationType)
	{
	case HandAnimationType::Punch:

		if (!LeftSide) //Left Hand Punch
			switch (t_AnimationFrame)
			{
			case 0://Init
				AEMtx33Identity(&Hand2PosData.first);
				AEMtx33ScaleApply(&Hand2PosData.first, &Hand2PosData.first, 191.5, 307);
				targetPos = { -804.25f + camOffset.x, -526.f + camOffset.y };
				AEMtx33TransApply(&Hand2PosData.first, &Hand2PosData.first, targetPos.x, targetPos.y);
				mX = mouseX;
				mY = mouseY;
				mX -= AEGfxGetWindowWidth() / 2;
				mY -= AEGfxGetWindowHeight() / 2;
				mY *= -1;
				if (t_AnimationDuration > 999) t_AnimationDuration = 0.0;
				break;
			case 1: //End 

				targetPos = { (float)mX - 166.0f + camX, (float)mY - 198.0f + camY };
				LerpSpeed = 1.05;
				Hand2PosData.first.m[0][2] += static_cast<float>(abs((targetPos.x - Hand2PosData.first.m[0][2]) / LerpSpeed) > 0.5 ? ((targetPos.x - Hand2PosData.first.m[0][2]) / LerpSpeed) : 0);
				Hand2PosData.first.m[1][2] += static_cast<float>(abs((targetPos.y - Hand2PosData.first.m[1][2]) / LerpSpeed) > 0.5 ? ((targetPos.y - Hand2PosData.first.m[1][2]) / LerpSpeed) : 0);
				if (t_AnimationDuration > 999) t_AnimationDuration = 0.25;
				break;
			}
		else
			switch (t_AnimationFrame)
			{
			case 0://Init
				AEMtx33Identity(&Hand2PosData.second);
				AEMtx33ScaleApply(&Hand2PosData.second, &Hand2PosData.second, 191.5, 307);
				targetPos = { 804.25f + camX, -526.f + camY };
				AEMtx33TransApply(&Hand2PosData.second, &Hand2PosData.second, targetPos.x, targetPos.y);
				mX = mouseX;
				mY = mouseY;
				mX -= AEGfxGetWindowWidth() / 2;
				mY -= AEGfxGetWindowHeight() / 2;
				mY *= -1;
				if (t_AnimationDuration > 999) t_AnimationDuration = 0.0;
				break;
			case 1: //End Point
				targetPos = { (float)mX + 166.0f + camX, (float)mY - 198.0f + camY };
				LerpSpeed = 1.05;
				Hand2PosData.second.m[0][2] += static_cast<float>(abs((targetPos.x - Hand2PosData.second.m[0][2]) / LerpSpeed) > 0.5 ? ((targetPos.x - Hand2PosData.second.m[0][2]) / LerpSpeed) : 0);
				Hand2PosData.second.m[1][2] += static_cast<float>(abs((targetPos.y - Hand2PosData.second.m[1][2]) / LerpSpeed) > 0.5 ? ((targetPos.y - Hand2PosData.second.m[1][2]) / LerpSpeed) : 0);
				if (t_AnimationDuration > 999) t_AnimationDuration = 0.25;
				break;
			}
		if (t_AnimationDuration < 0.0)
		{
			if (t_AnimationFrame < 1)
				++t_AnimationFrame;//Loop Animation // Remove this if u want one off
			else
			{
				t_AnimationFrame = 0;
				m_HandStateAnimationType = HandAnimationType::None;
			}
			t_AnimationDuration = 9999.0;
			if (t_AnimationFrame == 0)
			{
				LeftSide = rand() % 2 - 1;
				Hand1PosData.first = Hand1PosData.second = {};
				Hand2PosData.first = Hand2PosData.second = {};
				Hand3PosData.first = Hand3PosData.second = {};
				Hand4PosData.first = Hand4PosData.second = {};
			}
		}
		break;
	case HandAnimationType::Block:
		//_updateBlockingHands();
		break;

	case HandAnimationType::Ready: //For Getting ready in combat
		switch (t_AnimationFrame)
		{
			//case 4:		// blocking cooldown state, but does not exist for ready
			//	t_AnimationFrame = 0;
			//	[[fallthrough]];
		case 0://Init
			AEMtx33Identity(&Hand4PosData.second);
			Hand2PosData.first = Hand4PosData.second;
			AEMtx33ScaleApply(&Hand2PosData.first, &Hand2PosData.first, 191.5, 307);
			targetPos = { -39.0f + camX, -170.0f + camY };
			AEMtx33TransApply(&Hand2PosData.first, &Hand2PosData.first, targetPos.x, targetPos.y);
			AEMtx33ScaleApply(&Hand4PosData.second, &Hand4PosData.second, 252, 319);
			targetPos = { 45.1f + camX, -76.5f + camY };
			AEMtx33TransApply(&Hand4PosData.second, &Hand4PosData.second, targetPos.x, targetPos.y);
			if (t_AnimationDuration > 999) t_AnimationDuration = 0.0;
			break;
		case 1://Ready Up
			targetPos = { -39.0f + camX, -170.0f + camY };
			LerpSpeed = 2;
			Hand2PosData.first.m[0][2] += static_cast<float>((targetPos.x - Hand2PosData.first.m[0][2]) / LerpSpeed);
			Hand2PosData.first.m[1][2] += static_cast<float>((targetPos.y - Hand2PosData.first.m[1][2]) / LerpSpeed);
			targetPos = { 45.1f + camX, -76.5f + camY };
			LerpSpeed = 2;
			Hand4PosData.second.m[0][2] += static_cast<float>((targetPos.x - Hand4PosData.second.m[0][2]) / LerpSpeed);
			Hand4PosData.second.m[1][2] += static_cast<float>((targetPos.y - Hand4PosData.second.m[1][2]) / LerpSpeed);
			if (t_AnimationDuration > 999) t_AnimationDuration = 3;
			break;

		case 2: //Ready Down
			targetPos = { -39.0f + camX, -526.f + camY };
			LerpSpeed = 15;
			Hand2PosData.first.m[0][2] += static_cast<float>((targetPos.x - Hand2PosData.first.m[0][2]) / LerpSpeed);
			Hand2PosData.first.m[1][2] += static_cast<float>((targetPos.y - Hand2PosData.first.m[1][2]) / LerpSpeed);
			targetPos = { 45.1f + camX, -526.f + camY };
			LerpSpeed = 15;
			Hand4PosData.second.m[0][2] += static_cast<float>((targetPos.x - Hand4PosData.second.m[0][2]) / LerpSpeed);
			Hand4PosData.second.m[1][2] += static_cast<float>((targetPos.y - Hand4PosData.second.m[1][2]) / LerpSpeed);
			if (t_AnimationDuration > 999) t_AnimationDuration = 3;
			break;
		default:
			cout << "ERROR IN READYING ANIMATION\n";
		}
		if (t_AnimationDuration < 0.0)
		{
			t_AnimationFrame = t_AnimationFrame < 2 ? ++t_AnimationFrame : 1; //Loop Animation // Remove this if u want one off
			t_AnimationDuration = 9999.0;
			if (t_AnimationFrame == 0)
			{
				Hand1PosData.first = Hand1PosData.second = {};
				Hand2PosData.first = Hand2PosData.second = {};
				Hand3PosData.first = Hand3PosData.second = {};
				Hand4PosData.first = Hand4PosData.second = {};
			}
		}
		break;
	default:
		t_AnimationFrame = 0;	// go to cooldown if not not blocking(blocking or transition state)
		t_AnimationDuration = 9999.0;	// dont reset timer until cooldown is over
		Hand1PosData.first = Hand1PosData.second = {};
		Hand2PosData.first = Hand2PosData.second = {};
		Hand3PosData.first = Hand3PosData.second = {};
		Hand4PosData.first = Hand4PosData.second = {};
		break;
	}

	t_AnimationDuration -= t_dt;

	//AEMtx33ScaleApply(&Hand1PosData.first, &Hand1PosData.first, 200, 318);
//AEMtx33ScaleApply(&Hand2PosData.second, &Hand2PosData.second, 191.5, 307);
//AEMtx33ScaleApply(&Hand3PosData.second, &Hand3PosData.second, 238, 333);
//AEMtx33ScaleApply(&Hand4PosData.second, &Hand4PosData.second, 252, 319);
//AEMtx33TransApply(&Hand4PosData.second, &Hand4PosData.second, 200, 0);

}

void Player::_updateBlockingHands() {

	static PLAYER_BLOCKING_STATES prevState = PLAYER_BLOCKING_STATES::ON_COOLDOWN;

	//cout << static_cast<int>(m_blockingState) << ", " << static_cast<int>(prevState) << "\n";

	if (m_blockingState != PLAYER_BLOCKING_STATES::NOT_BLOCKING && prevState == PLAYER_BLOCKING_STATES::NOT_BLOCKING) {
		LeftSide = rand() % 2 - 1;
		if (!LeftSide) {
			AEMtx33Identity(&Hand3PosData.second);
			Hand1PosData.first = Hand3PosData.second;
			AEMtx33ScaleApply(&Hand1PosData.first, &Hand1PosData.first, 200, 318);
			targetPos = { -304.25f + camOffset.x, -526.f + camOffset.y };
			AEMtx33TransApply(&Hand1PosData.first, &Hand1PosData.first, targetPos.x, targetPos.y);
			AEMtx33ScaleApply(&Hand3PosData.second, &Hand3PosData.second, 238, 333);
			targetPos = { -160.95f + camOffset.x, -499.5f + camOffset.y };
			AEMtx33TransApply(&Hand3PosData.second, &Hand3PosData.second, targetPos.x, targetPos.y);
		}
		else {
			AEMtx33Identity(&Hand3PosData.first);
			Hand1PosData.second = Hand3PosData.first;
			AEMtx33ScaleApply(&Hand3PosData.first, &Hand3PosData.first, 238, 333);
			targetPos = { 160.95f + camOffset.x, -499.5f + camOffset.y };
			AEMtx33TransApply(&Hand3PosData.first, &Hand3PosData.first, targetPos.x, targetPos.y);
			AEMtx33ScaleApply(&Hand1PosData.second, &Hand1PosData.second, 200, 318);
			targetPos = { 304.25f + camOffset.x, -526.f + camOffset.y };
			AEMtx33TransApply(&Hand1PosData.second, &Hand1PosData.second, targetPos.x, targetPos.y);
		}
	}

	//cout << static_cast<int>(m_blockingState) << "\n";
	//LeftSide = false;

	if (!LeftSide) //Right Hand Blocking
		switch (m_blockingState)
		{
		case PLAYER_BLOCKING_STATES::NOT_BLOCKING:
			//Init
			break;
		case PLAYER_BLOCKING_STATES::ON_ENTER:
			//Start of block
			targetPos = { -304.25f + camOffset.x, -384.f + camOffset.y };
			LerpSpeed = 2;
			Hand1PosData.first.m[0][2] += static_cast<float>((targetPos.x - Hand1PosData.first.m[0][2]) / LerpSpeed);
			Hand1PosData.first.m[1][2] += static_cast<float>((targetPos.y - Hand1PosData.first.m[1][2]) / LerpSpeed);
			targetPos = { -160.95f + camOffset.x, -266.4f + camOffset.y };
			LerpSpeed = 2;
			Hand3PosData.second.m[0][2] += static_cast<float>((targetPos.x - Hand3PosData.second.m[0][2]) / LerpSpeed);
			Hand3PosData.second.m[1][2] += static_cast<float>((targetPos.y - Hand3PosData.second.m[1][2]) / LerpSpeed);
			break;
		case PLAYER_BLOCKING_STATES::ON_UPDATE:
			//Hold
			targetPos = { -304.25f + camOffset.x, -384.0f + camOffset.y };
			LerpSpeed = 1.1;
			Hand1PosData.first.m[0][2] += static_cast<float>((targetPos.x - Hand1PosData.first.m[0][2]) / LerpSpeed);
			Hand1PosData.first.m[1][2] += static_cast<float>((targetPos.y - Hand1PosData.first.m[1][2]) / LerpSpeed);
			targetPos = { 277.5f + camOffset.x, -94.35f + camOffset.y };
			LerpSpeed = 15;
			Hand3PosData.second.m[0][2] += static_cast<float>((targetPos.x - Hand3PosData.second.m[0][2]) / LerpSpeed);
			Hand3PosData.second.m[1][2] += static_cast<float>((targetPos.y - Hand3PosData.second.m[1][2]) / (LerpSpeed / 4));
			break;
		case PLAYER_BLOCKING_STATES::ON_EXIT:
			//Exit
			targetPos = { -425.35f + camOffset.x, -498.5f + camOffset.y };
			LerpSpeed = 5;
			Hand1PosData.first.m[0][2] += static_cast<float>((targetPos.x - Hand1PosData.first.m[0][2]) / LerpSpeed);
			Hand1PosData.first.m[1][2] += static_cast<float>((targetPos.y - Hand1PosData.first.m[1][2]) / LerpSpeed);
			targetPos = { 555.f + camOffset.x, -510.25f + camOffset.y };
			LerpSpeed = 5;
			Hand3PosData.second.m[0][2] += static_cast<float>((targetPos.x - Hand3PosData.second.m[0][2]) / LerpSpeed);
			Hand3PosData.second.m[1][2] += static_cast<float>((targetPos.y - Hand3PosData.second.m[1][2]) / LerpSpeed);

			break;
		case PLAYER_BLOCKING_STATES::ON_COOLDOWN:
			// cooldown
			//LeftSide = rand() % 2 - 1;		// huh why -1 here
			Hand1PosData.first = Hand1PosData.second = {};
			Hand2PosData.first = Hand2PosData.second = {};
			Hand3PosData.first = Hand3PosData.second = {};
			Hand4PosData.first = Hand4PosData.second = {};
			break;
		default:
			cout << "ERROR IN BLOCKING ANIMATION\n";
		}
	else //Left Hand Blocking
		switch (m_blockingState)
		{
		case PLAYER_BLOCKING_STATES::NOT_BLOCKING:
			//Init
			//AEMtx33Identity(&Hand3PosData.first);
			//Hand1PosData.second = Hand3PosData.first;
			//AEMtx33ScaleApply(&Hand3PosData.first, &Hand3PosData.first, 238, 333);
			//targetPos = { 160.95f + camOffset.x, -499.5f + camOffset.y };
			//AEMtx33TransApply(&Hand3PosData.first, &Hand3PosData.first, targetPos.x, targetPos.y);
			//AEMtx33ScaleApply(&Hand1PosData.second, &Hand1PosData.second, 200, 318);
			//targetPos = { 304.25f + camOffset.x, -526.f + camOffset.y };
			//AEMtx33TransApply(&Hand1PosData.second, &Hand1PosData.second, targetPos.x, targetPos.y);
			break;
		case PLAYER_BLOCKING_STATES::ON_ENTER:
			//Start of block
			targetPos = { 160.95f + camOffset.x, -266.4f + camOffset.y };
			LerpSpeed = 2;
			Hand3PosData.first.m[0][2] += static_cast<float>((targetPos.x - Hand3PosData.first.m[0][2]) / LerpSpeed);
			Hand3PosData.first.m[1][2] += static_cast<float>((targetPos.y - Hand3PosData.first.m[1][2]) / LerpSpeed);
			targetPos = { 304.25f + camOffset.x, -384.f + camOffset.y };
			LerpSpeed = 2;
			Hand1PosData.second.m[0][2] += static_cast<float>((targetPos.x - Hand1PosData.second.m[0][2]) / LerpSpeed);
			Hand1PosData.second.m[1][2] += static_cast<float>((targetPos.y - Hand1PosData.second.m[1][2]) / LerpSpeed);

			break;
		case PLAYER_BLOCKING_STATES::ON_UPDATE:
			//Hold
			targetPos = { -277.5f + camOffset.x, -94.35f + camOffset.y };
			LerpSpeed = 15;
			Hand3PosData.first.m[0][2] += static_cast<float>((targetPos.x - Hand3PosData.first.m[0][2]) / LerpSpeed);
			Hand3PosData.first.m[1][2] += static_cast<float>((targetPos.y - Hand3PosData.first.m[1][2]) / (LerpSpeed / 4));
			targetPos = { 304.25f + camOffset.x, -384.0f + camOffset.y };
			LerpSpeed = 1.1;
			Hand1PosData.second.m[0][2] += static_cast<float>((targetPos.x - Hand1PosData.second.m[0][2]) / LerpSpeed);
			Hand1PosData.second.m[1][2] += static_cast<float>((targetPos.y - Hand1PosData.second.m[1][2]) / LerpSpeed);
			break;
		case PLAYER_BLOCKING_STATES::ON_EXIT:
			//Exit
			targetPos = { -555.f + camOffset.x, -510.25f + camOffset.y };
			LerpSpeed = 5;
			Hand3PosData.first.m[0][2] += static_cast<float>((targetPos.x - Hand3PosData.first.m[0][2]) / LerpSpeed);
			Hand3PosData.first.m[1][2] += static_cast<float>((targetPos.y - Hand3PosData.first.m[1][2]) / LerpSpeed);
			targetPos = { 425.35f + camOffset.x, -498.5f + camOffset.y };
			LerpSpeed = 5;
			Hand1PosData.second.m[0][2] += static_cast<float>((targetPos.x - Hand1PosData.second.m[0][2]) / LerpSpeed);
			Hand1PosData.second.m[1][2] += static_cast<float>((targetPos.y - Hand1PosData.second.m[1][2]) / LerpSpeed);

			break;
		case PLAYER_BLOCKING_STATES::ON_COOLDOWN:
			// cooldown
			//LeftSide = rand() % 2 - 1;		// huh why -1 here
			Hand1PosData.first = Hand1PosData.second = {};
			Hand2PosData.first = Hand2PosData.second = {};
			Hand3PosData.first = Hand3PosData.second = {};
			Hand4PosData.first = Hand4PosData.second = {};
			break;
		default:
			cout << "ERROR IN BLOCKING ANIMATION\n";
		}

	prevState = m_blockingState;
}

void Player::_renderHands()
{
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetTransparency(1.0f);
	AEGfxSetTransform(Hand4PosData.first.m);
	AEGfxTextureSet(RenderHelper::getInstance()->getTextureByRef("Player_Fist_Left_04"), 0, 0);
	AEGfxMeshDraw(RenderHelper::getInstance()->getdefaultMesh(), AE_GFX_MDM_TRIANGLES);
	AEGfxSetTransform(Hand4PosData.second.m);
	AEGfxTextureSet(RenderHelper::getInstance()->getTextureByRef("Player_Fist_Right_04"), 0, 0);
	AEGfxMeshDraw(RenderHelper::getInstance()->getdefaultMesh(), AE_GFX_MDM_TRIANGLES);

	AEGfxSetTransform(Hand1PosData.first.m);
	AEGfxTextureSet(RenderHelper::getInstance()->getTextureByRef("Player_Fist_Left_01"), 0, 0);
	AEGfxMeshDraw(RenderHelper::getInstance()->getdefaultMesh(), AE_GFX_MDM_TRIANGLES);
	AEGfxSetTransform(Hand1PosData.second.m);
	AEGfxTextureSet(RenderHelper::getInstance()->getTextureByRef("Player_Fist_Right_01"), 0, 0);
	AEGfxMeshDraw(RenderHelper::getInstance()->getdefaultMesh(), AE_GFX_MDM_TRIANGLES);
	AEGfxSetTransform(Hand2PosData.first.m);
	AEGfxTextureSet(RenderHelper::getInstance()->getTextureByRef("Player_Fist_Left_02"), 0, 0);
	AEGfxMeshDraw(RenderHelper::getInstance()->getdefaultMesh(), AE_GFX_MDM_TRIANGLES);
	AEGfxSetTransform(Hand2PosData.second.m);
	AEGfxTextureSet(RenderHelper::getInstance()->getTextureByRef("Player_Fist_Right_02"), 0, 0);
	AEGfxMeshDraw(RenderHelper::getInstance()->getdefaultMesh(), AE_GFX_MDM_TRIANGLES);
	AEGfxSetTransform(Hand3PosData.first.m);
	AEGfxTextureSet(RenderHelper::getInstance()->getTextureByRef("Player_Fist_Left_03"), 0, 0);
	AEGfxMeshDraw(RenderHelper::getInstance()->getdefaultMesh(), AE_GFX_MDM_TRIANGLES);
	AEGfxSetTransform(Hand3PosData.second.m);
	AEGfxTextureSet(RenderHelper::getInstance()->getTextureByRef("Player_Fist_Right_03"), 0, 0);
	AEGfxMeshDraw(RenderHelper::getInstance()->getdefaultMesh(), AE_GFX_MDM_TRIANGLES);

}

void Player::_updateShield(double dt) {
	switch (m_blockingState) {
	case PLAYER_BLOCKING_STATES::NOT_BLOCKING:
		break;

	case PLAYER_BLOCKING_STATES::ON_ENTER:
		//cout << "Player blocking state: ON_ENTER\n";
		if (m_elapsedTimeMs >= m_shieldUpTransitionTimeMs || AEVec2Distance(&m_shield.pos, &m_shieldBlockingPos) <= m_snapThreshold) {
			// force m_shield to go to final pos
			AEVec2Set(&m_shield.pos, m_shieldBlockingPos.x, m_shieldBlockingPos.y);
			break;
		}

		// translate the m_shield up
		m_shield.pos.x += static_cast<float>(m_shieldInitialToShieldBlockingVector.x * m_transitionUpSpeed * dt);
		m_shield.pos.y += static_cast<float>(m_shieldInitialToShieldBlockingVector.y * m_transitionUpSpeed * dt);
		break;

	case PLAYER_BLOCKING_STATES::ON_UPDATE:
		break;

	case PLAYER_BLOCKING_STATES::ON_EXIT:
		//cout << "Player blocking state: ON_EXIT\n";
		if (m_elapsedTimeMs >= m_shieldDownTransitionTimeMs || AEVec2Distance(&m_shield.pos, &m_shieldInitialPos) <= m_snapThreshold) {
			// force m_shield to go to initial pos
			AEVec2Set(&m_shield.pos, m_shieldInitialPos.x, m_shieldInitialPos.y);
			break;
		}

		m_shield.pos.x -= static_cast<float>(m_shieldInitialToShieldBlockingVector.x * m_transitionDownSpeed * dt);
		m_shield.pos.y -= static_cast<float>(m_shieldInitialToShieldBlockingVector.y * m_transitionDownSpeed * dt);
		break;

	case PLAYER_BLOCKING_STATES::ON_COOLDOWN:
		break;
	}
}

void Player::_renderShield() {
	switch (m_blockingState) {
	case PLAYER_BLOCKING_STATES::NOT_BLOCKING:
	case PLAYER_BLOCKING_STATES::ON_ENTER:
	case PLAYER_BLOCKING_STATES::ON_UPDATE:
	case PLAYER_BLOCKING_STATES::ON_EXIT:
	case PLAYER_BLOCKING_STATES::ON_COOLDOWN:
		RenderHelper::getInstance()->texture("shield", m_shield.pos.x + camOffset.x, m_shield.pos.y + camOffset.y, m_shield.size.x, m_shield.size.y, 1, Color{ 0,0,0,0 }, Math::m_PI);
		break;
	}
}

int Player::getLevel() const {
	return m_playerLevel;
}

void Player::resetHealth() {
	m_health = m_maxHealth;
	m_attackedRenderX = m_health / 80;
	m_attackedRenderXprev = 0;
}

void Player::giveXpForEnemyKilled(int enemiesKilled) {
	auto& data = Database::getInstance().data;

	const int xpToGive = data["player"]["xpPerEnemyKilled"] * enemiesKilled * DIFFUCLTY_XP_MULTIPLIER.at(difficulty);
	cout << "Giving player " << xpToGive << "xp\n";
	data["player"]["currentXp"] = data["player"]["currentXp"] + xpToGive;

	// calculate xp needed for player to get to next level
	// formula:
	// level^2 + base
	//
	//	so it will get quadratically harder to level up
	const int xpNeeded = m_playerLevel * m_playerLevel + data["player"]["baseXpToLevelUp"];

	if (data["player"]["currentXp"] >= xpNeeded) {
		data["player"]["currentXp"] = data["player"]["currentXp"] - xpNeeded;
		m_playerLevel++;
		cout << "Player level up! Current level: " << m_playerLevel << "\n";

		// update damage
		m_dmg += m_dmg * ((m_playerLevel - 1) * m_LEVEL_DAMAGE_INC_PERCENTAGE);
	}
}
