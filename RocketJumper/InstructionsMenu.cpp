/* Start Header ************************************************************************/
/*!
\file         InstructionsMenu.cpp
\brief        In-game instructions overlay triggered by a "?" icon.
              When active the level's Update is skipped (game pauses).
              Closed via a "Back" button or the Escape key.
*/
/* End Header **************************************************************************/

#include "InstructionsMenu.h"
#include "AssetManager.h"
#include "MainMenu.h"   // MenuButton, MenuHelpers, screenWidth, screenLength
#include "Load.h"       // uiMesh, buttonMesh, buttonTex, menuTex
#include "Draw.h"       // renderlogic::drawTexture
#include "Main.h"       // extern bool pause  (needed to counteract Main.cpp Escape toggle)

// ---------------------------------------------------------------------------
// Module state -- file-scope statics (no globals leak out)
// ---------------------------------------------------------------------------

// Whether the full-screen instructions overlay is currently shown
static bool isMenuActive = false;

// The clickable "?" icon (rendered during normal gameplay in the top-right)
static MenuButton iconButton;

// The "Back" button shown at the bottom of the overlay
static MenuButton backButton;

// ---------------------------------------------------------------------------
// Layout constants
// ---------------------------------------------------------------------------

// "?" icon size in world pixels
static const f32 kIconSize = 50.0f;

// Margin from screen edge for the "?" icon (world pixels)
static const f32 kIconMargin = 40.0f;

// "Back" button dimensions (matches the confirmation / pause menu style)
static const f32 kBackButtonWidth  = 390.0f;
static const f32 kBackButtonHeight = 80.0f;

namespace InstructionsMenu {

// =========================================================================
// Load -- called once in each level's Load function
// =========================================================================
void Load() {
    // Load the full-screen instructions image
    AssetManager::LoadTexture(TEX_INSTRUCTIONS_MENU, "Assets/UI/instructionsMenu.png");

    // Load the "?" icon texture
    AssetManager::LoadTexture(TEX_INSTRUCTIONS_ICON, "Assets/UI/Menus/settings.png");
}

// =========================================================================
// Init -- called in each level's Initialize to reset state
// =========================================================================
void Init() {
    isMenuActive = false;

    // Position the "?" icon in the top-right corner of the screen.
    // screenWidth and screenLength are the window dimensions in pixels.
    // The world origin (0,0) is at the screen centre, so the top-right
    // corner is (+halfW, +halfH).
    f32 halfW = static_cast<f32>(screenWidth)  / 2.0f;
    f32 halfH = static_cast<f32>(screenLength) / 2.0f;

    f32 iconX = halfW - kIconMargin - (kIconSize / 2.0f);
    f32 iconY = halfH - kIconMargin - (kIconSize / 2.0f);

    iconButton = { iconX, iconY, kIconSize, kIconSize, 1.0f, 1.0f, "", false };

    // "Back" button centred horizontally, near the bottom of the screen
    f32 backY = -halfH * 0.778f;   // same ratio used by MainMenu's backButton
    backButton = { 0.0f, backY, kBackButtonWidth, kBackButtonHeight, 1.0f, 1.0f, "BACK", false };
}

// =========================================================================
// Update -- call at the VERY TOP of each level's Update.
// Returns true  => menu is open, caller must early-return (game is paused).
// Returns false => menu is closed, caller continues with normal Update.
// =========================================================================
bool Update() {
    if (!isMenuActive) {
        // ---- Normal gameplay: only update the "?" icon hover / click ----
        MenuHelpers::updateButtonHover(iconButton);

        if (AEInputCheckTriggered(AEVK_LBUTTON) && iconButton.isHovered) {
            isMenuActive = true;
            return true;  // pause this frame as well
        }
        return false;
    }

    // ---- Menu is active: handle closing ----
    MenuHelpers::updateButtonHover(backButton);

    // Close on Escape key
    if (AEInputCheckTriggered(AEVK_ESCAPE)) {
        isMenuActive = false;
        return false;
    }

    // Close on Back button click
    if (AEInputCheckTriggered(AEVK_LBUTTON) && backButton.isHovered) {
        isMenuActive = false;
        return false;
    }

    // Still active -- tell the caller to skip its Update logic
    return true;
}

// =========================================================================
// Draw -- call at the very END of each level's Draw function
// =========================================================================
void Draw() {
    // ------------------------------------------------------------------
    // Escape-key handling for closing the overlay.
    //
    // Main.cpp checks AEInputCheckTriggered(AEVK_ESCAPE) BEFORE calling
    // fpUpdate(), so when the instructions overlay is open and the user
    // presses Escape, Main.cpp toggles `pause` to true and skips our
    // Update entirely.  Draw is always called, so we intercept here:
    // if the overlay is active and pause just became true this frame
    // (meaning Escape was pressed), close the overlay and undo the pause.
    // ------------------------------------------------------------------
    if (isMenuActive && pause) {
        isMenuActive = false;
        pause = false;   // counteract Main.cpp's toggle so the game resumes
    }

    // Also handle the Back button click even when Update was skipped
    // (pause was true for one frame).  updateButtonHover reads mouse state
    // directly, so it works from Draw.
    if (isMenuActive) {
        MenuHelpers::updateButtonHover(backButton);
        if (AEInputCheckTriggered(AEVK_LBUTTON) && backButton.isHovered) {
            isMenuActive = false;
        }
    }

    AEGfxVertexList* quadMesh = AssetManager::GetMesh(MESH_QUAD);

    if (!isMenuActive) {
        // ---- Draw the "?" icon in the top-right corner ----
        AEGfxTexture* iconTex = AssetManager::GetTexture(TEX_INSTRUCTIONS_ICON);
        if (iconTex && quadMesh) {
            // If hovered, tint slightly to give visual feedback
            AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
            AEGfxSetBlendMode(AE_GFX_BM_BLEND);
            AEGfxSetTransparency(1.0f);

            if (iconButton.isHovered) {
                AEGfxSetColorToAdd(0.3f, 0.6f, 1.0f, 0.0f);
            }
            else {
                AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
            }
            AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);

            renderlogic::drawTexture(iconButton.x, iconButton.y, iconTex, quadMesh,
                                     kIconSize * iconButton.scale, kIconSize * iconButton.scale);

            // Reset color state
            AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
        }
        return;
    }

