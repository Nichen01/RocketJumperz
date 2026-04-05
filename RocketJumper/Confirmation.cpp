#include "Confirmation.h"
#include "Draw.h"
#include "Main.h"

void Confirmation_Init(MenuButton &yesButton, MenuButton &noButton) {
    float buttonwidth = 350.0f;
    float buttonlength = 70.0f;
    yesButton = {0.0f, -110.0f, buttonwidth, buttonlength, 1.0f, 1.0f, "YES", false };
    noButton = {0.0f, -210.0f, buttonwidth, buttonlength, 1.0f, 1.0f, "NO", false };
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
    f32 width, height;
    renderlogic::drawTexture(0.f, 0.f, menuTex, buttonMesh, 500.f, 640.f);

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

    MenuHelpers::TexdrawButton(yesButton, buttonMesh, font, buttonTex);
    MenuHelpers::TexdrawButton(noButton, buttonMesh, font, buttonTex);

}
