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


#include "CombatScene.h"

//#include "../../Backend/GameManager.h" //? Still thinking
#include "SceneManager.h"
#include "Pch.h"
#include "Event.h"
#include "CombatManager.h"
#include "CombatPlayer.h"

#include <vector>
#include <unordered_map>


//additional

CombatScene* CombatScene::sInstance = new CombatScene(SceneManager::GetInstance());

namespace {
    Player* player;

    std::vector<Point*> coordinates; // coordinates for the aninmals which changes based on the number of enemies as well as the screen size
    std::vector<Enemy*> enemies; // currently hardcoded for the 3 enemies, but how do we register the different amounts and their stats in?
    Enemy* SelectEnemy; // selected enemy for attack
    int vectorcounter;
    // panel rendering
    double panelvelocity;
    Point panelpos;
    double panelfinalY;
    bool panelflag;
    double currentTime;
    double totaltime;
    int randomEnemyStart;
    // enemy selection
    bool selectflag;
    int texSize;


    EVENT_RESULTS combatEventResult = EVENT_RESULTS::NONE_EVENT_RESULTS;
    enum ACTION_BTNS {
        MAIN,
        ATTACK,
        ITEMS,
        CONFIRMATION,
        SELECT
    };
    ACTION_BTNS currentState = MAIN;

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
        {"Select your enemy!"}
    };
    float padding = 100.f;
    float spacing = 50.f;
    enemiesGroup groups;
    float btnY = 550.f;
    float maxBtnHeight = 100.f;

    /*im so sorry this code very spaghet but time crunch!!*/
    // definitely not good practice to put event handling together with drawing but it helps with the time complexity..
    void updateRenderBtns(std::vector<std::string> bvalues) {
        float btnWidth = static_cast<float>((AEGfxGetWindowWidth() - (padding * 2) - (bvalues.size() - 1) * spacing) / bvalues.size());
        float btnHeight = btnWidth / 3.f;
        btnHeight = btnHeight > maxBtnHeight ? maxBtnHeight : btnHeight;
        float lBtnX = padding + btnWidth / 2.f;

        float bPosX = lBtnX;
        if (selectflag == false) {
            RenderHelper::getInstance()->text("Select your Enemy", AEGfxGetWindowWidth() / 2.f, AEGfxGetWindowHeight() / 2.f);
        }
        else {
            for (const std::string bv : bvalues) { // bruh wa this got got me too confused
                Point btnPos = stow(bPosX, btnY);  // button rendering position

                int mouseX, mouseY;
                AEInputGetCursorPosition(&mouseX, &mouseY);
                if (CollisionChecker::isMouseInRect(bPosX, btnY, btnWidth, btnHeight, static_cast<float>(mouseX), static_cast<float>(mouseY))) {
                    if (AEInputCheckTriggered(AEVK_LBUTTON)) {
                        /*click while on main menu*/
                        if (currentState == ACTION_BTNS::MAIN) {
                            currentState = stateMap.find(bv)->second;
                        }
                        else if (bv == "BACK" || bv == "NO") {
                            currentState = ACTION_BTNS::MAIN;
                        }
                        else if (currentState == ACTION_BTNS::ATTACK) {
                            if (SelectEnemy == NULL) {
                                //RenderHelper::getInstance()->text("Select your Enemy", AEGfxGetWindowWidth() / 2.f, AEGfxGetWindowHeight() / 2.f);
                                selectflag = false;



                            }
                            else {
                                /*if user presses attack*/
                                CombatManager::getInstance()->isPlayingEvent = true;

                                Event::getInstance()->startRandomEvent();


                                if (bv == "FIRE") {
                                    CombatManager::getInstance()->attackElement = Fire;
                                }
                                else if (bv == "WATER") {
                                    CombatManager::getInstance()->attackElement = Water;
                                }
                                else if (bv == "METAL") {
                                    CombatManager::getInstance()->attackElement = Metal;
                                }
                                else if (bv == "WOOD") {
                                    CombatManager::getInstance()->attackElement = Wood;
                                }
                                else if (bv == "EARTH") {
                                    CombatManager::getInstance()->attackElement = Earth;
                                }

                            }
                        }
                        else if (bv == "YES") {
                            std::cout << "Fleeing fight\n";
                        }
                    }
                    RenderHelper::getInstance()->rect(btnPos.x, btnPos.y, btnWidth, btnHeight, 0, Color{ 0.5f, 0.5f, 0.5f, 1.f });  // render highlight on hover. can consider doing transitions if got time?? but prob no time lel
                }
                else {
                    RenderHelper::getInstance()->rect(btnPos.x, btnPos.y, btnWidth, btnHeight, 0, Color{ 0.3f, 0.3f, 0.3f, 1.f });  // render normal when no hovering
                }

                RenderHelper::getInstance()->text(bv, bPosX, btnY);
                bPosX += btnWidth + spacing;
            }
        }
    }




}
void CombatScene::spawnEnemies(std::vector<std::string> enemyRefs) {
    // this function works by creating taking in the vector of enemies; but this means i dont have to 
    float Enemypadding = 50.0f;

    groups.size = enemyRefs.size(); // number of enemies;
    groups.coordinates.resize(groups.size); // setting the coordinates
    groups.enemies.resize(groups.size); // setting up the checking of enemies
    groups.activeEnemy.resize(groups.size);
    groups.name.resize(groups.size); // might not be needed, after getting the information from the
    float Enemyspacing = static_cast<float>((AEGfxGetWindowWidth() - (Enemypadding * 2) - (groups.size - 1) * spacing) / groups.size);
    for (int i = 0; i < groups.size; i++) {
        groups.activeEnemy[i] = true;
        // coordindates
        groups.coordinates[i].x = Enemypadding + i * Enemyspacing;
        groups.coordinates[i].y = AEGfxGetWindowHeight() / 2.f;
        // obtaining the infomation from json file
        groups.enemies[i] = new Enemy(Element::Fire,100, 10, "./Assets/animals/cat.jpg", i + "temr", groups.coordinates[i].x, groups.coordinates[i].y, texSize);
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
    RenderHelper::getInstance()->registerTexture("border", "./Assets/Combat/border.png");
    RenderHelper::getInstance()->registerTexture("panel", "./Assets/Combat/panel.png");
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


}


void CombatScene::Init()
{
    /*Event::getInstance()->setActiveEvent(EVENT_TYPES::SPAM_KEY);*/  // for testing only
    //player init

    player = new Player();
    std::vector<std::string> names = { "cat", "cat","cat"};
    CombatScene::spawnEnemies(names);
    SelectEnemy = NULL;
    selectflag = true;

    panelflag = true;
    panelpos.x = 0; // constant value
    panelpos.y = -AEGfxGetWindowHeight() / 1.7f; // starting posY for the panel
    panelfinalY = -AEGfxGetWindowHeight() / 2.9f;
    currentTime = 0;
    totaltime = 1.f;

    randomEnemyStart = rand() % 3;
    vectorcounter = 0;
    // all enemies init, not implemented using the py yet
    coordinates.push_back(new Point{ 100, AEGfxGetWindowHeight() / 2.f});
    enemies.push_back(new Enemy(Element::Water, 100, 10, "./Assets/animals/cat.jpg", "cat1", coordinates[vectorcounter]->x, coordinates[vectorcounter]->y, 50));
    coordinates.push_back(new Point{ ((AEGfxGetWindowWidth() / 3.f) + 50), AEGfxGetWindowHeight() / 2.f });
    vectorcounter++;
    enemies.push_back(new Enemy(Element::Water, 100, 10, "./Assets/animals/dragon.jpg", "cat2", coordinates[vectorcounter]->x, coordinates[vectorcounter]->y, 50));
    coordinates.push_back(new Point{ ((AEGfxGetWindowWidth() * 2 / 3.f) + 50), AEGfxGetWindowHeight() / 2.f });
    vectorcounter++;
    enemies.push_back(new Enemy(Element::Water, 100, 10, "./Assets/animals/horse.jpg", "cat3", coordinates[vectorcounter]->x, coordinates[vectorcounter]->y, 50));
    for (int i{0 }; i <= vectorcounter; i++) {
        coordinates[i]->x = AEGfxGetWindowWidth() * (1.f + i  / vectorcounter + 1.f) ;
    }
    // selected enemy init
    Enemy* selectedEnemy;
}

void CombatScene::Update(double dt)
{

    if (currentTime < totaltime) { // should include this in render.cpp instead
        currentTime += AEFrameRateControllerGetFrameTime();
        double percenttime = currentTime / totaltime;
        double t = percenttime;
        if (t > totaltime) {
            t = totaltime;
        }
        panelpos.y = lerp(-AEGfxGetWindowHeight() / 1.7f, panelfinalY, t);
    }
    else {
        panelflag = false;
    }

    RenderHelper::getInstance()->texture("panel", panelpos.x, panelpos.y , AEGfxGetWindowWidth(), 160);

    if (AEInputCheckTriggered(AEVK_RBUTTON)) {
        selectflag = true;
        SelectEnemy = enemies[0];
    }
    if (AEInputCheckTriggered(AEVK_3)) {
        Event::getInstance()->setActiveEvent(EVENT_TYPES::SPAM_KEY);
    }
    
    for (int i = 0; i < groups.size;i++) {
        //if()

        groups.enemies[i]->render(); // render all, draw all enemys
    }
    player->render();
    for (Enemy* enemy : enemies) { // check for dead/alive
        if (enemy->isDead()) {
            RenderHelper::getInstance()->text("Enemy is dead", AEGfxGetWindowWidth() / 2.f, AEGfxGetWindowHeight() / 2.f); // need to adapt to pointer to the pos
        }
        else if (player->isDead()) {
            RenderHelper::getInstance()->text("Player is dead", AEGfxGetWindowWidth() / 2.f, AEGfxGetWindowHeight() / 2.f); //set pos
        }
    }
    //!TODO
    //draw health of enemy
    //use function `ston` screen to normalized to render text

    //!TODO
    //draw text describing the action of enemy, disappear after 1 second, or render attack animation

    Point p = stow(100, 100);
    Event::getInstance()->updateRenderLoop(CombatManager::getInstance()->qtEventResult, dt);

    // if player has finished quicktime event
    if (CombatManager::getInstance()->qtEventResult != NONE_EVENT_RESULTS) {
        // end player's turn
        CombatManager::getInstance()->next();
        CombatManager::getInstance()->isPlayingEvent = false;

        /*check if success or failure and modify damage accordingly*/
        switch (CombatManager::getInstance()->qtEventResult) {
        case EVENT_RESULTS::SUCCESS:
            std::cout << "Event success. multiplier granted: " << Event::getInstance()->maxMultiplier << "\n";
            for (Enemy* enemy : enemies) {
                player->attack(*enemy, CombatManager::getInstance()->attackElement, Event::getInstance()->maxMultiplier);
            }
            break;
        case EVENT_RESULTS::FAILURE:
            std::cout << "Event failure. multiplier granted: " << Event::getInstance()->minMultiplier << "\n";
            for (Enemy* enemy : enemies) {
                player->attack(*enemy, CombatManager::getInstance()->attackElement, Event::getInstance()->minMultiplier);
            }
            break;
        case EVENT_RESULTS::CUSTOM_MULTIPLIER:
            std::cout << "Event custom multiplier granted: " << Event::getInstance()->eventMultiplier << "\n";
            for (Enemy* enemy : enemies) {
                player->attack(*enemy, CombatManager::getInstance()->attackElement, Event::getInstance()->eventMultiplier);
            }
            break;
        }
        CombatManager::getInstance()->qtEventResult = EVENT_RESULTS::NONE_EVENT_RESULTS;
    }

    // when is player turn and player is not playing a quicktime event
    if (CombatManager::getInstance()->turn == TURN::PLAYER && !CombatManager::getInstance()->isPlayingEvent && panelflag == false) {
        updateRenderBtns(btns[currentState]);  // render player action buttons
    }
    else if (CombatManager::getInstance()->turn == TURN::ENEMY){
            enemies[randomEnemyStart++]->attack(*player);  // Example: All enemies attack the player
            randomEnemyStart %= 3; // prevents from going out of vector
        
        CombatManager::getInstance()->next();  // perhaps can implement pause
    }

    



}

void CombatScene::Render()
{
    // unfortunately i cant use this render function lol, i mixed update and render together in event for ease of usage
    //cat->render();
    //player->render();

    //if (cat->isDead()) {
    //    RenderHelper::getInstance()->text("Enemy is dead", AEGfxGetWindowWidth() / 2.f, AEGfxGetWindowHeight() / 2.f);
    //}
    //else if (player->isDead()) {
    //    RenderHelper::getInstance()->text("Player is dead", AEGfxGetWindowWidth() / 2.f, AEGfxGetWindowHeight() / 2.f);
    //}
    // //!TODO
    // //draw health of enemy
    // //use function `ston` screen to normalized to render text

    // //!TODO
    // //draw text describing the action of enemy, disappear after 1 second, or render attack animation
    
}

void CombatScene::Exit()
{
    std::cout << "Exiting CombatScene\n";
    delete CombatManager::getInstance();
    for (Enemy* enemy : enemies) {
        delete enemy;
    }

    // Clear the vector after deleting the enemies
    enemies.clear();
    delete player;
}


