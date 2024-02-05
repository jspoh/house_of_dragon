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
    Draw::getInstance()->registerTexture("planet", "./Assets/PlanetTexture.png");
}

void TestScene::Init()
{
    //GameObjectManager::AddEntity(GameObject
}

static int speed = 100;  // pixels per
static std::vector<Point> rects = { Point{0,0}, Point{100,100} };
static Controls ipts[] = { Controls{AEVK_W, AEVK_S, AEVK_A, AEVK_D}, Controls{AEVK_UP, AEVK_DOWN, AEVK_LEFT, AEVK_RIGHT} };
static int j{};

void TestScene::Update(double dt)
{
    int i{};
    for (Point& rect : rects) {
        if (AEInputCheckCurr(ipts[i].UP)) {
            rect.y -= dt * speed;
        }
        if (AEInputCheckCurr(ipts[i].DOWN)) {
            rect.y += dt * speed;
        }
        if (AEInputCheckCurr(ipts[i].LEFT)) {
            rect.x -= dt * speed;
        }
        if (AEInputCheckCurr(ipts[i].RIGHT)) {
            rect.x += dt * speed;
        }
        i++;
    }

    if (AEInputCheckCurr(AEVK_LBUTTON)) {
        s32 mx, my;
        AEInputGetCursorPosition(&mx, &my);
        std::cout << mx << ", " << my << "\n";
        std::cout << stow(mx,0).x << ", " << stow(0,my).y << "\n";
    }

    if (CollisionChecker::areRectsIntersecting(rects[0].x, rects[0].y, 50, 50, rects[1].x, rects[1].y, 50, 50)) {
        std::cout << j++ << "colliding\n";
    }
}

void TestScene::Render()
{
    for (Point& rect : rects) {
        Point rpt = stow(rect.x, rect.y);
        Draw::getInstance()->texture("planet", rpt.x, rpt.y);
    }
    
}

void TestScene::Exit()
{
    std::cout << "Exiting TestScene" << std::endl;
    Draw::getInstance()->removeTextureByRef("planet");
}