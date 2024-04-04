/* Start Header ************************************************************************/
/*!
\file CombatScene.cpp
\author Poh Jing Seng, jingseng.poh, 2301363
\par jingseng.poh\@digipen.edu
\author Kuek wei jie, weijie.kuek, 2301325
\par weijie.kuek\@digipen.edu
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

#include "SceneStages.h"


extern std::unique_ptr<Player> player;
//Player* player = nullptr;

namespace {
	bool itemUsedSinceLastAttack = false;

	// game objects
	bool playerAlive;
	bool extraflagtest;
	bool deadfinalflag;
	bool winFlag;
	const float slideAnimationDuration = 1.f;
	float dialogueMaxTime = 0.8f;
	float dialougeTime;
	float winTime;
	float itemTime;
	bool winButtonFlag;
	float itemPanelY;
	AEVec2 ItemPanel;


	//camera coordinates;
	f32 camX, camY;


	// item drop
	std::vector<std::string> itemDrop;
	std::map<std::string, int> itemCounts;


	//death buttons values
	AEVec2 deathBtnMenuPoint;
	AEVec2 deathBtnRespawnPoint;
	float deathBtnWidthEnd;
	float deathbtnHeightEnd;
	AEVec2 deathBtncurrScale;

	//blocking variables
	float blockingRenderTime;
	bool blockNow;
	float enemyattackedRenderTime;



	//timer for the lerp
	//const float slideAnimationDuration = 1.0f;
	// panel rendering
	AEVec2 panelpos;
	float panelfinalY;
	bool panelflag;
	float currentTime;
	float startingPanelY;
	//world pos
	AEVec2 wpos;

	//size for the dead player screen
	AEVec2 initalScaleDead;
	AEVec2 FinalScaleDead;
	AEVec2 currScaleDead;

	//padding for the btns
	float btnWordPadding;
	float btnDecreaseY;
	float btnIncreaseY;
	float btnDecreaseStart = -170.f;
	float btnFinalY = 170.f;


	enemiesGroup groups;


	std::string itemUsed;
	std::string attackUsed;
	enum DIALOGUE {
		BLOCKING,
		PLAYER_ATTACK,
		PLAYER_DEATH,
		WIN,
		ENEMYDEATH,
		ITEM,
		ENEMY_ATTACK,
		EVENT,

		NONE
	};
	DIALOGUE dialogueState;

	EVENT_TYPES combatEventResult = EVENT_TYPES::NONE_EVENT_TYPE;
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
		{"BACON", "BEEF", "CHICKEN", "CAT", "BACK"},  // items
		{"YES", "NO"},  // confirmation. only used for flee option
	};
	float padding = 50.f;
	float spacing = 50.f;


	float btnY = 550;
	float maxBtnHeight = 100.f;

	std::vector<GameObject_Projectiles*> projectiles;

	void resetDialogue() {
		dialougeTime = 0;
		dialogueState = DIALOGUE::NONE;
	}


	void updateBtns(std::vector<std::string> bvalues) {
		// rendering coordinates 
		float btnWidth = static_cast<float>((AEGfxGetWindowWidth() - (padding * 2) - (bvalues.size() - 1) * spacing) / bvalues.size());
		float btnHeight = btnWidth / 3.f;
		btnHeight = btnHeight > maxBtnHeight ? maxBtnHeight : btnHeight;
		float lBtnX = padding + btnWidth / 2.f;
		float bPosX = lBtnX;
		AEVec2 btnText = wtos(bPosX, panelfinalY);
		if (playerAlive) {
			for (const std::string bv : bvalues) { // bruh wa this got got me too confused
				AEVec2 btnPos = stow(bPosX, btnY);  // button rendering position

				//cout << bPosX << " | " << btnY << "\n";
				if (CollisionChecker::isMouseInRect(bPosX, btnText.y, btnWidth, btnHeight, static_cast<float>(mouseX), static_cast<float>(mouseY))) {
					//cout << "mouse in rect" << bv << "\n";
					// clicked wt
					if (AEInputCheckTriggered(AEVK_LBUTTON)) {
						/*click while on main menu*/
						if (currentState == ACTION_BTNS::MAIN) {
							// ignore click to enter items submenu if item already used in this turn
							if (!(itemUsedSinceLastAttack && bv == "ITEMS")) {
								currentState = stateMap.find(bv)->second;
							}
						}
						else if (bv == "BACK" || bv == "NO") {
							currentState = ACTION_BTNS::MAIN;
						}
						else if (currentState == ACTION_BTNS::ATTACK) {
							if (bv == "FIRE") {
								CombatManager::getInstance().attackElement = Fire;
							}
							else if (bv == "WATER") {
								CombatManager::getInstance().attackElement = Water;
							}
							else if (bv == "METAL") {
								CombatManager::getInstance().attackElement = Metal;
							}
							else if (bv == "WOOD") {
								CombatManager::getInstance().attackElement = Wood;
							}
							else if (bv == "EARTH") {
								CombatManager::getInstance().attackElement = Earth;
							}
							attackUsed = bv;
							cout << bv << " pressed\n";

							if (CombatManager::getInstance().selectedEnemy != nullptr && CombatManager::getInstance().attackElement != Element::NO_ELEMENT) {
								/*if user presses attack*/
								CombatManager::getInstance().isPlayingEvent = true;


								Event::getInstance()->startRandomEvent();
							}
							else if (CombatManager::getInstance().selectedEnemy == nullptr && CombatManager::getInstance().attackElement != Element::NO_ELEMENT) {
								//RenderHelper::getInstance()->text("No enemy selected", AEGfxGetWindowWidth() / 2.f, AEGfxGetWindowHeight() / 2.f);
								cout << "No enemy selected\n";
							}
						}
						else if (bv == "YES") {
							cout << "Fleeing fight\n";
							CombatScene::getInstance().cleanup();
							currentState = ACTION_BTNS::MAIN;
							CombatManager::getInstance().end();
						}
						else if (currentState == ACTION_BTNS::ITEMS) {
							if (!itemUsedSinceLastAttack)
							{
								if (bv == "BACON") {
									int attackChange = rand() % 5 + 1; // Random value between 1-5
									player->attackMultipler(attackChange);
								}
								else if (bv == "CHICKEN") {
									int healthChange = -rand() % 11 - 5; // Random value between -15 and -5
									player->healthGain(healthChange);
								}
								else if (bv == "BEEF") {
									int healthChange = -rand() % 11 - 10; // Random value between -20 and -10
									player->healthGain(healthChange);
								}
								else if (bv == "CAT") {
									int healthChange = rand() % 11 + 10; // Random value between 10 and 20
									player->healthGain(healthChange);
								}
								cout << bv << " eaten\n";
								itemUsed = bv;
								itemUsedSinceLastAttack = true;
								dialogueState = DIALOGUE::ITEM;
								currentState = ACTION_BTNS::MAIN;
							}
							else {
								cout << "Item used since last attack\n";
								//currentState = ACTION_BTNS::MAIN;
							}
						}
					}

				}
				bPosX += btnWidth + spacing;
			}
		}
	}

	void updateDeathBtns() {
		//main menu
		if (CollisionChecker::isMouseInRect(deathBtnMenuPoint.x, deathBtnMenuPoint.y, deathBtnWidthEnd - 5.f, deathbtnHeightEnd, static_cast<float>(mouseX), static_cast<float>(mouseY))) {
			if (AEInputCheckTriggered(AEVK_LBUTTON)) {
				SceneManager::GetInstance()->SetActiveScene("SceneMenu");
			}
		}
		if (CollisionChecker::isMouseInRect(deathBtnRespawnPoint.x, deathBtnRespawnPoint.y, deathBtnWidthEnd - 5.f, deathbtnHeightEnd, static_cast<float>(mouseX), static_cast<float>(mouseY))) {
			if (AEInputCheckTriggered(AEVK_LBUTTON)) {
				//needs to reset combat scene
				cout << "does it work" << "\n";
			}
		}
	}
	void renderDeathBtns() {
		AEVec2 trueCoordinatesMenu = stow(deathBtnMenuPoint.x, deathBtnMenuPoint.y);
		AEVec2 trueCoordinatesRespawn = stow(deathBtnRespawnPoint.x, deathBtnRespawnPoint.y);


		RenderHelper::getInstance()->texture("button", trueCoordinatesMenu.x + camOffset.x, trueCoordinatesMenu.y + camOffset.y, deathBtnWidthEnd, deathbtnHeightEnd);
		RenderHelper::getInstance()->texture("button", trueCoordinatesRespawn.x + camOffset.x, trueCoordinatesRespawn.y + camOffset.y, deathBtnWidthEnd, deathbtnHeightEnd);
		RenderHelper::getInstance()->texture("mainMenu", trueCoordinatesMenu.x + camOffset.x, trueCoordinatesMenu.y + camOffset.y, deathBtnWidthEnd / 2, deathbtnHeightEnd / 2);
		RenderHelper::getInstance()->texture("respawn", trueCoordinatesRespawn.x + camOffset.x, trueCoordinatesRespawn.y + camOffset.y, deathBtnWidthEnd / 2, deathbtnHeightEnd / 2);




	}

	void updateWinBtns() {
		//main menu
		if (CollisionChecker::isMouseInRect(deathBtnMenuPoint.x, deathBtnMenuPoint.y, deathBtnWidthEnd - 5.f, deathbtnHeightEnd, static_cast<float>(mouseX), static_cast<float>(mouseY))) {
			if (AEInputCheckTriggered(AEVK_LBUTTON)) {
				SceneManager::GetInstance()->SetActiveScene("SceneMenu");
			}
		}
		if (CollisionChecker::isMouseInRect(deathBtnRespawnPoint.x, deathBtnRespawnPoint.y, deathBtnWidthEnd - 5.f, deathbtnHeightEnd, static_cast<float>(mouseX), static_cast<float>(mouseY))) {
			if (AEInputCheckTriggered(AEVK_LBUTTON)) {
				//needs to reset combat scene
				cout << "does it work" << "\n";
			}
		}
	}
	void renderWinBtns() {
		AEVec2 trueCoordinatesMenu = stow(deathBtnMenuPoint.x, deathBtnMenuPoint.y);
		AEVec2 trueCoordinatesRespawn = stow(deathBtnRespawnPoint.x, deathBtnRespawnPoint.y);


		RenderHelper::getInstance()->texture("button", trueCoordinatesMenu.x + camOffset.x, trueCoordinatesMenu.y + camOffset.y, deathBtnWidthEnd, deathbtnHeightEnd);
		RenderHelper::getInstance()->texture("button", trueCoordinatesRespawn.x + camOffset.x, trueCoordinatesRespawn.y + camOffset.y, deathBtnWidthEnd, deathbtnHeightEnd);
		RenderHelper::getInstance()->texture("mainMenu", trueCoordinatesMenu.x + camOffset.x, trueCoordinatesMenu.y + camOffset.y, deathBtnWidthEnd / 2, deathbtnHeightEnd / 2);
		RenderHelper::getInstance()->texture("respawn", trueCoordinatesRespawn.x + camOffset.x, trueCoordinatesRespawn.y + camOffset.y, deathBtnWidthEnd / 2, deathbtnHeightEnd / 2);




	}


	void renderBtns(std::vector<std::string> bvalues) {



		if (currentState == ACTION_BTNS::ATTACK && CombatManager::getInstance().attackElement != Element::NO_ELEMENT && CombatManager::getInstance().selectedEnemy == nullptr) {
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
			AEVec2 btnPos = stow(bPosX, panelfinalY);  // button rendering position
			AEVec2 btnText = wtos(bPosX, panelfinalY);

			int mX, mY;
			AEInputGetCursorPosition(&mX, &mY);
			if (!Pause::getInstance().isPaused && CollisionChecker::isMouseInRect(bPosX, btnText.y, btnWidth, btnHeight, static_cast<float>(mX), static_cast<float>(mY)) && playerAlive && !panelflag) {
				if (!(itemUsedSinceLastAttack && bv == "ITEMS")) {
					RenderHelper::getInstance()->texture("button", btnPos.x + camOffset.x, panelfinalY + camOffset.y, btnWidth, btnHeight + btnWordPadding * 2);
				}
				else {
					RenderHelper::getInstance()->texture("button", btnPos.x + camOffset.x, panelfinalY + camOffset.y - btnDecreaseY + btnIncreaseY, btnWidth, btnHeight + btnWordPadding, 1, Color{ 0, 0, 0, 0 }, 0, Color{ 0.5f,0.5f,0.5f,1 });
				}
				RenderHelper::getInstance()->rect(btnPos.x + camOffset.x, btnPos.y + camOffset.y, btnWidth, btnHeight, 0, Color{ 0.9f, 0.5f, 0.5f, 1.f });  // render highlight on hover. can consider doing transitions if got time?? but prob no time lel
			}
			else {
				RenderHelper::getInstance()->texture("button", btnPos.x + camOffset.x, panelfinalY + camOffset.y - btnDecreaseY + btnIncreaseY, btnWidth, btnHeight + btnWordPadding, 1, Color{ 0,0,0,0 }, 0, itemUsedSinceLastAttack && bv == "ITEMS" ? Color{ 0.5f,0.5f,0.5f,1 } : Color{ 1,1,1,1 });

				RenderHelper::getInstance()->rect(btnPos.x + camOffset.x, btnPos.y + camOffset.y, btnWidth, btnHeight, 0, Color{ 0.5f, 0.5f, 0.5f, 1.f });  // render normal when no hovering
			}
			RenderHelper::getInstance()->text(bv, bPosX, btnText.y + btnDecreaseY - btnIncreaseY);
			bPosX += btnWidth + spacing;
		}

	}

}