    // ==================================================================
    // ---- Full overlay: dim background, instructions image, Back btn --
    // ==================================================================
    f32 winW = static_cast<f32>(AEGfxGetWindowWidth());
    f32 winH = static_cast<f32>(AEGfxGetWindowHeight());

    // Semi-transparent dark overlay behind the menu (dims the game scene)
    if (quadMesh) {
        AEGfxSetRenderMode(AE_GFX_RM_COLOR);
        AEGfxSetBlendMode(AE_GFX_BM_BLEND);
        AEGfxSetTransparency(0.6f);
        AEGfxSetColorToMultiply(0.0f, 0.0f, 0.0f, 1.0f);
        AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);

        AEMtx33 dimScale, dimTrans, dimXform;
        AEMtx33Scale(&dimScale, winW, winH);
        AEMtx33Trans(&dimTrans, 0.0f, 0.0f);
        AEMtx33Concat(&dimXform, &dimTrans, &dimScale);
        AEGfxSetTransform(dimXform.m);
        AEGfxMeshDraw(quadMesh, AE_GFX_MDM_TRIANGLES);
    }

    // Draw the instructions image centred on screen
    AEGfxTexture* instrTex = AssetManager::GetTexture(TEX_INSTRUCTIONS_MENU);
    if (instrTex && quadMesh) {
        // Size the image to roughly 75% of screen width, maintain ~1.7:1 aspect
        f32 imgW = winW * 0.75f;
        f32 imgH = imgW / 1.7f;
        f32 imgY = winH * 0.06f;   // nudge up so back button has room

        AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
        AEGfxTextureSet(instrTex, 0.0f, 0.0f);
        AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
        AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
        AEGfxSetBlendMode(AE_GFX_BM_BLEND);
        AEGfxSetTransparency(1.0f);

        AEMtx33 imgScale, imgTrans, imgXform;
        AEMtx33Scale(&imgScale, imgW, imgH);
        AEMtx33Trans(&imgTrans, 0.0f, imgY);
        AEMtx33Concat(&imgXform, &imgTrans, &imgScale);
        AEGfxSetTransform(imgXform.m);
        AEGfxMeshDraw(quadMesh, AE_GFX_MDM_TRIANGLES);
    }

    // Draw the "Back" button at the bottom of the overlay
    AEGfxVertexList* btnMesh = AssetManager::GetMesh(MESH_BUTTON);
    AEGfxTexture*    btnTex  = AssetManager::GetTexture(TEX_BUTTON);
    if (btnMesh && btnTex) {
        // TexdrawButton needs a font -- use fontID -1 if no level font is
        // available.  However, drawTextCentered inside TexdrawButton will
        // simply call AEGfxPrint, which tolerates any valid s8 font.
        // We reuse the same button texture + mesh as the rest of the UI.

        // We need a valid font for the button label.  Because the menu is
        // drawn from many different levels, we create a small persistent
        // font here (only once).  It is fine to leak at this scope because
        // AEGfxDestroyFont is called by the engine on shutdown, and the font
        // is reused across levels.
        static s8 overlayFont = -1;
        if (overlayFont < 0) {
            overlayFont = AEGfxCreateFont("Assets/Fonts/gameover.ttf", 72);
        }

        MenuHelpers::TexdrawButton(backButton, btnMesh, overlayFont, btnTex);
    }

    // Reset colour state for anything drawn after us
    AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
    AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
}

// =========================================================================
// Unload -- placeholder (AssetManager::UnloadAllTextures handles cleanup)
// =========================================================================
void Unload() {
    // Textures are freed by AssetManager::UnloadAllTextures() which each
    // level already calls in its own Unload.  Nothing extra needed here.
}

} // namespace InstructionsMenu
