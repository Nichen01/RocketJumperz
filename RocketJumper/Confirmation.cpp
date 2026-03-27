#include "Confirmation.h"
#include "Draw.h"
#include "Main.h"

static s8 confirmfont = -1;
static f32 width, height;

static MenuButton yesButton;
static MenuButton noButton;

void Confirmation_Load() {

    load::pauseMenu();
    confirmfont = AEGfxCreateFont("Assets/Fonts/gameover.ttf", 72);
}
void Confirmation_Init() {
    AssetManager::BuildSqrMesh(MESH_BUTTON);
    buttonMesh = AssetManager::GetMesh(MESH_BUTTON);

    float buttonwidth = 390.0f;
    float buttonlength = 80.0f;
    yesButton = {-300.0f, 0.0f, buttonwidth, buttonlength, 1.0f, 1.0f, "YES", false };
    noButton = {300.0f, -120.0f, buttonwidth, buttonlength, 1.0f, 1.0f, "NO", false };
}
void Confirmation_Update() {
    MenuHelpers::updateButtonHover(yesButton);
    MenuHelpers::updateButtonHover(noButton);

    // Handle button clicks
    if (AEInputCheckTriggered(AEVK_LBUTTON)) {
        if (yesButton.isHovered) {
            next = GS_TUTORIAL;  // Change to test file if needed
            printf("Play button clicked - Starting game!\n");
        }
        if (noButton.isHovered) {
            next = GS_MAINMENU;  // Change to test file if needed
            printf("Play button clicked - Starting game!\n");
        }
    }
}
void Confirmation_Draw() {

    renderlogic::drawTexture(0.f, 0.f, menuTex, buttonMesh, 500.f, 640.f);

    AEGfxGetPrintSize(confirmfont, "YOU SURE?", 1.f, &width, &height);
    AEGfxPrint(confirmfont, "YOU SURE?", -width / 2, 0.60f - height / 2, 1, 1, 1, 1, 1);

    MenuHelpers::TexdrawButton(yesButton, buttonMesh, confirmfont, buttonTex);
    MenuHelpers::TexdrawButton(noButton, buttonMesh, confirmfont, buttonTex);

}
void Confirmation_Free() {
    AssetManager::FreeAllMeshes();
}
void Confirmation_Unload() {
    AssetManager::UnloadAllTextures();
}
