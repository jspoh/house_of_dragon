#include "CombatScene.h"

//#include "../../Backend/GameManager.h" //? Still thinking
#include "../../Scenes/SceneManager.h"
#include "../../Backend/Pch.h"
#include "../../utils/utils.h"
#include "../../Event/Event.h"

CombatScene* CombatScene::sInstance = new CombatScene(SceneManager::GetInstance());

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
    Event::getInstance()->setActiveEvent(EVENT_TYPES::SPAM_KEY);
}

EVENT_RESULTS eresult = EVENT_RESULTS::NONE_EVENT_RESULTS;
void CombatScene::Update(double dt)
{
    Event::getInstance()->updateLoop(eresult, dt, 100, 100);
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