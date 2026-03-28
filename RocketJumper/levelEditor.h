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
