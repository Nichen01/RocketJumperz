// ---------------------------------------------------------------------------
// includes
#pragma once
#include <crtdbg.h> // To check for memory leaks
#include "AEEngine.h"
#include "collision.h"
#include "GameStateManager.h"
#include "GameStateList.h"
#include "render.h"

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
	bool pause = false;

	// Changing the window title
	AESysSetWindowTitle("Rocket Jumperz");

	// reset the system modules
	AESysReset();
	printf("Team project test\n");

	
	GSM_Initialize(GS_LEVEL1);

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
			if ((AESysDoesWindowExist() == false) || AEInputCheckTriggered(AEVK_ESCAPE))
				next = GS_QUIT;

			if (AEInputCheckTriggered(AEVK_TAB)) {
				if (pause) {
					pause = false;
				}
				else {
					pause = true;
				}
			}

			AESysFrameStart();
			if (pause == false) {
				fpUpdate();
			}
			fpDraw();
			AESysFrameEnd();

			g_dt = AEFrameRateControllerGetFrameTime();

			//hack
			g_dt = g_fixedDT;

			g_appTime += g_dt;
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