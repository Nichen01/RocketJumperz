#include "pch.h"
#include "mapEditor_Level1.h"
#include "mapEditor_Level2.h"
#include "GameStateManager.h"
#include "MainMenu.h"
#include "projectileTest.h"

// Checking where to spawn character
bool firstTimeLevel1 = true;
bool firstTimeLevel2 = true;

// Tracks current, previous and next game states
int current = 0, previous = 0, next = 0;

// Function pointers for state-specific load, initialize, update, draw, free and unload
FP fpLoad = nullptr, fpInitialize = nullptr, fpUpdate = nullptr, fpDraw = nullptr, fpFree = nullptr, fpUnload = nullptr;

// Initialize the Game State Manager with a starting state
void GSM_Initialize(int startingState)
{
	current = previous = next = startingState;
}

// Updates the Game State Manager and assigns function pointers based on current state
void GSM_Update()
{
	switch (current)
	{
	case GS_LEVEL1:
		fpLoad = Level1_Load;
		fpInitialize = Level1_Initialize;
		fpUpdate = Level1_Update;
		fpDraw = Level1_Draw;
		fpFree = Level1_Free;
		fpUnload = Level1_Unload;
		break;
	case GS_PROJECTILE_TEST:
		fpLoad = ProjectileTest_Load;
		fpInitialize = ProjectileTest_Initialize;
		fpUpdate = ProjectileTest_Update;
		fpDraw = ProjectileTest_Draw;
		fpFree = ProjectileTest_Free;
		fpUnload = ProjectileTest_Unload;
		break;
	case GS_MAINMENU:
		fpLoad = MainMenu_Load;
		fpInitialize = MainMenu_Init;
		fpUpdate = MainMenu_Update;
		fpDraw = MainMenu_Draw;
		fpFree = MainMenu_Free;
		fpUnload = MainMenu_Unload;
		break;
	
	case GS_LEVEL2:
		fpLoad = Level2_Load;
		fpInitialize = Level2_Initialize;
		fpUpdate = Level2_Update;
		fpDraw = Level2_Draw;
		fpFree = Level2_Free;
		fpUnload = Level2_Unload;
		break;
	
	case GS_RESTART:break;
	case GS_QUIT:break;
	default: break;
	}
}