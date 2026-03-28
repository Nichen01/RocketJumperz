#pragma once
#include "AEEngine.h"
#include "MainMenu.h"
#include "load.h"

void Confirmation_Init(MenuButton& yesButton, MenuButton& noButton);
void Confirmation_Update(MenuButton& yesButton, MenuButton& noButton, bool& leave);
void Confirmation_Draw(s8 font, MenuButton& yesButton, MenuButton& noButton);


