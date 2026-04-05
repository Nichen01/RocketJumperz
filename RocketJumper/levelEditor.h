/* Start Header ************************************************************************/
/*!
\file		  LevelEditor.h
\author 	  Chan Joraye (c.joraye)
\date         April, 01, 2026
\brief        Header file for the Level Editor system. 
			  Defines core data structures for editor actions and UI buttons, 
			  and declares lifecycle functions (load, initialize, update, draw, free, unload) 
			  used to manage the Level Editor within the game.
			  
			  Copyright (C) 2026 DigiPen Institute of Technology.
			  Reproduction or disclosure of this file or its contents
			  without the prior written consent of DigiPen Institute of
			  Technology is prohibited.

/* End Header **************************************************************************/

#pragma once
#include "pch.h"
#include <vector>
#include "AssetManager.h"
#include "BinaryMap.h"
#include "Draw.h"
#include "GameStateManager.h"

extern int level;

struct TileAction {
	s32 row;
	s32 col;
	s32 prevValue;
	s32 newValue;
};

struct doorButton {
	f32 x;
	f32 y;
	f32 width;
	f32 height;
	f32 scale;
	f32 targetScale;
	const char* text;
	bool isHovered;
	int id;
};

struct errorPromptButton {
	f32 x = 0.f, y = -100.f; // position
	f32 width = 220.f, height = 80.f; // size
	f32 scale = 1.0f; // current scale
	f32 targetScale;
	const char* text = "CLOSE"; // button label
	bool isHovered = false; // hover state
	int id;
};

struct resetButton {
	f32 x = 620.f, y = -380.f;
	f32 width = 313.5f, height = 88.f;
	f32 scale = 1.0f;
	f32 targetScale;
	const char* text = "RESET LEVEL";
	bool isHovered = false;
};

void LevelEditor_Load();

void LevelEditor_Initialize();

void LevelEditor_Update();

void LevelEditor_Draw();

void LevelEditor_Free();

void LevelEditor_Unload();