void CombatScene::spawnEnemies(std::vector<std::string> enemyRefs) {
	// this function works by creating taking in the vector of enemies; but this means i dont have to 
	itemDrop = enemyRefs;

	float Enemypadding = 50.0f;
	float texSize = 60.f;
	float newspacing = 225.f;
	int sz = static_cast<int>(enemyRefs.size()); // number of enemies;
	for (const auto& str : enemyRefs) {
		// Check if the string is already in the map
		if (itemCounts.find(str) != itemCounts.end()) {
			// Increment the count
			itemCounts[str]++;
		}
		else {
			// If not found, add it to the map with count as 1
			itemCounts[str] = 1;
		}
	}
	groups.coordinates.resize(sz); // setting the coordinates
	groups.enemies.resize(sz); // setting up the checking of enemies
	groups.activeEnemy.resize(sz);
	groups.names.resize(sz); // might not be needed, after getting the information from the
	float Enemyspacing = static_cast<float>((AEGfxGetWindowWidth() - (Enemypadding * 2) - (sz - 1) * newspacing) / sz);
	for (int i = 0; i < sz; i++) {
		groups.activeEnemy[i] = true;
		// coordindates
		float totalWidth = sz * texSize + (sz - 1) * Enemyspacing;

		//float centerX = AEGfxGetWindowWidth() / 2.f;
		float startX = (AEGfxGetWindowWidth() - totalWidth) / 2.f + Enemypadding;
		groups.coordinates[i].x = startX + i * (texSize + Enemyspacing);

		//groups.coordinates[i].x = Enemypadding + i * Enemyspacing;
		groups.coordinates[i].y = AEGfxGetWindowHeight() / 2.f - 25.f;
		// obtaining the infomation from json file
		groups.enemies[i] = new Enemy(
			elementMap.find(Database::getInstance()->data["enemyAttributes"][enemyRefs[i]]["element"])->second,
			Database::getInstance()->data["enemyAttributes"][enemyRefs[i]]["health"],
			Database::getInstance()->data["enemyAttributes"][enemyRefs[i]]["damage"] * DIFFICULTY_ENEMY_DAMAGE_MULTIPLIER.at(difficulty),
			Database::getInstance()->data["enemyAttributes"][enemyRefs[i]]["texturePath"],
			enemyRefs[i],		// dont change this, using this for audio too
			groups.coordinates[i].x,
			groups.coordinates[i].y,
			texSize
		);
		groups.enemies[i]->elementstringinput(Database::getInstance()->data["enemyAttributes"][enemyRefs[i]]["element"]);
	}



}

