/* Start Header ************************************************************************/
/*!
\file         SplashScreen.cpp
\author       Nicholas Chen
\date         March 20, 2026
\brief        DigiPen splash screen implementation.
              The logo fades in over ~1 second, holds for ~1.5 seconds, then fades
              out over ~1 second. Pressing SPACE at any time skips to the main menu.
              The logo is centered on screen and sized to roughly half the screen width.

Copyright (C) 2026 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#include "SplashScreen.h"
#include "AssetManager.h"
#include "GameStateManager.h"
#include "GameStateList.h"
#include "Load.h"

// ==================== TIMING CONSTANTS ====================
// All durations are in seconds. Total splash duration = fade in + hold + fade out.
const float kFadeInDuration  = 1.0f;   // How long the logo takes to appear
const float kHoldDuration    = 1.5f;   // How long the logo stays fully visible
const float kFadeOutDuration = 1.0f;   // How long the logo takes to disappear

// ==================== SPLASH STATE ====================
// Tracks which phase of the animation we are in
enum SplashPhase {
    SPLASH_FADE_IN,
    SPLASH_HOLD,
    SPLASH_FADE_OUT,
    SPLASH_DONE
};

// ==================== FILE-SCOPE VARIABLES ====================
// These are static so they are only visible within this translation unit.
static SplashPhase currentPhase = SPLASH_FADE_IN;
static float phaseTimer         = 0.0f;   // Time elapsed in the current phase
static float logoAlpha          = 0.0f;   // Current opacity of the logo (0 = invisible, 1 = fully visible)

// Screen dimensions cached as floats to avoid repeated int-to-float casts
static float scrW  = 0.0f;
static float scrH  = 0.0f;
static float halfW = 0.0f;
static float halfH = 0.0f;

// Font handle for the "SPACE to skip" prompt text
static s8 splashFont = -1;

// ==================== EXTERN SCREEN SIZE ====================
// Defined in Main.cpp -- we read these to get the window dimensions
extern int screenWidth;
extern int screenLength;

// ==================== LOAD ====================
// Loads all assets needed for the splash screen: the DigiPen logo texture,
// a quad mesh for rendering it, and a font for the skip prompt.
void SplashScreen_Load() {
    // Build a standard white-vertex unit quad for textured rendering.
    // If other states already built MESH_QUAD, BuildSqrMesh will overwrite
    // it safely (the old mesh is freed first inside AssetManager).
    AssetManager::BuildSqrMesh(MESH_QUAD);

    // Load the DigiPen logo texture
    AssetManager::LoadTexture(TEX_DIGIPEN_LOGO, "Assets/UI/Menus/DigiPenWhite.png");
    if (!AssetManager::GetTexture(TEX_DIGIPEN_LOGO)) {
        printf("SplashScreen_Load: WARNING - DigiPenWhite.png not found!\n");
    }

    // Load a font for the skip prompt text
    splashFont = AEGfxCreateFont("Assets/Fonts/gameover.ttf", 36);
    if (splashFont < 0) {
        // Fallback to a system font if the game font is missing
        splashFont = AEGfxCreateFont("Arial.ttf", 36);
        printf("SplashScreen_Load: WARNING - gameover.ttf not found, using fallback.\n");
    }

    printf("SplashScreen_Load: Resources loaded.\n");
}

// ==================== INIT ====================
// Resets the splash animation to its starting state.
void SplashScreen_Init() {
    // Cache screen dimensions as floats
    scrW  = static_cast<float>(screenWidth);
    scrH  = static_cast<float>(screenLength);
    halfW = scrW / 2.0f;
    halfH = scrH / 2.0f;

    // Start the animation from the fade-in phase
    currentPhase = SPLASH_FADE_IN;
    phaseTimer   = 0.0f;
    logoAlpha    = 0.0f;

    // Set a dark background color for the splash screen
    AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);

    printf("SplashScreen_Init: Splash screen initialized.\n");
}

// ==================== UPDATE ====================
// Advances the splash animation timer and handles phase transitions.
// Pressing SPACE at any time skips directly to the main menu.
void SplashScreen_Update() {
    // Allow the player to skip the splash screen by pressing SPACE
    if (AEInputCheckTriggered(AEVK_SPACE)) {
        next = GS_MAINMENU;
        printf("SplashScreen_Update: Skipped to main menu.\n");
        return;
    }

    // Get the time elapsed since the last frame.
    // g_dt is set in the main loop; we use AEFrameRateControllerGetFrameTime
    // as a reliable per-frame delta.
    float dt = static_cast<float>(AEFrameRateControllerGetFrameTime());

    // Advance the timer for the current phase
    phaseTimer += dt;

    switch (currentPhase) {
    case SPLASH_FADE_IN:
        // Linearly interpolate alpha from 0 to 1 over the fade-in duration
        logoAlpha = phaseTimer / kFadeInDuration;
        if (logoAlpha >= 1.0f) {
            logoAlpha = 1.0f;
            // Move to the hold phase
            currentPhase = SPLASH_HOLD;
            phaseTimer   = 0.0f;
        }
        break;

    case SPLASH_HOLD:
        // Logo stays fully visible for the hold duration
        logoAlpha = 1.0f;
        if (phaseTimer >= kHoldDuration) {
            // Move to the fade-out phase
            currentPhase = SPLASH_FADE_OUT;
            phaseTimer   = 0.0f;
        }
        break;

    case SPLASH_FADE_OUT:
        // Linearly interpolate alpha from 1 to 0 over the fade-out duration
        logoAlpha = 1.0f - (phaseTimer / kFadeOutDuration);
        if (logoAlpha <= 0.0f) {
            logoAlpha = 0.0f;
            // Animation complete -- transition to the main menu
            currentPhase = SPLASH_DONE;
            next = GS_MAINMENU;
            printf("SplashScreen_Update: Splash complete, going to main menu.\n");
        }
        break;

    case SPLASH_DONE:
        // Nothing to do -- waiting for the game loop to switch states
        break;
    }
}

// ==================== DRAW ====================
// Renders the DigiPen logo centered on screen with the current alpha,
// and draws a "SPACE to skip" prompt near the bottom.
void SplashScreen_Draw() {
    AEGfxTexture*    logoTex  = AssetManager::GetTexture(TEX_DIGIPEN_LOGO);
    AEGfxVertexList* quadMesh = AssetManager::GetMesh(MESH_QUAD);

    if (logoTex && quadMesh) {
        // -- Draw the DigiPen logo --
        // Size the logo to roughly 50% of the screen width.
        // The DigiPen logo is a wide banner, so we use a ~3:1 aspect ratio.
        float logoWidth  = scrW * 0.50f;
        float logoHeight = logoWidth * 0.33f;

        AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
        AEGfxTextureSet(logoTex, 0.0f, 0.0f);

        // Use white color multiply so the texture renders at original colors,
        // and control visibility entirely through AEGfxSetTransparency.
        AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
        AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
        AEGfxSetBlendMode(AE_GFX_BM_BLEND);
        AEGfxSetTransparency(logoAlpha);

        // Build transform: scale the unit quad to logo size, centered at (0, 0)
        AEMtx33 scale, translate, transform;
        AEMtx33Scale(&scale, logoWidth, logoHeight);
        AEMtx33Trans(&translate, 0.0f, 0.0f);
        AEMtx33Concat(&transform, &translate, &scale);

        AEGfxSetTransform(transform.m);
        AEGfxMeshDraw(quadMesh, AE_GFX_MDM_TRIANGLES);
    }

    // -- Draw "SPACE to skip" prompt near the bottom of the screen --
    if (splashFont >= 0) {
        const char* skipText = "SPACE to skip";

        // Get text dimensions so we can center it horizontally
        float textWidth  = 0.0f;
        float textHeight = 0.0f;
        float textScale  = 0.75f;
        AEGfxGetPrintSize(splashFont, skipText, textScale, &textWidth, &textHeight);

        // Position near the bottom of the screen in normalized coordinates.
        // AEGfxPrint uses [-1, 1] range: (-1,-1) = bottom-left, (1,1) = top-right.
        float printX = -textWidth / 2.0f;   // Center horizontally
        float printY = -0.85f;               // Near the bottom

        // Set up rendering state for text
        AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
        AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
        AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
        AEGfxSetBlendMode(AE_GFX_BM_BLEND);

        // Render the skip prompt with the same alpha as the logo so it fades together
        AEGfxPrint(splashFont, skipText, printX, printY,
                   textScale, 1.0f, 1.0f, 1.0f, logoAlpha);
    }
}

// ==================== FREE ====================
// Frees meshes allocated during Load. Called when leaving this game state.
void SplashScreen_Free() {
    AssetManager::FreeAllMeshes();
    printf("SplashScreen_Free: Meshes freed.\n");
}

// ==================== UNLOAD ====================
// Unloads textures and destroys fonts. Called when leaving this game state.
void SplashScreen_Unload() {
    AssetManager::UnloadAllTextures();
    load::NullExternPointers();

    if (splashFont >= 0) {
        AEGfxDestroyFont(splashFont);
        splashFont = -1;
    }

    printf("SplashScreen_Unload: Resources unloaded.\n");
}
