/* Start Header ************************************************************************/
/*!
\file		  MainMenu.cpp
\author       Nicholas Chen(c.chen)
\date         January, 30, 2026
\brief        Main menu implementation with Instructions, Credits, and animations

Copyright (C) 2026 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#include "MainMenu.h"
#include "AssetManager.h"
#include "AEEngine.h"
#include "GameStateManager.h"
#include "GameStateList.h"
#include <cmath>

// ==================== FORWARD DECLARATIONS ====================
static void UpdateMainMenu();
static void UpdateInstructionsMenu();
static void UpdateCreditsMenu();
static void DrawBackground();
static void DrawMainMenu();
static void DrawInstructionsMenu();
static void DrawCreditsMenu();

// ==================== SCREEN-RELATIVE HELPERS ====================
// These are recomputed once per Init so every layout calculation can reference them.
// Using floats avoids repeated int-to-float casts throughout the draw code.
static f32 scrW  = 0.0f;   // full screen width  in pixels (e.g. 1600)
static f32 scrH  = 0.0f;   // full screen height in pixels (e.g. 900)
static f32 halfW = 0.0f;   // half screen width  (e.g. 800)
static f32 halfH = 0.0f;   // half screen height (e.g. 450)

// ==================== GLOBAL RESOURCES ====================
// Meshes and textures are now managed by AssetManager -- no local pointers needed.
static s8 menuFont = -1;

// Current menu state
static MenuState currentMenuState = MENU_MAIN;

// Credits scroll position -- expressed as a proportion of screen height at init
static f32 creditsScrollY   = 0.0f;
static const f32 CREDITS_SCROLL_SPEED = 2.5f;

// ==================== BUTTON DEFINITIONS ====================
// Main Menu Buttons
static MenuButton playButton;
static MenuButton instructionsButton;
static MenuButton creditsButton;
static MenuButton quitButton;

// Back button for sub-menus
static MenuButton backButton;


// Animation constants
static const f32 BUTTON_SCALE_NORMAL = 1.0f;
static const f32 BUTTON_SCALE_HOVER  = 1.15f;
static const f32 BUTTON_SCALE_SPEED  = 0.15f;

// ==================== HELPER FUNCTIONS ======================================================================================
namespace MenuHelpers {
    bool isMouseOverButton(const MenuButton& button) {
        f32 halfW = static_cast<f32>(screenWidth) / 2.0f;
        f32 halfH = static_cast<f32>(screenLength) / 2.0f;

        s32 mouseX, mouseY;
        AEInputGetCursorPosition(&mouseX, &mouseY);
        // Convert pixel cursor position to world coordinates using cached screen values
        f32 worldMouseX = static_cast<f32>(mouseX) - halfW;
        f32 worldMouseY = halfH - static_cast<f32>(mouseY);

        f32 bHalfWidth  = (button.width  * button.scale) / 2.0f;
        f32 bHalfHeight = (button.height * button.scale) / 2.0f;

        return (worldMouseX >= button.x - bHalfWidth  &&
                worldMouseX <= button.x + bHalfWidth  &&
                worldMouseY >= button.y - bHalfHeight &&
                worldMouseY <= button.y + bHalfHeight);
    }

    void updateButtonHover(MenuButton& button) {
        button.isHovered = isMouseOverButton(button);

        // Set target scale based on hover state
        button.targetScale = button.isHovered ? BUTTON_SCALE_HOVER : BUTTON_SCALE_NORMAL;

        // Smooth interpolation towards target scale
        if (button.scale < button.targetScale) {
            button.scale += BUTTON_SCALE_SPEED;
            if (button.scale > button.targetScale) {
                button.scale = button.targetScale;
            }
        }
        else if (button.scale > button.targetScale) {
            button.scale -= BUTTON_SCALE_SPEED;
            if (button.scale < button.targetScale) {
                button.scale = button.targetScale;
            }
        }
    }
    void TexdrawButton(const MenuButton& button, AEGfxVertexList* mesh, s8 fontID, AEGfxTexture* texture) {
        // Draw button background
        AEMtx33 scale, translate, transform;
        AEMtx33Scale(&scale, button.width * button.scale, button.height * button.scale);
        AEMtx33Trans(&translate, button.x, button.y);
        AEMtx33Concat(&transform, &translate, &scale);

        AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);

        // Color based on hover state
        if (button.isHovered) {
            AEGfxSetColorToAdd(0.3f, 0.6f, 1.0f, 0.8f);  // Bright blue when hovered
        }
        else {
            AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f); 
        }

        AEGfxTextureSet(texture, 0, 0);
        AEGfxSetTransform(transform.m);
        AEGfxMeshDraw(mesh, AE_GFX_MDM_TRIANGLES);

        // Draw button text
        drawTextCentered(button.text, button.x, button.y, button.scale*0.7f, fontID);
        AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
    }

    void drawButton(const MenuButton& button, AEGfxVertexList* mesh, s8 fontID) {
        // Draw button background
        AEMtx33 scale, translate, transform;
        AEMtx33Scale(&scale, button.width * button.scale, button.height * button.scale);
        AEMtx33Trans(&translate, button.x, button.y);
        AEMtx33Concat(&transform, &translate, &scale);

        AEGfxSetRenderMode(AE_GFX_RM_COLOR);

        // Color based on hover state
        if (button.isHovered) {
            AEGfxSetColorToAdd(0.3f, 0.6f, 1.0f, 0.8f);  // Bright blue when hovered
        }
        else {
            AEGfxSetColorToAdd(0.15f, 0.15f, 0.3f, 0.7f); // Dark blue-gray normally
        }

        AEGfxSetTransform(transform.m);
        AEGfxMeshDraw(mesh, AE_GFX_MDM_TRIANGLES);

        // Draw button text
        drawTextCentered(button.text, button.x, button.y, button.scale, fontID);
        AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);

    }

    void drawTextCentered(const char* text, f32 x, f32 y, f32 scale, s8 fontID) {
        AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
        if (fontID < 0) {
            printf("FONT IS NOT LOADED."); return;
        }
        f32 halfW = static_cast<f32>(screenWidth) / 2.0f;
        f32 halfH = static_cast<f32>(screenLength) / 2.0f;

        // Get text dimensions in normalized units (0 to 2 range per AE docs)
        f32 textWidth, textHeight;
        AEGfxGetPrintSize(fontID, text, scale, &textWidth, &textHeight);

        // Convert world coordinates to normalized coordinates for AEGfxPrint.
        // AEGfxPrint uses [-1, 1] range where (-1,-1) = bottom-left, (1,1) = top-right.
        // Our world coords have (0,0) = center, so dividing by half-screen gives normalized.
        f32 normalizedX = x / halfW;
        f32 normalizedY = y / halfH;

        // Center the text by offsetting half the text dimensions
        f32 printX = normalizedX - textWidth  / 2.0f;
        f32 printY = normalizedY - textHeight / 2.0f;

        // Switch to TEXTURE mode so the font glyph atlas can be sampled
        AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);

        // Reset color modifiers so the font renders with its own colors
        AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
        AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);

        AEGfxSetBlendMode(AE_GFX_BM_BLEND);
        AEGfxPrint(fontID, text, printX, printY,
            scale, 1.0f, 1.0f, 1.0f, 1.0f);
    }
}

// ==================== INITIALIZATION FUNCTIONS ========================================================================
void MainMenu_Load() {
    // Build meshes via AssetManager instead of manually creating them here.
    // MESH_QUAD = white-vertex unit quad (used for textured background & title).
    // MESH_MENU_BUTTON = black-vertex unit quad (used for color-mode button rects).
    AssetManager::BuildSqrMesh(MESH_QUAD);                            // white vertices
    AssetManager::BuildSqrMesh(MESH_MENU_BUTTON, 1, 1, 0xFF000000);  // black vertices

    // Load textures through AssetManager for centralized tracking
    AssetManager::LoadTexture(TEX_MAIN_MENU_BG, "Assets/Map/background.jpg");
    if (!AssetManager::GetTexture(TEX_MAIN_MENU_BG)) {
        printf("Warning: MainMenu.png not found. Using solid color background.\n");
    }

    AssetManager::LoadTexture(TEX_TITLE, "Assets/Title.png");
    if (!AssetManager::GetTexture(TEX_TITLE)) {
        printf("Warning: Title.png not found. Title banner will not render.\n");
    }

    // DigiPen logo for credits screen
    AssetManager::LoadTexture(TEX_DIGIPEN_LOGO, "Assets/UI/Menus/DigiPenWhite.png");
    if (!AssetManager::GetTexture(TEX_DIGIPEN_LOGO)) {
        printf("Warning: DigiPenWhite.png not found. Credits logo will not render.\n");
    }

    // Load font
    menuFont = AEGfxCreateFont("Assets/Fonts/gameover.ttf", 48);
    if (menuFont < 0) {
        menuFont = AEGfxCreateFont("Arial.ttf", 48);
        printf("(UI) Warning: MenuFont.ttf not found. Text will not render.\n");
    }

    printf("MainMenu_Load: Resources loaded!\n");
}

void MainMenu_Init() {
    // Cache screen dimensions as floats so every layout calc can use them directly.
    // These come from the global extern ints defined in Main.cpp.
    scrW  = static_cast<f32>(screenWidth);
    scrH  = static_cast<f32>(screenLength);
    halfW = scrW / 2.0f;
    halfH = scrH / 2.0f;

    // ----- Button sizing relative to screen -----
    // Button width  = ~23.4% of screen width   (375 / 1600)
    // Button height = ~8.9% of screen height    (80  / 900)
    // Vertical gap between buttons = ~13.3% of screen height (120 / 900)
    f32 btnW   = scrW * 0.234f;
    f32 btnH   = scrH * 0.089f;
    f32 btnGap = scrH * 0.133f;

    // First button sits at the vertical center of the screen (y = 0)
    f32 startY = 0.0f;

    playButton         = { 0.0f, startY,                btnW, btnH, 1.0f, 1.0f, "PLAY",         false };
    instructionsButton = { 0.0f, startY - btnGap,       btnW, btnH, 1.0f, 1.0f, "INSTRUCTIONS", false };
    creditsButton      = { 0.0f, startY - btnGap * 2.0f, btnW, btnH, 1.0f, 1.0f, "CREDITS",     false };
    quitButton         = { 0.0f, startY - btnGap * 3.0f, btnW, btnH, 1.0f, 1.0f, "QUIT",        false };

    // Back button (sub-menus): slightly smaller, near bottom of screen
    f32 backBtnW = scrW * 0.156f;   // ~250 / 1600
    f32 backBtnH = scrH * 0.078f;   // ~70  / 900
    f32 backBtnY = -halfH * 0.778f; // ~-350 / 450
    backButton = { 0.0f, backBtnY, backBtnW, backBtnH, 1.0f, 1.0f, "BACK", false };

    // Reset menu state
    currentMenuState = MENU_MAIN;

    // Credits scroll starts below the visible screen area
    creditsScrollY = -scrH * 0.667f;

    printf("MainMenu_Init: Main menu initialized\n");
}

// ==================== UPDATE FUNCTIONS ========================================================================================
void MainMenu_Update() {
    switch (currentMenuState) {
    case MENU_MAIN:
        UpdateMainMenu();
        break;
    case MENU_INSTRUCTIONS:
        UpdateInstructionsMenu();
        break;
    case MENU_CREDITS:
        UpdateCreditsMenu();
        break;
    }
}

void UpdateMainMenu() {
    // Update button hover states
    MenuHelpers::updateButtonHover(playButton);
    MenuHelpers::updateButtonHover(instructionsButton);
    MenuHelpers::updateButtonHover(creditsButton);
    MenuHelpers::updateButtonHover(quitButton);

    // Handle button clicks
    if (AEInputCheckTriggered(AEVK_LBUTTON)) {
        if (playButton.isHovered) {
            movement::bulletCount = 10;
            next = GS_TUTORIAL;  // Change to test file if needed
            printf("Play button clicked - Starting game!\n");
        }
        else if (instructionsButton.isHovered) {
            currentMenuState = MENU_INSTRUCTIONS;
            printf("Instructions button clicked!\n");
        }
        else if (creditsButton.isHovered) {
            currentMenuState = MENU_CREDITS;
            creditsScrollY = -scrH * 0.667f; // Reset scroll position
            printf("Credits button clicked!\n");
        }
        else if (quitButton.isHovered) {
            next = GS_QUIT;
            printf("Exiting game!\n");
        }
    }
}

void UpdateInstructionsMenu() {
    // Update back button
    MenuHelpers::updateButtonHover(backButton);

    // Handle back button click
    if (AEInputCheckTriggered(AEVK_LBUTTON)) {
        if (backButton.isHovered) {
            currentMenuState = MENU_MAIN;
            printf("Returning to main menu\n");
        }
    }

    // Also allow ESC to go back
    if (AEInputCheckTriggered(AEVK_ESCAPE)) {
        currentMenuState = MENU_MAIN;
    }
}

void UpdateCreditsMenu() {
    // Update back button
    MenuHelpers::updateButtonHover(backButton);

    // Scroll credits upward each frame
    creditsScrollY += CREDITS_SCROLL_SPEED;

    // The credits content is tall (~28 lines). Once it scrolls far enough past the
    // top of the screen, wrap back to the starting position below the viewport.
    // Total content height is roughly 28 lines * lineSpacing (~60px) = ~1680px,
    // so we allow scrolling up to about 2x screen height before resetting.
    if (creditsScrollY > scrH * 2.2f) {
        creditsScrollY = -scrH * 0.667f;
    }

    // Handle back button click
    if (AEInputCheckTriggered(AEVK_LBUTTON)) {
        if (backButton.isHovered) {
            currentMenuState = MENU_MAIN;
            printf("Returning to main menu\n");
        }
    }

    // Also allow ESC to go back
    if (AEInputCheckTriggered(AEVK_ESCAPE)) {
        currentMenuState = MENU_MAIN;
    }
}

// ==================== DRAW FUNCTIONS =================================================================
void MainMenu_Draw() {
    // Draw background
    DrawBackground();

    switch (currentMenuState) {
    case MENU_MAIN:
        DrawMainMenu();
        break;
    case MENU_INSTRUCTIONS:
        DrawInstructionsMenu();
        break;
    case MENU_CREDITS:
        DrawCreditsMenu();
        break;
    }
}

void DrawBackground() {
    AEGfxTexture* bgTex = AssetManager::GetTexture(TEX_MAIN_MENU_BG);
    AEGfxVertexList* quadMesh = AssetManager::GetMesh(MESH_QUAD);

    if (bgTex && quadMesh) {
        AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
        AEGfxTextureSet(bgTex, 0, 0);

        AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
        AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
        AEGfxSetBlendMode(AE_GFX_BM_BLEND);
        AEGfxSetTransparency(1.0f);

        // Scale the unit quad to fill the entire screen
        AEMtx33 scale, translate, transform;
        AEMtx33Scale(&scale, scrW, scrH);
        AEMtx33Trans(&translate, 0.0f, 0.0f);
        AEMtx33Concat(&transform, &translate, &scale);

        AEGfxSetTransform(transform.m);
        AEGfxMeshDraw(quadMesh, AE_GFX_MDM_TRIANGLES);
    }
    else {
        // Fallback to gradient background
        AEGfxSetBackgroundColor(0.1f, 0.15f, 0.25f);
    }
}

void DrawMainMenu() {
    // --- Draw the title banner image in the top portion of the screen ---
    // Banner width  = ~43.75% of screen width   (700 / 1600)
    // Banner height = ~50%    of screen height   (450 / 900)
    // Banner Y      = ~55.6%  of half-height     (250 / 450)
    f32 titleBannerW = scrW * 0.4375f;
    f32 titleBannerH = scrH * 0.5f;
    f32 titleBannerY = halfH * 0.556f;

    AEGfxTexture*    titleTex  = AssetManager::GetTexture(TEX_TITLE);
    AEGfxVertexList* quadMesh  = AssetManager::GetMesh(MESH_QUAD);

    if (titleTex && quadMesh) {
        AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
        AEGfxTextureSet(titleTex, 0.0f, 0.0f);

        // Use full-brightness white so the texture renders at its original colors
        AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
        AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
        AEGfxSetBlendMode(AE_GFX_BM_BLEND);
        AEGfxSetTransparency(1.0f);

        // Build the transform: scale the unit quad to banner size, then translate up
        AEMtx33 bannerScale, bannerTranslate, bannerTransform;
        AEMtx33Scale(&bannerScale, titleBannerW, titleBannerH);
        AEMtx33Trans(&bannerTranslate, 0.0f, titleBannerY);
        AEMtx33Concat(&bannerTransform, &bannerTranslate, &bannerScale);

        AEGfxSetTransform(bannerTransform.m);
        AEGfxMeshDraw(quadMesh, AE_GFX_MDM_TRIANGLES);
    }
    else {
        // Fallback: render the title as text if the texture failed to load
        if (menuFont >= 0) {
            f32 fallbackTitleY = halfH * 0.711f; // ~320 / 450
            MenuHelpers::drawTextCentered("DEAD WEIGHT", 0.0f, fallbackTitleY, 1.5f, menuFont);
        }
    }

    // Draw all buttons using the black-vertex button mesh from AssetManager
    AEGfxVertexList* btnMesh = AssetManager::GetMesh(MESH_MENU_BUTTON);
    MenuHelpers::drawButton(playButton,         btnMesh, menuFont);
    MenuHelpers::drawButton(instructionsButton,  btnMesh, menuFont);
    MenuHelpers::drawButton(creditsButton,       btnMesh, menuFont);
    MenuHelpers::drawButton(quitButton,          btnMesh, menuFont);
}

void DrawInstructionsMenu() {
    // Draw title -- positioned near top of screen
    f32 titleY = halfH * 0.778f; // ~350 / 450
    if (menuFont >= 0) {
        MenuHelpers::drawTextCentered("INSTRUCTIONS", 0.0f, titleY, 1.3f, menuFont);
    }

    // Draw instruction panel background using the button mesh (color mode)
    AEGfxVertexList* btnMesh = AssetManager::GetMesh(MESH_MENU_BUTTON);

    AEGfxSetRenderMode(AE_GFX_RM_COLOR);
    AEGfxSetColorToAdd(0.2f, 0.2f, 0.3f, 0.85f);

    // Panel dimensions relative to screen size
    f32 panelW = scrW * 0.375f;   // ~600 / 1600
    f32 panelH = scrH * 0.556f;   // ~500 / 900
    f32 panelY = scrH * 0.056f;   // ~50  / 900

    AEMtx33 scale, translate, transform;
    AEMtx33Scale(&scale, panelW, panelH);
    AEMtx33Trans(&translate, 0.0f, panelY);
    AEMtx33Concat(&transform, &translate, &scale);
    AEGfxSetTransform(transform.m);
    AEGfxMeshDraw(btnMesh, AE_GFX_MDM_TRIANGLES);

    // Draw instruction text
    if (menuFont >= 0) {
        f32 baseY       = halfH * 0.444f;   // ~200 / 450
        f32 lineSpacing = scrH * 0.056f;     // ~50  / 900
        f32 textScale   = 0.7f;

        MenuHelpers::drawTextCentered("CONTROLS", 0.0f, baseY, 0.9f, menuFont);

        MenuHelpers::drawTextCentered("SPACEBAR - Jetpack Thrust",   0.0f, baseY - lineSpacing * 1.0f, textScale, menuFont);
        MenuHelpers::drawTextCentered("LEFT CLICK - Fire Projectile", 0.0f, baseY - lineSpacing * 2.0f, textScale, menuFont);
        MenuHelpers::drawTextCentered("MOUSE - Aim Direction",        0.0f, baseY - lineSpacing * 3.0f, textScale, menuFont);
        MenuHelpers::drawTextCentered("ESC - Pause/Quit",             0.0f, baseY - lineSpacing * 4.0f, textScale, menuFont);

        f32 objectiveY = baseY - lineSpacing * 5.5f;
        MenuHelpers::drawTextCentered("OBJECTIVE", 0.0f, objectiveY, 0.9f, menuFont);
        MenuHelpers::drawTextCentered("Use your jetpack and projectiles",        0.0f, objectiveY - lineSpacing, textScale, menuFont);
        MenuHelpers::drawTextCentered("to navigate levels and defeat enemies!",  0.0f, objectiveY - lineSpacing * 2.0f, textScale, menuFont);
    }

    // Draw back button
    MenuHelpers::drawButton(backButton, btnMesh, menuFont);

    // Reset color state so subsequent draw calls are not tinted
    AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
    AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
}

void DrawCreditsMenu() {
    // ---- Fixed "CREDITS" header at the top of the screen ----
    /*
    f32 titleY = halfH * 0.778f; // near top
    if (menuFont >= 0) {
        MenuHelpers::drawTextCentered("CREDITS", 0.0f, titleY, 1.3f, menuFont);
    }
    */

    // ---- Scrolling credits body ----
    // All Y positions are offset by creditsScrollY so the text rolls upward.
    // lineSpacing controls vertical distance between individual lines.
    // sectionGap adds extra space between logical sections.
    if (menuFont >= 0) {
        f32 y              = creditsScrollY;       // current scroll origin
        f32 lineSpacing    = scrH * 0.080f;        // ~54px at 900p
        f32 sectionGap     = scrH * 0.100f;        // ~90px gap between sections
        f32 headerScale    = 1.0f;                 // scale for section headings
        f32 nameScale      = 0.75f;                // scale for individual names
        f32 smallScale     = 0.65f;                // scale for copyright / fine print

        // ---------- Team Name ----------
        MenuHelpers::drawTextCentered("A Game by Team RocketJumperz", 0.0f, y, headerScale, menuFont);
        y -= lineSpacing;
        MenuHelpers::drawTextCentered("Dead Weight", 0.0f, y, nameScale, menuFont);

        // ---------- Development Team ----------
        y -= sectionGap;
        MenuHelpers::drawTextCentered("DEVELOPMENT TEAM", 0.0f, y, headerScale, menuFont);
        y -= lineSpacing;
        MenuHelpers::drawTextCentered("Ivan",      0.0f, y, nameScale, menuFont);
        y -= lineSpacing;
        MenuHelpers::drawTextCentered("Nicholas",  0.0f, y, nameScale, menuFont);
        y -= lineSpacing;
        MenuHelpers::drawTextCentered("Joraye",    0.0f, y, nameScale, menuFont);
        y -= lineSpacing;
        MenuHelpers::drawTextCentered("Jeremiah",  0.0f, y, nameScale, menuFont);

        // ---------- Faculty and Advisors ----------
        y -= sectionGap;
        MenuHelpers::drawTextCentered("FACULTY AND ADVISORS", 0.0f, y, headerScale, menuFont);
        y -= lineSpacing;
        MenuHelpers::drawTextCentered("Gerald Wong",    0.0f, y, nameScale, menuFont);
        y -= lineSpacing;
        MenuHelpers::drawTextCentered("Soroor Malekmohammadi Faradounbeh", 0.0f, y, nameScale, menuFont);
        y -= lineSpacing;
        MenuHelpers::drawTextCentered("Tommy Tan",      0.0f, y, nameScale, menuFont);

        // ---------- DigiPen Logo (drawn as a textured quad) ----------
        y -= sectionGap;
        AEGfxTexture*    logoTex  = AssetManager::GetTexture(TEX_DIGIPEN_LOGO);
        AEGfxVertexList* quadMesh = AssetManager::GetMesh(MESH_QUAD);
        if (logoTex && quadMesh) {
            // Logo dimensions: roughly 20% of screen width, keeping a ~3:1 aspect ratio
            f32 logoW = scrW * 0.20f;
            f32 logoH = logoW * 0.33f;   // wide banner shape

            AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
            AEGfxTextureSet(logoTex, 0.0f, 0.0f);
            AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
            AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
            AEGfxSetBlendMode(AE_GFX_BM_BLEND);
            AEGfxSetTransparency(1.0f);

            AEMtx33 logoScale, logoTrans, logoXform;
            AEMtx33Scale(&logoScale, logoW, logoH);
            AEMtx33Trans(&logoTrans, 0.0f, y);
            AEMtx33Concat(&logoXform, &logoTrans, &logoScale);
            AEGfxSetTransform(logoXform.m);
            AEGfxMeshDraw(quadMesh, AE_GFX_MDM_TRIANGLES);
        }
        y -= lineSpacing;
        MenuHelpers::drawTextCentered("Created at DigiPen Institute of Technology", 0.0f, y, nameScale, menuFont);

        // ---------- President ----------
        y -= sectionGap;
        MenuHelpers::drawTextCentered("PRESIDENT", 0.0f, y, headerScale, menuFont);
        y -= lineSpacing;
        MenuHelpers::drawTextCentered("Claude Comair", 0.0f, y, nameScale, menuFont);

        // ---------- Executives ----------
        y -= sectionGap;
        MenuHelpers::drawTextCentered("EXECUTIVES", 0.0f, y, headerScale, menuFont);
        y -= lineSpacing;
        MenuHelpers::drawTextCentered("Jason Chu",          0.0f, y, nameScale, menuFont);
        y -= lineSpacing;
        MenuHelpers::drawTextCentered("Samir Abou Samra",   0.0f, y, nameScale, menuFont);
        y -= lineSpacing;
        MenuHelpers::drawTextCentered("Michele Comair",     0.0f, y, nameScale, menuFont);
        y -= lineSpacing;
        MenuHelpers::drawTextCentered("Angela Kugler",      0.0f, y, nameScale, menuFont);
        y -= lineSpacing;
        MenuHelpers::drawTextCentered("Erik Mohrmann",      0.0f, y, nameScale, menuFont);
        y -= lineSpacing;
        MenuHelpers::drawTextCentered("Benjamin Ellinger",  0.0f, y, nameScale, menuFont);
        y -= lineSpacing;
        MenuHelpers::drawTextCentered("Melvin Gonsalvez",   0.0f, y, nameScale, menuFont);

        // ---------- Additional Credits ----------
        y -= sectionGap;
        MenuHelpers::drawTextCentered("ADDITIONAL CREDITS", 0.0f, y, headerScale, menuFont);
        y -= lineSpacing;
        MenuHelpers::drawTextCentered("Sprite Assets: Craftpix, itch.io creators", 0.0f, y, nameScale, menuFont);
        y -= lineSpacing;
        MenuHelpers::drawTextCentered("Game Engine: Alpha Engine", 0.0f, y, nameScale, menuFont);

        // ---------- Copyright ----------
        y -= sectionGap;
        MenuHelpers::drawTextCentered("All content (C) 2026 DigiPen Institute of Technology Singapore",
                                       0.0f, y, smallScale, menuFont);
        y -= lineSpacing;
        MenuHelpers::drawTextCentered("All Rights Reserved", 0.0f, y, smallScale, menuFont);
    }

    // ---- Fixed "BACK" button at the bottom of the screen ----
    AEGfxVertexList* btnMesh = AssetManager::GetMesh(MESH_MENU_BUTTON);
    MenuHelpers::drawButton(backButton, btnMesh, menuFont);
}

// ==================== CLEANUP FUNCTIONS ====================
void MainMenu_Free() {
    // Meshes are managed by AssetManager -- free them through the centralized system.
    // Only free the menu-specific button mesh here; MESH_QUAD may be shared with other levels.
    AssetManager::FreeAllMeshes();

    printf("MainMenu_Free: Meshes freed!\n");
}

void MainMenu_Unload() {
    // Textures are managed by AssetManager -- unload through the centralized system
    AssetManager::UnloadAllTextures();

    /*
    if (titleTexture) {
        AEGfxTextureUnload(titleTexture);
        titleTexture = nullptr;
    }
    */

    if (menuFont >= 0) {
        AEGfxDestroyFont(menuFont);
        menuFont = -1;
    }

    printf("MainMenu_Unload: Resources unloaded!\n");
}
