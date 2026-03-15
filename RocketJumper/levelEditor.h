#pragma once
#include "pch.h"
#include <vector>
#include "AssetManager.h"
#include "BinaryMap.h"
#include "Draw.h"
#include "GameStateManager.h"

struct TileAction {
	s32 row;
	s32 col;
	s32 prevValue;
	s32 newValue;
};

void LevelEditor_Load();

void LevelEditor_Initialize();

void LevelEditor_Update();

void LevelEditor_Draw();

void LevelEditor_Free();

void LevelEditor_Unload();
