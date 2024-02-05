#include "CombatScene.h"

//#include "../../Backend/GameManager.h" //? Still thinking
#include "../../Scenes/SceneManager.h"
#include "../../Backend/Pch.h"
#include "../../utils/utils.h"
#include "../../Event/Event.h"
#include "../../utils/utils.h"
#include <vector>
#include <unordered_map>


/**
 * !TODO move these out.
 */

Mob::Mob(Element element, double health, double dmg) : health(health), dmg(dmg), element(element), maxHealth(health) {

}

Enemy::Enemy(Element element, double health, double dmg, std::string texturePath, std::string textureRef ,float screenX, float screenY, float size) 
    : Mob(element, health, dmg), _textureRef(textureRef), _size(size) {
    this->_spos.x = screenX;
    this->_spos.y = screenY;

    //_spos = Point{ AEGfxGetWindowWidth() / 2, AEGfxGetWindowHeight() / 2 };
    this->_wpos = stow(_spos.x, _spos.y);

    Draw::getInstance()->registerTexture(textureRef, texturePath);  // problematic code stopping execution

    //Draw::getInstance()->texture(_textureRef, _wpos.x, _wpos.y, _size, _size);
}

void Enemy::render() {
    std::cout << Draw::getInstance()->getTextureByRef(this->_textureRef) << ", " << this->_textureRef << "\n";
    Draw::getInstance()->texture(_textureRef, _wpos.x, _wpos.y, _size, _size);
}

Enemy::~Enemy() {
    Draw::getInstance()->removeTextureByRef(this->_textureRef);
}

Cat::Cat(Element element, double health, double dmg, std::string texturePath, std::string textureRef, float screenX, float screenY, float size) : Enemy(element, health, dmg, texturePath, textureRef, screenX, screenY, size) {

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

void Mob::reset() {
    this->health = maxHealth;
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

namespace {
    enum TURN {
        PLAYER,
        ENEMY,
        NUM_TURNS
    };
}

class CombatManager {
private:
    static CombatManager* _instance;

    static CombatManager* getInstance() {
        if (!_instance) {
            _instance = new CombatManager();
        }
        return _instance;
    }

    ~CombatManager() {
        if (_instance) {
            delete _instance;
        }
    }


public:
    TURN turn = TURN::PLAYER;

    void next() {
        turn = static_cast<TURN>((turn + 1) % TURN::NUM_TURNS);
    }

    static void destroy() {
        if (_instance) {
            delete _instance;
        }
    }
};
CombatManager* CombatManager::_instance = nullptr;



/*actual combatscene stuff*/

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



    Enemy* cat;
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
    combatEventResult = EVENT_RESULTS::NONE_EVENT_RESULTS;
    /*Event::getInstance()->setActiveEvent(EVENT_TYPES::SPAM_KEY);*/  // for testing only


    cat = new Cat(Element::Water, 100, 10, "./Assets/animals/cat.jpg", "cat", AEGfxGetWindowWidth() / 2, AEGfxGetWindowHeight() / 2, 200);  // rmb to clear memory!!
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

    
    //cat->render();


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
    //CombatManager::destroy();
    //delete cat;
}


