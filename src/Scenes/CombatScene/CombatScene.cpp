#include "CombatScene.h"

//#include "../../Backend/GameManager.h" //? Still thinking
#include "../../Scenes/SceneManager.h"
#include "../../Backend/Pch.h"
#include "../../utils/utils.h"
#include "../../Event/Event.h"
#include "../../utils/utils.h"
#include "../../GameObject/CombatObjects/CombatManager.h"
#include "../../GameObject/CombatObjects/CombatPlayer/CombatPlayer.h"
#include <vector>
#include <unordered_map>

Player* player;
Enemy* cat;
//std::vector<Enemy> enemies;

CombatScene* CombatScene::sInstance = new CombatScene(SceneManager::GetInstance());


namespace {
    EVENT_RESULTS combatEventResult = EVENT_RESULTS::NONE_EVENT_RESULTS;
    enum ACTION_BTNS {
        MAIN,
        ATTACK,
        ITEMS,
        CONFIRMATION
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
        {"YES", "NO"}  // confirmation. only used for flee option
    };
    float padding = 100.f;
    float spacing = 50.f;

    float btnY = 550.f;
    float maxBtnHeight = 100.f;

    /*im so sorry this code very spaghet but time crunch!!*/
    // definitely not good practice to put event handling together with drawing but it helps with the time complexity..
    void renderBtns(std::vector<std::string> bvalues) {
        float btnWidth = static_cast<float>((AEGfxGetWindowWidth() - (padding * 2) - (bvalues.size() - 1) * spacing) / bvalues.size());
        float btnHeight = btnWidth / 3.f;
        btnHeight = btnHeight > maxBtnHeight ? maxBtnHeight : btnHeight;
        float lBtnX = padding + btnWidth / 2.f;

        float bPosX = lBtnX;
        for (const std::string bv : bvalues) {
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
}


void CombatScene::Init()
{
    /*Event::getInstance()->setActiveEvent(EVENT_TYPES::SPAM_KEY);*/  // for testing only

    player = new Player();
    cat = new Enemy(Element::Water, 100, 10, "./Assets/animals/cat.jpg", "cat", AEGfxGetWindowWidth() / 2.f, AEGfxGetWindowHeight() / 2.f, 200);
}

void CombatScene::Update(double dt)
{
    if (AEInputCheckTriggered(AEVK_3)) {
        Event::getInstance()->setActiveEvent(EVENT_TYPES::SPAM_KEY);
    }

    //RenderHelper::getInstance()->text("IM SO TIRED", AEGfxGetWindowWidth() / 2.f, AEGfxGetWindowHeight() / 2.f);

    Point p = stow(100, 100);
    Event::getInstance()->updateLoop(CombatManager::getInstance()->qtEventResult, dt, p.x, p.y);

    if (CombatManager::getInstance()->qtEventResult != NONE_EVENT_RESULTS) {
        // end player's turn
        CombatManager::getInstance()->next();
        CombatManager::getInstance()->isPlayingEvent = false;

        /*check if success or failure and modify damage accordingly*/
        switch (CombatManager::getInstance()->qtEventResult) {
        case EVENT_RESULTS::SUCCESS:
            player->attack(*cat, CombatManager::getInstance()->attackElement, 2);
            break;
        case EVENT_RESULTS::FAILURE:
            player->attack(*cat, CombatManager::getInstance()->attackElement, 0.5);
            break;
        case EVENT_RESULTS::CUSTOM_MULTIPLIER:
            // apply custom multiplier. use combatmanager.qtmultiplier or smtg like that
            break;
        }
        CombatManager::getInstance()->qtEventResult = EVENT_RESULTS::NONE_EVENT_RESULTS;
    }

    if (CombatManager::getInstance()->turn == TURN::PLAYER && !CombatManager::getInstance()->isPlayingEvent) {
        renderBtns(btns[currentState]);
    }
    else if (CombatManager::getInstance()->turn == TURN::ENEMY){
        cat->attack(*player);
        CombatManager::getInstance()->next();  // perhaps can implement pause
    }

    
    cat->render();
    player->render();

    if (cat->isDead()) {
        RenderHelper::getInstance()->text("Enemy is dead", AEGfxGetWindowWidth() / 2.f, AEGfxGetWindowHeight() / 2.f);
    }
    else if (player->isDead()) {
        RenderHelper::getInstance()->text("Player is dead", AEGfxGetWindowWidth() / 2.f, AEGfxGetWindowHeight() / 2.f);
    }


     //!TODO
     //draw health of enemy (just use number for now)
     //use function `ston` screen to normalized to render text

     //!TODO
     //draw text describing the action of enemy, disappear after 1 second
}

void CombatScene::Render()
{
    
}

void CombatScene::Exit()
{
    std::cout << "Exiting CombatScene\n";
    delete CombatManager::getInstance();
    delete cat;
}


