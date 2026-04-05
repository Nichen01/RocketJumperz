/* Start Header ************************************************************************/
/*!
\file		  DeathScreen.cpp
\author 	  Ivan Chong (i.chong)
\date         April, 04, 2026
\brief        functions used to create Death Screen

Copyright (C) 2026 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#include "DeathScreen.h"
#include "Draw.h"
#include "movement.h"
#include "Confirmation.h"
#include "DoorSystem.h"
#include "player.h"
#include "main.h"

static s8 deathfont = -1;
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

AEAudio lose;

static AEGfxTexture* menutex;
static AEGfxTexture* buttontex;
extern bool prevCleared1, prevCleared2, prevCleared3;

void DeathScreen_Load() {
    //load audio
    lose = AEAudioLoadMusic("Assets/Sounds/Lose.wav");
    bgm = AEAudioCreateGroup();

    // load textures
    load::pauseMenu();
    TitleTex = AEGfxTextureLoad("Assets/UI/Menus/TitleFrame.png");
    backgroundTexture = AEGfxTextureLoad("Assets/UI/MainMenu.png");

    if (!backgroundTexture) {
        printf("Warning: MenuBackground.png not found. Using solid color background.\n");
    }
    // load font
    deathfont = AEGfxCreateFont("Assets/Fonts/gameover.ttf", static_cast<int>(48 * screenscale));
}
void DeathScreen_Init() {
    //play audio
    AEAudioPlay(lose, bgm, MainVolume, 1.f, 0);
    
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
    tomenuButton = { 0.0f, -120.0f * screenscale , buttonwidth, buttonlength, 1.0f, 1.0f, "MAIN MENU", false };
    exitButton = { 0.0f, -240.0f * screenscale , buttonwidth, buttonlength, 1.0f, 1.0f, "EXIT", false };
}
void DeathScreen_Update() {
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
        if (restartButton.isHovered) {
            // Restore checkpoint stats so the player keeps progress from prior levels
            // but loses anything collected during the level they died on.
            movement::bulletCount = savedAmmo;
            wireCount             = savedWireCount;

            // Only reset the keycard for the level the player died on
            switch (currentGameLevel) {
            case 0: keycardCollected0 = false; playerEnteredDoor0 = false; break;
            case 1: keycardCollected1 = false; playerEnteredDoor1 = false; break;
            case 2: keycardCollected2 = false; playerEnteredDoor2 = false; break;
            case 3: keycardCollected3 = false; break;
            }
            //confirmation check
            destructive = true;
            if (leave == 1) {
                destructive = false;
                leave = 0;

                // Send the player back to the level they died on, not the tutorial
                switch (currentGameLevel) {
                case 0: next = GS_TUTORIAL; break;
                case 1: next = GS_LEVEL1;   break;
                case 2: next = GS_LEVEL2;   break;
                case 3: next = GS_LEVEL3;   break;
                default: next = GS_TUTORIAL; break;
                }
            }
            else if (leave == 2) {
                destructive = false;
                leave = 0;
            }
            printf("Restart button clicked - Restarting level %d!\n", currentGameLevel);
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
            prevCleared1 = 0;
            prevCleared2 = 0;
            prevCleared3 = 0;
            wireDropsSpawned = 0;
            doorState = 0;
            //confirmation check
            destructive = true;
            if (leave == 1) {
                destructive = false;
                leave = 0;
                next = GS_MAINMENU;  // Change to test file if needed
            }
            else if (leave == 2) {
                destructive = false;
                leave = 0;
            }
            
            printf("Play button clicked - Starting game!\n");
        }
        else if (exitButton.isHovered) {
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
void DeathScreen_Draw() {
    // render deathscreen
    DDrawBackground();

    float multi = 1.3f;
    AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
    AEGfxTextureSet(TitleTex, 0, 0);
    AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
    renderlogic::drawSquare(0.0f, 247.0f * screenscale, (285.0f * multi) * screenscale, (115.0f * multi) * screenscale);
    AEGfxMeshDraw(buttonMesh, AE_GFX_MDM_TRIANGLES);

    AEGfxGetPrintSize(deathfont, "DEATH", 1.f, &width, &height);
    AEGfxPrint(deathfont, "DEATH", -width / 2, 0.60f - height / 2, 1, 1, 1, 1, 1);

    MenuHelpers::TexdrawButton(restartButton, buttonMesh, deathfont, buttonTex);
    MenuHelpers::TexdrawButton(tomenuButton, buttonMesh, deathfont, buttonTex);
    MenuHelpers::TexdrawButton(exitButton, buttonMesh, deathfont, buttonTex);
    // render conformation screen
    if (destructive) {
        Confirmation_Draw(deathfont, yesButton, noButton);
    }

}
void DeathScreen_Free() {
    // Free meshes
    if (backgroundMesh) {
        AEGfxMeshFree(backgroundMesh);
        backgroundMesh = nullptr;
    }
    AssetManager::FreeAllMeshes();
}
void DeathScreen_Unload() {
    // unload textures and sound
    AEAudioUnloadAudio(lose);
    AEAudioUnloadAudioGroup(bgm);

    if (TitleTex) {
        AEGfxTextureUnload(TitleTex);
        TitleTex = nullptr;
    }
    if (backgroundTexture) {
        AEGfxTextureUnload(backgroundTexture);
        backgroundTexture = nullptr;
    }

    if (deathfont != -1) { AEGfxDestroyFont(deathfont); deathfont = -1; }
    AssetManager::UnloadAllTextures();
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