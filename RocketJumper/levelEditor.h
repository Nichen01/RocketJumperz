#pragma once
#include "pch.h"
#include "BinaryMap.h"
#include <vector>
#include "Draw.h"

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
