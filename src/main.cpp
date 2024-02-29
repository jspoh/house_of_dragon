// ---------------------------------------------------------------------------
// includes
#include <crtdbg.h> // To check for memory leaks
#include <stdlib.h>
#include "GameManager.h"

// ---------------------------------------------------------------------------
// main

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
    //DID YOU CALL DESTROY FOR ALL SINGLETON CLASSES???
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	int gGameRunning = 1;

	// Using custom window procedure
	AESysInit(hInstance, nCmdShow, 1200, 650, 1, 60, true, NULL);

    //AESysSetFullScreen(true); //Dont do fullscreen
    // Changing the window title
    AESysSetWindowTitle("Version Alpha 0.1.0 - 3D Engine is RUNNING");
    
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
    
    	if (AEInputCheckTriggered(AEVK_0))
    		AESysSetFullScreen(1);
    	if (AEInputCheckTriggered(AEVK_9))
    		AESysSetFullScreen(0);
    }
    
    //Free & Unload Game System
    Game.Exit();
	// free the system
	AESysExit();
	
    //// check for memory leaks. i think our project nv config for checking of memory leaks in c++..
    //_CrtDumpMemoryLeaks();

    system("pause");
}