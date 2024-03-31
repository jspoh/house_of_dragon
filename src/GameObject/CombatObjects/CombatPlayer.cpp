/* Start Header ************************************************************************/
/*!
\file CombatPlayer.cpp
\author Poh Jing Seng, jingseng.poh, 2301363
\author Soh Wei Jie, weijie.soh,
\par jingseng.poh\@digipen.edu
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


Player::Player(float _health, float _dmg, Element element) : Mob(element, _health, _dmg * DIFFICULTY_PLAYER_DAMAGE_MULTIPLIER.at(difficulty)) {
	RenderHelper::getInstance()->registerTexture("shield", "./Assets/Combat_UI/shield.png");
	//float StartHealth = health;		// what is this for?
	// set shield properties
	AEVec2Set(&shield.pos, -AEGfxGetWindowWidth() / 2.f, -AEGfxGetWindowHeight() / 2.f * 2.f);
	AEVec2Set(&shield.size, AEGfxGetWindowWidth() * 0.75f, (AEGfxGetWindowWidth() / 2.f) * 2.f);

	AEVec2Set(&shieldInitialPos, shield.pos.x, shield.pos.y);
	AEVec2Set(&shieldBlockingPos, 0, -AEGfxGetWindowHeight() * 0.8f);

	// set vector from shield inital to final position
	AEVec2Set(&shieldInitialToShieldBlocking_vector, shieldBlockingPos.x - shieldInitialPos.x, shieldBlockingPos.y - shieldInitialPos.y);
	AEVec2Normalize(&shieldInitialToShieldBlocking_vector, &shieldInitialToShieldBlocking_vector);

	// get transition speed
	float initialFinalDistance = AEVec2Distance(&shieldInitialPos, &shieldBlockingPos);
	transitionUpSpeed = initialFinalDistance / (shieldUpTransitionTimeMs / 1000.f);
	transitionDownSpeed = initialFinalDistance / (shieldDownTransitionTimeMs / 1000.f);

	initialAttack = this->dmg;

	cout << "Player initialized with " << health << " health and " << dmg << " damage\n";
}

Player::~Player() {
	//RenderHelper::getInstance()->removeTextureByRef("shield");	// let renderhelper manage
}



void Player::healthGain(float healthIncrease) {
	float newhealth = this->health + healthIncrease;
	if (newhealth < 100.f ) {
		this->health = newhealth;
	}
	else {
		this->health = 100.f;
	}

}
void Player::_drawHealth(float screenX, float screenY) {
	std::string name = "Jackie";
		std::string level = "  Lv:1";
	float paddingY = 50;
	float paddingX = 90;
	Point panelPos = stow(screenX, screenY);
	//panel rendering
	f32 truex, truey;
	AEGfxGetCamPosition(&truex, &truey);
	RenderHelper::getInstance()->texture("panel", panelPos.x + truex  , panelPos.y   + truey + paddingY, 270, 100);
	RenderHelper::getInstance()->text(name + level, screenX, screenY - 65);
	if (this->health > 66) {
		RenderHelper::getInstance()->texture("greenbar1", panelPos.x - paddingX + truex, panelPos.y + truey + paddingY - 20  , 10, 15); //start point, but coordinates is centralised so need to take account of the widthw
		RenderHelper::getInstance()->texture("greenbar3", panelPos.x + truex - paddingX + 5 + (health / 80) * 50, panelPos.y + truey + paddingY - 20, (health / 80) * 100, 15);
		RenderHelper::getInstance()->texture("greenbar2", panelPos.x + truex + (health / 80) * 100 - paddingX + 10, panelPos.y + truey + paddingY - 20, 10, 15);
	}
	else if (this->health > 33) {
		RenderHelper::getInstance()->texture("yellowbar1", panelPos.x - paddingX + truex, panelPos.y + truey + paddingY - 20, 10, 15); //start point, but coordinates is centralised so need to take account of the widthw
		RenderHelper::getInstance()->texture("yellowbar3", panelPos.x + truex - paddingX + 5 + (health / 80) * 50, panelPos.y + truey + paddingY - 20, (health / 80) * 100, 15);
		RenderHelper::getInstance()->texture("yellowbar2", panelPos.x + truex + (health / 80) * 100 - paddingX + 10, panelPos.y + truey + paddingY - 20, 10, 15);
	}
	else {
		RenderHelper::getInstance()->texture("redbar1", panelPos.x - paddingX + truex, panelPos.y + truey + paddingY - 20, 10, 15); //start point, but coordinates is centralised so need to take account of the widthw
		RenderHelper::getInstance()->texture("redbar3", panelPos.x + truex - paddingX + 5 + (health / 80) * 50, panelPos.y + truey + paddingY - 20, (health / 80) * 100, 15);
		RenderHelper::getInstance()->texture("redbar2", panelPos.x + truex + (health / 80) * 100 - paddingX + 10, panelPos.y + truey + paddingY - 20, 10, 15);

	}
}

void Player::update(double dt) {
	elapsedTimeMs += static_cast<int>(dt * 1000);

	updateHands(static_cast<float>(dt));

	//cout << mouseX << " | " << mouseY << "\n";
	//cout << camOffset.x << " | " << camOffset.y << "\n";

	/* blocking stuff */
	//HandStateAnimationType;


	if (
		AEInputCheckTriggered(AEVK_SPACE) 
		&& blockingState == PLAYER_BLOCKING_STATES::NOT_BLOCKING 
		&& CombatManager::getInstance().turn == CombatManager::TURN::ENEMY
		) {
		HandStateAnimationType = HandAnimationType::Block;
		elapsedTimeMs = 0;
		blockingState = PLAYER_BLOCKING_STATES::ON_ENTER;
	}
	//else if (!AEInputCheckCurr(AEVK_SPACE) && blockingState != PLAYER_BLOCKING_STATES::NOT_BLOCKING && blockingState != PLAYER_BLOCKING_STATES::ON_COOLDOWN) {
	//	elapsedTimeMs = 0;
	//	blockingState = PLAYER_BLOCKING_STATES::ON_EXIT;
	//}

	//cout << static_cast<int>(blockingState) << "\n";

	switch (blockingState) {
	case PLAYER_BLOCKING_STATES::NOT_BLOCKING:
		//cout << "Player blocking state: NOT_BLOCKING\n";
		if (HandStateAnimationType == HandAnimationType::Block) {
			HandStateAnimationType = HandAnimationType::None;
			blockingState = PLAYER_BLOCKING_STATES::ON_ENTER;
		}
		elapsedTimeMs = 0;
		break;
	case PLAYER_BLOCKING_STATES::ON_ENTER:
		//cout << "Player blocking state: ON_ENTER\n";
		if (elapsedTimeMs >= shieldUpTransitionTimeMs || AEVec2Distance(&shield.pos, &shieldBlockingPos) <= snapThreshold) {
			blockingState = PLAYER_BLOCKING_STATES::ON_UPDATE;
			elapsedTimeMs = 0;
		}
		break;
	case PLAYER_BLOCKING_STATES::ON_UPDATE:
		//cout << "Player blocking state: ON_UPDATE\n";
		if (elapsedTimeMs >= shieldUpTimeMs) {
			blockingState = PLAYER_BLOCKING_STATES::ON_EXIT;
			elapsedTimeMs = 0;
		}
		break;
	case PLAYER_BLOCKING_STATES::ON_EXIT:
		//cout << "Player blocking state: ON_EXIT\n";
		if (elapsedTimeMs >= shieldDownTransitionTimeMs || AEVec2Distance(&shield.pos, &shieldInitialPos) <= snapThreshold) {
			blockingState = PLAYER_BLOCKING_STATES::ON_COOLDOWN;
			elapsedTimeMs = 0;
		}
		break;
	case PLAYER_BLOCKING_STATES::ON_COOLDOWN:
		//cout << "Player blocking state: ON_COOLDOWN\n";
		if (elapsedTimeMs >= timeBeforeNextBlockMs) {
			elapsedTimeMs = 0;
			blockingState = PLAYER_BLOCKING_STATES::NOT_BLOCKING;
		}
		break;
	}

	//_updateShield(dt);
	_updateBlockingHands();
	//cout << "Shield pos: " << shield.pos.x << " | " << shield.pos.y << "\n";
	//cout << elapsedTimeMs << " / " << shieldTransitionTimeMs << "\n";
}

