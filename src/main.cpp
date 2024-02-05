// ---------------------------------------------------------------------------
// includes
#include <crtdbg.h> // To check for memory leaks
#include "Backend/GameManager.h"
// ---------------------------------------------------------------------------
// main

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	int gGameRunning = 1;

	// Using custom window procedure
	AESysInit(hInstance, nCmdShow, 1200, 650, 1, 60, true, NULL);

	AESysSetFullScreen(true); //Dont do fullscreen
	// Changing the window title
	AESysSetWindowTitle("Version Alpha 0.0.1 - White Screen");

	//Show Cursor - Disable when have custom cursor
	AEInputShowCursor(1);

	//Load & Initialise Game System
	GameManager& Game = GameManager::GetInstance();
	Game.Init();

	// Game System Loop
	while (gGameRunning)
	{
		//std::cout << AEGfxGetWindowWidth() << " " << AEGfxGetWindowHeight() << "\n";

		// Informing the system about the loop's start
		AESysFrameStart();

		AEGfxSetRenderMode(AE_GFX_RM_NONE);  // MUST BE CALLED AT LEAST ONCE PER FRAME
		
		//Update & Render Game System
		Game.Run();

		// Informing the system about the loop's end
		AESysFrameEnd();
	
		// check if forcing the application to quit
		if (AEInputCheckTriggered(AEVK_ESCAPE) || 0 == AESysDoesWindowExist())
			gGameRunning = 0;
	
		if (AEInputCheckTriggered(AEVK_1))
			AESysSetFullScreen(1);
		if (AEInputCheckTriggered(AEVK_2))
			AESysSetFullScreen(0);
	}

	//Free & Unload Game System
	Game.Exit();

	// free the system
	AESysExit();
	
}