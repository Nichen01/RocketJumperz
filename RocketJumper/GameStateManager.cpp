#include "pch.h"
#include "mapEditor.h"
#include "GameStateManager.h"

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
	case GS_RESTART:
		break;
	case GS_QUIT:
		break;
	default:
		break;
	}
}