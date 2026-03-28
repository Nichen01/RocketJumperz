#include "DeathScreen.h"
#include "Draw.h"
#include "movement.h"
static s8 deathfont = -1;
static f32 width, height;

static AEGfxTexture* backgroundTexture = nullptr;

static MenuButton restartButton;
static MenuButton exitButton;
static MenuButton tomenuButton;

static AEGfxVertexList* backgroundMesh = nullptr;

static AEGfxTexture* menutex;
static AEGfxTexture* buttontex;

void DeathScreen_Load() {
    AEGfxMeshStart();

    AEGfxTriAdd(
        -0.5f, -0.5f, 0xFF000000, 0.0f, 1.0f,
        0.5f, -0.5f, 0xFF000000, 1.0f, 1.0f,
        -0.5f, 0.5f, 0xFF000000, 0.0f, 0.0f);

    AEGfxTriAdd(
        0.5f, -0.5f, 0xFF000000, 1.0f, 1.0f,
        0.5f, 0.5f, 0xFF000000, 1.0f, 0.0f,
        -0.5f, 0.5f, 0xFF000000, 0.0f, 0.0f);

    buttonMesh = AEGfxMeshEnd();

    AEGfxMeshStart();

    AEGfxTriAdd(
        -0.5f, -0.5f, 0xFFFFFFFF, 0.0f, 1.0f,
        0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f,
        -0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f);

    AEGfxTriAdd(
        0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f,
        0.5f, 0.5f, 0xFFFFFFFF, 1.0f, 0.0f,
        -0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f);

    backgroundMesh = AEGfxMeshEnd();

    menutex = AEGfxTextureLoad("Assets/UI/Menus/TitleFrame.png");
    buttontex = AEGfxTextureLoad("Assets/UI/Menus/button.png");
    backgroundTexture = AEGfxTextureLoad("Assets/MainMenu.png");

    deathfont = AEGfxCreateFont("Assets/Fonts/gameover.ttf", 72);
}
void DeathScreen_Init() {
    float buttonwidth = 390.0f;
    float buttonlength = 80.0f;
    restartButton = { 0.0f, 0.0f, buttonwidth, buttonlength, 1.0f, 1.0f, "RESTART", false };
    tomenuButton = { 0.0f, -120.0f, buttonwidth, buttonlength, 1.0f, 1.0f, "MAIN MENU", false };
    exitButton = { 0.0f, -240.0f, buttonwidth, buttonlength, 1.0f, 1.0f, "EXIT", false };
}
void DeathScreen_Update() {
    MenuHelpers::updateButtonHover(restartButton);
    MenuHelpers::updateButtonHover(tomenuButton);
    MenuHelpers::updateButtonHover(exitButton);

    if (AEInputCheckTriggered(AEVK_LBUTTON)) {
        if (restartButton.isHovered) {
            movement::bulletCount = 50;
            wireCount = 0;             // Reset Wires
            keycardCollected1 = false;  // Reset Keycard
            keycardCollected2 = false;  // Reset Keycard
            keycardCollected3 = false;  // Reset Keycard
            wireDropsSpawned = 0;
            doorState = 0;
            next = GS_TUTORIAL;  // Change to test file if needed
            printf("Play button clicked - Starting game!\n");
        }
        if (tomenuButton.isHovered) {
            wireCount = 0;             // Reset Wires
            keycardCollected1 = false;  // Reset Keycard
            keycardCollected2 = false;  // Reset Keycard
            keycardCollected3 = false;  // Reset Keycard
            wireDropsSpawned = 0;
            doorState = 0;
            next = GS_MAINMENU;  // Change to test file if needed
            printf("Play button clicked - Starting game!\n");
        }
        else if (exitButton.isHovered) {
            next = GS_QUIT;
            printf("Exiting game!\n");
        }
    }
}
void DeathScreen_Draw() {
    DDrawBackground();

    float multi = 1.3f;
    AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
    AEGfxTextureSet(menutex, 0, 0);
    AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
    renderlogic::drawSquare(0.0f, 247.0f, 285.0f * multi, 115.0f * multi);
    AEGfxMeshDraw(buttonMesh, AE_GFX_MDM_TRIANGLES);

    AEGfxGetPrintSize(deathfont, "DEATH", 1.f, &width, &height);
    AEGfxPrint(deathfont, "DEATH", -width / 2, 0.60f - height / 2, 1, 1, 1, 1, 1);

    MenuHelpers::TexdrawButton(restartButton, buttonMesh, deathfont, buttontex);
    MenuHelpers::TexdrawButton(tomenuButton, buttonMesh, deathfont, buttontex);
    MenuHelpers::TexdrawButton(exitButton, buttonMesh, deathfont, buttontex);



}
void DeathScreen_Free() {
    if (buttonMesh) {
        AEGfxMeshFree(buttonMesh);
        buttonMesh = nullptr;
    }
    if (backgroundMesh) {
        AEGfxMeshFree(backgroundMesh);
        backgroundMesh = nullptr;
    }
}
void DeathScreen_Unload() {
    if (backgroundTexture) {
        AEGfxTextureUnload(backgroundTexture);
        backgroundTexture = nullptr;
    }

    if (deathfont != -1) { AEGfxDestroyFont(deathfont); deathfont = -1; }
    if (menutex != nullptr) { AEGfxTextureUnload(menutex); };
    if (buttontex != nullptr) { AEGfxTextureUnload(buttontex); };
}

void DDrawBackground() {
    if (backgroundTexture) {
        // Draw textured background
        AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
        AEGfxTextureSet(backgroundTexture, 0, 0);

        AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
        AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
        AEGfxSetBlendMode(AE_GFX_BM_BLEND);
        AEGfxSetTransparency(1.0f);

        AEMtx33 scale, translate, transform;
        AEMtx33Scale(&scale, static_cast<f32>(screenWidth), static_cast<f32>(screenLength));
        AEMtx33Trans(&translate, 0.0f, 0.0f);
        AEMtx33Concat(&transform, &translate, &scale);

        AEGfxSetTransform(transform.m);
        AEGfxMeshDraw(backgroundMesh, AE_GFX_MDM_TRIANGLES);
    }
    else {
        // Fallback to gradient background
        AEGfxSetBackgroundColor(0.1f, 0.15f, 0.25f);
    }
}