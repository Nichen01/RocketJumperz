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
    // set button data
    float buttonwidth = 390.0f;
    float buttonlength = 80.0f;
    yesButton = {0.0f, 50.0f, buttonwidth, buttonlength, 1.0f, 1.0f, "YES", false };
    noButton = {0.0f, -50.0f, buttonwidth, buttonlength, 1.0f, 1.0f, "NO", false };
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

    AEGfxGetPrintSize(font, "YOU SURE?", 1.f, &width, &height);
    AEGfxPrint(font, "YOU SURE?", -width / 2, 0.40f - height / 2, 1, 1, 1, 1, 1);

    MenuHelpers::TexdrawButton(yesButton, buttonMesh, font, buttonTex);
    MenuHelpers::TexdrawButton(noButton, buttonMesh, font, buttonTex);

}
