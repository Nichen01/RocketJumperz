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

// ==================== GLOBAL RESOURCES ====================
static AEGfxVertexList* buttonMesh = nullptr;
static AEGfxVertexList* backgroundMesh = nullptr;
static AEGfxTexture* backgroundTexture = nullptr;
static s8 menuFont = -1;

// Current menu state
static MenuState currentMenuState = MENU_MAIN;

// Credits scroll position
static f32 creditsScrollY = -600.0f;
static const f32 CREDITS_SCROLL_SPEED = 1.0f;

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
static const f32 BUTTON_SCALE_HOVER = 1.15f;
static const f32 BUTTON_SCALE_SPEED = 0.15f;

// ==================== HELPER FUNCTIONS ======================================================================================
namespace MenuHelpers {
    bool isMouseOverButton(const MenuButton& button) {
        s32 mouseX, mouseY;
        AEInputGetCursorPosition(&mouseX, &mouseY);

        f32 worldMouseX = static_cast<f32>(mouseX) - static_cast<f32>(screenWidth / 2);
        f32 worldMouseY = static_cast<f32>(screenLength / 2) - static_cast<f32>(mouseY);

        f32 halfWidth = (button.width * button.scale) / 2.0f;
        f32 halfHeight = (button.height * button.scale) / 2.0f;

        return (worldMouseX >= button.x - halfWidth &&
            worldMouseX <= button.x + halfWidth &&
            worldMouseY >= button.y - halfHeight &&
            worldMouseY <= button.y + halfHeight);
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
    }

    void drawTextCentered(const char* text, f32 x, f32 y, f32 scale, s8 fontID) {
        if (fontID < 0) return; // Font not loaded

        // Get text dimensions
        f32 textWidth, textHeight;
        AEGfxGetPrintSize(fontID, text, scale, &textWidth, &textHeight);

        // Convert world coordinates to screen coordinates for text rendering
        f32 screenX = x + (screenWidth / 2.0f);
        f32 screenY = (screenLength / 2.0f) - y;

        // Draw centered text - use calculated screen positions with centering offset
        AEGfxSetBlendMode(AE_GFX_BM_BLEND);
        AEGfxPrint(fontID, text, screenX - textWidth / 2.0f, screenY - textHeight / 2.0f,
            scale, 1.0f, 1.0f, 1.0f, 1.0f);
    }
}

// ==================== INITIALIZATION FUNCTIONS ========================================================================
void MainMenu_Load() {
    // Create button mesh (rounded rectangle)
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

    // Create background mesh (fullscreen quad)
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

    // Load background texture 
    backgroundTexture = AEGfxTextureLoad("Assets/MainMenu.png");
    if (!backgroundTexture) {
        printf("Warning: MenuBackground.png not found. Using solid color background.\n");
    }

    // Load font 
    menuFont = AEGfxCreateFont("Assets/Fonts/gameover.ttf", 48);
    if (menuFont < 0) {
        printf("(UI) Warning: MenuFont.ttf not found. Text will not render.\n");
    }

    printf("MainMenu_Load: Resources loaded!\n");
}

