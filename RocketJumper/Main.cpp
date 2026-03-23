// ---------------------------------------------------------------------------
// includes
#pragma once
#include <crtdbg.h> // To check for memory leaks
#include "Collision.h"
#include "Main.h"
#include "Load.h"
#include "Sound.h"
#include "PauseMenu.h"

// ---------------------------------------------------------------------------
// main

bool pause = false;
bool canpause = true;
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



	// Changing the window title
	AESysSetWindowTitle("Rocket Jumperz");

	// reset the system modules
	AESysReset();
	printf("Team project test\n");

	GSM_Initialize(GS_SPLASH);

	while (current != GS_QUIT)
	{
		if (current == GS_SPLASH) {
			canpause = false;
		}
		if (current == GS_MAINMENU) {
			canpause = false;
		}
		if (current == GS_VICTORY) {
			canpause = false;
		}
		if (current == GS_DEATH) {
			canpause = false;
		}
		if (current == GS_LEVELEDITOR) {
			canpause = false;
		}

		if (current != GS_RESTART) {
			GSM_Update();
			fpLoad();
			if (canpause) {
				Pause_Load();			
			}
		}
		else {
			current = previous;	
			next = previous;
		}
		if (canpause) {
			Pause_Initialize();
		}
		
		fpInitialize();

		while (next == current)
		{
			AESysFrameStart();

			if (canpause) {
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
					Pause_Update();
					Pause_Draw();
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
		if (canpause) {
			Pause_Free();
		}
		fpFree();

		if (next != GS_RESTART) {
			if (canpause) {
				Pause_Unload();
			}
			fpUnload();
			previous = current;
		}
		canpause = true;
		current = next;
	}

	// free the system
	AESysExit();

	return 0;
}