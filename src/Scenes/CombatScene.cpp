/* Start Header ************************************************************************/
/*!
\file CombatScene.cpp
\author Poh Jing Seng, jingseng.poh, 2301363
\par jingseng.poh\@digipen.edu
\date 28 feb 2024
\brief renders and manages the combat scene
/*
Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/


#include "Pch.h"
#include "CombatScene.h"

//#include "../../Backend/GameManager.h" //? Still thinking
#include "SceneManager.h"
#include "Event.h"
#include "CombatManager.h"
#include "CombatPlayer.h"

#include <vector>
#include <string>
#include <unordered_map>


//additional

CombatScene* CombatScene::sInstance = new CombatScene(SceneManager::GetInstance());

namespace {
	// game objects
	Player* player;
	bool playerAlive;
	bool extraflagtest;
	//camera coordinates;
	f32 camX, camY;

	//timer for the lerp
	//const float slideAnimationDuration = 1.0f;
	// panel rendering
	Point panelpos;
	float panelfinalY;
	bool panelflag;
	float currentTime;
	float totaltime;
	float startingPanelY;
	//world pos
	Point wpos;

	//size for the dead player screen
	Point initalScaleDead;
	Point FinalScaleDead;
	Point currScaleDead;
	
	//padding for the btns
	float btnWordPadding;
	float btnDecreaseY;
	float btnIncreaseY;
	float btnDecreaseStart = -170.f;
	float btnFinalY = 170.f;


	enemiesGroup groups;

	EVENT_RESULTS combatEventResult = EVENT_RESULTS::NONE_EVENT_RESULTS;
	enum ACTION_BTNS {
		MAIN,
		ATTACK,
		ITEMS,
		CONFIRMATION,
		SELECT
	};
	ACTION_BTNS currentState = MAIN;
	std::unordered_map<std::string, Element> elementMap = {
		{"NO_ELEMENT", Element::NO_ELEMENT},
		{"water",  Element::Water},
		{"earth",  Element::Earth},
		{"metal",  Element::Metal},
		{"fire",  Element::Fire},
		{"wood",  Element::Wood}
	};


	std::unordered_map<std::string, ACTION_BTNS> stateMap = {
		{"MAIN", MAIN},
		{"ATTACK", ATTACK},
		{"ITEMS", ITEMS},
		{"FLEE", CONFIRMATION},
	};

	std::vector<std::vector<std::string>> btns = {
		{"ATTACK", "ITEMS", "FLEE"},  // main buttons. the rest are submenu
		{"FIRE", "WATER", "METAL", "WOOD", "EARTH", "BACK"},  // attack elements
		{"BACON", "BEEF", "CHICKEN", "CAT(jk pls)", "BACK"},  // items
		{"YES", "NO"},  // confirmation. only used for flee option
	};
	float padding = 50.f;
	float spacing = 50.f;


	float btnY = 550.f;
	float maxBtnHeight = 100.f;

	/*im so sorry this code very spaghet but time crunch!!*/
	void updateBtns(std::vector<std::string> bvalues) {
		// rendering coordinates 
		float btnWidth = static_cast<float>((AEGfxGetWindowWidth() - (padding * 2) - (bvalues.size() - 1) * spacing) / bvalues.size());
		float btnHeight = btnWidth / 3.f;
		btnHeight = btnHeight > maxBtnHeight ? maxBtnHeight : btnHeight;
		float lBtnX = padding + btnWidth / 2.f;

		float bPosX = lBtnX;
		if (playerAlive) {
			for (const std::string bv : bvalues) { // bruh wa this got got me too confused
				Point btnPos = stow(bPosX, btnY);  // button rendering position

				int mouseX, mouseY;
				AEInputGetCursorPosition(&mouseX, &mouseY);
				//std::cout << bPosX << " | " << btnY << "\n";
				if (CollisionChecker::isMouseInRect(bPosX, btnY, btnWidth, btnHeight, static_cast<float>(mouseX), static_cast<float>(mouseY))) {
					//std::cout << "mouse in rect" << bv << "\n";
					// clicked wt
					if (AEInputCheckTriggered(AEVK_LBUTTON)) {
						/*click while on main menu*/
						if (currentState == ACTION_BTNS::MAIN) {
							currentState = stateMap.find(bv)->second;
						}
						else if (bv == "BACK" || bv == "NO") {
							currentState = ACTION_BTNS::MAIN;
						}
						else if (currentState == ACTION_BTNS::ATTACK) {
							if (bv == "FIRE") {
								std::cout << "fire pressed\n";
								CombatManager::getInstance()->attackElement = Fire;
							}
							else if (bv == "WATER") {
								std::cout << "water pressed\n";
								CombatManager::getInstance()->attackElement = Water;
							}
							else if (bv == "METAL") {
								std::cout << "metal pressed\n";
								CombatManager::getInstance()->attackElement = Metal;
							}
							else if (bv == "WOOD") {
								std::cout << "wood pressed\n";
								CombatManager::getInstance()->attackElement = Wood;
							}
							else if (bv == "EARTH") {
								std::cout << "earth pressed\n";
								CombatManager::getInstance()->attackElement = Earth;
							}

							if (CombatManager::getInstance()->selectedEnemy != nullptr && CombatManager::getInstance()->attackElement != Element::NO_ELEMENT) {
								/*if user presses attack*/
								CombatManager::getInstance()->isPlayingEvent = true;

								Event::getInstance()->startRandomEvent();
							}
							else if (CombatManager::getInstance()->selectedEnemy == nullptr && CombatManager::getInstance()->attackElement != Element::NO_ELEMENT) {
								//RenderHelper::getInstance()->text("No enemy selected", AEGfxGetWindowWidth() / 2.f, AEGfxGetWindowHeight() / 2.f);
								std::cout << "No enemy selected\n";
							}
						}
						else if (bv == "YES") {
							std::cout << "Fleeing fight\n";
						}
					}

				}
				bPosX += btnWidth + spacing;
			}
		}
	}


	void renderBtns(std::vector<std::string> bvalues) {
		f32 truex, truey;
		AEGfxGetCamPosition(&truex, &truey);
		


		if (currentState == ACTION_BTNS::ATTACK && CombatManager::getInstance()->attackElement != Element::NO_ELEMENT && CombatManager::getInstance()->selectedEnemy == nullptr) {
			RenderHelper::getInstance()->text("No enemy selected", AEGfxGetWindowWidth() / 2.f, AEGfxGetWindowHeight() / 2.f);
			return;
		}

		// rendering coordinates 
		float btnWidth = static_cast<float>((AEGfxGetWindowWidth() - (padding * 2) - (bvalues.size() - 1) * spacing) / bvalues.size());
		float btnHeight = btnWidth / 3.f;
		btnHeight = btnHeight > maxBtnHeight ? maxBtnHeight : btnHeight;
		float lBtnX = padding + btnWidth / 2.f;
		 
		float bPosX = lBtnX;

		for (const std::string bv : bvalues) { // bruh wa this got got me too confused
			Point btnPos = stow(bPosX, btnY);  // button rendering position

			int mX, mY;
			AEInputGetCursorPosition(&mX, &mY);
			if (CollisionChecker::isMouseInRect(bPosX, btnY, btnWidth, btnHeight, static_cast<float>(mX), static_cast<float>(mY)) && playerAlive) {
				RenderHelper::getInstance()->texture("button", btnPos.x + truex, btnPos.y + truey, btnWidth, btnHeight + btnWordPadding * 2);
				//RenderHelper::getInstance()->rect(btnPos.x + truex, btnPos.y + truey, btnWidth, btnHeight, 0, Color{ 0.9f, 0.5f, 0.5f, 1.f });  // render highlight on hover. can consider doing transitions if got time?? but prob no time lel
			}
			else {
				RenderHelper::getInstance()->texture("button", btnPos.x + truex, btnPos.y + truey - btnDecreaseY + btnIncreaseY, btnWidth, btnHeight + btnWordPadding );

				//RenderHelper::getInstance()->rect(btnPos.x + truex, btnPos.y + truey, btnWidth, btnHeight, 0, Color{ 0.3f, 0.3f, 0.3f, 1.f });  // render normal when no hovering
			}

			RenderHelper::getInstance()->text(bv, bPosX, btnY + btnDecreaseY - btnIncreaseY);
			bPosX += btnWidth + spacing;
		}

	}

}
void CombatScene::spawnEnemies(std::vector<std::string> enemyRefs) {
	// this function works by creating taking in the vector of enemies; but this means i dont have to 

	float Enemypadding = 50.0f;
	float texSize = 50.f;
	int sz = static_cast<int>(enemyRefs.size()); // number of enemies;
	groups.coordinates.resize(sz); // setting the coordinates
	groups.enemies.resize(sz); // setting up the checking of enemies
	groups.activeEnemy.resize(sz);
	groups.names.resize(sz); // might not be needed, after getting the information from the
	float Enemyspacing = static_cast<float>((AEGfxGetWindowWidth() - (Enemypadding * 2) - (sz - 1) * spacing) / sz);
	for (int i = 0; i < sz; i++) {
		groups.activeEnemy[i] = true;
		// coordindates
		groups.coordinates[i].x = Enemypadding + i * Enemyspacing;
		groups.coordinates[i].y = AEGfxGetWindowHeight() / 2.f - 100.f;
		// obtaining the infomation from json file
		groups.enemies[i] = new Enemy(
			elementMap.find(Database::getInstance()->data["enemyAttributes"][enemyRefs[i]]["element"])->second,
			Database::getInstance()->data["enemyAttributes"][enemyRefs[i]]["health"],
			Database::getInstance()->data["enemyAttributes"][enemyRefs[i]]["damage"],
			Database::getInstance()->data["enemyAttributes"][enemyRefs[i]]["texturePath"],
			enemyRefs[i],
			groups.coordinates[i].x,
			groups.coordinates[i].y,
			texSize
		);
		// error with json file input
	}



}

