#include "TestScene.h"

//#include "../../Backend/GameManager.h" //? Still thinking
#include "../../Scenes/SceneManager.h"
#include "../../Backend/Pch.h"
#include "../../utils/utils.h"

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
    
}

void TestScene::Update(double dt)
{

}

void TestScene::Render()
{
    
}

void TestScene::Exit()
{
    std::cout << "Exiting TestScene" << std::endl;
}