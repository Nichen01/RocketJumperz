#pragma once
#include "pch.h"
#include "binaryMap.h"
#include <vector>

struct TileAction {
	s32 row;
	s32 col;
	s32 prevValue;
	s32 newValue;
};

void levelEditor_Load();

void levelEditor_Initialize();

void levelEditor_Update();

void levelEditor_Draw();

void levelEditor_Free();

void levelEditor_Unload();
