#include "CombatScene.h"

//#include "../../Backend/GameManager.h" //? Still thinking
#include "../../Scenes/SceneManager.h"
#include "../../Backend/Pch.h"
#include "../../utils/utils.h"
#include "../../Event/Event.h"
#include "../../utils/utils.h"

CombatScene* CombatScene::sInstance = new CombatScene(SceneManager::GetInstance());
EVENT_RESULTS combatEventResult = EVENT_RESULTS::NONE_EVENT_RESULTS;

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
}

void CombatScene::Update(double dt)
{
    if (AEInputCheckTriggered(AEVK_3)) {
        Event::getInstance()->setActiveEvent(EVENT_TYPES::SPAM_KEY);
    }

    Point p = stow(100, 100);
    Event::getInstance()->updateLoop(combatEventResult, dt, p.x, p.y);
    Draw::getInstance()->text("IM SO TIRED", AEGfxGetWindowWidth() / 2, AEGfxGetWindowHeight() / 2);
    //f64 time;
    //AEGetTime(&time);
    //std::cout << dt << "\n";
}

void CombatScene::Render()
{
    
}

void CombatScene::Exit()
{
    std::cout << "Exiting CombatScene\n";
    //Draw::getInstance()->removeTextureByRef("planet");
}