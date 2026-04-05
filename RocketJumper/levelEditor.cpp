/* Start Header ************************************************************************/
/*!
\file		  LevelEditor.cpp
\author       Chan Joraye (c.joraye)
\date         April, 01, 2026
\brief        Provides functions, resources, and UI logic for building and modifying level maps in the game. Handles tile placement, door linking, traps, enemies, and collectible items. 
			  Includes input handling for editor shortcuts (undo, save, reset), hover states for buttons, and error prompts when invalid placements occur. 
			  Tracks the current game level, tile indices, and action history to support editing and exporting map data back to text files.

			  Copyright (C) 2026 DigiPen Institute of Technology.
			  Reproduction or disclosure of this file or its contents
			  without the prior written consent of DigiPen Institute of
			  Technology is prohibited.
*/
/* End Header **************************************************************************/

#include "LevelEditor.h"

// ==================== GLOBAL RESOURCES ==================== //
static AEGfxTexture* door;
static AEGfxTexture* tileTextures[25];
static const char* pText1{ "Level 1" };
static const char* pText2{ "Level 2" };
static const char* pText3{ "Level 3" };
static s8 font;
int level{ 1 };
s32 currentTileIndex{}, doorCount{ 1 };
static std::vector<TileAction> actionHistory;

// door prompt
static f32 doorPromptAlpha = 1.0f;
static bool showDoorPrompt = false;
static f32 errorPromptAlpha = 1.0f;
static u32 doorID{};

static int promptRow = -1;
static int promptCol = -1;
 
static char strBuffer[100];
static char errorMessage[256];

resetButton resetBtn;

errorPromptButton errorPromptBtn;
static bool enemyExistError = false;
static bool finaldoorLevelError = false;
static bool finaldoorExistError = false;
static bool healthExistError = false;
static bool doorlinkExistError = false;
static bool keycardExistError = false;
static bool wrongKeyLevelError = false;

// ==================== FORWARD DECLARATIONS ====================//
static bool isMouseOverDoorButton(const doorButton& button);
static bool isMouseOverCloseButton(const errorPromptButton& button);
static bool isMouseOverResetButton(const resetButton& button);
static void updateDoorButtonHover(doorButton& button);
static void updateCloseButtonHover(errorPromptButton& button);
static void updateResetButtonHover(resetButton& button);
static void drawDoorButton(const doorButton& button, AEGfxVertexList* mesh, s8 fontID);
static void drawErrorButton(const errorPromptButton& button, AEGfxVertexList* mesh, s8 fontID);
static void drawDoorTextCentered(const char* text, f32 x, f32 y, f32 scale, s8 fontID);
static void drawResetTextCentered(const char* text, f32 x, f32 y, f32 scale, s8 fontID);

// ==================== BUTTON ARRAY ==================== //
static const f32 BUTTON_SCALE_NORMAL = 1.0f;
static const f32 BUTTON_SCALE_HOVER = 1.15f;
static const f32 BUTTON_SCALE_SPEED = 0.15f;
static std::vector<doorButton> buttonArr;

static bool placeFinalDoor = false;

static AEGfxTexture* errorOverlayTex;
static AEGfxTexture* errorCloseTex;

