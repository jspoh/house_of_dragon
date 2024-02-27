#include "CombatScene.h"

//#include "../../Backend/GameManager.h" //? Still thinking
#include "../../Scenes/SceneManager.h"
#include "../../Backend/Pch.h"
#include "../../utils/utils.h"
#include "../../Event/Event.h"
#include "../../utils/utils.h"
#include "../../GameObject/CombatObjects/CombatManager.h"
#include <vector>
#include <unordered_map>

Player* player;
Cat* cat;


/**
 * !TODO move these out.
 */

Mob::Mob(Element element, double health, double dmg) : health(health), dmg(dmg), element(element), maxHealth(health) {

}

Enemy::Enemy(Element element, double health, double dmg, std::string texturePath, std::string textureRef ,float screenX, float screenY, float size) 
    : Mob(element, health, dmg), _textureRef(textureRef), _size(size) {
    this->_spos.x = screenX;
    this->_spos.y = screenY;

    _spos = Point{ AEGfxGetWindowWidth() / 2.f, AEGfxGetWindowHeight() / 2.f };
    this->_wpos = stow(_spos.x, _spos.y);

    Draw::getInstance()->registerTexture(textureRef, texturePath);  // problematic code stopping execution

    Draw::getInstance()->texture(_textureRef, _wpos.x, _wpos.y, _size, _size);
}

void Enemy::render() {
    std::cout << Draw::getInstance()->getTextureByRef(this->_textureRef) << ", " << this->_textureRef << "\n";
    Draw::getInstance()->texture(_textureRef, _wpos.x, _wpos.y, _size, _size);
    Draw::getInstance()->text(std::to_string(this->health), this->_spos.x, this->_spos.y - _size / 3 * 2);
}

Enemy::~Enemy() {
    Draw::getInstance()->removeTextureByRef(this->_textureRef);
}

Cat::Cat(Element element, double health, double dmg, std::string texturePath, std::string textureRef, float screenX, float screenY, float size) : Enemy(element, health, dmg, texturePath, textureRef, screenX, screenY, size) {

}

Player::Player(double health, double dmg, Element element) : Mob(element, health, dmg) {

}

void Player::_drawHealth(float screenX, float screenY) {
    Draw::getInstance()->text(std::to_string(this->health), screenX, screenY);
}

void Player::render() {
    this->_drawHealth(150, 150);
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

    double damage = this->dmg * multiplier * qtMultiplier;
    target.health -= damage;
    return damage;
}

bool Mob::isDead() {
    return this->health <= 0;
}



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
        {"FIRE", "WATER", "METAL", "WOOD", "EARTH", "BACK"},  // attack elements
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
}


void CombatScene::Init()
{
    /*Event::getInstance()->setActiveEvent(EVENT_TYPES::SPAM_KEY);*/  // for testing only

    player = new Player();
    cat = new Cat(Element::Water, 100, 10, "./Assets/animals/cat.jpg", "cat", AEGfxGetWindowWidth() / 2, AEGfxGetWindowHeight() / 2, 200);  // rmb to clear memory!!
}

void CombatScene::Update(double dt)
{
    if (AEInputCheckTriggered(AEVK_3)) {
        Event::getInstance()->setActiveEvent(EVENT_TYPES::SPAM_KEY);
    }

    Draw::getInstance()->text("IM SO TIRED", AEGfxGetWindowWidth() / 2, AEGfxGetWindowHeight() / 2);

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
        Draw::getInstance()->text("Enemy is dead", AEGfxGetWindowWidth() / 2, AEGfxGetWindowHeight() / 2);
    }
    else if (player->isDead()) {
        Draw::getInstance()->text("Player is dead", AEGfxGetWindowWidth() / 2, AEGfxGetWindowHeight() / 2);
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


