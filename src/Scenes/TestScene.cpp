#include "TestScene.h"

//#include "../../Backend/GameManager.h" //? Still thinking
#include "SceneManager.h"
#include "Pch.h"
#include "utils.h"
#include "Event.h"

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
    RenderHelper::getInstance()->registerTexture(0, "./Assets/ame.png");
}

void TestScene::Init()
{
    //Event::getInstance()->setActiveEvent(EVENT_TYPES::OSCILLATING_TIMER);
}

void TestScene::Update(double dt)
{
    //EVENT_RESULTS results;
    //Event::getInstance()->updateRenderLoop(results, dt, EVENT_KEYS::E, EVENT_KEYS::SPACE);

    RenderHelper::getInstance()->texture(0);
}

void TestScene::Render()
{
    
}

void TestScene::Exit()
{
    std::cout << "Exiting TestScene" << std::endl;
    RenderHelper::getInstance()->removeTextureByRef(0);
}