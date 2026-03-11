#pragma once
#include "AEEngine.h"

extern s8 pausefont = -1;
//MenuButton playButton;
//MenuButton quitButton;
extern f32 width, height;

void Pause_Load();

void Pause_Initialize();

void Pause_Update();

void Pause_Draw();

void Pause_Free();

void Pause_Unload();