CombatScene::CombatScene()
{
}

CombatScene::~CombatScene()
{
}

CombatScene& CombatScene::getInstance() {
	static CombatScene instance;
	return instance;
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
	RenderHelper::getInstance()->registerTexture("cat", "./Assets/Combat_Enemy/cat.png");
	RenderHelper::getInstance()->registerTexture("horse", "./Assets/Combat_Enemy/horse.png");
	RenderHelper::getInstance()->registerTexture("dragon", "./Assets/Combat_Enemy/dragon.png");
	RenderHelper::getInstance()->registerTexture("playerdead", "./Assets/Combat_UI/playerdeadscreen.png");
	RenderHelper::getInstance()->registerTexture("button", "./Assets/Combat_UI/Button.png");
	RenderHelper::getInstance()->registerTexture("respawn", "./Assets/Combat_UI/respawn.png");
	RenderHelper::getInstance()->registerTexture("mainMenu", "./Assets/Combat_UI/MainMenu.png");
	RenderHelper::getInstance()->registerTexture("victory", "./Assets/Combat_UI/victory.png");
	//RenderHelper::getInstance()->registerTexture("blockwait1", "./Assets/Combat_UI/waitingForBlock1.png");
	//RenderHelper::getInstance()->registerTexture("blockwait2", "./Assets/Combat_UI/waitingForBlock2.png");
	RenderHelper::getInstance()->registerTexture("blockwait3", "./Assets/Combat_UI/waitingForBlock3.png");
	RenderHelper::getInstance()->registerTexture("blockNow", "./Assets/Combat_UI/BlockHappens.png");
	RenderHelper::getInstance()->registerTexture("nian", "./Assets/Combat_UI/nian.png");
	RenderHelper::getInstance()->registerTexture("Player_Fist_Left_01", "Assets/Combat_UI/MyFist_Left_1.png");
	RenderHelper::getInstance()->registerTexture("Player_Fist_Left_02", "Assets/Combat_UI/MyFist_Left_2.png");
	RenderHelper::getInstance()->registerTexture("Player_Fist_Left_03", "Assets/Combat_UI/MyFist_Left_3.png");
	RenderHelper::getInstance()->registerTexture("Player_Fist_Left_04", "Assets/Combat_UI/MyFist_Left_4.png");
	RenderHelper::getInstance()->registerTexture("Player_Fist_Right_01", "Assets/Combat_UI/MyFist_Right_1.png");
	RenderHelper::getInstance()->registerTexture("Player_Fist_Right_02", "Assets/Combat_UI/MyFist_Right_2.png");
	RenderHelper::getInstance()->registerTexture("Player_Fist_Right_03", "Assets/Combat_UI/MyFist_Right_3.png");
	RenderHelper::getInstance()->registerTexture("Player_Fist_Right_04", "Assets/Combat_UI/MyFist_Right_4.png");
	RenderHelper::getInstance()->registerTexture("spamE", "./Assets/Combat_UI/spamE.png");
	RenderHelper::getInstance()->registerTexture("timer", "./Assets/Combat_UI/timer.png");
	RenderHelper::getInstance()->registerTexture("multiClick", "./Assets/Combat_UI/multiClick.png");
	RenderHelper::getInstance()->registerTexture("orangeThrowing", "./Assets/Combat_UI/orangeThrowing.png");
	RenderHelper::getInstance()->registerTexture("blockTime", "./Assets/Combat_UI/blockTime.png");
	RenderHelper::getInstance()->registerTexture("fire", "./Assets/Combat_UI/fire.png");
	RenderHelper::getInstance()->registerTexture("water", "./Assets/Combat_UI/water.png");
	RenderHelper::getInstance()->registerTexture("earth", "./Assets/Combat_UI/earth.png");
	RenderHelper::getInstance()->registerTexture("metal", "./Assets/Combat_UI/metal.png");
	RenderHelper::getInstance()->registerTexture("wood", "./Assets/Combat_UI/wood.png");
	RenderHelper::getInstance()->registerTexture("pigname", "./Assets/Combat_UI/pig.png");
	RenderHelper::getInstance()->registerTexture("goatname", "./Assets/Combat_UI/goat.png");
	RenderHelper::getInstance()->registerTexture("dragonname", "./Assets/Combat_UI/dragon.png");
	RenderHelper::getInstance()->registerTexture("cowname", "./Assets/Combat_UI/cow.png");
	RenderHelper::getInstance()->registerTexture("monkeyname", "./Assets/Combat_UI/monkey.png");
	RenderHelper::getInstance()->registerTexture("snakename", "./Assets/Combat_UI/snake.png");
	RenderHelper::getInstance()->registerTexture("enemyPanel", "./Assets/Combat_UI/enemyPanel.png");













	cout << "CombatScene loaded\n";
}


