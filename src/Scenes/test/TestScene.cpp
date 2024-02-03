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
    //Still debating whether need this
}

void TestScene::Init()
{
    //GameObjectManager::AddEntity(GameObject)
}

int speed = 1000;  // pixels per 
Point rectPos1 = { 0,0 };

void TestScene::Update(double dt)
{
    if (AEInputCheckCurr(AEVK_W)) {
        rectPos1.y -= dt * speed;
    }
    if (AEInputCheckCurr(AEVK_S)) {
        rectPos1.y += dt * speed;
    }
    if (AEInputCheckCurr(AEVK_A)) {
        rectPos1.x -= dt * speed;
    }
    if (AEInputCheckCurr(AEVK_D)) {
        rectPos1.x += dt * speed;
    }
}

void TestScene::Render()
{
    //std::cout << "rect screen pos: " << rectPos1.x << ", " << rectPos1.y << "\n";
    Point rpt1 = stow(rectPos1.x, rectPos1.y);
    Draw::getInstance()->rect(rpt1.x, rpt1.y);
    
}

void TestScene::Exit()
{
    std::cout << "Exiting Scene Base" << std::endl;
}