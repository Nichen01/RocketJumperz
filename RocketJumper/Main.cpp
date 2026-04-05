/* Start Header ************************************************************************/
/*!
\file		  Main.cpp
\date         April, 04, 2026
\brief        Entry Point file 

Copyright (C) 2026 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

// ---------------------------------------------------------------------------
// includes
#pragma once
#include <crtdbg.h> // To check for memory leaks
#include "Collision.h"
#include "Main.h"
#include "Load.h"
#include "Sound.h"
#include "PauseMenu.h"
#include "Confirmation.h"

// ---------------------------------------------------------------------------
// main 

bool pause = false;
bool canpause = true;

int screenWidth = 1600, screenLength = 900; // change main screen values here, include with extern int
f32  screenscale = static_cast<f32>(screenWidth) / 1600.0f;
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

	if (AESysInit(hInstance, nCmdShow, screenWidth, screenLength,0, 60, true, NULL) == 0)
	{
		return 0;
	}


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
				if (AEInputCheckTriggered(AEVK_ESCAPE)) {
					if (pause) {
						pause = false;
					}
					else {
						pause = true;
					}
				}
			}
			
			if (canpause) {
				

				if (!pause) {
					fpUpdate();
				}

				fpDraw();

				if (pause) {
					Pause_Update();
					Pause_Draw();
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
			pause = false;
			previous = current;
		}
		canpause = true;
		current = next;
	}

	// free the system
	AESysExit();

	return 0;
}