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

	myMenu.bg = AEGfxTextureLoad("Assets/bg.jpg");
	myMenu.pointer = AEGfxTextureLoad("Assets/buttons/dagger.png");
	myMenu.button[0] = AEGfxTextureLoad("Assets/buttons/olay.png");
	myMenu.button[1] = AEGfxTextureLoad("Assets/buttons/xredit.png");
	myMenu.button[2] = AEGfxTextureLoad("Assets/buttons/aetting.png");
	myMenu.button[3] = AEGfxTextureLoad("Assets/buttons/quit.png");

}

void SceneMenu::Init()
{

	myMenu.buttonWidth = 400.0f; // Example initialization
	myMenu.buttonHeight = 120.0f; // Example initialization
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



	//for (int i = 3; i >= 0; --i)
	//{

	//	if (myMenu.levelActived[i] == 1) {
	//		myMenu.hovering[i] = true;
	//		static float counter = 0;
	//if (counter >= 2.0f)
	//	counter -= 2.0f;
	//else
	//	counter += dt;

	//   AEVec2 xx ;
 //  AEVec2Lerp(xx, 0.0f, 360.0f, counter);
 //  ///// need do lerp for the transition 



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
							SceneManager::GetInstance()->SetActiveScene("SceneStage1");
							break;
						case 1:
							SceneManager::GetInstance()->SetActiveScene("SceneCredit");
						case 2:
							SceneManager::GetInstance()->SetActiveScene("");
							break;
						case 3:
							SceneManager::GetInstance()->SetActiveScene("");

							break;
						}
					}


				}

			}

			//else myMenu.hovering[i] = false;

		//}
	//}
	return;

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
		AEMtx33Concat(&model, &transform, &scale);
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
	AEGfxMeshFree(myMenu.mesh);
}