/* Start Header ************************************************************************/
/*!
\file		  Confirmation.cpp
\author 	  Ivan Chong (i.chong)
\date         April, 04, 2026
\brief        Contatins functions that create Confirmation screen

Copyright (C) 2026 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#include "Confirmation.h"
#include "Draw.h"
#include "Main.h"

void Confirmation_Init(MenuButton &yesButton, MenuButton &noButton) {
    float buttonWidth = AEGfxGetWindowWidth() * 0.25f;  // 25% of screen width
    float buttonHeight = AEGfxGetWindowHeight() * 0.1f;   // 10% of screen height
    yesButton = {0.0f, -110.0f * screenscale, buttonWidth, buttonHeight, 1.0f, 1.0f, "YES", false };
    noButton = {0.0f, -210.0f * screenscale, buttonWidth, buttonHeight, 1.0f, 1.0f, "NO", false };
}
void Confirmation_Update(MenuButton& yesButton, MenuButton& noButton,s8& leave) {
    MenuHelpers::updateButtonHover(yesButton);
    MenuHelpers::updateButtonHover(noButton);

    // Handle button clicks
    if (AEInputCheckTriggered(AEVK_LBUTTON)) {
        if (yesButton.isHovered) {
            leave = 1;
        }
        if (noButton.isHovered) {
            leave = 2;
        }
    }
}
void Confirmation_Draw(s8 font, MenuButton& yesButton, MenuButton& noButton) {
    // draw backgoround and buttons
    f32 width, height;
    // Scale the confirmation panel relative to screen size
    f32 scrW = static_cast<f32>(screenWidth);
    f32 scrH = static_cast<f32>(screenLength);
    f32 panelW = scrW * 0.3125f;  // ~500 / 1600
    f32 panelH = scrH * 0.711f;   // ~640 / 900
    renderlogic::drawTexture(0.f, 0.f, menuTex, buttonMesh, panelW, panelH);

    AEGfxGetPrintSize(font, "YOU SURE?", 1.f, &width, &height);
    AEGfxPrint(font, "YOU SURE?", -0.06f -width / 2, 0.55f - height / 2, 1, 1, 1, 1, 1);

    MenuHelpers::TexdrawButton(yesButton, buttonMesh, font, buttonTex);
    MenuHelpers::TexdrawButton(noButton, buttonMesh, font, buttonTex);
}

void fsDraw(s8 font, MenuButton& yesButton, MenuButton& noButton) {
    f32 width, height;

    AEGfxGetPrintSize(font, "Fullscreen Mode", 0.8f, &width, &height);
    renderlogic::drawTexture(0.f, 0.f, menuTex, buttonMesh, 470.f * screenscale, 600.f * screenscale);
    AEGfxPrint(font, "Fullscreen Mode", -0.26f, 0.5f, 0.8f, 1, 1, 1, 1);
    AEGfxPrint(font, "Our game is best experienced ", -0.25f, 0.3f, 0.55f, 1, 1, 1, 1);
    AEGfxPrint(font, "in fullscreen.", -0.13f, 0.2f, 0.55f, 1, 1, 1, 1);
    AEGfxPrint(font, "Would you like to try that?", -0.25f, 0.f, 0.6f, 1, 1, 1, 1);

    MenuHelpers::TexdrawButton(yesButton, buttonMesh, font, buttonTex);
    MenuHelpers::TexdrawButton(noButton, buttonMesh, font, buttonTex);

}
