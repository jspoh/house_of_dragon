#include "CombatScene.h"

//#include "../../Backend/GameManager.h" //? Still thinking
#include "../../Scenes/SceneManager.h"
#include "../../Backend/Pch.h"
#include "../../utils/utils.h"
#include "../../Event/Event.h"
#include "../../utils/utils.h"
#include <vector>

CombatScene* CombatScene::sInstance = new CombatScene(SceneManager::GetInstance());


namespace {
    EVENT_RESULTS combatEventResult = EVENT_RESULTS::NONE_EVENT_RESULTS;
    std::vector<std::string> btns = {
        "ATTACK",
        "ITEMS",
        "FLEE"
    };
    int padding = 100;
    int spacing = 50;
    int btnWidth;
    int btnHeight;

    int btnY = 800;
    int lBtnX;
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
    btnWidth = (AEGfxGetWindowWidth() - (padding * 2) - (btns.size() - 1) * spacing) / btns.size();
    btnHeight = btnWidth / 3;
    lBtnX = padding + btnWidth / 2;
    std::cout << btnWidth << "\n";
}

void CombatScene::Update(double dt)
{
    if (AEInputCheckTriggered(AEVK_3)) {
        Event::getInstance()->setActiveEvent(EVENT_TYPES::SPAM_KEY);
    }

    int bPosX = lBtnX;
    for (int i{}; i < btns.size(); i++) {
        Point btnPos = stow(bPosX, btnY);
        Draw::getInstance()->rect(btnPos.x, btnPos.y, btnWidth, btnHeight, 0, Color{0.3, 0.3, 0.3, 1});
        Draw::getInstance()->text(btns[i], bPosX, btnY);
        bPosX += btnWidth + spacing;
    }


    //Draw::getInstance()->text("IM SO TIRED", AEGfxGetWindowWidth() / 2, AEGfxGetWindowHeight() / 2);

    Point p = stow(100, 100);
    Event::getInstance()->updateLoop(combatEventResult, dt, p.x, p.y);
}

void CombatScene::Render()
{
    
}

void CombatScene::Exit()
{
    std::cout << "Exiting CombatScene\n";
    //Draw::getInstance()->removeTextureByRef("planet");
}