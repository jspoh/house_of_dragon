#include "TestScene.h"

//#include "../../Backend/GameManager.h" //? Still thinking
#include "../../Scenes/SceneManager.h"
#include "../../Backend/Pch.h"
#include "../../utils/utils.h"
#include "../../Event/Event.h"

TestScene* TestScene::sInstance = new TestScene(SceneManager::GetInstance());

TestScene::TestScene()
{
}

TestScene::TestScene(SceneManager* _sceneMgr)
{
    _sceneMgr->AddScene("TestScene", this);
}

TestScene::~TestScene()
{
}

void TestScene::Load()
{

}

void TestScene::Init()
{
    Event::getInstance()->setActiveEvent(EVENT_TYPES::OSCILLATING_TIMER);
}

void TestScene::Update(double dt)
{
    EVENT_RESULTS results;
    Event::getInstance()->updateRenderLoop(results, dt, 600, 325);
}

void TestScene::Render()
{
    
}

void TestScene::Exit()
{
    std::cout << "Exiting TestScene" << std::endl;
}