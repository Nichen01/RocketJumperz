#include "Confirmation.h"
#include "Draw.h"
#include "Main.h"


void Confirmation_Init(MenuButton &yesButton, MenuButton &noButton) {
    float buttonwidth = 390.0f;
    float buttonlength = 80.0f;
    yesButton = {-300.0f, 0.0f, buttonwidth, buttonlength, 1.0f, 1.0f, "YES", false };
    noButton = {300.0f, 0.0f, buttonwidth, buttonlength, 1.0f, 1.0f, "NO", false };
}
bool Confirmation_Update(MenuButton& yesButton, MenuButton& noButton) {
    MenuHelpers::updateButtonHover(yesButton);
    MenuHelpers::updateButtonHover(noButton);

    // Handle button clicks
    if (AEInputCheckTriggered(AEVK_LBUTTON)) {
        if (yesButton.isHovered) {
            return true;
            printf("Play button clicked - Starting game!\n");
        }
        if (noButton.isHovered) {
            return false;
            printf("Play button clicked - Starting game!\n");
        }
    }
}
void Confirmation_Draw(f32 font, MenuButton& yesButton, MenuButton& noButton) {
    f32 width, height;
    renderlogic::drawTexture(0.f, 0.f, menuTex, buttonMesh, 500.f, 640.f);

    AEGfxGetPrintSize(font, "YOU SURE?", 1.f, &width, &height);
    AEGfxPrint(font, "YOU SURE?", -width / 2, 0.60f - height / 2, 1, 1, 1, 1, 1);

    MenuHelpers::TexdrawButton(yesButton, buttonMesh, font, buttonTex);
    MenuHelpers::TexdrawButton(noButton, buttonMesh, font, buttonTex);

}
