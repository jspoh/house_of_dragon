/* Start Header ************************************************************************/
/*!
\file main.cpp
\author Poh Jing Seng, jingseng.poh, 2301363 (90%, debug config, structure)
\par jingseng.poh\@digipen.edu
\author Soh Wei Jie, weijie.soh, 2301289 (10%, game manager init)
\par weijie.soh\@digipen.edu
\date 20 Jan 2024
\brief entry point of program
/*
Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/


// ---------------------------------------------------------------------------
// includes
#include "Pch.h"
#include <crtdbg.h> // To check for memory leaks
#include <stdlib.h>
#include "GameManager.h"

// ---------------------------------------------------------------------------
// main

bool gGameRunning = true;
bool DEBUG = false;
NullStream nullstream;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
    //DID YOU CALL DESTROY FOR ALL SINGLETON CLASSES???
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);


	// Using custom window procedure
    // 
    // printing (cout, cerr) is only available when compiling in debug mode!
    // check pch.h for more details
//#ifndef NDEBUG
	AESysInit(hInstance, nCmdShow, 1200, 750, 1, 60, true, NULL);
//#else
//    AESysInit(hInstance, nCmdShow, 1200, 750, 0, 60, true, NULL);
//#endif

    // Changing the window title
    AESysSetWindowTitle("HOUSE OF DRAGON");

#ifndef NDEBUG
    DEBUG = true;
#endif
    
    //Show Cursor - Disable when have custom cursor
    AEInputShowCursor(1);
    
    //load & Initialise Game System
    GameManager& Game = GameManager::getInstance();
    Game.init();
    
    // Game System Loop
    while (gGameRunning)
    {
    	// Informing the system about the loop's start
    	AESysFrameStart();
    
    	AEGfxSetRenderMode(AE_GFX_RM_NONE);  // MUST BE CALLED AT LEAST ONCE PER FRAME
    	
    	//Update & Render Game System
    	Game.run();
    
    	// Informing the system about the loop's end
    	AESysFrameEnd();
    
    	// check if forcing the application to quit
    	if (0 == AESysDoesWindowExist())
    		gGameRunning = false; //Check with prof how to run game.exit when player forcebly closes the game
    
    	if (AEInputCheckTriggered(AEVK_0))
    		AESysSetFullScreen(1);
    	if (AEInputCheckTriggered(AEVK_9))
    		AESysSetFullScreen(0);
    }
    
    //Free & Unload Game System
    Game.exit();
	// free the system
	AESysExit();
}