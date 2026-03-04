#pragma once
/* Start Header ************************************************************************/
/*!
\file		  MainMenu.h
\author       Nicholas Chen(c.chen)
\date         January, 30, 2026
\brief        Main menu with multiple screens, font rendering, and animations

Copyright (C) 2026 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#include "AEEngine.h"
#include <iostream>

extern int screenWidth;
extern int screenLength;

// Menu states
enum MenuState {
    MENU_MAIN,
    MENU_INSTRUCTIONS,
    MENU_CREDITS
};

// Button structure
struct MenuButton {
    f32 x;
    f32 y;
    f32 width;
    f32 height;
    f32 scale;           // For hover animation
    f32 targetScale;     // Target scale for smooth transitions
    const char* text;
    bool isHovered;
};

// Function declarations
void MainMenu_Load();
void MainMenu_Init();
void MainMenu_Update();
void MainMenu_Draw();
void MainMenu_Free();
void MainMenu_Unload();

// Helper functions
namespace MenuHelpers {
    bool isMouseOverButton(const MenuButton& button);
    void updateButtonHover(MenuButton& button);
    void drawButton(const MenuButton& button, AEGfxVertexList* mesh, s8 fontID);
    void drawTextCentered(const char* text, f32 x, f32 y, f32 scale, s8 fontID);
}