void MainMenu_Init() {
    // Initialize Main Menu buttons
    playButton = { 0.0f, 150.0f, 300.0f, 80.0f, 1.0f, 1.0f, "PLAY", false };
    instructionsButton = { 0.0f, 30.0f, 300.0f, 80.0f, 1.0f, 1.0f, "INSTRUCTIONS", false };
    creditsButton = { 0.0f, -90.0f, 300.0f, 80.0f, 1.0f, 1.0f, "CREDITS", false };
    quitButton = { 0.0f, -210.0f, 300.0f, 80.0f, 1.0f, 1.0f, "QUIT", false };

    // Initialize back button (used in sub-menus)
    backButton = { 0.0f, -350.0f, 250.0f, 70.0f, 1.0f, 1.0f, "BACK", false };

    // Reset menu state
    currentMenuState = MENU_MAIN;
    creditsScrollY = -600.0f;

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
            next = GS_PROJECTILE_TEST;  // Change to test file if needed
            printf("Play button clicked - Starting game!\n");
        }
        else if (instructionsButton.isHovered) {
            currentMenuState = MENU_INSTRUCTIONS;
            printf("Instructions button clicked!\n");
        }
        else if (creditsButton.isHovered) {
            currentMenuState = MENU_CREDITS;
            creditsScrollY = -600.0f; // Reset scroll position
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

    // Scroll credits upward
    creditsScrollY += CREDITS_SCROLL_SPEED;

    // Reset if scrolled too far
    if (creditsScrollY > 800.0f) {
        creditsScrollY = -600.0f;
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

void DrawMainMenu() {
    // Draw title
    if (menuFont >= 0) {
        MenuHelpers::drawTextCentered("ROCKET JUMPERZ", 0.0f, 320.0f, 1.5f, menuFont);
    }

    // Draw all buttons
    MenuHelpers::drawButton(playButton, buttonMesh, menuFont);
    MenuHelpers::drawButton(instructionsButton, buttonMesh, menuFont);
    MenuHelpers::drawButton(creditsButton, buttonMesh, menuFont);
    MenuHelpers::drawButton(quitButton, buttonMesh, menuFont);
}

void DrawInstructionsMenu() {
    // Draw title
    if (menuFont >= 0) {
        MenuHelpers::drawTextCentered("INSTRUCTIONS", 0.0f, 350.0f, 1.3f, menuFont);
    }

    // Draw instruction panels
    AEGfxSetRenderMode(AE_GFX_RM_COLOR);
    AEGfxSetColorToAdd(0.2f, 0.2f, 0.3f, 0.85f);

    // Create instruction panel background
    AEMtx33 scale, translate, transform;
    AEMtx33Scale(&scale, 600.0f, 500.0f);
    AEMtx33Trans(&translate, 0.0f, 50.0f);
    AEMtx33Concat(&transform, &translate, &scale);
    AEGfxSetTransform(transform.m);
    AEGfxMeshDraw(buttonMesh, AE_GFX_MDM_TRIANGLES);

    // Draw instruction text
    if (menuFont >= 0) {
        f32 baseY = 200.0f;
        f32 lineSpacing = 50.0f;
        f32 textScale = 0.7f;

        MenuHelpers::drawTextCentered("CONTROLS", 0.0f, baseY, 0.9f, menuFont);

        MenuHelpers::drawTextCentered("SPACEBAR - Jetpack Thrust", 0.0f, baseY - lineSpacing * 1, textScale, menuFont);
        MenuHelpers::drawTextCentered("LEFT CLICK - Fire Projectile", 0.0f, baseY - lineSpacing * 2, textScale, menuFont);
        MenuHelpers::drawTextCentered("MOUSE - Aim Direction", 0.0f, baseY - lineSpacing * 3, textScale, menuFont);
        MenuHelpers::drawTextCentered("ESC - Pause/Quit", 0.0f, baseY - lineSpacing * 4, textScale, menuFont);

        MenuHelpers::drawTextCentered("OBJECTIVE", 0.0f, baseY - lineSpacing * 5.5f, 0.9f, menuFont);
        MenuHelpers::drawTextCentered("Use your jetpack and projectiles", 0.0f, baseY - lineSpacing * 6.5f, textScale, menuFont);
        MenuHelpers::drawTextCentered("to navigate levels and defeat enemies!", 0.0f, baseY - lineSpacing * 7.5f, textScale, menuFont);
    }

    // Draw back button
    MenuHelpers::drawButton(backButton, buttonMesh, menuFont);
}

void DrawCreditsMenu() {
    // Draw title
    if (menuFont >= 0) {
        MenuHelpers::drawTextCentered("CREDITS", 0.0f, 350.0f, 1.3f, menuFont);
    }

    // Draw scrolling credits
    if (menuFont >= 0) {
        f32 baseY = creditsScrollY;
        f32 lineSpacing = 60.0f;
        f32 sectionSpacing = 90.0f;

        // Team Section
        MenuHelpers::drawTextCentered("DEVELOPMENT TEAM", 0.0f, baseY, 1.0f, menuFont);
        MenuHelpers::drawTextCentered("Ivan Chong", 0.0f, baseY - lineSpacing, 0.8f, menuFont);
        MenuHelpers::drawTextCentered("Chan Joraye", 0.0f, baseY - lineSpacing * 2, 0.8f, menuFont);

        // Engine Section
        MenuHelpers::drawTextCentered("GAME ENGINE", 0.0f, baseY - sectionSpacing * 2, 1.0f, menuFont);
        MenuHelpers::drawTextCentered("Alpha Engine", 0.0f, baseY - sectionSpacing * 2 - lineSpacing, 0.8f, menuFont);
        MenuHelpers::drawTextCentered("DigiPen Institute of Technology", 0.0f, baseY - sectionSpacing * 2 - lineSpacing * 2, 0.8f, menuFont);

        // Special Thanks
        MenuHelpers::drawTextCentered("SPECIAL THANKS", 0.0f, baseY - sectionSpacing * 3.5f, 1.0f, menuFont);
        MenuHelpers::drawTextCentered("DigiPen Faculty", 0.0f, baseY - sectionSpacing * 3.5f - lineSpacing, 0.8f, menuFont);
        MenuHelpers::drawTextCentered("Playtesters", 0.0f, baseY - sectionSpacing * 3.5f - lineSpacing * 2, 0.8f, menuFont);

        // Copyright
        MenuHelpers::drawTextCentered("Copyright (C) 2026", 0.0f, baseY - sectionSpacing * 5, 0.7f, menuFont);
        MenuHelpers::drawTextCentered("DigiPen Institute of Technology", 0.0f, baseY - sectionSpacing * 5 - lineSpacing * 0.8f, 0.7f, menuFont);
    }

    // Draw back button
    MenuHelpers::drawButton(backButton, buttonMesh, menuFont);
}

// ==================== CLEANUP FUNCTIONS ====================
void MainMenu_Free() {
    if (buttonMesh) {
        AEGfxMeshFree(buttonMesh);
        buttonMesh = nullptr;
    }

    if (backgroundMesh) {
        AEGfxMeshFree(backgroundMesh);
        backgroundMesh = nullptr;
    }

    printf("MainMenu_Free: Meshes freed!\n");
}

void MainMenu_Unload() {
    if (backgroundTexture) {
        AEGfxTextureUnload(backgroundTexture);
        backgroundTexture = nullptr;
    }

    if (menuFont >= 0) {
        AEGfxDestroyFont(menuFont);
        menuFont = -1;
    }

    printf("MainMenu_Unload: Resources unloaded!\n");
}