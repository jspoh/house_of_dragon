#include "SceneMenu.h"

SceneMenu* SceneMenu::sInstance = new SceneMenu(SceneManager::GetInstance());

SceneMenu::SceneMenu()
{
}

SceneMenu::SceneMenu(SceneManager* _sceneMgr)
{
	_sceneMgr->AddScene("SceneMenu", this);
}

SceneMenu::~SceneMenu()
{
}

void SceneMenu::Load()
{
	AEGfxMeshStart();
	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFFFF00FF, 0.0f, 1.0f,
		0.5f, -0.5f, 0xFFFFFF00, 1.0f, 1.0f,
		-0.5f, 0.5f, 0xFF00FFFF, 0.0f, 0.0f);
	AEGfxTriAdd(
		0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f,
		0.5f, 0.5f, 0xFFFFFFFF, 1.0f, 0.0f,
		-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f);

	myMenu.mesh = AEGfxMeshEnd();

	myMenu.bg = AEGfxTextureLoad("Assets/Menu/bg.jpg");

	myMenu.button[0] = AEGfxTextureLoad("Assets/Menu/buttons/play.png");
	myMenu.button[1] = AEGfxTextureLoad("Assets/Menu/buttons/credit.png");
	myMenu.button[2] = AEGfxTextureLoad("Assets/Menu/buttons/setting.png");
	myMenu.button[3] = AEGfxTextureLoad("Assets/Menu/buttons/exit.png");

}

void SceneMenu::Init()
{

	myMenu.buttonWidth = 200.0f; // Example initialization
	myMenu.buttonHeight = 40.0f; // Example initialization
	myMenu.transitionEnd = false; // Example initialization
	myMenu.transitionTimer = 0.0f; // Example initialization
	myMenu.transitionElapse = 0.0f; // Example initialization
	myMenu.nextLevel = 0; // Example initialization
	for (int i = 0; i < 4; ++i)
	{
		myMenu.buttonX[i] = 0;
		myMenu.buttonY[i] = -i * (myMenu.buttonHeight + 10) + 200;
	}

}

void SceneMenu::Update(double dt)
{
	if (AEInputCheckCurr(AEVK_1))
		SceneManager::GetInstance()->SetActiveScene("SceneStage1");
	if (AEInputCheckTriggered(AEVK_LBUTTON))
	{
		s32 mxx, myy;
		AEInputGetCursorPosition(&mxx, &myy);
		float mx = mxx;
		float my = myy;
		mx -= 1200 / 2;

		my = -my;
		my += 650.0f / 2.0f;

		//std::cout << "X: " << mx << "    Y: " << my << std::endl;
		AEVec2 p1 = { myMenu.buttonX[0] , myMenu.buttonY[0] };
		//AEVec2 p2 = { myMenu.buttonX[3] + myMenu.buttonWidth, myMenu.buttonY[3] + myMenu.buttonHeight };

		for (int i = 0; i < 4; ++i)
		{
			AEVec2 p1 = { myMenu.buttonX[i] - myMenu.buttonWidth / 2.f, myMenu.buttonY[i] + myMenu.buttonHeight / 2.f };
			AEVec2 p2 = { myMenu.buttonX[i] + myMenu.buttonWidth / 2.f, myMenu.buttonY[i] - myMenu.buttonHeight / 2.f };
			if (p1.x<mx && p1.y>my && p2.x > mx && p2.y < my)
			{
				switch (i)
				{
				case 0:
					SceneManager::GetInstance()->SetActiveScene("CombatScene");
					break;
				case 1:
					SceneManager::GetInstance()->SetActiveScene("SceneCredits");
					break;
				}
			}


		}

	}
	return;



	//if (AEInputCheckCurr(AEVK_1))
	//	SceneManager::GetInstance()->SetActiveScene("SceneStage1");
	//if (AEInputCheckCurr(AEVK_2))
	//	SceneManager::GetInstance()->SetActiveScene("CombatScene");

	//AEInputGetCursorPosition(&myMenu.mouseX, &myMenu.mouseY);
	////myMenu.mouseX -= myMenu.mouseX;
	//myMenu.mouseY -= myMenu.mouseY;


	//	for (int i = 0; i < 4; ++i)
	//	{
	//		float buttonLeft = myMenu.buttonX[i];
	//		float buttonRight = myMenu.buttonX[i] + myMenu.buttonWidth;
	//		float buttonTop = myMenu.buttonY[i];
	//		float buttonBottom = myMenu.buttonY[i] + myMenu.buttonHeight;

	//		// Check if the mouse cursor is within the bounds of the button
	//		if (myMenu.mouseX >= buttonLeft && myMenu.mouseX <= buttonRight &&
	//			myMenu.mouseY >= buttonTop && myMenu.mouseY <= buttonBottom)
	//			myMenu.hovering[i] = true;
	//

	//			if (AEInputCheckTriggered(AEVK_LBUTTON))
	//			{

	//				{
	//					// Button i is clicked, perform appropriate action
	//					switch (i)
	//					{
	//					case 0:
	//						// Handle click for button 0 (play button)
	//						SceneManager::GetInstance()->SetActiveScene("CombatScene");
	//						break;
	//					case 1:
	//						// Handle click for button 1 (credit button)
	//						// Add your code here
	//						SceneManager::GetInstance()->SetActiveScene("SceneCredits");
	//						break;
	//					case 2:
	//						// Handle click for button 2 (setting button)
	//						// Add your code here
	//						break;
	//					case 3:
	//				
	//						break;
	//					default:
	//						break;
	//					}
	//				}
	//			}
	//		


	//	}

}

void SceneMenu::Render()
{


	AEGfxSetBackgroundColor(0.0f, 0.2f, 1.0f);
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 1.0f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);


	AEMtx33 scale = { 0 };
	AEMtx33Scale(&scale, 5670, 3780);

	AEMtx33 transform;
	AEMtx33Trans(&transform, 0, 0);

	AEMtx33 model = { 0 };
	AEMtx33Concat(&model, &scale, &transform);

	// prepare to draw
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetColorToAdd(0, 0, 0, 1);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(1);
	AEGfxSetTransform(model.m);
	AEGfxTextureSet(myMenu.bg, 0, 0);
	AEGfxMeshDraw(myMenu.mesh, AE_GFX_MDM_TRIANGLES);




	for (int i = 3; i >= 0; --i)
	{
		AEMtx33 scale = { 0 };
		AEMtx33Scale(&scale, myMenu.buttonWidth, myMenu.buttonHeight);

		AEMtx33 transform;
		AEMtx33Trans(&transform, myMenu.buttonX[i], myMenu.buttonY[i]);

		AEMtx33 model = { 0 };
		//AEMtx33Concat(&model, &scale, &transform);
		AEMtx33Concat(&model, &transform, &scale);
		// prepare to draw
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
		AEGfxSetColorToAdd(0, 0, 0, 1);
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		AEGfxSetTransparency(1);
		AEGfxSetTransform(model.m);
		AEGfxTextureSet(myMenu.button[i], 0, 0);
		AEGfxMeshDraw(myMenu.mesh, AE_GFX_MDM_TRIANGLES);

	}

}

void SceneMenu::Exit()
{
	std::cout << "Exiting Scene Menu" << std::endl;
	for (int i = 0; i < 4; i++)
	{
		AEGfxTextureUnload(myMenu.button[i]);
	}
	AEGfxTextureUnload(myMenu.bg);
	AEGfxMeshFree(myMenu.mesh);
}