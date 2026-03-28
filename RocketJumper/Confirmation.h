#pragma once
#include "AEEngine.h"
#include "MainMenu.h"
#include "load.h"

void Confirmation_Init(MenuButton& yesButton, MenuButton& noButton);
bool Confirmation_Update(f32 buttontex, f32 backtex, f32 font);
void Confirmation_Draw();


