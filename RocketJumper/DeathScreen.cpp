#include "DeathScreen.h"
#include "Draw.h"
static s8 deathfont = -1;
static f32 width, height;

static AEGfxTexture* backgroundTexture = nullptr;

static MenuButton restartButton;
static MenuButton exitButton;
static MenuButton tomenuButton;

static AEGfxVertexList* buttonMesh = nullptr;
static AEGfxVertexList* backgroundMesh = nullptr;
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

    backgroundTexture = AEGfxTextureLoad("Assets/MainMenu.png");
    if (!backgroundTexture) {
        printf("Warning: MenuBackground.png not found. Using solid color background.\n");
    }

    deathfont = AEGfxCreateFont("Assets/Fonts/gameover.ttf", 72);
}
void DeathScreen_Init() {
    restartButton = { 0.0f, 0.0f, 375.0f, 80.0f, 1.0f, 1.0f, "RESTART", false };
    tomenuButton = { 0.0f, -120.0f, 375.0f, 80.0f, 1.0f, 1.0f, "MAIN MENU", false };
    exitButton = { 0.0f, -240.0f, 375.0f, 80.0f, 1.0f, 1.0f, "EXIT", false };
}
void DeathScreen_Update() {
    MenuHelpers::updateButtonHover(restartButton);
    MenuHelpers::updateButtonHover(tomenuButton);
    MenuHelpers::updateButtonHover(exitButton);

    if (AEInputCheckTriggered(AEVK_LBUTTON)) {
        if (restartButton.isHovered) {
            next = GS_TUTORIAL;  // Change to test file if needed
            printf("Play button clicked - Starting game!\n");
        }
        if (tomenuButton.isHovered) {
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

    AEGfxSetRenderMode(AE_GFX_RM_COLOR);
    AEGfxSetColorToAdd(0.3f, 0.3f, 0.3f, 0.8f);  // Bright blue when hovered
    renderlogic::drawSquare(0.0f, 0.0f, 500.0f, 640.0f);
    AEGfxMeshDraw(buttonMesh, AE_GFX_MDM_TRIANGLES);

    MenuHelpers::drawButton(restartButton, buttonMesh, deathfont);
    MenuHelpers::drawButton(tomenuButton, buttonMesh, deathfont);
    MenuHelpers::drawButton(exitButton, buttonMesh, deathfont);

    AEGfxGetPrintSize(deathfont, "DEATH", 1.f, &width, &height);
    AEGfxPrint(deathfont, "DEATH", -width / 2, 0.60f - height / 2, 1, 1, 1, 1, 1);
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