CombatScene::CombatScene()
{
}

CombatScene::CombatScene(SceneManager* _sceneMgr)
{
	_sceneMgr->AddScene("CombatScene", this);
}

CombatScene::~CombatScene()
{
}

void CombatScene::Load()
{
 	Event::getInstance();
	//healthbar load
	RenderHelper::getInstance()->registerTexture("border", "./Assets/Combat_Items/Combat/border.png");
	RenderHelper::getInstance()->registerTexture("panel", "./Assets/Combat_Items/Combat/panel.png");
	RenderHelper::getInstance()->registerTexture("greenbar1", "./Assets/Health/green/start.png");
	RenderHelper::getInstance()->registerTexture("greenbar2", "./Assets/Health/green/end.png");
	RenderHelper::getInstance()->registerTexture("greenbar3", "./Assets/Health/green/bar.png");
	RenderHelper::getInstance()->registerTexture("redbar1", "./Assets/Health/red/start.png");
	RenderHelper::getInstance()->registerTexture("redbar2", "./Assets/Health/red/end.png");
	RenderHelper::getInstance()->registerTexture("redbar3", "./Assets/Health/red/bar.png");
	RenderHelper::getInstance()->registerTexture("yellowbar1", "./Assets/Health/yellow/start.png");
	RenderHelper::getInstance()->registerTexture("yellowbar2", "./Assets/Health/yellow/end.png");
	RenderHelper::getInstance()->registerTexture("yellowbar3", "./Assets/Health/yellow/bar.png");
	RenderHelper::getInstance()->registerTexture("bar1", "./Assets/Health/start.png");
	RenderHelper::getInstance()->registerTexture("bar2", "./Assets/Health/end.png");
	RenderHelper::getInstance()->registerTexture("bar3", "./Assets/Health/bar.png");
	RenderHelper::getInstance()->registerTexture("cat", "./Assets/Combat_Enemy/cat.jpg");
	RenderHelper::getInstance()->registerTexture("horse", "./Assets/Combat_Enemy/horse.jpg");
	RenderHelper::getInstance()->registerTexture("dragon", "./Assets/Combat_Enemy/dragon.jpg");
	RenderHelper::getInstance()->registerTexture("playerdead", "./Assets/Combat_UI/playerdeadscreen.png");
	RenderHelper::getInstance()->registerTexture("button", "./Assets/Combat_UI/Button.png");




	std::cout << "CombatScene loaded\n";
}


