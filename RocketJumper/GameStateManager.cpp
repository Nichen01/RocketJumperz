#include "GameStateManager.h"

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
	case GS_LEVEL2:
		fpLoad = Level2_Load;
		fpInitialize = Level2_Initialize;
		fpUpdate = Level2_Update;
		fpDraw = Level2_Draw;
		fpFree = Level2_Free;
		fpUnload = Level2_Unload;
		break;
	case GS_LEVEL3:
		fpLoad = Level3_Load;
		fpInitialize = Level3_Initialize;
		fpUpdate = Level3_Update;
		fpDraw = Level3_Draw;
		fpFree = Level3_Free;
		fpUnload = Level3_Unload;
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
	case GS_LEVELEDITOR:
		fpLoad = LevelEditor_Load;
		fpInitialize = LevelEditor_Initialize;
		fpUpdate = LevelEditor_Update;
		fpDraw = LevelEditor_Draw;
		fpFree = LevelEditor_Free;
		fpUnload = LevelEditor_Unload;
		break;		
	case GS_VICTORY:
		fpLoad = VictoryScreen_Load;
		fpInitialize = VictoryScreen_Init;
		fpUpdate = VictoryScreen_Update;
		fpDraw = VictoryScreen_Draw;
		fpFree = VictoryScreen_Free;
		fpUnload = VictoryScreen_Unload;
		break;
	case GS_DEATH:
		fpLoad = DeathScreen_Load;
		fpInitialize = DeathScreen_Init;
		fpUpdate = DeathScreen_Update;
		fpDraw = DeathScreen_Draw;
		fpFree = DeathScreen_Free;
		fpUnload = DeathScreen_Unload;
		break;
	case GS_TUTORIAL:
		fpLoad = Tutorial_Load;
		fpInitialize = Tutorial_Initialize;
		fpUpdate = Tutorial_Update;
		fpDraw = Tutorial_Draw;
		fpFree = Tutorial_Free;
		fpUnload = Tutorial_Unload;
		break;
	case GS_RESTART:
		break;
	case GS_QUIT:
		break;
	default: break;
	}
}