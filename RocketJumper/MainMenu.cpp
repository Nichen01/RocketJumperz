#include "MainMenu.h"
#include "AEEngine.h"
#include "GameStateManager.h"
#include "GameStateList.h"

// Static variables for menu resources
static AEGfxVertexList* buttonMesh = nullptr;
static AEGfxTexture* backgroundTexture = nullptr;
static AEGfxTexture* titleTexture = nullptr;

// Local helper function declarations
static bool isMouseOverButton(f32 btnX, f32 btnY, f32 btnWidth, f32 btnHeight);
static void drawButton(f32 x, f32 y, f32 width, f32 height, bool isHovered);

static f32 playButtonX;
static f32 playButtonY;

static f32 quitButtonX;
static f32 quitButtonY;

static f32 buttonWidth;
static f32 buttonHeight;

void MainMenu_Load()
{
    // Initialize button positions and sizes
    playButtonX = 0.0f;
    playButtonY = 100.0f;
    quitButtonX = 0.0f;
    quitButtonY = -100.0f;
    buttonWidth = 200.0f;
    buttonHeight = 80.0f;

    // Create button mesh
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
}

void MainMenu_Init()
{
    printf("MainMenu_Initialize: Main menu initialized!\n");
}

void MainMenu_Update()
{
    // Get mouse position
    s32 mouseX, mouseY;
    AEInputGetCursorPosition(&mouseX, &mouseY);

    // Convert screen coordinates to world coordinates
    f32 worldMouseX = static_cast<f32>(mouseX) - 640.0f;  // Half of 1280
    f32 worldMouseY = 400.0f - static_cast<f32>(mouseY);   // Half of 800

    // Check if mouse is over Play button
    bool playHovered = isMouseOverButton(playButtonX, playButtonY, buttonWidth, buttonHeight);

    // Check if mouse is over Quit button
    bool quitHovered = isMouseOverButton(quitButtonX, quitButtonY, buttonWidth, buttonHeight);

    // Handle button clicks
    if (AEInputCheckTriggered(AEVK_LBUTTON))
    {
        if (playHovered)
        {
            next = GS_PROJECTILE_TEST;  // Or GS_TEST for Level1
            printf("Play button clicked!\n");
        }
        else if (quitHovered)
        {
            next = GS_QUIT;
            printf("Quit button clicked!\n");
        }
    }
}

void MainMenu_Draw()
{
    // Set background color
    AEGfxSetBackgroundColor(0.1f, 0.1f, 0.2f);

    AEGfxSetRenderMode(AE_GFX_RM_COLOR);
    AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
    AEGfxSetBlendMode(AE_GFX_BM_BLEND);
    AEGfxSetTransparency(1.0f);

    // Get mouse position for hover detection
    s32 mouseX, mouseY;
    AEInputGetCursorPosition(&mouseX, &mouseY);
    f32 worldMouseX = static_cast<f32>(mouseX) - 640.0f;
    f32 worldMouseY = 400.0f - static_cast<f32>(mouseY);

    // Check hover states
    bool playHovered = isMouseOverButton(playButtonX, playButtonY, buttonWidth, buttonHeight);
    bool quitHovered = isMouseOverButton(quitButtonX, quitButtonY, buttonWidth, buttonHeight);

    // Draw Play button
    drawButton(playButtonX, playButtonY, buttonWidth, buttonHeight, playHovered);

    // Draw Quit button
    drawButton(quitButtonX, quitButtonY, buttonWidth, buttonHeight, quitHovered);
}

void MainMenu_Free()
{
    if (buttonMesh)
    {
        AEGfxMeshFree(buttonMesh);
        buttonMesh = nullptr;
    }

    printf("MainMenu_Free: Resources freed!\n");
}

void MainMenu_Unload()
{
    printf("MainMenu_Unload: Main menu unloaded!\n");
}


// 
static bool isMouseOverButton(f32 btnX, f32 btnY, f32 btnWidth, f32 btnHeight)
{
    s32 mouseX, mouseY;
    AEInputGetCursorPosition(&mouseX, &mouseY);

    f32 worldMouseX = static_cast<f32>(mouseX) - 640.0f;
    f32 worldMouseY = 400.0f - static_cast<f32>(mouseY);

    return (worldMouseX >= btnX - btnWidth / 2.0f &&
        worldMouseX <= btnX + btnWidth / 2.0f &&
        worldMouseY >= btnY - btnHeight / 2.0f &&
        worldMouseY <= btnY + btnHeight / 2.0f);
}

// Draw button
static void drawButton(f32 x, f32 y, f32 width, f32 height, bool isHovered)
{
    // Set color based on hover state
    if (isHovered)
    {
        AEGfxSetColorToAdd(0.3f, 0.5f, 1.0f, 1.0f);  // Blue when hovered
    }
    else
    {
        AEGfxSetColorToAdd(0.2f, 0.2f, 0.2f, 1.0f);  // Gray when not hovered
    }

    // Create transformation matrix
    AEMtx33 scale, rotate, translate, transform;
    AEMtx33Scale(&scale, width, height);
    AEMtx33Rot(&rotate, 0.0f);
    AEMtx33Trans(&translate, x, y);

    AEMtx33Concat(&transform, &rotate, &scale);
    AEMtx33Concat(&transform, &translate, &transform);

    AEGfxSetTransform(transform.m);
    AEGfxMeshDraw(buttonMesh, AE_GFX_MDM_TRIANGLES);
}