void CombatScene::Init()
{
	/*Event::getInstance()->setActiveEvent(EVENT_TYPES::SPAM_KEY);*/  // for testing only
	//player init
	 wpos = stow(AEGfxGetWindowWidth() / 2, AEGfxGetWindowHeight() / 2);
	player = new Player(100, 1000);
	playerAlive = true;
	extraflagtest = true;
	panelflag = true;
	panelpos.x = 0; // constant value
	panelpos.y = -AEGfxGetWindowHeight() / 1.7f; // starting posY for the panel
	startingPanelY = panelpos.y;
	panelfinalY = -AEGfxGetWindowHeight() / 2.9f;
	currentTime = 0;
	totaltime = 1.f;
	initalScaleDead.x = 250;
	FinalScaleDead.x = 500;
	initalScaleDead.y = 50;
	FinalScaleDead.y = 100;

	currScaleDead.x = initalScaleDead.x;
	currScaleDead.y = initalScaleDead.y;

	btnWordPadding = 20.f;
	btnDecreaseY = 0.f;

	CombatManager::getInstance()->start();
}

void CombatScene::Update(double dt)
{
	if (!CombatManager::getInstance()->isInCombat) {
		return;
	}


		for (Enemy* enemy : groups.enemies) { // check for dead/alive
		if (enemy->isDead()) {
			RenderHelper::getInstance()->text("Enemy is dead", AEGfxGetWindowWidth() / 2.f, AEGfxGetWindowHeight() / 2.f); // need to adapt to pointer to the pos
		}
		if (player->isDead()) {
			// Set player to dead
			//if (playerAlive) {
			//	playerAlive = false;
			//}
			//else {
			//	// Reset the panel position to slide back down
			//	panelflag = true;
			//	currentTime = 0.0f; // Reset the time for sliding animation
			//}
		}
		}

		if (!playerAlive) {
			if (extraflagtest == true) {
				extraflagtest = false;
				panelflag = true;
				currentTime = 0.0f; // Reset the time for sliding animation
			}
		}

	//if (AEInputCheckTriggered(AEVK_K)) {
	//	// kill all enemies
	//	for (const Enemy* e : groups.enemies) {
	//		delete e;
	//	}

	//	groups.enemies.clear();
	//	
	//	CombatManager::getInstance()->end();
	//	delete player;
	//	player = nullptr;
	//	return;
	//}
		if (AEInputCheckTriggered(AEVK_G)) {
			// kill player
			playerAlive = false;
		}
	//updating panel 
		const float slideAnimationDuration = 1.f;

	AEGfxGetCamPosition(&camX, &camY);
	if (!playerAlive && currentTime < slideAnimationDuration) {
		currentTime += static_cast<float>(AEFrameRateControllerGetFrameTime());
		float percenttime = static_cast<float>(currentTime / slideAnimationDuration);
		float t = percenttime;
		if (t > slideAnimationDuration) {
			t = slideAnimationDuration;
		}
		currScaleDead.x = lerp(initalScaleDead.x, FinalScaleDead.x, t);
		currScaleDead.y = lerp(initalScaleDead.y, FinalScaleDead.y, t);
		btnDecreaseY = lerp(0, btnFinalY, t);

		panelpos.y = lerp(panelfinalY, startingPanelY, t); // Reverse direction for sliding down
	}
	else {
		panelflag = false; // Reset panel flag when animation is complete
	}
	if (playerAlive && currentTime < totaltime) { // should include this in render.cpp instead
		currentTime += static_cast<float>(AEFrameRateControllerGetFrameTime());
		float percenttime = static_cast<float>(currentTime / totaltime);
		float t = percenttime;
		if (t > totaltime) {
			t = totaltime;
		}
		panelpos.y = lerp(-AEGfxGetWindowHeight() / 1.7f, panelfinalY, t);
		btnIncreaseY = lerp(btnDecreaseStart, 0, t);
	}
	else {
		panelflag = false;
	}

	player->update(dt);

	//if (AEInputCheckTriggered(AEVK_RBUTTON)) {
	//	selectflag = true;
	//	SelectEnemy = groups.enemies[0];
	//}

	// select enemy
	for (Enemy* e : groups.enemies) {
		e->update(dt);

		if (e->isSelected) {
			// deselect all other enemies
			CombatManager::getInstance()->selectedEnemy = e;

			for (Enemy* e2 : groups.enemies) {
				if (e != e2) {
					e2->isSelected = false;
				}
			}
		}
		else {
			if (CombatManager::getInstance()->selectedEnemy == e) {
				// enemy deselected
				CombatManager::getInstance()->selectedEnemy = nullptr;
			}
		}
	}


	Point p = stow(100, 100);
	Event::getInstance()->update(CombatManager::getInstance()->qtEventResult, dt);



	// if player has finished quicktime event
	if (CombatManager::getInstance()->qtEventResult != NONE_EVENT_RESULTS) {
		// end player's turn
		CombatManager::getInstance()->next();
		std::cout << "Enemy next turn in " << CombatManager::getInstance()->enemyNextTurnMs << "ms\n";
		CombatManager::getInstance()->isPlayingEvent = false;

		/*check if success or failure and modify damage accordingly*/
		switch (CombatManager::getInstance()->qtEventResult) {
		case EVENT_RESULTS::SUCCESS:
			std::cout << "Event success. multiplier granted: " << Event::getInstance()->maxMultiplier << "\n";
				player->attack(*CombatManager::getInstance()->selectedEnemy, CombatManager::getInstance()->attackElement, Event::getInstance()->maxMultiplier);

			break;
		case EVENT_RESULTS::FAILURE:
			std::cout << "Event failure. multiplier granted: " << Event::getInstance()->minMultiplier << "\n";
				player->attack(*CombatManager::getInstance()->selectedEnemy, CombatManager::getInstance()->attackElement, Event::getInstance()->minMultiplier);

			break;
		case EVENT_RESULTS::CUSTOM_MULTIPLIER:
			std::cout << "Event custom multiplier granted: " << Event::getInstance()->eventMultiplier << "\n";
				player->attack(*CombatManager::getInstance()->selectedEnemy, CombatManager::getInstance()->attackElement, Event::getInstance()->eventMultiplier);

			break;
		}
		CombatManager::getInstance()->qtEventResult = EVENT_RESULTS::NONE_EVENT_RESULTS;

		// reset states
		CombatManager::getInstance()->selectedEnemy = nullptr;
		for (Enemy* e : groups.enemies) {
			e->isSelected = false;
		}
		CombatManager::getInstance()->attackElement = Element::NO_ELEMENT;
		currentState = ACTION_BTNS::MAIN;
	}

	// when is player turn and player is not playing a quicktime event
	if (CombatManager::getInstance()->turn == TURN::PLAYER && !CombatManager::getInstance()->isPlayingEvent && panelflag == false) {
		updateBtns(btns[currentState]);  // render player action buttons
	}
	else if (CombatManager::getInstance()->turn == TURN::ENEMY) {
		CombatManager::getInstance()->enemyNextTurnMs -= static_cast<int>(dt * 1000);

		if (CombatManager::getInstance()->enemyNextTurnMs <= 0) {
			float multiplier = 1.f;
			switch (player->blockingState) {
			case PLAYER_BLOCKING_STATES::NOT_BLOCKING:
			case PLAYER_BLOCKING_STATES::ON_COOLDOWN:
				multiplier = 1.f;
				std::cout << "Attack not blocked by player at all, receiving " << multiplier << " damage multiplier against player\n";
				break;
			case PLAYER_BLOCKING_STATES::ON_ENTER:
			case PLAYER_BLOCKING_STATES::ON_EXIT:
				multiplier = 0.5f;
				std::cout << "Attack not fully blocked by player, receiving " << multiplier << " damage multiplier against player\n";
				break;
			case PLAYER_BLOCKING_STATES::ON_UPDATE:
				multiplier = 0.3f;
				std::cout << "Attack blocked by player, receiving " << multiplier << " damage multiplier against player\n";
				break;
			}

			// enemy to attack player if there are still enemies left
			if (groups.enemies.size()) {
				int randEnemyIndex = rand() % groups.enemies.size();
				std::cout << "Enemy with index " << randEnemyIndex << " is attacking player\n";
				groups.enemies[randEnemyIndex]->attack(*player, multiplier);  // Example: All enemies attack the player
				CombatManager::getInstance()->next();
			}
			else {
				std::cout << "Transition to next level\n";
				// all enemies shldve been deleted
				delete player;
				player = nullptr;
				CombatManager::getInstance()->end();
				return;
			}
		}

	}

}

