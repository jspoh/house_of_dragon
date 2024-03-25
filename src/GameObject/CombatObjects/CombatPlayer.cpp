/* Start Header ************************************************************************/
/*!
\file CombatPlayer.cpp
\author Poh Jing Seng, jingseng.poh, 2301363
\par jingseng.poh\@digipen.edu
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
#include "RenderHelper.h"
#include "MyMath.h"
#include <iostream>


Player::Player(float health, float dmg, Element element) : Mob(element, health, dmg) {
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
}

Player::~Player() {
	RenderHelper::getInstance()->removeTextureByRef("shield");
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

	if (AEInputCheckCurr(AEVK_SPACE) && blockingState == PLAYER_BLOCKING_STATES::NOT_BLOCKING) {
		elapsedTimeMs = 0;
		blockingState = PLAYER_BLOCKING_STATES::ON_ENTER;
	}
	else if (!AEInputCheckCurr(AEVK_SPACE) && blockingState != PLAYER_BLOCKING_STATES::NOT_BLOCKING && blockingState != PLAYER_BLOCKING_STATES::ON_COOLDOWN) {
		elapsedTimeMs = 0;
		blockingState = PLAYER_BLOCKING_STATES::ON_EXIT;
	}

	switch (blockingState) {
	case PLAYER_BLOCKING_STATES::NOT_BLOCKING:
		//std::cout << "Player blocking state: NOT_BLOCKING\n";
		elapsedTimeMs = 0;
		break;
	case PLAYER_BLOCKING_STATES::ON_ENTER:
		//std::cout << "Player blocking state: ON_ENTER\n";
		if (elapsedTimeMs >= shieldUpTransitionTimeMs || AEVec2Distance(&shield.pos, &shieldBlockingPos) <= snapThreshold) {
			blockingState = PLAYER_BLOCKING_STATES::ON_UPDATE;
			elapsedTimeMs = 0;

			// force shield to go to final pos
			AEVec2Set(&shield.pos, shieldBlockingPos.x, shieldBlockingPos.y);
			break;
		}

		// translate the shield up
		shield.pos.x += static_cast<float>(shieldInitialToShieldBlocking_vector.x * transitionUpSpeed * dt);
		shield.pos.y += static_cast<float>(shieldInitialToShieldBlocking_vector.y * transitionUpSpeed * dt);
		break;
	case PLAYER_BLOCKING_STATES::ON_UPDATE:
		//std::cout << "Player blocking state: ON_UPDATE\n";
		if (elapsedTimeMs >= shieldUpTimeMs) {
			blockingState = PLAYER_BLOCKING_STATES::ON_EXIT;
			elapsedTimeMs = 0;
		}
		break;
	case PLAYER_BLOCKING_STATES::ON_EXIT:
		//std::cout << "Player blocking state: ON_EXIT\n";
		if (elapsedTimeMs >= shieldDownTransitionTimeMs || AEVec2Distance(&shield.pos, &shieldInitialPos) <= snapThreshold) {
			blockingState = PLAYER_BLOCKING_STATES::ON_COOLDOWN;
			elapsedTimeMs = 0;

			// force shield to go to initial pos
			AEVec2Set(&shield.pos, shieldInitialPos.x, shieldInitialPos.y);
			break;
		}

		shield.pos.x -= static_cast<float>(shieldInitialToShieldBlocking_vector.x * transitionDownSpeed * dt);
		shield.pos.y -= static_cast<float>(shieldInitialToShieldBlocking_vector.y * transitionDownSpeed * dt);
		break;
	case PLAYER_BLOCKING_STATES::ON_COOLDOWN:
		//std::cout << "Player blocking state: ON_COOLDOWN\n";
		if (elapsedTimeMs >= timeBeforeNextBlockMs) {
			elapsedTimeMs = 0;
			blockingState = PLAYER_BLOCKING_STATES::NOT_BLOCKING;
		}
		break;
	}

	//std::cout << "Shield pos: " << shield.pos.x << " | " << shield.pos.y << "\n";
	//std::cout << elapsedTimeMs << " / " << shieldTransitionTimeMs << "\n";
}

void Player::render() {
	AEGfxGetCamPosition(&camOffset.x, &camOffset.y);
	this->_drawHealth(150, 150);

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

float Player::attack(Mob& target, Element attackEl, float qtMultiplier) {
	DamageMultiplier dm = ElementProperties::getEffectiveDamage(attackEl, target.element);
	float multiplier = 1;
	std::cout << "attackEl enum: " << attackEl << "\n";
	std::cout << "targetEl enum: " << target.element << "\n";
	std::cout << "Damage multiplier enum: " << dm << "\n";
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
