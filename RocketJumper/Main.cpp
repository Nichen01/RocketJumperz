// ---------------------------------------------------------------------------
// includes
#pragma once
#include <crtdbg.h> // To check for memory leaks
#include "Collision.h"
#include "Main.h"
#include "Load.h"
#include "Sound.h"

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
	//int gGameRunning = 1;
	bool pause = false;
	s8 pausefont = -1; 
	//MenuButton playButton;
	//MenuButton quitButton;
	f32 width, height;

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
			if (current != GS_MAINMENU) {
				// Destroy previous pause font before creating a new one to
				// avoid leaking a font handle on every level transition.
				if (pausefont != -1) { AEGfxDestroyFont(pausefont); pausefont = -1; }
				pausefont = AEGfxCreateFont("Assets/Fonts/gameover.ttf", 72);
			}
		}
		else {
			current = previous;	
			next = previous;
		}

		fpInitialize();

		while (next == current)
		{
			AESysFrameStart();

			if (current != GS_MAINMENU) {
				if (AEInputCheckTriggered(AEVK_TAB)) {
					if (pause) {
						pause = false;
					}
					else {
						pause = true;
					}
				}

				if (!pause) {
					audio::audiolevel(1.0f);
					fpUpdate();
				}

				fpDraw();

				if (pause) {
					AEGfxGetPrintSize(pausefont, "PAUSE", 1.f, &width, &height);
					AEGfxPrint(pausefont, "PAUSE", -width / 2, height, 1, 1, 1, 1, 1);
					audio::audiolevel(0.2f);
				}
			}
			else {
				fpUpdate();
				fpDraw();
			}
			
			
			AESysFrameEnd();
			if (AESysDoesWindowExist() == false){
				next = GS_QUIT;
			}
			g_dt = AEFrameRateControllerGetFrameTime();

			//hack
			g_dt = g_fixedDT;

			g_appTime += g_dt;
		}

		fpFree();

		if (next != GS_RESTART) {
			fpUnload();
			previous = current;
		}

		current = next;
	}

	// free the system
	AEGfxDestroyFont(pausefont);
	AESysExit();

	return 0;
}