void Player::render() {
	this->_drawHealth(150, 150);

	//_renderShield();
	_renderHands();
}

float Player::attack(Mob& target, Element attackEl, float qtMultiplier) {

	if (this->attackMultiplerTurn > 0) {
		if (attackMultiplerTurnStart >= this->attackMultiplerTurn) {
			//item finished its usage
			attackMultiplerTurn = 0;
			attackMultiplerTurnStart = 0;
			this->dmg = initialAttack;
		}
		else {
			if (attackMultiplerTurnStart == 0) {
				//increase by 25 percent
				this->dmg = initialAttack * 105.25f;
			}
			attackMultiplerTurnStart++;
		}
	}


	DamageMultiplier dm = ElementProperties::getEffectiveDamage(attackEl, target.element);
	float multiplier = 1;
	cout << "attackEl enum: " << attackEl << "\n";
	cout << "targetEl enum: " << target.element << "\n";
	cout << "Damage multiplier enum: " << dm << "\n";
	switch (dm) {
	case Weak:
		multiplier = 0.5;
		break;
	case Strong:
		multiplier = 2;
		break;
	}

	float damage = this->dmg * multiplier * qtMultiplier;
	target.health -= damage;
	return damage;
}


void Player::attackMultipler(int turn) {
	this->attackMultiplerTurn = turn;
}