void CombatScene::Render()
{
	// dont render if no longer in combat
	if (!CombatManager::getInstance()->isInCombat) {
		return;
	}
	//if (!playerAlive) {
	//	RenderHelper::getInstance()->text("you have died", AEGfxGetWindowWidth() / 2.f, AEGfxGetWindowHeight() / 2.f); // need to adapt to pointer to the pos

	//}

	//panel rendering
	f32 truex, truey;
	AEGfxGetCamPosition(&truex, &truey);

	//rendering enemies


	std::vector<int> deadEnemies;
	int i{};
	// rendering whether enemies is dead
	if (playerAlive) {
		player->render();
		RenderHelper::getInstance()->texture("panel", panelpos.x + truex, panelpos.y + truey, static_cast<float>(AEGfxGetWindowWidth()), 160.f);

		for (Enemy* enemy : groups.enemies) { // check for dead/alive
			if (enemy->isDead()) {
				RenderHelper::getInstance()->text("Enemy is dead", AEGfxGetWindowWidth() / 2.f, AEGfxGetWindowHeight() / 2.f); // need to adapt to pointer to the pos
				deadEnemies.push_back(i);
			}
			//else if (player->isDead()) {
			//	RenderHelper::getInstance()->text("Player is dead", AEGfxGetWindowWidth() / 2.f, AEGfxGetWindowHeight() / 2.f); //set pos
			//}

			if (CombatManager::getInstance()->turn == TURN::PLAYER && !CombatManager::getInstance()->isPlayingEvent && panelflag == false) {
				renderBtns(btns[currentState]);  // render player action buttons
			}
			else if (CombatManager::getInstance()->turn == TURN::ENEMY) {
				RenderHelper::getInstance()->text("Time your block with [SPACE]!", AEGfxGetWindowWidth() / 2.f, AEGfxGetWindowHeight() * 0.85f);
			}
			i++;
		}
	}


	else if (!playerAlive){
		//rendering out the objects
		RenderHelper::getInstance()->texture("panel", panelpos.x + truex, panelpos.y + truey, static_cast<float>(AEGfxGetWindowWidth()), 160.f);
		RenderHelper::getInstance()->texture("playerdead", wpos.x + truex, wpos.y+  truey, currScaleDead.x, currScaleDead.y); //start point, but coordinates is centralised so need to take account of the widthw
		renderBtns(btns[currentState]);

	}

	// remove all dead enemies
std::sort(deadEnemies.rbegin(), deadEnemies.rend());		// sort in reverse order. else removing multiple might cause indexoutofrange
for (const int index : deadEnemies) {
	// !TODO: add death animation (perhaps smoke particles to signify death)

		delete groups.enemies[index];
		groups.enemies.erase(groups.enemies.begin() + index);
	}
	Event::getInstance()->render();
	for (i = 0; i < groups.enemies.size(); i++) {
		//if()

		groups.enemies[i]->render(); // render all, draw all enemys
	}



	// remove all dead enemies

}

void CombatScene::Exit()
{
	std::cout << "Exiting CombatScene\n";
	for (Enemy* enemy : groups.enemies) {
		delete enemy;
	}
	groups.enemies.clear();
	delete CombatManager::getInstance();
	// Clear the vector after deleting the enemies
	delete player;
}


