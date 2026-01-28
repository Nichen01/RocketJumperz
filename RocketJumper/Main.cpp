// ---------------------------------------------------------------------------
// includes
#pragma once
#include <crtdbg.h> // To check for memory leaks
#include "AEEngine.h"
#include "collision.h"
#include "Input.h"
#include "GameStateManager.h"
#include "GameStateList.h"
#include "render.h"
#include "drawWallsLevel1.h" 

// ---------------------------------------------------------------------------
// main
int screenWidth = 1600, screenLength = 900; // change main screen values here, include with extern int
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	if (AESysInit(hInstance, nCmdShow, screenWidth, screenLength, 1, 60, true, NULL) == 0)
	{
		return 0;
	}

	// Using custom window procedure
	int gGameRunning = 1;

	// Changing the window title
	AESysSetWindowTitle("Rocket Jumperz");

	// reset the system modules
	AESysReset();
	printf("Team project test\n");

	
	GSM_Initialize(GS_MAINMENU);

	while (current != GS_QUIT)
	{
		if (current != GS_RESTART) {
			GSM_Update();
			fpLoad();
		}
		else {
			current = previous;	
			next = previous;
		}

		fpInitialize();

		while (next == current)
		{
			AESysFrameStart();
			Input_Handle();
			fpUpdate();
			fpDraw();
			AESysFrameEnd();
		}

		fpFree();

		if (next != GS_RESTART) {
			fpUnload();
		}

		previous = current;
		current = next;
	}

	// free the system
	AESysExit();
}