static bool isMouseOverDoorButton(const doorButton& button) {
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

static bool isMouseOverCloseButton(const errorPromptButton& button) {
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

static bool isMouseOverResetButton(const resetButton& button) {
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

static void updateDoorButtonHover(doorButton& button) {
	button.isHovered = isMouseOverDoorButton(button);

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

static void updateCloseButtonHover(errorPromptButton& button) {
	button.isHovered = isMouseOverCloseButton(button);

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

static void updateResetButtonHover(resetButton& button) {
	button.isHovered = isMouseOverResetButton(button);

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

static void drawDoorButton(const doorButton& button, AEGfxVertexList* mesh, s8 fontID) {
	// Draw button background
	AEMtx33 scale, translate, transform;
	AEMtx33Scale(&scale, button.width * button.scale, button.height * button.scale);
	AEMtx33Trans(&translate, button.x, button.y);
	AEMtx33Concat(&transform, &translate, &scale);

	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetColorToMultiply(0.0f, 0.0f, 0.0f, 1.0f);  // black multiply so add color is pure

	// Color based on hover state
	if (button.id == -1) {
		if (button.isHovered) {
			AEGfxSetColorToAdd(0.9f, 0.2f, 0.2f, 0.8f);  // Bright red when hovered
		}
		else {
			AEGfxSetColorToAdd(0.7f, 0.2f, 0.2f, 0.8f);  // Dark red normally
		}
	}
	else {
		if (button.isHovered) {
			AEGfxSetColorToAdd(0.3f, 0.6f, 1.0f, 0.8f);  // Bright blue when hovered
		}
		else {
			AEGfxSetColorToAdd(0.15f, 0.15f, 0.3f, 0.7f); // Dark blue-gray normally
		}
	}

	AEGfxSetTransform(transform.m);
	AEGfxMeshDraw(mesh, AE_GFX_MDM_TRIANGLES);

	// Draw button text
	drawDoorTextCentered(button.text, button.x, button.y, button.scale, fontID);
	AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
}

static void drawErrorButton(const errorPromptButton& button, AEGfxVertexList* mesh, s8 fontID) {
	// Build transform for button quad
	AEMtx33 scale, translate, transform;
	AEMtx33Scale(&scale, button.width * button.scale, button.height * button.scale);
	AEMtx33Trans(&translate, button.x, button.y);
	AEMtx33Concat(&transform, &translate, &scale);

	// Render textured button (redButton)
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxTextureSet(redButton, 0, 0);
	AEGfxSetTransform(transform.m);

	// Apply hover tint
	if (button.isHovered) {
		// Slight brightening tint when hovered
		AEGfxSetColorToAdd(0.2f, 0.2f, 0.2f, 0.3f);
	}
	else {
		// No tint normally
		AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
	}

	AEGfxMeshDraw(mesh, AE_GFX_MDM_TRIANGLES);

	// Reset color state
	AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);

	// Draw button text (e.g. "Close")
	drawDoorTextCentered(button.text, button.x, button.y, button.scale, fontID);
}

static void drawResetButton(const resetButton& button, AEGfxVertexList* mesh, s8 fontID) {
	// Build transform for button quad
	AEMtx33 scale, translate, transform;
	AEMtx33Scale(&scale, button.width * button.scale, button.height * button.scale);
	AEMtx33Trans(&translate, button.x, button.y);
	AEMtx33Concat(&transform, &translate, &scale);

	// Render textured button (redButton)
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxTextureSet(errorOverlayTex, 0, 0);
	AEGfxSetTransform(transform.m);

	// Apply hover tint
	if (button.isHovered) {
		// Slight brightening tint when hovered
		AEGfxSetColorToAdd(0.2f, 0.2f, 0.2f, 0.3f);
	}
	else {
		// No tint normally
		AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
	}

	AEGfxMeshDraw(mesh, AE_GFX_MDM_TRIANGLES);

	// Reset color state
	AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);

	// Draw button text (e.g. "Close")
	drawResetTextCentered(button.text, button.x, button.y, button.scale - 0.3f, fontID);
}

static void drawDoorTextCentered(const char* text, f32 x, f32 y, f32 scale, s8 fontID) {
	AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
	if (fontID < 0) {
		printf("FONT IS NOT LOADED."); return;
	}

	// Get text dimensions in normalized units (0 to 2 range per AE docs)
	f32 textWidth, textHeight;
	AEGfxGetPrintSize(fontID, text, scale, &textWidth, &textHeight);

	// Convert world coordinates to normalized coordinates for AEGfxPrint.
	// AEGfxPrint uses [-1, 1] range where (-1,-1) = bottom-left, (1,1) = top-right.
	// Our world coords have (0,0) = center, so dividing by half-screen gives normalized.
	f32 halfW = screenWidth / 2.0f;
	f32 halfH = screenLength / 2.0f;
	f32 normalizedX = x / halfW;
	f32 normalizedY = y / halfH;

	// Center the text by offsetting half the text dimensions
	f32 printX = normalizedX - textWidth / 2.0f;
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

static void drawResetTextCentered(const char* text, f32 x, f32 y, f32 scale, s8 fontID) {
	AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
	if (fontID < 0) {
		printf("FONT IS NOT LOADED."); return;
	}

	// Get text dimensions in normalized units (0 to 2 range per AE docs)
	f32 textWidth, textHeight;
	AEGfxGetPrintSize(fontID, text, scale, &textWidth, &textHeight);

	// Convert world coordinates to normalized coordinates for AEGfxPrint.
	// AEGfxPrint uses [-1, 1] range where (-1,-1) = bottom-left, (1,1) = top-right.
	// Our world coords have (0,0) = center, so dividing by half-screen gives normalized.
	f32 halfW = screenWidth / 2.0f;
	f32 halfH = screenLength / 2.0f;
	f32 normalizedX = x / halfW;
	f32 normalizedY = y / halfH;

	// Center the text by offsetting half the text dimensions
	f32 printX = normalizedX - textWidth / 2.0f;
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

void LevelEditor_Load() {

	currentGameLevel = 1;
	font = AEGfxCreateFont("Assets/Fonts/gameover.ttf", 50);

	load::ui();
	load::redButtonOption();
	load::brokenDoor();
	// Load sound
	audio::loadsound();

	AssetManager::LoadTexture(TEX_BUTTON, "Assets/UI/Menus/button.png");      // overlay panel
	AssetManager::LoadTexture(TEX_REDBUTTON, "Assets/UI/Menus/redbutton.png"); // close

	errorOverlayTex = AssetManager::GetTexture(TEX_BUTTON);
	redButton = AssetManager::GetTexture(TEX_REDBUTTON);

	// Load textures via AssetManager (enum-based IDs)
	AssetManager::LoadTexture(TEX_PLATFORM1, "Assets/Platform/platform1.png");
	AssetManager::LoadTexture(TEX_PLATFORM2, "Assets/Platform/platform2.png");
	AssetManager::LoadTexture(TEX_PLATFORM3, "Assets/Platform/platform3.png");
	AssetManager::LoadTexture(TEX_PLATFORM4, "Assets/Platform/platform4.png");
	AssetManager::LoadTexture(TEX_PLATFORM5, "Assets/Platform/platform5.png");
	AssetManager::LoadTexture(TEX_PLATFORM6, "Assets/Platform/platform6.png");
	AssetManager::LoadTexture(TEX_PLATFORM7, "Assets/Platform/platform7.png");
	AssetManager::LoadTexture(TEX_PLATFORM8, "Assets/Platform/platform8.png");
	AssetManager::LoadTexture(TEX_PLATFORM9, "Assets/Platform/platform9.png");
	AssetManager::LoadTexture(TEX_STATIC_DOOR, "Assets/Platform/staticDoor.jpg");
	AssetManager::LoadTexture(TEX_KEYCARD, "Assets/Items/keycard.png");
	AssetManager::LoadTexture(TEX_HEALTH, "Assets/Items/health.png");
	AssetManager::LoadTexture(TEX_EYETRAP, "Assets/Items/eye.png");
	AssetManager::LoadTexture(TEX_STATICSAW, "Assets/Traps/staticSaw.png");
	AssetManager::LoadTexture(TEX_RANGED_ENEMY, "Assets/Enemy/RangedEnemy.png");
	AssetManager::LoadTexture(TEX_MUSHROOM_IDLE0, "Assets/Enemy/MushroomIdle/mushroomIdle0.png");
	AssetManager::LoadTexture(TEX_BORDER_TL, "Assets/Platform/borderTL.png");
	AssetManager::LoadTexture(TEX_BORDER_T, "Assets/Platform/borderT.png");
	AssetManager::LoadTexture(TEX_BORDER_TR, "Assets/Platform/borderTR.png");
	AssetManager::LoadTexture(TEX_BORDER_CL, "Assets/Platform/borderCL.png");
	AssetManager::LoadTexture(TEX_BORDER_CR, "Assets/Platform/borderCR.png");
	AssetManager::LoadTexture(TEX_BORDER_BL, "Assets/Platform/borderBL.png");
	AssetManager::LoadTexture(TEX_BORDER_B, "Assets/Platform/borderB.png");
	AssetManager::LoadTexture(TEX_BORDER_BR, "Assets/Platform/borderBR.png");

	tileTextures[0] = AssetManager::GetTexture(TEX_PLATFORM1);
	tileTextures[1] = AssetManager::GetTexture(TEX_PLATFORM2);
	tileTextures[2] = AssetManager::GetTexture(TEX_PLATFORM3);
	tileTextures[3] = AssetManager::GetTexture(TEX_PLATFORM4);
	tileTextures[4] = AssetManager::GetTexture(TEX_PLATFORM5);
	tileTextures[5] = AssetManager::GetTexture(TEX_PLATFORM6);
	tileTextures[6] = AssetManager::GetTexture(TEX_PLATFORM7);
	tileTextures[7] = AssetManager::GetTexture(TEX_PLATFORM8);
	tileTextures[8] = AssetManager::GetTexture(TEX_PLATFORM9);
	tileTextures[9] = AssetManager::GetTexture(TEX_STATIC_DOOR);
	tileTextures[10] = AssetManager::GetTexture(TEX_KEYCARD);
	tileTextures[11] = AssetManager::GetTexture(TEX_HEALTH);
	tileTextures[12] = AssetManager::GetTexture(TEX_BROKENDOOR0);
	tileTextures[13] = AssetManager::GetTexture(TEX_EYETRAP);
	tileTextures[14] = AssetManager::GetTexture(TEX_STATICSAW);
	tileTextures[15] = AssetManager::GetTexture(TEX_RANGED_ENEMY);
	tileTextures[16] = AssetManager::GetTexture(TEX_MUSHROOM_IDLE0);
	tileTextures[17] = AssetManager::GetTexture(TEX_BORDER_TL);
	tileTextures[18] = AssetManager::GetTexture(TEX_BORDER_T);
	tileTextures[19] = AssetManager::GetTexture(TEX_BORDER_TR);
	tileTextures[20] = AssetManager::GetTexture(TEX_BORDER_CL);
	tileTextures[21] = AssetManager::GetTexture(TEX_BORDER_CR);
	tileTextures[22] = AssetManager::GetTexture(TEX_BORDER_BL);
	tileTextures[23] = AssetManager::GetTexture(TEX_BORDER_B);
	tileTextures[24] = AssetManager::GetTexture(TEX_BORDER_BR);
}

void LevelEditor_Initialize() {

	AssetManager::BuildSqrMesh(MESH_QUAD);
	platformMesh = AssetManager::GetMesh(MESH_QUAD);
	uiMesh       = AssetManager::GetMesh(MESH_QUAD);

	// To load the buttons for the door link
	buttonArr.clear();
	std::cout << currentGameLevel;
	switch (currentGameLevel) {
		case 1: {
			ImportMapDataFromFile("Assets/Map/Level1_Map.txt");
			buttonArr.push_back({ 0.f, -120.f, 680.f, 80.f, 1.f, 1.f, "Cancel", false, -1 });
			buttonArr.push_back({ -185.f, 0.f, 300.f, 80.f, 1.f, 1.f, "Tut", false, 0 });
			buttonArr.push_back({ 185.f, 0.f, 300.f, 80.f, 1.f, 1.f, "Level 2", false, 1 });
			break;
		}
		case 2: {
			ImportMapDataFromFile("Assets/Map/Level2_Map.txt");
			buttonArr.push_back({ 0.f, -120.f, 680.f, 80.f, 1.f, 1.f, "Cancel", false, -1 });
			buttonArr.push_back({ -185.f, 0.f, 300.f, 80.f, 1.f, 1.f, "Level 1", false, 1 });
			buttonArr.push_back({ 185.f, 0.f, 300.f, 80.f, 1.f, 1.f, "Level 3", false, 2 });
			break;
		}
		case 3: {
			ImportMapDataFromFile("Assets/Map/Level3_Map.txt");
			buttonArr.push_back({ 0.f, -120.f, 680.f, 80.f, 1.f, 1.f, "Cancel", false, -1 });
			buttonArr.push_back({ 0.f, 0.f, 680.f, 80.f, 1.f, 1.f, "Level 2", false, 2 });
			break;
		}
	}
}

void LevelEditor_Update() {

	if (AEInputCheckCurr(AEVK_LCTRL) && AEInputCheckTriggered(AEVK_1)) {
		level = 1;
		currentGameLevel = 1;
		next = GS_RESTART;
	}
	else if (AEInputCheckCurr(AEVK_LCTRL) && AEInputCheckTriggered(AEVK_2)) {
		level = 2;
		currentGameLevel = 2;
		next = GS_RESTART;
	}
	else if (AEInputCheckCurr(AEVK_LCTRL) && AEInputCheckTriggered(AEVK_3)) {
		level = 3;
		currentGameLevel = 3;
		next = GS_RESTART;
	}
	if (AEInputCheckTriggered(AEVK_L)) next = (level == 1) ? GS_LEVEL1 : (level == 2) ? GS_LEVEL2 : GS_LEVEL3;

	// When user is at the door's tile index, this prompt will be shown to ask the user to pick a level to link to the current level they are at.
	if (showDoorPrompt) {
		for (doorButton& currentButton : buttonArr) {
			updateDoorButtonHover(currentButton);
			if (currentButton.isHovered && AEInputCheckTriggered(AEVK_LBUTTON)) {
				if (currentButton.id == -1) { // Close button
					errorPromptAlpha = 1.0f;
					break;
				}

				// check individual door counter
				bool canPlace = false;
				if (currentButton.id == 0 && tutDoorCount == 0) canPlace = true;
				else if (currentButton.id == 1 && door1Count == 0) canPlace = true;
				else if (currentButton.id == 2 && door2Count == 0) canPlace = true;
				else if (currentButton.id == 3 && door3Count == 0) canPlace = true;

				if (canPlace) {
					TileAction action{};
					action.row = promptRow;
					action.col = promptCol;
					action.prevValue = MapData[promptRow][promptCol];
					action.newValue = 21 + currentButton.id;
					MapData[promptRow][promptCol] = action.newValue;
					actionHistory.push_back(action);

					if (currentButton.id == 0) tutDoorCount++;
					else if (currentButton.id == 1) door1Count++;
					else if (currentButton.id == 2) door2Count++;
					else if (currentButton.id == 3) door3Count++;

					showDoorPrompt = false;
					doorPromptAlpha = 0.f;
				}
				else {
					doorlinkExistError = true;
					AEAudioPlay(Error, soundEffects, 1.f, 1.f, 0);
					showDoorPrompt = false;
					doorPromptAlpha = 0.f;
					errorPromptAlpha = 1.0f;
				}
				break;
			}
		}
	}

	// Cycling through the assets
	if (AEInputCheckTriggered(AEVK_RIGHT)) {
		AEAudioPlay(ArrowSound, soundEffects, 1.f, 1.f, 0);
		currentTileIndex++;
		if (currentTileIndex >= 25) currentTileIndex = 0;
	}
	else if (AEInputCheckTriggered(AEVK_LEFT)) {
		AEAudioPlay(ArrowSound, soundEffects, 1.f, 1.f, 0);
		currentTileIndex--;
		if (currentTileIndex < 0) currentTileIndex = 24;
	}

	// Keybind to undo the action
	if (AEInputCheckCurr(AEVK_LCTRL) && AEInputCheckTriggered(AEVK_Z)) {
		if (!actionHistory.empty()) {
			TileAction last = actionHistory.back();
			actionHistory.pop_back();
			// Restores the action before they assigned it to a new one
			MapData[last.row][last.col] = last.prevValue;
		}
	}

	// Saves and export the array back to the txt file
	if (AEInputCheckCurr(AEVK_LCTRL) && AEInputCheckTriggered(AEVK_S)) {
		if (level == 1) {
			ExportMapDataToFile("Assets/Map/Level1_Map.txt");
		}
		else if (level == 2) {
			ExportMapDataToFile("Assets/Map/Level2_Map.txt");
		}
		else if (level == 3) {
			ExportMapDataToFile("Assets/Map/Level3_Map.txt");
		}
	}

	// Check if there's a keycard when left click & error plays when there's  more than one keycard
	if (currentTileIndex == 10 && AEInputCheckCurr(AEVK_LCTRL) && AEInputCheckTriggered(AEVK_LBUTTON)) {
		if (level == 1 && keyCountLevel1 >= 1) {
			AEAudioPlay(Error, soundEffects, 1, 1, 0);
			keycardExistError = true;
		}
		else if (level == 2 && keyCountLevel2 >= 1) {
			AEAudioPlay(Error, soundEffects, 1, 1, 0);
			keycardExistError = true;
		}
	}

	// If user clicks on the "RESET" button
	if (resetBtn.isHovered && AEInputCheckCurr(AEVK_LBUTTON)) {
		actionHistory.clear();
		ResetMapData();
	}
}

void LevelEditor_Draw() {

    AEGfxSetRenderMode(AE_GFX_RM_COLOR);
    AEGfxSetBlendMode(AE_GFX_BM_NONE);
	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
	// Reset additive color to prevent leftover tint from other draw functions
	AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);

	// Getting mouse coordinates
	s32 mouseX, mouseY;
	AEInputGetCursorPosition(&mouseX, &mouseY);
	float worldMouseX = static_cast<f32>(mouseX) - AEGfxGetWindowWidth() / 2.f;
	float worldMouseY = AEGfxGetWindowHeight() / 2.f - static_cast<f32>(mouseY);

	// Text to print
	f32 levelWidth, levelHeight;
	float offsetX = 10.0f / (AEGfxGetWindowWidth() / 2.0f); 
	float offsetY = 10.0f / (AEGfxGetWindowHeight() / 2.0f);
	if (level == 1) {
		AEGfxGetPrintSize(font, pText1, 2.f, &levelWidth, &levelHeight);
		AEGfxPrint(font, pText1, -1.f + offsetX, -1.f + offsetY, 1, 1, 1, 1, 1);
	}
	else if (level == 2) {
		AEGfxGetPrintSize(font, pText2, 1.f, &levelWidth, &levelHeight);
		AEGfxPrint(font, pText2, -1.f + offsetX, -1.f + offsetY, 1, 1, 1, 1, 1);
	}
	else if (level == 3) {
		AEGfxGetPrintSize(font, pText3, 1.f, &levelWidth, &levelHeight);
		AEGfxPrint(font, pText3, -1.f + offsetX, -1.f + offsetY, 1, 1, 1, 1, 1);
	}

	// Display level in a grid system
	const float tileSizeLE = 65.f;
	const float gap = 3.f;
	for (s32 row = 0; row < BINARY_MAP_HEIGHT; ++row) {
		for (s32 col = 0; col < BINARY_MAP_WIDTH; ++col) {

			float xPos = col * tileSizeLE - 750.f;
			float yPos = -row * tileSizeLE + 410.f;
			float halfSize = (tileSizeLE - gap) / 2.f;

			bool isGridHovered = (worldMouseX >= xPos - halfSize && worldMouseX <= xPos + halfSize &&
				worldMouseY >= yPos - halfSize && worldMouseY <= yPos + halfSize);

			// Checks what grid the mouse is currently hovering on and edits that value in the array
			if (isGridHovered) {
				AEGfxSetColorToMultiply(0.4f, 0.37f, 0.42f, 1.0f); // purple highlight
				if (AEInputCheckCurr(AEVK_LCTRL) && AEInputCheckTriggered(AEVK_LBUTTON)) {
					if (currentTileIndex == 9) { // Door
						promptRow = row;
						promptCol = col;
						showDoorPrompt = true;
						doorPromptAlpha = 1.0f;
					}
					else if (currentTileIndex >= 0 && currentTileIndex <= 8) { // Platforms
						TileAction action;
						action.row = row;
						action.col = col;
						action.prevValue = MapData[row][col];
						action.newValue = 11 + currentTileIndex;

						MapData[row][col] = action.newValue;
						actionHistory.push_back(action);
					}
					else if (currentTileIndex == 10) { // Keycard
						bool placeKey = false;
						if (level == 1 && keyCountLevel1 == 0) placeKey = true;
						else if (level == 2 && keyCountLevel2 == 0) placeKey = true;
						else if (level == 3) wrongKeyLevelError = true;

						if (placeKey) {
							TileAction action;
							action.row = row;
							action.col = col;
							action.prevValue = MapData[row][col];
							action.newValue = 67; // key tile ID

							MapData[row][col] = action.newValue;
							actionHistory.push_back(action);

							// increment the counter right here
							if (level == 1) keyCountLevel1 = 1;
							else if (level == 2) keyCountLevel2 = 1;
						}
					}
					else if (currentTileIndex == 11) { // Healthpack
						bool placeHealth = false;
						if (level == 1 && healthCountLevel1 == 0) placeHealth = true;
						else if (level == 2 && healthCountLevel2 == 0) placeHealth = true;
						else if (level == 3 && healthCountLevel3 == 0) placeHealth = true;

						if (placeHealth) {
							TileAction action;
							action.row = row;
							action.col = col;
							action.prevValue = MapData[row][col];
							action.newValue = 60;

							MapData[row][col] = action.newValue;
							actionHistory.push_back(action);

							if (level == 1) healthCountLevel1++;
							else if (level == 2) healthCountLevel2++;
							else if (level == 3) healthCountLevel3++;
						}
						else {
							healthExistError = true;
							AEAudioPlay(Error, soundEffects, 1.f, 1.f, 0);
						}
					}
					else if (currentTileIndex == 12) { // Final Door
						if (level != 3) {
							finaldoorLevelError = true;
							AEAudioPlay(Error, soundEffects, 1.f, 1.f, 0);
						}
						else {
							if (finalDoorCount == 0) {
								TileAction action;
								action.row = row;
								action.col = col;
								action.prevValue = MapData[row][col];
								action.newValue = 69; // final door tile ID

								MapData[row][col] = action.newValue;
								actionHistory.push_back(action);

								finalDoorCount = 1;
							}
							else {
								finaldoorExistError = true;
								AEAudioPlay(Error, soundEffects, 1.f, 1.f, 0);
							}
						}
					}
					else if (currentTileIndex == 13) { // Suction Trap
						TileAction action;
						action.row = row;
						action.col = col;
						action.prevValue = MapData[row][col];
						action.newValue = 50;

						MapData[row][col] = action.newValue;
						actionHistory.push_back(action);
					}
					else if (currentTileIndex == 14) { // Saw Trap
						TileAction action;
						action.row = row;
						action.col = col;
						action.prevValue = MapData[row][col];
						action.newValue = 51;

						MapData[row][col] = action.newValue;
						actionHistory.push_back(action);
					}
					else if (currentTileIndex == 15) { // Ranged enemy
						bool placeEnemy = false;

						if (level == 1 && rEnemyLevel1 == 0) placeEnemy = true;
						else if (level == 2 && rEnemyLevel2 == 0) placeEnemy = true;
						else if (level == 3 && rEnemyLevel3 == 0) placeEnemy = true;

						if (placeEnemy) {
							TileAction action;
							action.row = row;
							action.col = col;
							action.prevValue = MapData[row][col];
							action.newValue = 81;

							MapData[row][col] = action.newValue;
							actionHistory.push_back(action);

							if (level == 1) rEnemyLevel1 = 1;
							else if (level == 2) rEnemyLevel2 = 1;
							else if (level == 3) rEnemyLevel3 = 1;
						}
						else {
							enemyExistError = true;
							AEAudioPlay(Error, soundEffects, 1.f, 1.f, 0);
						}
					}
					else if (currentTileIndex == 16) { // Melee Enemy
						bool placeEnemy = false;

						if (level == 1 && mEnemyLevel1 == 0) placeEnemy = true;
						else if (level == 2 && mEnemyLevel2 == 0) placeEnemy = true;
						else if (level == 3 && mEnemyLevel3 == 0) placeEnemy = true;

						if (placeEnemy) {
							TileAction action;
							action.row = row;
							action.col = col;
							action.prevValue = MapData[row][col];
							action.newValue = 82;

							MapData[row][col] = action.newValue;
							actionHistory.push_back(action);

							if (level == 1) mEnemyLevel1 = 1;
							else if (level == 2) mEnemyLevel2 = 1;
							else if (level == 3) mEnemyLevel3 = 1;
						}
						else {
							enemyExistError = true;
							AEAudioPlay(Error, soundEffects, 1.f, 1.f, 0);
						}
					}
					else if (currentTileIndex >= 17 && currentTileIndex <= 24) { // Aesthetic Tiles
						TileAction action;
						action.row = row;
						action.col = col;
						action.prevValue = MapData[row][col];
						action.newValue = 54 + currentTileIndex;

						MapData[row][col] = action.newValue;
						actionHistory.push_back(action);
					}
				}
				if (AEInputCheckTriggered(AEVK_RBUTTON)) { // Right Click to Erase
					TileAction action;
					action.row = row;
					action.col = col;
					action.prevValue = MapData[row][col];
					action.newValue = 0;

					if (MapData[row][col] == 21) {
						tutDoorCount = 0;
					}
					else if (MapData[row][col] == 22) {
						door1Count = 0;
					}
					else if (MapData[row][col] == 23) {
						door2Count = 0;
					}
					else if (MapData[row][col] == 24) {
						door2Count = 0;
					}
					else if (MapData[row][col] == 67) {
						if (level == 1) keyCountLevel1 = 0;
						else if (level == 2) keyCountLevel2 = 0;
					}
					else if (MapData[row][col] == 69) {
						finalDoorCount = 0;
					}
					else if (MapData[row][col] == 60) {
						if (level == 1) healthCountLevel1 = 0;
						else if (level == 2) healthCountLevel2 = 0;
						else if (level == 3) healthCountLevel3 = 0;
					}
					else if (MapData[row][col] == 81) {
						if (level == 1) rEnemyLevel1 = 0;
						else if (level == 2) rEnemyLevel2 = 0;
						else if (level == 3) rEnemyLevel3 = 0;
					}
					else if (MapData[row][col] == 82) {
						if (level == 1) mEnemyLevel1 = 0;
						else if (level == 2) mEnemyLevel2 = 0;
						else if (level == 3) mEnemyLevel3 = 0;
					}

					MapData[row][col] = action.newValue;
					actionHistory.push_back(action);
				}
			}
			// Drawing the colors in the LE
			else if (MapData[row][col] >= 11 && MapData[row][col] <= 19) {
				AEGfxSetColorToMultiply(0.49f, 0.49f, 0.49f, 1.0f); // dark gray
			}
			else if (MapData[row][col] <= 29 && MapData[row][col] >= 21) {
				AEGfxSetColorToMultiply(0.57f, 0.76f, 0.72f, 1.f);
			}
			else if (MapData[row][col] == 60) { // healthpack
				AEGfxSetColorToMultiply(0.5f, 0.8f, 0.5f, 1.f);
			}
			else if (MapData[row][col] == 67) { // key
				AEGfxSetColorToMultiply(0.49f, 0.5f, 0.45f, 1.f);
			}
			else if (MapData[row][col] == 69) { // broken door
				AEGfxSetColorToMultiply(0.5f, 0.41f, 0.65f, 1.f);
			}
			else if (MapData[row][col] == 50 || MapData[row][col] == 51) { // traps
				AEGfxSetColorToMultiply(0.42f, 0.59f, 0.72f, 1.f);
			}
			else if (MapData[row][col] <= 79 && MapData[row][col] >= 71) { // aesthetic walls
				AEGfxSetColorToMultiply(0.83f, 0.79f, 0.69f, 1.f);
			}
			else if (MapData[row][col] == 81 || MapData[row][col] == 82) { // enemy
				AEGfxSetColorToMultiply(0.59f, 0.4f, 0.4f, 1.f);
			}
			else {
				AEGfxSetColorToMultiply(0.8f, 0.8f, 0.8f, 1.0f); // default light gray
			}


			AEMtx33 scl, rot, transl, transf;
			AEMtx33Scale(&scl, tileSizeLE - gap, tileSizeLE - gap);
			AEMtx33Rot(&rot, 0);
			AEMtx33Trans(&transl, xPos, yPos);

			AEMtx33Concat(&transf, &rot, &scl);
			AEMtx33Concat(&transf, &transl, &transf);

			AEGfxSetTransform(transf.m);
			AEGfxMeshDraw(platformMesh, AE_GFX_MDM_TRIANGLES);

			float textW, textH;

			// On hover features
			if (isGridHovered && (MapData[row][col] >= 11 && MapData[row][col] <= 19)) {
				// normalize tile center
				AEGfxGetPrintSize(font, "Tile", 0.4f, &textW, &textH);
				float normX = xPos / (AEGfxGetWindowWidth() / 2.0f) - textW / 2.0f;
				float normY = yPos / (AEGfxGetWindowHeight() / 2.0f) - textH / 2.0f;
				AEGfxPrint(font, "Tile", normX, normY, 0.4f, 1, 1, 1, 1);
			}
			else if (isGridHovered && (MapData[row][col] >= 21 && MapData[row][col] <= 29)) {
				// normalize tile center
				AEGfxGetPrintSize(font, "Door", 0.4f, &textW, &textH);
				float normX = xPos / (AEGfxGetWindowWidth() / 2.0f) - textW / 2.0f;
				float normY = yPos / (AEGfxGetWindowHeight() / 2.0f) - textH / 2.0f;
				AEGfxPrint(font, "Door", normX, normY, 0.4f, 1, 1, 1, 1);
			}
			else if (isGridHovered && MapData[row][col] == 60) {
				// normalize tile center
				AEGfxGetPrintSize(font, "Health", 0.4f, &textW, &textH);
				float normX = xPos / (AEGfxGetWindowWidth() / 2.0f) - textW / 2.0f;
				float normY = yPos / (AEGfxGetWindowHeight() / 2.0f) - textH / 2.0f;
				AEGfxPrint(font, "Health", normX, normY, 0.4f, 1, 1, 1, 1);
			}
			else if (isGridHovered && MapData[row][col] == 67) { // key
				// normalize tile center
				AEGfxGetPrintSize(font, "Key", 0.4f, &textW, &textH);
				float normX = xPos / (AEGfxGetWindowWidth() / 2.0f) - textW / 2.0f;
				float normY = yPos / (AEGfxGetWindowHeight() / 2.0f) - textH / 2.0f;
				AEGfxPrint(font, "Key", normX, normY, 0.4f, 1, 1, 1, 1);
			}
			else if (isGridHovered && MapData[row][col] == 69) { // final door
				// normalize tile center
				AEGfxGetPrintSize(font, "Final", 0.4f, &textW, &textH);
				float normX = xPos / (AEGfxGetWindowWidth() / 2.0f) - textW / 2.0f;
				float normY = yPos / (AEGfxGetWindowHeight() / 2.0f) - textH / 2.0f;
				AEGfxPrint(font, "Final", normX, normY + 0.02f, 0.4f, 1, 1, 1, 1);
				AEGfxGetPrintSize(font, "Door", 0.4f, &textW, &textH);
				AEGfxPrint(font, "Door", normX, normY - 0.03f, 0.4f, 1, 1, 1, 1);
			}
			else if (isGridHovered && (MapData[row][col] == 50 || MapData[row][col] == 51)) { // traps
				// normalize tile center
				AEGfxGetPrintSize(font, "Trap", 0.4f, &textW, &textH);
				float normX = xPos / (AEGfxGetWindowWidth() / 2.0f) - textW / 2.0f;
				float normY = yPos / (AEGfxGetWindowHeight() / 2.0f) - textH / 2.0f;
				AEGfxPrint(font, "Trap", normX, normY, 0.4f, 1, 1, 1, 1);
			}
			else if (isGridHovered && (MapData[row][col] <= 79 && MapData[row][col] >= 71)) { // aesthetic walls
				// normalize tile center
				AEGfxGetPrintSize(font, "Deco", 0.4f, &textW, &textH);
				float normX = xPos / (AEGfxGetWindowWidth() / 2.0f) - textW / 2.0f;
				float normY = yPos / (AEGfxGetWindowHeight() / 2.0f) - textH / 2.0f;
				AEGfxPrint(font, "Deco", normX, normY, 0.4f, 1, 1, 1, 1);
			}
			else if (isGridHovered && (MapData[row][col] == 81 || MapData[row][col] == 82)) { // enemy
				// normalize tile center
				AEGfxGetPrintSize(font, "Enemy", 0.4f, &textW, &textH);
				float normX = xPos / (AEGfxGetWindowWidth() / 2.0f) - textW / 2.0f;
				float normY = yPos / (AEGfxGetWindowHeight() / 2.0f) - textH / 2.0f;
				AEGfxPrint(font, "Enemy", normX, normY, 0.4f, 1, 1, 1, 1);
			}
			else if (!isGridHovered && (MapData[row][col] == 11 || MapData[row][col] == 21)) {
				float normX = xPos / (AEGfxGetWindowWidth() / 2.0f);
				float normY = yPos / (AEGfxGetWindowHeight() / 2.0f);
				AEGfxPrint(font, "1", normX, normY, 0.5f, 1, 1, 1, 1);
			}
			else if (!isGridHovered && (MapData[row][col] == 12 || MapData[row][col] == 22)) {
				float normX = xPos / (AEGfxGetWindowWidth() / 2.0f);
				float normY = yPos / (AEGfxGetWindowHeight() / 2.0f);
				AEGfxPrint(font, "2", normX, normY, 0.5f, 1, 1, 1, 1);
			}
			else if (!isGridHovered && (MapData[row][col] == 13 || MapData[row][col] == 23)) {
				float normX = xPos / (AEGfxGetWindowWidth() / 2.0f);
				float normY = yPos / (AEGfxGetWindowHeight() / 2.0f);
				AEGfxPrint(font, "3", normX, normY, 0.5f, 1, 1, 1, 1);
			}
			else if (!isGridHovered && (MapData[row][col] == 14 || MapData[row][col] == 24)) {
				float normX = xPos / (AEGfxGetWindowWidth() / 2.0f);
				float normY = yPos / (AEGfxGetWindowHeight() / 2.0f);
				AEGfxPrint(font, "4", normX, normY, 0.5f, 1, 1, 1, 1);
			}
			else if (!isGridHovered && (MapData[row][col] == 15 || MapData[row][col] == 25)) {
				float normX = xPos / (AEGfxGetWindowWidth() / 2.0f);
				float normY = yPos / (AEGfxGetWindowHeight() / 2.0f);
				AEGfxPrint(font, "5", normX, normY, 0.5f, 1, 1, 1, 1);
			}
			else if (!isGridHovered && (MapData[row][col] == 16 || MapData[row][col] == 26)) {
				float normX = xPos / (AEGfxGetWindowWidth() / 2.0f);
				float normY = yPos / (AEGfxGetWindowHeight() / 2.0f);
				AEGfxPrint(font, "6", normX, normY, 0.5f, 1, 1, 1, 1);
			}
			else if (!isGridHovered && (MapData[row][col] == 17 || MapData[row][col] == 27)) {
				float normX = xPos / (AEGfxGetWindowWidth() / 2.0f);
				float normY = yPos / (AEGfxGetWindowHeight() / 2.0f);
				AEGfxPrint(font, "7", normX, normY, 0.5f, 1, 1, 1, 1);
			}
			else if (!isGridHovered && (MapData[row][col] == 18 || MapData[row][col] == 28)) {
				float normX = xPos / (AEGfxGetWindowWidth() / 2.0f);
				float normY = yPos / (AEGfxGetWindowHeight() / 2.0f);
				AEGfxPrint(font, "8", normX, normY, 0.5f, 1, 1, 1, 1);
			}
			else if (!isGridHovered && (MapData[row][col] == 19 || MapData[row][col] == 29)) {
				float normX = xPos / (AEGfxGetWindowWidth() / 2.0f);
				float normY = yPos / (AEGfxGetWindowHeight() / 2.0f);
				AEGfxPrint(font, "9", normX, normY, 0.5f, 1, 1, 1, 1);
			}
			else if (!isGridHovered && currentTileIndex == 13) {
				if (MapData[row][col] == 50) {
					float W, H;
					AEGfxGetPrintSize(font, "This", 0.5f, &W, &H);
					float normX = xPos / (AEGfxGetWindowWidth() / 2.0f) - W / 2.0f;
					float normY = yPos / (AEGfxGetWindowHeight() / 2.0f) - H / 2.0f;
					AEGfxPrint(font, "This", normX, normY, 0.5f, 1, 1, 1, 1);
				}
			}
			else if (!isGridHovered && currentTileIndex == 14) {
				if (MapData[row][col] == 51) {
					float W, H;
					AEGfxGetPrintSize(font, "This", 0.5f, &W, &H);
					float normX = xPos / (AEGfxGetWindowWidth() / 2.0f) - W / 2.0f;
					float normY = yPos / (AEGfxGetWindowHeight() / 2.0f) - H / 2.0f;
					AEGfxPrint(font, "This", normX, normY, 0.5f, 1, 1, 1, 1);
				}
			}
		}
	}

	// to print the asset that user is currently at
	AEMtx33 tileScl, tileRot, tileTransl, tileTransf;
	AEMtx33Scale(&tileScl, 60.f, 60.f);
	AEMtx33Rot(&tileRot, 0);
	AEMtx33Trans(&tileTransl, -100.f, -330.f);

	AEMtx33Concat(&tileTransf, &tileRot, &tileScl);
	AEMtx33Concat(&tileTransf, &tileTransl, &tileTransf);

	// Reset color modifiers before rendering textured tile preview
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(1.0f);
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);

	// Drawing the asset when the user cycles through the LE
	if (currentTileIndex >= 0 && currentTileIndex < 25 && tileTextures[currentTileIndex]) {
		AEGfxTextureSet(tileTextures[currentTileIndex], 0, 0);
	}

	AEGfxSetTransform(tileTransf.m);
	AEGfxMeshDraw(platformMesh, AE_GFX_MDM_TRIANGLES);

	// Printing the text below to render the specific words
	memset(strBuffer, 0, 100 * sizeof(char));
	f32 tileTextWidth, tileTextHeight;
	AEGfxGetPrintSize(font, strBuffer, 0.3f, &tileTextWidth, &tileTextHeight);
	// to print the text below the asset
	switch (currentTileIndex) {
	case 0:
		sprintf_s(strBuffer, "Top Left Corner");
		AEGfxPrint(font, strBuffer, -0.3f, -0.9f, 0.7f, 1.f, 1.f, 1.f, 1.f);
		break;
	case 1:
		sprintf_s(strBuffer, "Top Middle");
		AEGfxPrint(font, strBuffer, -0.24f, -0.9f, 0.7f, 1.f, 1.f, 1.f, 1.f);
		break;
	case 2:
		sprintf_s(strBuffer, "Top Right Corner");
		AEGfxPrint(font, strBuffer, -0.32f, -0.9f, 0.7f, 1.f, 1.f, 1.f, 1.f);
		break;
	case 3:
		sprintf_s(strBuffer, "Middle Left");
		AEGfxPrint(font, strBuffer, -0.25f, -0.9f, 0.7f, 1.f, 1.f, 1.f, 1.f);
		break;
	case 4:
		sprintf_s(strBuffer, "Middle");
		AEGfxPrint(font, strBuffer, -0.2f, -0.9f, 0.7f, 1.f, 1.f, 1.f, 1.f);
		break;
	case 5:
		sprintf_s(strBuffer, "Middle Right");
		AEGfxPrint(font, strBuffer, -0.26f, -0.9f, 0.7f, 1.f, 1.f, 1.f, 1.f);
		break;
	case 6:
		sprintf_s(strBuffer, "Bottom Left Corner");
		AEGfxPrint(font, strBuffer, -0.33f, -0.9f, 0.7f, 1.f, 1.f, 1.f, 1.f);
		break;
	case 7:
		sprintf_s(strBuffer, "Bottom Middle");
		AEGfxPrint(font, strBuffer, -0.28f, -0.9f, 0.7f, 1.f, 1.f, 1.f, 1.f);
		break;
	case 8:
		sprintf_s(strBuffer, "Bottom Right Corner");
		AEGfxPrint(font, strBuffer, -0.35f, -0.9f, 0.7f, 1.f, 1.f, 1.f, 1.f);
		break;
	case 9:
		sprintf_s(strBuffer, "Door");
		AEGfxPrint(font, strBuffer, -0.18f, -0.9f, 0.7f, 1.f, 1.f, 1.f, 1.f);
		break;
	case 10:
		sprintf_s(strBuffer, "Key");
		AEGfxPrint(font, strBuffer, -0.17f, -0.9f, 0.7f, 1.f, 1.f, 1.f, 1.f);
		break;
	case 11:
		sprintf_s(strBuffer, "Health Pack");
		AEGfxPrint(font, strBuffer, -0.25f, -0.9f, 0.7f, 1.f, 1.f, 1.f, 1.f);
		break;
	case 12:
		sprintf_s(strBuffer, "Final Door");
		AEGfxPrint(font, strBuffer, -0.23f, -0.9f, 0.7f, 1.f, 1.f, 1.f, 1.f);
		break;
	case 13:
		sprintf_s(strBuffer, "Trap: Pulls Player towards it");
		AEGfxPrint(font, strBuffer, -0.43f, -0.9f, 0.7f, 1.f, 1.f, 1.f, 1.f);
		break;
	case 14:
		sprintf_s(strBuffer, "Trap: Damages Player");
		AEGfxPrint(font, strBuffer, -0.35f, -0.9f, 0.7f, 1.f, 1.f, 1.f, 1.f);
		break;
	case 15:
		sprintf_s(strBuffer, "Ranged Enemy");
		AEGfxPrint(font, strBuffer, -0.28f, -0.9f, 0.7f, 1.f, 1.f, 1.f, 1.f);
		break;
	case 16:
		sprintf_s(strBuffer, "Melee Enemy");
		AEGfxPrint(font, strBuffer, -0.26f, -0.9f, 0.7f, 1.f, 1.f, 1.f, 1.f);
		break;
	case 17:
		sprintf_s(strBuffer, "Decoration: Top Left");
		AEGfxPrint(font, strBuffer, -0.35f, -0.9f, 0.7f, 1.f, 1.f, 1.f, 1.f);
		break;
	case 18:
		sprintf_s(strBuffer, "Decoration: Top Middle");
		AEGfxPrint(font, strBuffer, -0.37f, -0.9f, 0.7f, 1.f, 1.f, 1.f, 1.f);
		break;
	case 19:
		sprintf_s(strBuffer, "Decoration: Top Right");
		AEGfxPrint(font, strBuffer, -0.36f, -0.9f, 0.7f, 1.f, 1.f, 1.f, 1.f);
		break;
	case 20:
		sprintf_s(strBuffer, "Decoration: Middle Left");
		AEGfxPrint(font, strBuffer, -0.38f, -0.9f, 0.7f, 1.f, 1.f, 1.f, 1.f);
		break;
	case 21:
		sprintf_s(strBuffer, "Decoration: Middle Right");
		AEGfxPrint(font, strBuffer, -0.38f, -0.9f, 0.7f, 1.f, 1.f, 1.f, 1.f);
		break;
	case 22:
		sprintf_s(strBuffer, "Decoration: Bottom Left");
		AEGfxPrint(font, strBuffer, -0.4f, -0.9f, 0.7f, 1.f, 1.f, 1.f, 1.f);
		break;
	case 23:
		sprintf_s(strBuffer, "Decoration: Bottom Middle");
		AEGfxPrint(font, strBuffer, -0.4f, -0.9f, 0.7f, 1.f, 1.f, 1.f, 1.f);
		break;
	case 24:
		sprintf_s(strBuffer, "Decoration: Bottom Right");
		AEGfxPrint(font, strBuffer, -0.4f, -0.9f, 0.7f, 1.f, 1.f, 1.f, 1.f);
		break;
	}

	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	
	renderlogic::drawTexture(-170.f, -330.f, leftArrow, uiMesh, 50.f, 50.f);
	renderlogic::drawTexture(-20.f, -330.f, rightArrow, uiMesh, 50.f, 50.f);
	renderlogic::drawTexture(570.f, 400.f, ctrl1, uiMesh, 50.f, 50.f);
	renderlogic::drawTexture(620.f, 400.f, ctrl2, uiMesh, 50.f, 50.f);
	renderlogic::drawTexture(670.f, 400.f, leftClick, uiMesh, 50.f, 50.f);
	renderlogic::drawTexture(570.f, 280.f, rightClick, uiMesh, 50.f, 50.f);
	renderlogic::drawTexture(570.f, 170.f, ctrl1, uiMesh, 50.f, 50.f);
	renderlogic::drawTexture(620.f, 170.f, ctrl2, uiMesh, 50.f, 50.f);
	renderlogic::drawTexture(670.f, 170.f, sButton, uiMesh, 50.f, 50.f);
	renderlogic::drawTexture(570.f, 280.f, rightClick, uiMesh, 50.f, 50.f);
	renderlogic::drawTexture(570.f, 60.f, ctrl1, uiMesh, 50.f, 50.f);
	renderlogic::drawTexture(620.f, 60.f, ctrl2, uiMesh, 50.f, 50.f);
	renderlogic::drawTexture(670.f, 60.f, zButton, uiMesh, 50.f, 50.f);
	renderlogic::drawTexture(570.f, -60.f, ctrl1, uiMesh, 50.f, 50.f);
	renderlogic::drawTexture(620.f, -60.f, ctrl2, uiMesh, 50.f, 50.f);
	renderlogic::drawTexture(650.f, -60.f, button1, uiMesh, 45.f, 50.f);
	renderlogic::drawTexture(690.f, -60.f, button2, uiMesh, 45.f, 45.f);
	renderlogic::drawTexture(730.f, -60.f, button3, uiMesh, 45.f, 45.f);
	renderlogic::drawTexture(570.f, -210.f, lButton, uiMesh, 50.f, 50.f);

	// UI TEXT
	f32 uiTextWidth, uiTextHeight;
	AEGfxGetPrintSize(font, strBuffer, 0.5f, &uiTextWidth, &uiTextHeight);
	sprintf_s(strBuffer, "CTRL + Left Click - Set");
	AEGfxPrint(font, strBuffer, 0.67f, 0.76f, 0.4f, 1.f, 1.f, 1.f, 1.f);

	sprintf_s(strBuffer, "Right Click - Clear");
	AEGfxPrint(font, strBuffer, 0.67f, 0.5f, 0.4f, 1.f, 1.f, 1.f, 1.f);

	sprintf_s(strBuffer, "CTRL + S - Save");
	AEGfxPrint(font, strBuffer, 0.67f, 0.25f, 0.4f, 1.f, 1.f, 1.f, 1.f);

	sprintf_s(strBuffer, "CTRL + Z - Undo");
	AEGfxPrint(font, strBuffer, 0.67f, 0.f, 0.4f, 1.f, 1.f, 1.f, 1.f);

	sprintf_s(strBuffer, "CTRL + (Number)");
	AEGfxPrint(font, strBuffer, 0.67f, -0.3f, 0.4f, 1.f, 1.f, 1.f, 1.f);

	sprintf_s(strBuffer, "- Level");
	AEGfxPrint(font, strBuffer, 0.67f, -0.35f, 0.4f, 1.f, 1.f, 1.f, 1.f);

	sprintf_s(strBuffer, "L - Return to Level");
	AEGfxPrint(font, strBuffer, 0.67f, -0.6f, 0.4f, 1.f, 1.f, 1.f, 1.f);

	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	AEGfxSetBlendMode(AE_GFX_BM_NONE);   // no blending, just solid fill
	AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);

	// door prompt background box
	if (showDoorPrompt && doorPromptAlpha > 0.0f) {
		// Main prompt panel
		float boxWidth = 850.0f;
		float boxHeight = 400.0f;
		AEGfxSetColorToMultiply(0.f, 0.f, 0.f, doorPromptAlpha); // dark blue background
		renderlogic::drawSquare(0, 0, boxWidth, boxHeight);
		AEGfxMeshDraw(platformMesh, AE_GFX_MDM_TRIANGLES);

		AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
		AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);

		// Title text
		AEGfxPrint(font, "What level does this door lead to?", -0.44f, 0.25f, 0.8f, 1, 1, 1, 1);

		// Assign the buttons

		for (doorButton& currentButton : buttonArr) {
			drawDoorButton(currentButton, uiMesh, font);
		}
	}

	// Printing the error prompt
	if (errorPromptAlpha > 0.0f) {
		if (showDoorPrompt) {
			for (doorButton& currentButton : buttonArr) {
				updateDoorButtonHover(currentButton);
				if (currentButton.isHovered && AEInputCheckTriggered(AEVK_LBUTTON)) {
					if (currentButton.id == -1) { // Cancel button
						showDoorPrompt = false;
						doorPromptAlpha = 0.f;
						doorlinkExistError = false;
						errorPromptAlpha = 0.f;
						break;
					}
					// check individual door counter
					bool canPlace = false;
					if (currentButton.id == 0 && tutDoorCount == 0) canPlace = true;
					else if (currentButton.id == 1 && door1Count == 0) canPlace = true;
					else if (currentButton.id == 2 && door2Count == 0) canPlace = true;
					else if (currentButton.id == 3 && door3Count == 0) canPlace = true;

					if (canPlace) {
						TileAction action{};
						action.row = promptRow;
						action.col = promptCol;
						action.prevValue = MapData[promptRow][promptCol];
						action.newValue = 21 + currentButton.id;
						MapData[promptRow][promptCol] = action.newValue;
						actionHistory.push_back(action);

						if (currentButton.id == 0) tutDoorCount++;
						else if (currentButton.id == 1) door1Count++;
						else if (currentButton.id == 2) door2Count++;
						else if (currentButton.id == 3) door3Count++;

						showDoorPrompt = false;
						doorPromptAlpha = 0.f;
					}
					else {
						AEAudioPlay(Error, soundEffects, 1.f, 1.f, 0);
						showDoorPrompt = false;
						doorPromptAlpha = 0.f;
					}
					break;
				}
			}
		}
	}

	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(1.0f);
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);

	// ERROR PROMPT for different conditions
	if (keycardExistError) {
		renderlogic::drawTexture(0.f, 0.f, errorOverlayTex, uiMesh, 855.f, 240.f);
		sprintf_s(errorMessage, "Keycard already exists!");
		AEGfxPrint(font, errorMessage, -0.37f, -0.05f, 1.f, 1.f, 1.f, 1.f, 1.f);
		drawErrorButton(errorPromptBtn, uiMesh, font);
		updateCloseButtonHover(errorPromptBtn);

		if (AEInputCheckTriggered(AEVK_LBUTTON) && errorPromptBtn.isHovered) {
			keycardExistError = false;
			errorPromptAlpha = 0.f;
		}
	}
	if (wrongKeyLevelError) {
		renderlogic::drawTexture(0.f, 0.f, errorOverlayTex, uiMesh, 855.f, 240.f);
		sprintf_s(errorMessage, "Keycard can't be placed in this level!");
		AEGfxPrint(font, errorMessage, -0.4f, -0.05f, 0.7f, 1.f, 1.f, 1.f, 1.f);
		drawErrorButton(errorPromptBtn, uiMesh, font);
		updateCloseButtonHover(errorPromptBtn);

		if (AEInputCheckTriggered(AEVK_LBUTTON) && errorPromptBtn.isHovered) {
			wrongKeyLevelError = false;
			errorPromptAlpha = 0.f;
		}
	}
	if (doorlinkExistError) {
		renderlogic::drawTexture(0.f, 0.f, errorOverlayTex, uiMesh, 855.f, 240.f);
		sprintf_s(errorMessage, "Door already exists!");
		AEGfxPrint(font, errorMessage, -0.31f, -0.05f, 1.f, 1.f, 1.f, 1.f, 1.f);
		drawErrorButton(errorPromptBtn, uiMesh, font);
		updateCloseButtonHover(errorPromptBtn);

		if (AEInputCheckTriggered(AEVK_LBUTTON) && errorPromptBtn.isHovered) {
			doorlinkExistError = false;
			errorPromptAlpha = 0.f;
		}
	}
	if (healthExistError) {
		renderlogic::drawTexture(0.f, 0.f, errorOverlayTex, uiMesh, 855.f, 240.f);
		sprintf_s(errorMessage, "Health Pack already exists!");
		AEGfxPrint(font, errorMessage, -0.43f, -0.05f, 1.f, 1.f, 1.f, 1.f, 1.f);
		drawErrorButton(errorPromptBtn, uiMesh, font);
		updateCloseButtonHover(errorPromptBtn);

		if (AEInputCheckTriggered(AEVK_LBUTTON) && errorPromptBtn.isHovered) {
			healthExistError = false;
			errorPromptAlpha = 0.f;
		}
	}
	if (finaldoorLevelError) {
		renderlogic::drawTexture(0.f, 0.f, errorOverlayTex, uiMesh, 855.f, 240.f);
		sprintf_s(errorMessage, "Door can't be placed in this level!");
		AEGfxPrint(font, errorMessage, -0.42f, -0.05f, 0.8f, 1.f, 1.f, 1.f, 1.f);
		drawErrorButton(errorPromptBtn, uiMesh, font);
		updateCloseButtonHover(errorPromptBtn);

		if (AEInputCheckTriggered(AEVK_LBUTTON) && errorPromptBtn.isHovered) {
			finaldoorLevelError = false;
			errorPromptAlpha = 0.f;
		}
	}
	if (finaldoorExistError) {
		renderlogic::drawTexture(0.f, 0.f, errorOverlayTex, uiMesh, 855.f, 240.f);
		sprintf_s(errorMessage, "Final Door already exists!");
		AEGfxPrint(font, errorMessage, -0.41f, -0.05f, 1.f, 1.f, 1.f, 1.f, 1.f);
		drawErrorButton(errorPromptBtn, uiMesh, font);
		updateCloseButtonHover(errorPromptBtn);

		if (AEInputCheckTriggered(AEVK_LBUTTON) && errorPromptBtn.isHovered) {
			finaldoorExistError = false;
			errorPromptAlpha = 0.f;
		}
	}
	if (enemyExistError) {
		renderlogic::drawTexture(0.f, 0.f, errorOverlayTex, uiMesh, 855.f, 240.f);
		sprintf_s(errorMessage, "Enemy already exists!");
		AEGfxPrint(font, errorMessage, -0.35f, -0.05f, 1.f, 1.f, 1.f, 1.f, 1.f);
		drawErrorButton(errorPromptBtn, uiMesh, font);
		updateCloseButtonHover(errorPromptBtn);

		if (AEInputCheckTriggered(AEVK_LBUTTON) && errorPromptBtn.isHovered) {
			enemyExistError = false;
			errorPromptAlpha = 0.f;
		}
	}

	//========== DRAW RESET BUTTON ==========//
	drawResetButton(resetBtn, uiMesh, font);
	updateResetButtonHover(resetBtn);
}

void LevelEditor_Free() {

	AssetManager::FreeAllMeshes();

	platformMesh = nullptr;
	uiMesh = nullptr;
	FreeMapData();
}

void LevelEditor_Unload() {
	if (font != -1) { AEGfxDestroyFont(font); font = -1; }

	AssetManager::UnloadAllTextures();

	// Null out the pointers safely
	for (int i = 0; i < 10; i++)
	{
		tileTextures[i] = nullptr;
	}

	audio::unloadsound();
}