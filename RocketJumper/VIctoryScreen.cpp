#include "VictoryScreen.h"
#include "Draw.h"

static s8 victoryfont = -1;
static f32 width, height;

static AEGfxTexture* backgroundTexture = nullptr;

static MenuButton restartButton;
static MenuButton exitButton;
static MenuButton tomenuButton;

static AEGfxVertexList* backgroundMesh = nullptr;

static AEGfxTexture* menutex;
static AEGfxTexture* buttontex;

void VictoryScreen_Load() {
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
    if (!backgroundTexture) {
        printf("Warning: MenuBackground.png not found. Using solid color background.\n");
    }

    victoryfont = AEGfxCreateFont("Assets/Fonts/gameover.ttf", 72);
}
void VictoryScreen_Init() {
    float buttonwidth = 390.0f;
    float buttonlength = 80.0f;
    restartButton = { 0.0f, 0.0f, buttonwidth, buttonlength, 1.0f, 1.0f, "RESUME", false };
    tomenuButton = { 0.0f, -120.0f, buttonwidth, buttonlength, 1.0f, 1.0f, "MAIN MENU", false };
    exitButton = { 0.0f, -240.0f, buttonwidth, buttonlength, 1.0f, 1.0f, "EXIT", false };
}
void VictoryScreen_Update() {
    MenuHelpers::updateButtonHover(restartButton);
    MenuHelpers::updateButtonHover(tomenuButton);
    MenuHelpers::updateButtonHover(exitButton);

    // Handle button clicks
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
void VictoryScreen_Draw() {
	VDrawBackground();

    float multi = 1.3f;
    AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
    AEGfxTextureSet(menutex, 0, 0);
    AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
    renderlogic::drawSquare(0.0f, 247.0f, 285.0f*multi, 115.0f * multi);
    AEGfxMeshDraw(buttonMesh, AE_GFX_MDM_TRIANGLES);

    AEGfxGetPrintSize(victoryfont, "VICTORY", 1.f, &width, &height);
    AEGfxPrint(victoryfont, "VICTORY", -width / 2, 0.60f - height / 2, 1, 1, 1, 1, 1);


    MenuHelpers::TexdrawButton(restartButton, buttonMesh, victoryfont, buttontex);
    MenuHelpers::TexdrawButton(tomenuButton, buttonMesh, victoryfont, buttontex);
    MenuHelpers::TexdrawButton(exitButton, buttonMesh, victoryfont, buttontex);

}
void VictoryScreen_Free() {
    if (buttonMesh) {
        AEGfxMeshFree(buttonMesh);
        buttonMesh = nullptr;
    }
    if (backgroundMesh) {
        AEGfxMeshFree(backgroundMesh);
        backgroundMesh = nullptr;
    }
}
void VictoryScreen_Unload() {
    if (backgroundTexture) {
        AEGfxTextureUnload(backgroundTexture);
        backgroundTexture = nullptr;
    }

    if (victoryfont != -1) { AEGfxDestroyFont(victoryfont); victoryfont = -1; }
    if (menutex != nullptr) { AEGfxTextureUnload(menutex); };
    if (buttontex != nullptr) { AEGfxTextureUnload(buttontex); };
}

void VDrawBackground() {
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