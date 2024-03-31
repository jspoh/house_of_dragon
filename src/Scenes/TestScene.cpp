#include "Pch.h"
#include "TestScene.h"

//#include "../../Backend/GameManager.h" //? Still thinking
#include "SceneManager.h"

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
    UNREFERENCED_PARAMETER(dt);
    RenderHelper::getInstance()->texture(0);
}

void TestScene::Render()
{
    
}

void TestScene::Exit()
{
    cout << "Exiting TestScene" << "\n";
    RenderHelper::getInstance()->removeTextureByRef(0);
}