/* Start Header ************************************************************************/
/*!
\file		  Confirmation.cpp
\date         April, 04, 2026
\brief        Contatins functions that create confirmation screen

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
    float buttonwidth = 350.0f;
    float buttonlength = 70.0f;
    yesButton = {0.0f, -110.0f, buttonwidth, buttonlength, 1.0f, 1.0f, "YES", false };
    noButton = {0.0f, -210.0f, buttonwidth, buttonlength, 1.0f, 1.0f, "NO", false };
    // Scale button dimensions relative to screen size so they look correct
    // in both windowed and fullscreen modes.
    f32 scrW = static_cast<f32>(screenWidth);
    f32 scrH = static_cast<f32>(screenLength);
    float buttonwidth  = scrW * 0.244f;   // ~390 / 1600
    float buttonheight = scrH * 0.089f;   // ~80  / 900
    float btnGap       = scrH * 0.056f;   // ~50  / 900 (vertical offset from center)
    yesButton = {0.0f,  btnGap, buttonwidth, buttonheight, 1.0f, 1.0f, "YES", false };
    noButton  = {0.0f, -btnGap, buttonwidth, buttonheight, 1.0f, 1.0f, "NO",  false };
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
    renderlogic::drawTexture(0.f, 0.f, menuTex, buttonMesh, 500.f, 640.f);
    // Scale the confirmation panel relative to screen size
    f32 scrW = static_cast<f32>(screenWidth);
    f32 scrH = static_cast<f32>(screenLength);
    f32 panelW = scrW * 0.3125f;  // ~500 / 1600
    f32 panelH = scrH * 0.711f;   // ~640 / 900
    renderlogic::drawTexture(0.f, 0.f, menuTex, buttonMesh, panelW, panelH);

    AEGfxGetPrintSize(font, "YOU SURE?", 1.f, &width, &height);
    AEGfxPrint(font, "YOU SURE?", -0.06f -width / 2, 0.55f - height / 2, 1, 1, 1, 1, 1);

    AEGfxGetPrintSize(font, "YOU SURE?", 0.8f, &width, &height);
    AEGfxPrint(font, "YOU SURE?", -0.26f, 0.51f, 0.8f, 1, 1, 1, 1);
    MenuHelpers::TexdrawButton(yesButton, buttonMesh, font, buttonTex);
    MenuHelpers::TexdrawButton(noButton, buttonMesh, font, buttonTex);
}

void fsDraw(s8 font, MenuButton& yesButton, MenuButton& noButton) {
    f32 width, height;

    AEGfxGetPrintSize(font, "Fullscreen Mode", 0.8f, &width, &height);
    renderlogic::drawTexture(0.f, 0.f, menuTex, buttonMesh, 470.f, 600.f);
    AEGfxPrint(font, "Fullscreen Mode", -0.26f, 0.5f, 0.8f, 1, 1, 1, 1);
    AEGfxPrint(font, "Our game is best experienced ", -0.25f, 0.3f, 0.55f, 1, 1, 1, 1);
    AEGfxPrint(font, "with fullscreen.", -0.13f, 0.2f, 0.55f, 1, 1, 1, 1);
    AEGfxPrint(font, "Would you like to try that?", -0.25f, 0.f, 0.6f, 1, 1, 1, 1);
    f32 scrW = static_cast<f32>(screenWidth);
    f32 scrH = static_cast<f32>(screenLength);
    AEGfxGetPrintSize(font, "RocketJumper is best experienced in", 1.f, &width, &height);
    // Scale the panel to 60% of screen width and ~71% of screen height
    f32 panelW = scrW * 0.6f;
    f32 panelH = scrH * 0.711f;  // ~640 / 900
    renderlogic::drawTexture(0.f, 0.f, menuTex, buttonMesh, panelW, panelH);
    AEGfxPrint(font, "RocketJumper is best experienced in ", -width / 2, 0.40f - height / 2, 1, 1, 1, 1, 1);
    AEGfxPrint(font, "fullscreen. Go into fullscreen mode?", -width / 2, 0.40f - 3*height / 2, 1, 1, 1, 1, 1);

    MenuHelpers::TexdrawButton(yesButton, buttonMesh, font, buttonTex);
    MenuHelpers::TexdrawButton(noButton, buttonMesh, font, buttonTex);

}