void CombatScene::Init(CombatManager::TURN startingTurn)
{
	/*Event::getInstance()->setActiveEvent(EVENT_TYPES::SPAM_KEY);*/  // for testing only
	//player init
	winFlag = false;
	winTime = 0.f;
	dialogueState = DIALOGUE::NONE;
	wpos = stow(static_cast<float>(AEGfxGetWindowWidth()) / 2, static_cast<float>(AEGfxGetWindowHeight()) / 2);
	playerAlive = true;
	extraflagtest = true;
	deadfinalflag = false;
	panelflag = true;
	panelpos.x = 0; // constant value
	panelpos.y = -AEGfxGetWindowHeight() / 1.7f; // starting posY for the panel
	startingPanelY = panelpos.y;
	panelfinalY = -AEGfxGetWindowHeight() / 2.9f;
	currentTime = 0;
	initalScaleDead.x = 250;
	FinalScaleDead.x = 500;
	initalScaleDead.y = 50;
	FinalScaleDead.y = 100;

	dialougeTime = 0.f;
	winTime = 0.0f;
	winButtonFlag = false;
	itemTime = 0.f;

	ItemPanel.x = wpos.x;
	ItemPanel.y = wpos.y;
	itemPanelY = wpos.y + 150.f;


	blockingRenderTime = 0.f;
	blockNow = false;
	enemyattackedRenderTime = 0.f;

	deathBtnWidthEnd = 300.f;
	deathbtnHeightEnd = 150.f;

	deathBtnRespawnPoint.x = AEGfxGetWindowWidth() / 2 - deathBtnWidthEnd / 2 - 50.f;
	deathBtnMenuPoint.x = AEGfxGetWindowWidth() / 2 + deathBtnWidthEnd / 2 + 50.f;
	deathBtnRespawnPoint.y = static_cast<float> (AEGfxGetWindowHeight()) / 2 + 120;
	deathBtnMenuPoint.y = static_cast<float> (AEGfxGetWindowHeight()) / 2 + 120;

	currScaleDead.x = initalScaleDead.x;
	currScaleDead.y = initalScaleDead.y;

	btnWordPadding = 20.f;
	btnDecreaseY = 0.f;

	projectiles.clear();

	CombatManager::getInstance().start(startingTurn);

	Event::getInstance()->init();
}

