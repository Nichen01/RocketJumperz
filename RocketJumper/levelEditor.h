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
	f32 x, y; // position
	f32 width, height; // size
	f32 scale; // current scale
	f32 targetScale;
	const char* text; // button label
	bool isHovered; // hover state
};

void LevelEditor_Load();

void LevelEditor_Initialize();

void LevelEditor_Update();

void LevelEditor_Draw();

void LevelEditor_Free();

void LevelEditor_Unload();
