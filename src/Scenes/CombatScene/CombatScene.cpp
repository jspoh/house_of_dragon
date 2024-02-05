#include "CombatScene.h"

//#include "../../Backend/GameManager.h" //? Still thinking
#include "../../Scenes/SceneManager.h"
#include "../../Backend/Pch.h"
#include "../../utils/utils.h"
#include "../../Event/Event.h"
#include "../../utils/utils.h"
#include <vector>
#include <unordered_map>

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

    int btnY = 550;
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
                        /*if user presses attack*/

                        // start a random quicktime event
                        double time;
                        AEGetTime(&time);
                        srand(time);
                        EVENT_TYPES e = static_cast<EVENT_TYPES>(rand() % NUM_EVENT_TYPES);
                        e = EVENT_TYPES::SPAM_KEY;  // hardcoded for now as we dont have multiple quicktime events yet

                        if (bv == "FIRE") {
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
    Draw::getInstance()->registerTexture("cat", "./Assets/animals/cat.jpg");
}


void CombatScene::Init()
{
    combatEventResult = EVENT_RESULTS::NONE_EVENT_RESULTS;
    /*Event::getInstance()->setActiveEvent(EVENT_TYPES::SPAM_KEY);*/  // for testing only
}

void CombatScene::Update(double dt)
{
    if (AEInputCheckTriggered(AEVK_3)) {
        Event::getInstance()->setActiveEvent(EVENT_TYPES::SPAM_KEY);
    }

    //Draw::getInstance()->text("IM SO TIRED", AEGfxGetWindowWidth() / 2, AEGfxGetWindowHeight() / 2);

    Point p = stow(100, 100);
    Event::getInstance()->updateLoop(combatEventResult, dt, p.x, p.y);

    if (combatEventResult != NONE_EVENT_RESULTS) {
        /*check if success or failure and modify damage accordingly*/
    }

    renderBtns(btns[currentState]);

    Point catPos = { AEGfxGetWindowWidth() / 2, AEGfxGetWindowHeight() / 2 };
    catPos = stow(catPos.x, catPos.y);
    Draw::getInstance()->texture("cat", catPos.x, catPos.y, 200, 200);

    // !TODO
    // draw health of enemy (just use number for now)
    // use function `ston` screen to normalized to render text

    // !TODO
    // draw text describing the action of enemy, disappear after 1 second
}

void CombatScene::Render()
{
    
}

void CombatScene::Exit()
{
    std::cout << "Exiting CombatScene\n";
    Draw::getInstance()->removeTextureByRef("cat");
}














/**
 * !TODO move these out.
 */

Mob::Mob(Element element, double health, double dmg) : health(health), dmg(dmg), element(element) {

}

Enemy::Enemy(Element element, double health, double dmg) : Mob(element, health, dmg) {

}

Cat::Cat(Element element, double health, double dmg) : Enemy(element, health, dmg) {

}

Player::Player(double health, double dmg, Element element) : Mob(element, health, dmg) {

}

double Mob::attack(Mob& target) {
    DamageMultiplier dm = ElementProperties::getEffectiveDamage(this->element, target.element);
    float multiplier = 1;
    switch (dm) {
    case Weak:
        multiplier = 0.5;
        break;
    case Strong:
        multiplier = 2;
        break;
    }

    double damage = this->dmg * multiplier;
    target.health -= damage;
    return damage;
}

double Player::attack(Mob& target, Element attackEl, double qtMultiplier) {
    DamageMultiplier dm = ElementProperties::getEffectiveDamage(attackEl, target.element);
    float multiplier = 1;
    switch (dm) {
    case Weak:
        multiplier = 0.5;
        break;
    case Strong:
        multiplier = 2;
        break;
    }

    double damage = this->dmg * multiplier * qtMultiplier;
    target.health -= damage;
    return damage;
}

bool Mob::isDead() {
    return this->health <= 0;
}