void Player::setHandStateAnimationType(HandAnimationType t) {
	HandStateAnimationType = t;
}

void Player::updateHands(float t_dt)
{
	LerpSpeed = 10.0;
	AEGfxGetCamPosition(&camX, &camY);
	camX += AEGfxGetWindowWidth() / 2;
	camY -= AEGfxGetWindowHeight() / 2;
	int mX{}, mY{};
	switch (HandStateAnimationType)
	{
	case HandAnimationType::Punch:
		if (!LeftSide) //Left Hand Punch
			switch (t_AnimationFrame)
			{
			case 0://Init
				AEMtx33Identity(&Hand2PosData.first);
				AEMtx33ScaleApply(&Hand2PosData.first, &Hand2PosData.first, 191.5, 307);
				targetPos = { -804.25f + camX, -526.f + camY };
				AEMtx33TransApply(&Hand2PosData.first, &Hand2PosData.first, targetPos.x, targetPos.y);
				mX = mouseX;
				mY = mouseY;
				mX -= AEGfxGetWindowWidth() / 2;
				mY -= AEGfxGetWindowHeight() / 2;
				mY *= -1;
				if (t_AnimationDuration > 999) t_AnimationDuration = 0.0;
				break;
			case 1: //End Point
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
				HandStateAnimationType = HandAnimationType::None;
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

	if (blockingState != PLAYER_BLOCKING_STATES::NOT_BLOCKING && prevState == PLAYER_BLOCKING_STATES::NOT_BLOCKING) {
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

	//cout << static_cast<int>(blockingState) << "\n";
	//LeftSide = false;

	if (!LeftSide) //Right Hand Blocking
		switch (blockingState)
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
			LeftSide = rand() % 2 - 1;		// huh why -1 here
			Hand1PosData.first = Hand1PosData.second = {};
			Hand2PosData.first = Hand2PosData.second = {};
			Hand3PosData.first = Hand3PosData.second = {};
			Hand4PosData.first = Hand4PosData.second = {};
			break;
		default:
			cout << "ERROR IN BLOCKING ANIMATION\n";
		}
	else //Left Hand Blocking
		switch (blockingState)
		{
		case PLAYER_BLOCKING_STATES::NOT_BLOCKING:
			//Init
			AEMtx33Identity(&Hand3PosData.first);
			Hand1PosData.second = Hand3PosData.first;
			AEMtx33ScaleApply(&Hand3PosData.first, &Hand3PosData.first, 238, 333);
			targetPos = { 160.95f + camOffset.x, -499.5f + camOffset.y };
			AEMtx33TransApply(&Hand3PosData.first, &Hand3PosData.first, targetPos.x, targetPos.y);
			AEMtx33ScaleApply(&Hand1PosData.second, &Hand1PosData.second, 200, 318);
			targetPos = { 304.25f + camOffset.x, -526.f + camOffset.y };
			AEMtx33TransApply(&Hand1PosData.second, &Hand1PosData.second, targetPos.x, targetPos.y);
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
			LeftSide = rand() % 2 - 1;		// huh why -1 here
			Hand1PosData.first = Hand1PosData.second = {};
			Hand2PosData.first = Hand2PosData.second = {};
			Hand3PosData.first = Hand3PosData.second = {};
			Hand4PosData.first = Hand4PosData.second = {};
			break;
		default:
			cout << "ERROR IN BLOCKING ANIMATION\n";
		}

	prevState = blockingState;
}

void Player::_renderHands()
{
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetTransparency(1.0f);
	AEGfxSetTransform(Hand4PosData.first.m);
	AEGfxTextureSet(RenderHelper::getInstance()->getTextureByRef("Player_Fist_Left_04"), 0, 0);
	AEGfxMeshDraw(RenderHelper::getInstance()->GetdefaultMesh(), AE_GFX_MDM_TRIANGLES);
	AEGfxSetTransform(Hand4PosData.second.m);
	AEGfxTextureSet(RenderHelper::getInstance()->getTextureByRef("Player_Fist_Right_04"), 0, 0);
	AEGfxMeshDraw(RenderHelper::getInstance()->GetdefaultMesh(), AE_GFX_MDM_TRIANGLES);

	AEGfxSetTransform(Hand1PosData.first.m);
	AEGfxTextureSet(RenderHelper::getInstance()->getTextureByRef("Player_Fist_Left_01"), 0, 0);
	AEGfxMeshDraw(RenderHelper::getInstance()->GetdefaultMesh(), AE_GFX_MDM_TRIANGLES);
	AEGfxSetTransform(Hand1PosData.second.m);
	AEGfxTextureSet(RenderHelper::getInstance()->getTextureByRef("Player_Fist_Right_01"), 0, 0);
	AEGfxMeshDraw(RenderHelper::getInstance()->GetdefaultMesh(), AE_GFX_MDM_TRIANGLES);
	AEGfxSetTransform(Hand2PosData.first.m);
	AEGfxTextureSet(RenderHelper::getInstance()->getTextureByRef("Player_Fist_Left_02"), 0, 0);
	AEGfxMeshDraw(RenderHelper::getInstance()->GetdefaultMesh(), AE_GFX_MDM_TRIANGLES);
	AEGfxSetTransform(Hand2PosData.second.m);
	AEGfxTextureSet(RenderHelper::getInstance()->getTextureByRef("Player_Fist_Right_02"), 0, 0);
	AEGfxMeshDraw(RenderHelper::getInstance()->GetdefaultMesh(), AE_GFX_MDM_TRIANGLES);
	AEGfxSetTransform(Hand3PosData.first.m);
	AEGfxTextureSet(RenderHelper::getInstance()->getTextureByRef("Player_Fist_Left_03"), 0, 0);
	AEGfxMeshDraw(RenderHelper::getInstance()->GetdefaultMesh(), AE_GFX_MDM_TRIANGLES);
	AEGfxSetTransform(Hand3PosData.second.m);
	AEGfxTextureSet(RenderHelper::getInstance()->getTextureByRef("Player_Fist_Right_03"), 0, 0);
	AEGfxMeshDraw(RenderHelper::getInstance()->GetdefaultMesh(), AE_GFX_MDM_TRIANGLES);

}

void Player::_updateShield(double dt) {
	switch (blockingState) {
	case PLAYER_BLOCKING_STATES::NOT_BLOCKING:
		break;

	case PLAYER_BLOCKING_STATES::ON_ENTER:
		//cout << "Player blocking state: ON_ENTER\n";
		if (elapsedTimeMs >= shieldUpTransitionTimeMs || AEVec2Distance(&shield.pos, &shieldBlockingPos) <= snapThreshold) {
			// force shield to go to final pos
			AEVec2Set(&shield.pos, shieldBlockingPos.x, shieldBlockingPos.y);
			break;
		}

		// translate the shield up
		shield.pos.x += static_cast<float>(shieldInitialToShieldBlocking_vector.x * transitionUpSpeed * dt);
		shield.pos.y += static_cast<float>(shieldInitialToShieldBlocking_vector.y * transitionUpSpeed * dt);
		break;

	case PLAYER_BLOCKING_STATES::ON_UPDATE:
		break;

	case PLAYER_BLOCKING_STATES::ON_EXIT:
		//cout << "Player blocking state: ON_EXIT\n";
		if (elapsedTimeMs >= shieldDownTransitionTimeMs || AEVec2Distance(&shield.pos, &shieldInitialPos) <= snapThreshold) {
			// force shield to go to initial pos
			AEVec2Set(&shield.pos, shieldInitialPos.x, shieldInitialPos.y);
			break;
		}

		shield.pos.x -= static_cast<float>(shieldInitialToShieldBlocking_vector.x * transitionDownSpeed * dt);
		shield.pos.y -= static_cast<float>(shieldInitialToShieldBlocking_vector.y * transitionDownSpeed * dt);
		break;

	case PLAYER_BLOCKING_STATES::ON_COOLDOWN:
		break;
	}
}

void Player::_renderShield() {
	switch (blockingState) {
	case PLAYER_BLOCKING_STATES::NOT_BLOCKING:
	case PLAYER_BLOCKING_STATES::ON_ENTER:
	case PLAYER_BLOCKING_STATES::ON_UPDATE:
	case PLAYER_BLOCKING_STATES::ON_EXIT:
	case PLAYER_BLOCKING_STATES::ON_COOLDOWN:
		RenderHelper::getInstance()->texture("shield", shield.pos.x + camOffset.x, shield.pos.y + camOffset.y, shield.size.x, shield.size.y, 1, Color{ 0,0,0,0 }, Math::m_PI);
		break;
	}
}

