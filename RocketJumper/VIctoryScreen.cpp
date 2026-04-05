/* Start Header ************************************************************************/
/*!
\file		  VictoryScreen.cpp
\author 	  Ivan Chong (i.chong)
\date         April, 04, 2026
\brief        functions used to create Victory Screen

Copyright (C) 2026 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#include "VictoryScreen.h"
#include "Draw.h"
#include "Confirmation.h"
#include "main.h"

static s8 victoryfont = -1;
static f32 width, height;

static AEGfxTexture* backgroundTexture = nullptr;
static AEGfxTexture* TitleTex;

static MenuButton restartButton;
static MenuButton exitButton;
static MenuButton tomenuButton;
static MenuButton yesButton;
static MenuButton noButton;

static bool destructive = false;
static s8 leave = 0;
static AEGfxVertexList* backgroundMesh = nullptr;

AEAudio win;

static AEGfxTexture* menutex;
static AEGfxTexture* buttontex;
extern bool prevCleared1, prevCleared2, prevCleared3;

void VictoryScreen_Load() {
    //load audio
    win = AEAudioLoadMusic("Assets/Sounds/Victory.wav");
    bgm = AEAudioCreateGroup();

    // load textures
    load::pauseMenu();
    TitleTex = AEGfxTextureLoad("Assets/UI/Menus/TitleFrame.png");
    backgroundTexture = AEGfxTextureLoad("Assets/UI/MainMenu.png");

    if (!backgroundTexture) {
        printf("Warning: MenuBackground.png not found. Using solid color background.\n");
    }
    // load font
    victoryfont = AEGfxCreateFont("Assets/Fonts/gameover.ttf", static_cast<int>(48 * screenscale));
}
void VictoryScreen_Init() {
    //play audio
    AEAudioPlay(win, bgm, MainVolume, 1.f, -1);

    //initilise meshes
    AssetManager::BuildSqrMesh(MESH_BUTTON);
    buttonMesh = AssetManager::GetMesh(MESH_BUTTON);

    AEGfxTriAdd(
        -0.5f, -0.5f, 0xFFFFFFFF, 0.0f, 1.0f,
        0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f,
        -0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f);

    AEGfxTriAdd(
        0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f,
        0.5f, 0.5f, 0xFFFFFFFF, 1.0f, 0.0f,
        -0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f);

    backgroundMesh = AEGfxMeshEnd();
    // initialize data
    Confirmation_Init(yesButton, noButton);

    float buttonwidth = 390.0f * screenscale;
    float buttonlength = 80.0f * screenscale;
    restartButton = { 0.0f, 0.0f, buttonwidth, buttonlength, 1.0f, 1.0f, "RESTART", false };
    tomenuButton = { 0.0f, -120.0f * screenscale, buttonwidth, buttonlength, 1.0f, 1.0f, "MAIN MENU", false };
    exitButton = { 0.0f, -240.0f * screenscale, buttonwidth, buttonlength, 1.0f, 1.0f, "EXIT", false };
}
void VictoryScreen_Update() {
    //checks if button are hovered over
    if (!destructive) {
        MenuHelpers::updateButtonHover(restartButton);
        MenuHelpers::updateButtonHover(tomenuButton);
        MenuHelpers::updateButtonHover(exitButton);
    }
    else {
        Confirmation_Update(yesButton, noButton, leave);
    }
    // Handle button clicks
    if (AEInputCheckTriggered(AEVK_LBUTTON)) {
        // Restore checkpoint stats so the player keeps progress from prior levels
        // but loses anything collected during the level they died on.
        if (restartButton.isHovered) {
            movement::bulletCount = 50;
            wireCount = 0;             // Reset Wires
            keycardCollected0 = false;  // Reset Keycard
            keycardCollected1 = false;  // Reset Keycard
            keycardCollected2 = false;  // Reset Keycard
            keycardCollected3 = false;  // Reset Keycard
            playerEnteredDoor0 = false;
            playerEnteredDoor1 = false;
            playerEnteredDoor2 = false;
            wireDropsSpawned = 0;
            doorState = 0;             // Reset Final Door
            //confirmation check
            destructive = true;
            if (leave == 1) {
                next = GS_TUTORIAL;
            }
            else if (leave == 2) {
                destructive = false;
                leave = 0;
            }
            
            printf("Play button clicked - Starting game!\n");
        }
        if (tomenuButton.isHovered) {
            wireCount = 0;             // Reset Wires
            keycardCollected0 = false;  // Reset Keycard
            keycardCollected1 = false;  // Reset Keycard
            keycardCollected2 = false;  // Reset Keycard
            keycardCollected3 = false;  // Reset Keycard
            playerEnteredDoor0 = false;
            playerEnteredDoor1 = false;
            playerEnteredDoor2 = false;
            wireDropsSpawned = 0;
            prevCleared1 = 0;
            prevCleared2 = 0;
            prevCleared3 = 0;
            doorState = 0;             // Reset Final Door
            //confirmation check
            destructive = true;
            if (leave == 1) {
                destructive = false;
                leave = 0;
                next = GS_MAINMENU;
            }
            else if (leave == 2) {
                destructive = false;
                leave = 0;
            }

            printf("Play button clicked - Starting game!\n");
        }
        else if (exitButton.isHovered) {
            //confirmation check
            destructive = true;
            if (leave == 1) {
                destructive = false;
                leave = 0;
                next = GS_QUIT;
            }
            else if (leave == 2) {
                destructive = false;
                leave = 0;
            }
            printf("Exiting game!\n");
        }
    }
}
void VictoryScreen_Draw() {
    // render victoryscreen
	VDrawBackground();

    float multi = 1.3f;
    AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
    AEGfxTextureSet(TitleTex, 0, 0);
    AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
    renderlogic::drawSquare(0.0f, 247.0f * screenscale, 285.0f*multi * screenscale, 115.0f * multi * screenscale);
    AEGfxMeshDraw(buttonMesh, AE_GFX_MDM_TRIANGLES);

    AEGfxGetPrintSize(victoryfont, "VICTORY", 1.f, &width, &height);
    AEGfxPrint(victoryfont, "VICTORY", -width / 2, 0.60f - height / 2, 1, 1, 1, 1, 1);

    MenuHelpers::TexdrawButton(restartButton, buttonMesh, victoryfont, buttonTex);
    MenuHelpers::TexdrawButton(tomenuButton, buttonMesh, victoryfont, buttonTex);
    MenuHelpers::TexdrawButton(exitButton, buttonMesh, victoryfont, buttonTex);
    // render conformation screen
    if (destructive) {
        Confirmation_Draw(victoryfont, yesButton, noButton);
    }

}
void VictoryScreen_Free() {
    // Free meshes
    if (backgroundMesh) {
        AEGfxMeshFree(backgroundMesh);
        backgroundMesh = nullptr;
    }
    AssetManager::FreeAllMeshes();

}
void VictoryScreen_Unload() {
    // unload textures and sound
    AEAudioUnloadAudio(win);
    AEAudioUnloadAudioGroup(bgm);

    if (TitleTex) {
        AEGfxTextureUnload(TitleTex);
        TitleTex = nullptr;
    }
    if (backgroundTexture) {
        AEGfxTextureUnload(backgroundTexture);
        backgroundTexture = nullptr;
    }


    if (victoryfont != -1) { AEGfxDestroyFont(victoryfont); victoryfont = -1; }
    AssetManager::UnloadAllTextures();
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