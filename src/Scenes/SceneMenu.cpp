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

	myMenu.button[3] = AEGfxTextureLoad("Assets/buttons/play.png");
	myMenu.button[2] = AEGfxTextureLoad("Assets/buttons/credit.png");
	myMenu.button[1] = AEGfxTextureLoad("Assets/buttons/setting.png");
	myMenu.button[0] = AEGfxTextureLoad("Assets/buttons/exit.png");

}

void SceneMenu::Init()
{

	myMenu.buttonWidth = 600.0f; // Example initialization
	myMenu.buttonHeight = 80.0f; // Example initialization
	myMenu.transitionEnd = false; // Example initialization
	myMenu.transitionTimer = 0.0f; // Example initialization
	myMenu.transitionElapse = 0.0f; // Example initialization
	myMenu.nextLevel = 0; // Example initialization


}

void SceneMenu::Update(double dt)
{
	if (AEInputCheckCurr(AEVK_1))
		SceneManager::GetInstance()->SetActiveScene("SceneStage1");
	if (AEInputCheckCurr(AEVK_2))
		SceneManager::GetInstance()->SetActiveScene("CombatScene");

	AEInputGetCursorPosition(&myMenu.mouseX, &myMenu.mouseY);
	myMenu.mouseX -= myMenu.mouseX;
	myMenu.mouseY -= myMenu.mouseY;


		for (int i = 0; i < 4; ++i)
		{
			float buttonLeft = myMenu.buttonX[i];
			float buttonRight = myMenu.buttonX[i] + myMenu.buttonWidth;
			float buttonTop = myMenu.buttonY[i];
			float buttonBottom = myMenu.buttonY[i] + myMenu.buttonHeight;

			// Check if the mouse cursor is within the bounds of the button
			if (myMenu.mouseX >= buttonLeft && myMenu.mouseX <= buttonRight &&
				myMenu.mouseY >= buttonTop && myMenu.mouseY <= buttonBottom)
				myMenu.hovering[i] = true;
			if (AEInputCheckTriggered(AEVK_LBUTTON))
			{
			 
				{
					// Button i is clicked, perform appropriate action
					switch (i)
					{
					case 0:
						// Handle click for button 0 (play button)
						SceneManager::GetInstance()->SetActiveScene("SceneStage1");
						break;
					case 1:
						// Handle click for button 1 (credit button)
						// Add your code here
						break;
					case 2:
						// Handle click for button 2 (setting button)
						// Add your code here
						break;
					case 3:
						// Handle click for button 3 (exit button)
						// Add your code here
						break;
					default:
						break;
					}
				}
			}



		}

}

void SceneMenu::Render()
{

	AEGfxSetBackgroundColor(0.0f, 0.2f, 1.0f);
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 1.0f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	


	for (int i = 3; i >=0; --i)
	{
		AEMtx33 scale = { 0 };
		AEMtx33Scale(&scale, 180, 30);

		AEMtx33 transform;
		AEMtx33Trans(&transform, 0, i + 0.15);

		AEMtx33 model = { 0 };
		AEMtx33Concat(&model, &scale, &transform);

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
}