void CombatScene::Update(double dt)
{
	updateGlobals();
	//cout << mouseX << "," << mouseY << " | " << camOffset.x << "," << camOffset.y << "\n";

	if (!CombatManager::getInstance().isInCombat) {
		return;
	}

	// update projectiles 
	std::vector<int> inactiveProjectileIdxs;
	inactiveProjectileIdxs.reserve(5);
	int ipIdx{ -1 };
	for (GameObject_Projectiles* pp : projectiles) {
		ipIdx++;
		if (!pp->m_Active) {
			inactiveProjectileIdxs.push_back(ipIdx);
			continue;
		}
		pp->Update(dt);
	}

	// remove inactive projectiles
	std::sort(inactiveProjectileIdxs.rbegin(), inactiveProjectileIdxs.rend());
	for (const int i : inactiveProjectileIdxs) {
		projectiles.erase(projectiles.begin() + i);
	}

	if (dialogueState != DIALOGUE::NONE) {
		if (dialougeTime < dialogueMaxTime) {
			dialougeTime += static_cast<float>(AEFrameRateControllerGetFrameTime());
		}
		else if (dialougeTime >= dialogueMaxTime) {
			resetDialogue();
		}
	}

	for (Enemy* enemy : groups.enemies) { // check for dead/alive
		if (enemy->isDead()) {
			RenderHelper::getInstance()->text("Enemy is dead", AEGfxGetWindowWidth() / 2.f, AEGfxGetWindowHeight() / 2.f); // need to adapt to pointer to the pos
		}
		if (player->isDead()) {
			//Set player to dead
			if (playerAlive) {
				playerAlive = false;
				deadfinalflag = false;
			}

		}
	}

	if (!playerAlive) {
		if (extraflagtest == true) {
			extraflagtest = false;
			panelflag = true;
			currentTime = 0.0f; // Reset the time for sliding animation
		}
	}
	if (!playerAlive) {
		updateDeathBtns();
	}
	if (winFlag && itemTime > slideAnimationDuration) {
		if (AEInputCheckTriggered(AEVK_SPACE)) {
			winButtonFlag = true;
		}
	}
	
		//	// kill all enemies
		//	for (const Enemy* e : groups.enemies) {
		//		delete e;
		//	}

		//	groups.enemies.clear();
		//	
		//	CombatManager::getInstance().end();
		//	delete player;
		//	player = nullptr;
		//	return;
		//}
		//updating panel 

		AEGfxGetCamPosition(&camX, &camY);

	//death lerp
	if ((!playerAlive && currentTime < slideAnimationDuration)) {
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
	else if (!playerAlive && currentTime >= slideAnimationDuration) {
		deadfinalflag = true; // Reset panel flag when animation is complete
	}
	else {
		deadfinalflag = false;
	}

	//win lerp
	if ((winFlag && winTime < slideAnimationDuration)) {
		winTime += static_cast<float>(AEFrameRateControllerGetFrameTime());
		float percenttime = static_cast<float>(winTime / slideAnimationDuration);
		float t = percenttime;
		if (t > slideAnimationDuration) {
			t = slideAnimationDuration;
		}
		currScaleDead.x = lerp(initalScaleDead.x, FinalScaleDead.x, t);
		currScaleDead.y = lerp(initalScaleDead.y, FinalScaleDead.y, t);
		btnDecreaseY = lerp(0, btnFinalY, t);

		panelpos.y = lerp(panelfinalY, startingPanelY, t); // Reverse direction for sliding down
	}
	if (winFlag && itemTime < slideAnimationDuration && slideAnimationDuration + 0.25f < winTime) {
		itemTime += static_cast<float>(AEFrameRateControllerGetFrameTime());
		float percenttime = static_cast<float>(itemTime / slideAnimationDuration);
		float t = percenttime;
		if (t > slideAnimationDuration) {
			t = slideAnimationDuration;
		}
		ItemPanel.y = lerp(wpos.y, itemPanelY, t);

	}


	//ini lerp
	if (playerAlive && currentTime < slideAnimationDuration) { // should include this in render.cpp instead
		currentTime += static_cast<float>(AEFrameRateControllerGetFrameTime());
		float percenttime = static_cast<float>(currentTime / slideAnimationDuration);
		float t = percenttime;
		if (t > slideAnimationDuration) {
			t = slideAnimationDuration;
		}
		panelpos.y = lerp(-AEGfxGetWindowHeight() / 1.7f, panelfinalY, t);
		btnIncreaseY = lerp(btnDecreaseStart, 0, t);
	}
	else if (playerAlive) {
		panelflag = false;
	}

	//player->update(dt);		// moved to level builder

	//if (AEInputCheckTriggered(AEVK_RBUTTON)) {
	//	selectflag = true;
	//	SelectEnemy = groups.enemies[0];
	//}
	combatEventResult = Event::getInstance()->getActiveEvent();

	// select enemy
	if (!CombatManager::getInstance().isPlayingEvent) {
		//cout << mouseX << "," << mouseY << " | " << camOffset.x << "," << camOffset.y << "\n";
		for (Enemy* e : groups.enemies) {
			e->update(dt);

			if (e->isSelected) {
				// deselect all other enemies
				CombatManager::getInstance().selectedEnemy = e;

				for (Enemy* e2 : groups.enemies) {
					if (e != e2) {
						e2->isSelected = false;
					}
				}
			}
			else {
				if (CombatManager::getInstance().selectedEnemy == e) {
					// enemy deselected
					CombatManager::getInstance().selectedEnemy = nullptr;
				}
			}
		}
	}


	AEVec2 p = stow(100, 100);
	Event::getInstance()->update(CombatManager::getInstance().qtEventResult, dt);



	// if player has finished quicktime event
	if (CombatManager::getInstance().qtEventResult != NONE_EVENT_RESULTS) {
		// end player's turn
		CombatManager::getInstance().selectedEnemy->enemyAttacked();

		// before ending, should have a second to display dialogue;
	/*	if (enemyattackedRenderTime < slideAnimationDuration) {
			CombatManager::getInstance().selectedEnemy->enemyAttacked();
			dialogueState = DIALOGUE::ENEMY_ATTACK;
			enemyattackedRenderTime += static_cast<float>(AEFrameRateControllerGetFrameTime());
		}
		else {*/
		CombatManager::getInstance().next();
		enemyattackedRenderTime = 0.f;

		dialogueState = DIALOGUE::PLAYER_ATTACK;

		cout << "Enemy next turn in " << CombatManager::getInstance().enemyNextTurnMs << "ms\n";
		CombatManager::getInstance().isPlayingEvent = false;
		CombatManager::getInstance().selectedEnemy->enemyAttacked();
		/*check if success or failure and modify damage accordingly*/
		switch (CombatManager::getInstance().qtEventResult) {
		case EVENT_RESULTS::SUCCESS:
			cout << "Event success. multiplier granted: " << Event::getInstance()->maxMultiplier << "\n";
			player->attack(*CombatManager::getInstance().selectedEnemy, CombatManager::getInstance().attackElement, Event::getInstance()->maxMultiplier);

			break;
		case EVENT_RESULTS::FAILURE:
			cout << "Event failure. multiplier granted: " << Event::getInstance()->minMultiplier << "\n";
			player->attack(*CombatManager::getInstance().selectedEnemy, CombatManager::getInstance().attackElement, Event::getInstance()->minMultiplier);

			break;
		case EVENT_RESULTS::CUSTOM_MULTIPLIER:
			cout << "Event custom multiplier granted: " << Event::getInstance()->eventMultiplier << "\n";
			player->attack(*CombatManager::getInstance().selectedEnemy, CombatManager::getInstance().attackElement, Event::getInstance()->eventMultiplier);

			break;
		}
		SoundPlayer::CombatAudio::getInstance().playSfxAnimal(CombatManager::getInstance().selectedEnemy->getTextureRef());
		CombatManager::getInstance().qtEventResult = EVENT_RESULTS::NONE_EVENT_RESULTS;

		// reset states
		CombatManager::getInstance().selectedEnemy = nullptr;
		for (Enemy* e : groups.enemies) {
			e->isSelected = false;
		}
		CombatManager::getInstance().attackElement = Element::NO_ELEMENT;
		currentState = ACTION_BTNS::MAIN;
		itemUsedSinceLastAttack = false; // Reset the flag after each attack

		//}
	}

	// when is player turn and player is not playing a quicktime event
	if (CombatManager::getInstance().turn == CombatManager::TURN::PLAYER && !CombatManager::getInstance().isPlayingEvent && panelflag == false && dialogueState == DIALOGUE::NONE) {
		updateBtns(btns[currentState]);  // render player action buttons
		blockNow = false;
	}
	else if (CombatManager::getInstance().turn == CombatManager::TURN::ENEMY && groups.enemies.size()) {
		//CombatManager::getInstance().selectedEnemy->enemyAttacking(CombatManager::getInstance().enemyNextTurnMs);
		CombatManager::getInstance().enemyNextTurnMs -= static_cast<int>(dt * 1000);
		if (CombatManager::getInstance().enemyNextTurnMs < CombatManager::PLAYER_BLOCKING_REACTION_ALLOWANCE_MS) {
			blockNow = true;
		}
		if (CombatManager::getInstance().enemyNextTurnMs < 0) {
			blockingRenderTime = 0.f; //reset the rendering time
			SceneStages::sInstance->Util_Camera_Shake(0.5f, 100);

			// fire projectile at player

			//blockNow = false;
			//Util_Camera_Shake(0.5, 100);
			player->playerAttacked();
			float multiplier = 1.f;
			switch (player->blockingState) {
			case PLAYER_BLOCKING_STATES::NOT_BLOCKING:
			case PLAYER_BLOCKING_STATES::ON_COOLDOWN:
				multiplier = 1.f;
				cout << "Attack not blocked by player at all, receiving " << multiplier << " damage multiplier against player\n";
				break;
			case PLAYER_BLOCKING_STATES::ON_ENTER:
			case PLAYER_BLOCKING_STATES::ON_EXIT:
				multiplier = 0.5f;
				cout << "Attack not fully blocked by player, receiving " << multiplier << " damage multiplier against player\n";
				break;
			case PLAYER_BLOCKING_STATES::ON_UPDATE:
				multiplier = 0.3f;
				cout << "Attack blocked by player, receiving " << multiplier << " damage multiplier against player\n";
				break;
			}

			// enemy to attack player if there are still enemies left
			if (groups.enemies.size()) {
				int randEnemyIndex = rand() % groups.enemies.size();
				cout << "Enemy with index " << randEnemyIndex << " is attacking player\n";

				Enemy* e = groups.enemies[randEnemyIndex];

				SoundPlayer::CombatAudio::getInstance().playSfxAnimal(e->getTextureRef());
				e->attack(*player, multiplier);  // Example: All enemies attack the player
				CombatManager::getInstance().next();

 				GameObject_Projectiles* np = Create::Projectiles();
				//cout << "Projectile pos: " << e->getWorldPos().x << ", " << e->getWorldPos().y << "\n";
				projectiles.push_back(np);
				np->FireAtPlayer(e->getWorldPos(), e->getSize(), static_cast<GameObject_Projectiles::ProjectileType>(rand() % GameObject_Projectiles::ProjectileType::NUM_PROJECTILE_TYPES));
			}
		}

	}
	else if (groups.enemies.size() == 0) {
		CombatManager::getInstance().turn = CombatManager::TURN::NONE_TURN;

		winTime += static_cast<float>(AEFrameRateControllerGetFrameTime());
		if (!winFlag) {
			dialogueState = DIALOGUE::WIN;
			winFlag = true;

		}
		else if (winFlag && winButtonFlag) {
			cout << "Transition to next level\n";
			CombatManager::getInstance().end();

			//delete player;
			//player = nullptr;
			//CombatManager::getInstance().end();
			//return;
		}
		// all enemies shldve been deleted
		//delete player;
		//player = nullptr;
		//CombatManager::getInstance().end();
		return;
	}

}

void CombatScene::Render()
{
	// dont render if no longer in combat
	if (!CombatManager::getInstance().isInCombat) {
		return;
	}

	updateGlobals();
	//cout << mouseX << "," << mouseY << " | " << camOffset.x << "," << camOffset.y << "\n";

	//if (!playerAlive) {
	//	RenderHelper::getInstance()->text("you have died", AEGfxGetWindowWidth() / 2.f, AEGfxGetWindowHeight() / 2.f); // need to adapt to pointer to the pos

	//}

	//panel rendering

	//rendering enemies



	std::vector<int> deadEnemies;
	int i{};
	// rendering whether enemies is dead
	if (playerAlive && !winFlag) {

		// rendering health when player active in the game and dont playing an event
		if (!CombatManager::getInstance().isPlayingEvent) {
			// !TODO: kuek no magic numbers pls
			player->renderHealth(150, 150);
		}

		for (Enemy* enemy : groups.enemies) { // check for dead/alive
			if (enemy->isDead()) {
				RenderHelper::getInstance()->text("Enemy is dead", AEGfxGetWindowWidth() / 2.f, AEGfxGetWindowHeight() / 2.f); // need to adapt to pointer to the pos
				deadEnemies.push_back(i);
			}
			//else if (player->isDead()) {
			//	RenderHelper::getInstance()->text("Player is dead", AEGfxGetWindowWidth() / 2.f, AEGfxGetWindowHeight() / 2.f); //set pos
			//}

			enemy->render();
			RenderHelper::getInstance()->texture("panel", panelpos.x + camOffset.x, panelpos.y + camOffset.y, static_cast<float>(AEGfxGetWindowWidth()), 160.f);

			if (CombatManager::getInstance().turn == CombatManager::TURN::PLAYER && !CombatManager::getInstance().isPlayingEvent && panelflag == false && dialogueState == DIALOGUE::NONE) {

				renderBtns(btns[currentState]);  // render player action buttons
			}
			else if (CombatManager::getInstance().turn == CombatManager::TURN::PLAYER && CombatManager::getInstance().isPlayingEvent) { // playing event, render the text on the panel!
				//std::string fulloutput;
				switch (combatEventResult) {
				case EVENT_TYPES::SPAM_KEY:
					RenderHelper::getInstance()->texture("spamE", panelpos.x + camOffset.x, panelpos.y + camOffset.y, static_cast<float>(AEGfxGetWindowWidth()) - 125.f, 85.f);
					break;
				case EVENT_TYPES::OSCILLATING_TIMER:
					RenderHelper::getInstance()->texture("timer", panelpos.x + camOffset.x, panelpos.y + camOffset.y, static_cast<float>(AEGfxGetWindowWidth()) - 125.f, 85.f);
					break;
				case EVENT_TYPES::MULTI_CLICK:
					RenderHelper::getInstance()->texture("multiClick", panelpos.x + camOffset.x, panelpos.y + camOffset.y, static_cast<float>(AEGfxGetWindowWidth()) - 125.f, 85.f);
					break;
				case EVENT_TYPES::ORANGE_THROWING:
					RenderHelper::getInstance()->texture("orangeThrowing", panelpos.x + camOffset.x, panelpos.y + camOffset.y, static_cast<float>(AEGfxGetWindowWidth()) - 125.f, 85.f);
					break;
				default:
					break;
				}
				//RenderHelper::getInstance()->text(fulloutput, AEGfxGetWindowWidth() / 2.f, AEGfxGetWindowHeight() * 0.85f);


			}
			else if (CombatManager::getInstance().turn == CombatManager::TURN::ENEMY) {
				//if (blockingRenderTime < 0.5f) {
				//if (!blockNow) {
				//	RenderHelper::getInstance()->texture("blockwait3", wpos.x + camOffset.x, wpos.y + camOffset.y, FinalScaleDead.x, FinalScaleDead.y); //start point, but coordinates is centralised so need to take account of the widthw
				//}
				//else {
				//	RenderHelper::getInstance()->texture("blockNow", wpos.x + camOffset.x, wpos.y + camOffset.y, FinalScaleDead.x, FinalScaleDead.y); //start point, but coordinates is centralised so need to take account of the widthw
				//}
				//RenderHelper::getInstance()->texture("blockwait1", wpos.x + camOffset.x, wpos.y + camOffset.y, FinalScaleDead.x, FinalScaleDead.y); //start point, but coordinates is centralised so need to take account of the widthw
			//}
			//else if(blockingRenderTime < 1.f){
			//	RenderHelper::getInstance()->texture("blockwait2", wpos.x + camOffset.x, wpos.y + camOffset.y, FinalScaleDead.x, FinalScaleDead.y); //start point, but coordinates is centralised so need to take account of the widthw

			//}
			//else {
			//	RenderHelper::getInstance()->texture("blockwait3", wpos.x + camOffset.x, wpos.y + camOffset.y, FinalScaleDead.x, FinalScaleDead.y); //start point, but coordinates is centralised so need to take account of the widthw

			//}
			//blockingRenderTime += AEFrameRateControllerGetFrameTime();
			//if (blockingRenderTime >= 1.5f) {
			//	blockingRenderTime = 0.f;
			//}

			//panel text
				RenderHelper::getInstance()->texture("blockTime", panelpos.x + camOffset.x, panelpos.y + camOffset.y, static_cast<float>(AEGfxGetWindowWidth()) - 125.f, 85.f);
			}
			else if (dialogueState != DIALOGUE::NONE && !CombatManager::getInstance().isPlayingEvent && playerAlive && !winFlag) {
				if (dialogueState == DIALOGUE::ITEM) {
					std::string fulloutput = "You have consumed " + itemUsed + "!";
					RenderHelper::getInstance()->text(fulloutput, AEGfxGetWindowWidth() / 2.f, AEGfxGetWindowHeight() * 0.85f);
				}
				else if (dialogueState == DIALOGUE::PLAYER_ATTACK) {
					std::string fulloutput = "You used " + attackUsed + "!";
					RenderHelper::getInstance()->text(fulloutput, AEGfxGetWindowWidth() / 2.f, AEGfxGetWindowHeight() * 0.85f);

				}
				else if (dialogueState == DIALOGUE::ENEMY_ATTACK) {
					std::string fulloutput = "You used " + attackUsed + "!\n";

					RenderHelper::getInstance()->text(fulloutput, AEGfxGetWindowWidth() / 2.f, AEGfxGetWindowHeight() * 0.85f);


				}
			}

			i++;

		}

	}


	if (CombatManager::getInstance().turn == CombatManager::TURN::ENEMY && !winFlag && playerAlive) {

		//if (blockingRenderTime < 0.5f) {
		if (!blockNow) {
			RenderHelper::getInstance()->texture("blockwait3", wpos.x + camOffset.x, wpos.y + camOffset.y, FinalScaleDead.x, FinalScaleDead.y); //start point, but coordinates is centralised so need to take account of the widthw

		}
		else {
			RenderHelper::getInstance()->texture("blockNow", wpos.x + camOffset.x, wpos.y + camOffset.y, FinalScaleDead.x, FinalScaleDead.y); //start point, but coordinates is centralised so need to take account of the widthw

		}
	}
	else if (!playerAlive) {
		//rendering out the objects
		RenderHelper::getInstance()->texture("panel", panelpos.x + camOffset.x, panelpos.y + camOffset.y, static_cast<float>(AEGfxGetWindowWidth()), 160.f);
		/*for (Enemy* enemy : groups.enemies) {
			enemy->render();
		}*/

		RenderHelper::getInstance()->texture("playerdead", wpos.x + camOffset.x, wpos.y + camOffset.y, currScaleDead.x, currScaleDead.y); //start point, but coordinates is centralised so need to take account of the widthw
		//renderBtns(btns[currentState]);
		//
		if (deadfinalflag == true)
			renderDeathBtns();
		//}

	}
	if (winFlag) {
		//rendering out the objects
		RenderHelper::getInstance()->texture("panel", panelpos.x + camOffset.x, panelpos.y + camOffset.y, static_cast<float>(AEGfxGetWindowWidth()), 160.f);
		RenderHelper::getInstance()->texture("victory", ItemPanel.x + camOffset.x, ItemPanel.y + camOffset.y, currScaleDead.x, currScaleDead.y); //start point, but coordinates is centralised so need to take account of the widthw
		// to do: new btns
		// new panel
		if (itemTime > slideAnimationDuration) {
			// panel for the item drop
			RenderHelper::getInstance()->texture("panel", ItemPanel.x + camOffset.x, wpos.y - 100.f + camOffset.y, 550.f, 350.f);


			// item drops 

		}
	}

	// remove all dead enemies
	std::sort(deadEnemies.rbegin(), deadEnemies.rend());		// sort in reverse order. else removing multiple might cause indexoutofrange
	for (const int index : deadEnemies) {
		// !TODO: add death animation (perhaps smoke particles to signify death)

		delete groups.enemies[index];
		groups.enemies.erase(groups.enemies.begin() + index);
	}
	Event::getInstance()->render();

	for (GameObject_Projectiles* pp : projectiles) {
		pp->Render();
	}

	player->render();		// rendering for combat scene. level builder will render while not in combat, else will default to this.
}

void CombatScene::cleanup() {
	for (Enemy* enemy : groups.enemies) {
		delete enemy;
	}
	groups.enemies.clear();
	//delete CombatManager::getInstance();
	// Clear the vector after deleting the enemies
	//delete player;
	//player = nullptr;
}

void CombatScene::Exit()
{
	cout << "Exiting CombatScene\n";
	cleanup();
}


bool CombatScene::getWinFlag()
{
	return winButtonFlag;
}