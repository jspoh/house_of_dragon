#include "CombatScene.h"

//#include "../../Backend/GameManager.h" //? Still thinking
#include "../../Scenes/SceneManager.h"
#include "../../Backend/Pch.h"
#include "../../utils/utils.h"
#include "../../Event/Event.h"
#include "../../utils/utils.h"
#include "../../GameObject/Animals/Elements.hpp"
#include "../../GameObject/Animals/ZodiacAnimals.hpp"
#include "../../GameObject/CombatManager.hpp"
#include <vector>
#include <unordered_map>
#include <iomanip>
#include <sstream>

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
        {"FIRE", "WATER", "METAL", "WOOD", "WIND", "BACK"},  // attack elements
        {"BACON", "BEEF", "CHICKEN", "CAT(jk pls)", "BACK"},  // items
        {"YES", "NO"}  // confirmation. only used for flee option
    };
    int padding = 100;
    int spacing = 50;

    int btnY = 800;
    int maxBtnHeight = 100;

    /*im so sorry this code very spaghet but time crunch!!*/
    // definitely not good practice to put event handling together with drawing but it helps with the time complexity..
    void renderBtns(std::vector<std::string> bvalues) {
        int btnWidth = (AEGfxGetWindowWidth() - (padding * 2) - (bvalues.size() - 1) * spacing) / bvalues.size();
        int btnHeight = btnWidth / 3;
        btnHeight = btnHeight > maxBtnHeight ? maxBtnHeight : btnHeight;
        int lBtnX = padding + btnWidth / 2;

        int bPosX = lBtnX;
        for (const std::string bv : bvalues) {
            Point btnPos = stow(bPosX, btnY);  // button rendering position

            int mouseX, mouseY;
            AEInputGetCursorPosition(&mouseX, &mouseY);
            if (CollisionChecker::isMouseInRect(bPosX, btnY, btnWidth, btnHeight, mouseX, mouseY)) {
                if (AEInputCheckTriggered(AEVK_LBUTTON)) {
                    /*click while on main menu*/
                    if (currentState == ACTION_BTNS::MAIN) {
                        currentState = stateMap.find(bv)->second;
                    }
                    else if (bv == "BACK" || bv == "NO") {
                        currentState = ACTION_BTNS::MAIN;
                    }
                    else if (currentState == ACTION_BTNS::ATTACK) {
                        // start a random quicktime event
                        double time;
                        AEGetTime(&time);
                        srand(time);
                        EVENT_TYPES e = static_cast<EVENT_TYPES>(rand() % NUM_EVENT_TYPES);
                        e = EVENT_TYPES::SPAM_KEY;  // hardcoded for now as we dont have multiple quicktime events yet

                        if (bv == "FIRE") {
                            Draw::getInstance()->text(ElementProperties::getPlayerAbilityNames(Element::Fire), 50, 50);

                            // fire attack
                        }
                        else if (bv == "WATER") {

                        }
                        else if (bv == "METAL") {

                        }
                        else if (bv == "WOOD") {

                        }
                        else if (bv == "WIND") {

                        }

                        Event::getInstance()->setActiveEvent(e);
                    }
                    else if (bv == "YES") {
                        std::cout << "Fleeing fight\n";
                    }
                }
                Draw::getInstance()->rect(btnPos.x, btnPos.y, btnWidth, btnHeight, 0, Color{ 0.5, 0.5, 0.5, 1 });  // render highlight on hover. can consider doing transitions if got time?? but prob no time lel
            }
            else {
                Draw::getInstance()->rect(btnPos.x, btnPos.y, btnWidth, btnHeight, 0, Color{ 0.3, 0.3, 0.3, 1 });  // render normal when no hovering
            }

            Draw::getInstance()->text(bv, bPosX, btnY);
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
<<<<<<< Updated upstream
=======
    Draw::getInstance()->registerTexture("cat", "./Assets/animals/cat.jpg");

>>>>>>> Stashed changes
}


void CombatScene::Init()
{
    combatEventResult = EVENT_RESULTS::NONE_EVENT_RESULTS;
    /*Event::getInstance()->setActiveEvent(EVENT_TYPES::SPAM_KEY);*/  // for testing only
}

void CombatScene::Update(double dt)
{
<<<<<<< Updated upstream
    //if (AEInputCheckTriggered(AEVK_3)) {
    //    Event::getInstance()->setActiveEvent(EVENT_TYPES::SPAM_KEY);
    //}
=======
    Tiger myTiger("TigerName");
    double* healthpointer = (myTiger.getHealth());
    if (AEInputCheckTriggered(AEVK_3)) {
        Event::getInstance()->setActiveEvent(EVENT_TYPES::SPAM_KEY);
    }
>>>>>>> Stashed changes

    //Draw::getInstance()->text("IM SO TIRED", AEGfxGetWindowWidth() / 2, AEGfxGetWindowHeight() / 2);

    Point p = stow(100, 100);
    Event::getInstance()->updateLoop(combatEventResult, dt, p.x, p.y);

    renderBtns(btns[currentState]);
<<<<<<< Updated upstream
=======

    Point catPos = { AEGfxGetWindowWidth() / 2, AEGfxGetWindowHeight() / 2 };
    catPos = stow(catPos.x, catPos.y);
    Draw::getInstance()->texture("cat", catPos.x, catPos.y, 200, 200);

    // !TODO
    // draw health of enemy (just use number for now)
    // use function `ston` screen to normalized to render text
    std::stringstream animalhealth;
    double newone = *healthpointer;
    animalhealth << std::fixed<< std::setprecision(2) << std::to_string(newone);
    Draw::getInstance()->text("EARTH TIGER : " + animalhealth.str(), 400, 50); // health.
    myTiger.setHealth(69);

    // !TODO
    // draw text describing the action of enemy, disappear after 1 second
>>>>>>> Stashed changes
}

void CombatScene::Render()
{
    
}

void CombatScene::Exit()
{
    std::cout << "Exiting CombatScene\n";
    //Draw::getInstance()->removeTextureByRef("planet");
}