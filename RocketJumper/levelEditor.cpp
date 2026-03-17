
#include "LevelEditor.h"

// ==================== GLOBAL RESOURCES ==================== //
static AEGfxTexture* door;
static AEGfxTexture* tileTextures[11];
static const char* pText1{ "Level 1" };
static const char* pText2{ "Level 2" };
static const char* pText3{ "Level 3" };
static s8 font;
int level{ 1 };
s32 currentTileIndex{}, doorCount{ 1 };
static std::vector<TileAction> actionHistory;

// door prompt
static float doorPromptAlpha = 1.0f;
static bool showDoorPrompt = false;
static u32 doorID{};

static int promptRow = -1;
static int promptCol = -1;
 
static char strBuffer[100];

// ==================== FORWARD DECLARATIONS ====================//
static bool isMouseOverDoorButton(const doorButton& button);
static void updateDoorButtonHover(doorButton& button);
static void drawDoorButton(const doorButton& button, AEGfxVertexList* mesh, s8 fontID);
static void drawDoorTextCentered(const char* text, f32 x, f32 y, f32 scale, s8 fontID);

// ==================== BUTTON ARRAY ==================== //
static const f32 BUTTON_SCALE_NORMAL = 1.0f;
static const f32 BUTTON_SCALE_HOVER = 1.15f;
static const f32 BUTTON_SCALE_SPEED = 0.15f;
static std::vector<doorButton> buttonArr;

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

void LevelEditor_Load() {

	font = AEGfxCreateFont("Assets/Fonts/gameover.ttf", 50);

	load::ui();
	// Load sound
	audio::loadsound();

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
}

void LevelEditor_Initialize() {



	AssetManager::BuildSqrMesh(MESH_PLATFORM);
	AssetManager::BuildSqrMesh(MESH_UI);
	platformMesh = AssetManager::GetMesh(MESH_PLATFORM);
	uiMesh       = AssetManager::GetMesh(MESH_UI);

	// ideally should be separated into loading the imported file, and initialising the map from the file
	buttonArr.clear();
	switch (level) {
		case 1: {
			ImportMapDataFromFile("Assets/Map/Level1_Map.txt");
			buttonArr.push_back({ 0.f, -120.f, 680.f, 60.f, 1.f, 1.f, "Cancel", false, -1 });
			buttonArr.push_back({ -270.f, 0.f, 150.f, 60.f, 1.f, 1.f, "Tut", false, 0});
			buttonArr.push_back({ 0.f, 0.f, 150.f, 60.f, 1.f, 1.f, "02", false, 2 });
			buttonArr.push_back({ 270.f, 0.f, 150.f, 60.f, 1.f, 1.f, "03", false, 3 });
			break;
		}
		case 2: {
			ImportMapDataFromFile("Assets/Map/Level2_Map.txt");
			buttonArr.push_back({ 0.f, -120.f, 680.f, 60.f, 1.f, 1.f, "Cancel", false, -1 });
			buttonArr.push_back({ -270.f, 0.f, 150.f, 60.f, 1.f, 1.f, "Tut", false, 0 });
			buttonArr.push_back({ 0.f, 0.f, 150.f, 60.f, 1.f, 1.f, "01", false, 1 });
			buttonArr.push_back({ 270.f, 0.f, 150.f, 60.f, 1.f, 1.f, "03", false, 3 });
			break;
		}
		case 3: {
			ImportMapDataFromFile("Assets/Map/Level3_Map.txt");
			buttonArr.push_back({ 0.f, -120.f, 680.f, 60.f, 1.f, 1.f, "Cancel", false, -1 });
			buttonArr.push_back({ -270.f, 0.f, 150.f, 60.f, 1.f, 1.f, "Tut", false, 0 });
			buttonArr.push_back({ 0.f, 0.f, 150.f, 60.f, 1.f, 1.f, "01", false, 1 });
			buttonArr.push_back({ 270.f, 0.f, 150.f, 60.f, 1.f, 1.f, "02", false, 2 });
			break;
		}
	}
}

void LevelEditor_Update() {
	// blocking the tab button
	if (AEInputCheckTriggered(AEVK_TAB)) {}

	if (AEInputCheckCurr(AEVK_LCTRL) && AEInputCheckTriggered(AEVK_1)) {
		level = 1;
		next = GS_RESTART;
	}
	else if (AEInputCheckCurr(AEVK_LCTRL) && AEInputCheckTriggered(AEVK_2)) {
		level = 2;
		next = GS_RESTART;
	}
	if (AEInputCheckTriggered(AEVK_L)) next = (level == 1) ? GS_LEVEL1 : GS_LEVEL2;

	if (showDoorPrompt) {
		for (doorButton& currentButton : buttonArr) {
			updateDoorButtonHover(currentButton);
			if (currentButton.isHovered && AEInputCheckTriggered(AEVK_LBUTTON)) {
				if (currentButton.id == -1) {
					showDoorPrompt = false;
					doorPromptAlpha = 0.f;
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
					std::cout << "This door link already exists!" << std::endl;
					AEAudioPlay(Error, soundEffects, 1.f, 1.f, 0);
					showDoorPrompt = false;
					doorPromptAlpha = 0.f;
				}
				break;
			}
		}
	}

	// selecting asset
	if (AEInputCheckTriggered(AEVK_RIGHT)) {
		AEAudioPlay(ArrowSound, soundEffects, 1.f, 1.f, 0);
		currentTileIndex++;
		if (currentTileIndex >= 11) currentTileIndex = 0;
	}
	else if (AEInputCheckTriggered(AEVK_LEFT)) {
		AEAudioPlay(ArrowSound, soundEffects, 1.f, 1.f, 0);
		currentTileIndex--;
		if (currentTileIndex < 0) currentTileIndex = 10;
	}

	// undo
	if (AEInputCheckCurr(AEVK_LCTRL) && AEInputCheckTriggered(AEVK_Z)) {
		if (!actionHistory.empty()) {
			TileAction last = actionHistory.back();
			actionHistory.pop_back();

			MapData[last.row][last.col] = last.prevValue;
		}
	}

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

	// Check if there's a keycard when left click
	if (currentTileIndex == 10 && AEInputCheckTriggered(AEVK_LBUTTON)) {
		if (level == 1 && keyCountLevel1 >= 1) {
			std::cout << "Keycard already exists" << std::endl;
		}
		else if (level == 2 && keyCountLevel2 >= 1) {
			std::cout << "Keycard already exists" << std::endl;
		}
		else if (level == 3 && keyCountLevel3 >= 1) {
			std::cout << "Keycard already exists" << std::endl;
		}
	}
}

void LevelEditor_Draw() {

    AEGfxSetRenderMode(AE_GFX_RM_COLOR);
    AEGfxSetBlendMode(AE_GFX_BM_NONE);
	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);

	// getting mouse coordinates
	s32 mouseX, mouseY;
	AEInputGetCursorPosition(&mouseX, &mouseY);
	float worldMouseX = static_cast<f32>(mouseX) - AEGfxGetWindowWidth() / 2.f;
	float worldMouseY = AEGfxGetWindowHeight() / 2.f - static_cast<f32>(mouseY);

	// text to print
	f32 levelWidth, levelHeight;
	float offsetX = 10.0f / (AEGfxGetWindowWidth() / 2.0f); 
	float offsetY = 10.0f / (AEGfxGetWindowHeight() / 2.0f);
	if (level == 1) {
		AEGfxGetPrintSize(font, pText1, 1.f, &levelWidth, &levelHeight);
		AEGfxPrint(font, pText1, -1.f + offsetX, -1.f + offsetY, 1, 1, 1, 1, 1);
	}
	else if (level == 2) {
		AEGfxGetPrintSize(font, pText2, 1.f, &levelWidth, &levelHeight);
		AEGfxPrint(font, pText2, -1.f + offsetX, -1.f + offsetY, 1, 1, 1, 1, 1);
	}

	// display level in a grid system
	const float tileSize = 65.f;
	const float gap = 3.f;
	for (s32 row = 0; row < BINARY_MAP_HEIGHT; ++row) {
		for (s32 col = 0; col < BINARY_MAP_WIDTH; ++col) {

			float xPos = col * tileSize - 750.f;
			float yPos = -row * tileSize + 410.f;
			float halfSize = (tileSize - gap) / 2.f;

			bool isGridHovered = (worldMouseX >= xPos - halfSize && worldMouseX <= xPos + halfSize &&
				worldMouseY >= yPos - halfSize && worldMouseY <= yPos + halfSize);

			if (isGridHovered) {
				AEGfxSetColorToMultiply(0.4f, 0.37f, 0.42f, 1.0f); // purple highlight
				if (AEInputCheckCurr(AEVK_LCTRL) && AEInputCheckTriggered(AEVK_LBUTTON)) {
					if (currentTileIndex == 9) {
						promptRow = row;
						promptCol = col;
						showDoorPrompt = true;
						doorPromptAlpha = 1.0f;
					}
					else if (currentTileIndex >= 0 && currentTileIndex <= 8) {
						TileAction action;
						action.row = row;
						action.col = col;
						action.prevValue = MapData[row][col];
						action.newValue = 11 + currentTileIndex;

						MapData[row][col] = action.newValue;
						actionHistory.push_back(action);
					}
					else if (currentTileIndex == 10) {
						bool placeKey = false;
						if (level == 1 && keyCountLevel1 == 0) placeKey = true;
						else if (level == 2 && keyCountLevel2 == 0) placeKey = true;
						else if (level == 3 && keyCountLevel3 == 0) placeKey = true;

						if (placeKey) {
							TileAction action;
							action.row = row;
							action.col = col;
							action.prevValue = MapData[row][col];
							action.newValue = 67; // key tile ID

							MapData[row][col] = action.newValue;
							actionHistory.push_back(action);

							// increment the counter right here
							if (level == 1) keyCountLevel1++;
							else if (level == 2) keyCountLevel2++;
							else if (level == 3) keyCountLevel3++;
						}
						else {
							std::cout << "Keycard already exists in this level!" << std::endl;
							AEAudioPlay(Error, soundEffects, 1.f, 1.f, 0);
						}
					}


				}
				if (AEInputCheckTriggered(AEVK_RBUTTON)) {
					TileAction action;
					action.row = row;
					action.col = col;
					action.prevValue = MapData[row][col];
					action.newValue = 0;

					if (MapData[row][col] == 67) {
						if (level == 1) keyCountLevel1 = 0;
						else if (level == 2) keyCountLevel2 = 0;
						else if (level == 3) keyCountLevel3 = 0;
					}

					MapData[row][col] = action.newValue;
					actionHistory.push_back(action);
				}
			}                                                           
			else if (MapData[row][col] >= 11 && MapData[row][col] <= 19) {
				AEGfxSetColorToMultiply(0.49f, 0.49f, 0.49f, 1.0f); // dark gray
			}
			else if (MapData[row][col] <= 29 && MapData[row][col] >= 21) {
				AEGfxSetColorToMultiply(0.5f, 0.8f, 0.5f, 1.f);
			}
			else if (MapData[row][col] == 67) { // key
				AEGfxSetColorToMultiply(0.49f, 0.36f, 0.37f, 1.f);
			}
			else {
				AEGfxSetColorToMultiply(0.8f, 0.8f, 0.8f, 1.0f); // default light gray
			}


			AEMtx33 scl, rot, transl, transf;
			AEMtx33Scale(&scl, tileSize - gap, tileSize - gap);
			AEMtx33Rot(&rot, 0);
			AEMtx33Trans(&transl, xPos, yPos);

			AEMtx33Concat(&transf, &rot, &scl);
			AEMtx33Concat(&transf, &transl, &transf);

			AEGfxSetTransform(transf.m);
			AEGfxMeshDraw(platformMesh, AE_GFX_MDM_TRIANGLES);

			float textW, textH;

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
			else if (MapData[row][col] == 67) { // key
				// normalize tile center
				AEGfxGetPrintSize(font, "Key", 0.4f, &textW, &textH);
				float normX = xPos / (AEGfxGetWindowWidth() / 2.0f) - textW / 2.0f;
				float normY = yPos / (AEGfxGetWindowHeight() / 2.0f) - textH / 2.0f;
				AEGfxPrint(font, "Key", normX, normY, 0.4f, 1, 1, 1, 1);
			}
			if (!isGridHovered && (MapData[row][col] == 11 || MapData[row][col] == 21)) {
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
		}
	}

	// to print the asset that user is currently at
	AEMtx33 tileScl, tileRot, tileTransl, tileTransf;
	AEMtx33Scale(&tileScl, 60.f, 60.f);
	AEMtx33Rot(&tileRot, 0);
	AEMtx33Trans(&tileTransl, -100.f, -330.f);

	AEMtx33Concat(&tileTransf, &tileRot, &tileScl);
	AEMtx33Concat(&tileTransf, &tileTransl, &tileTransf);

	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	if (currentTileIndex >= 0 && currentTileIndex < 11 && tileTextures[currentTileIndex]) {
		AEGfxTextureSet(tileTextures[currentTileIndex], 0, 0);
	}

	AEGfxSetTransform(tileTransf.m);
	AEGfxMeshDraw(platformMesh, AE_GFX_MDM_TRIANGLES);

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
	}


	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	
	renderlogic::drawTexture(-170.f, -330.f, leftArrow, uiMesh);
	renderlogic::drawTexture(-20.f, -330.f, rightArrow, uiMesh);
	renderlogic::drawTexture(570.f, 400.f, leftClick, uiMesh);
	renderlogic::drawTexture(570.f, 280.f, rightClick, uiMesh);
	renderlogic::drawTexture(570.f, 170.f, ctrl1, uiMesh);
	renderlogic::drawTexture(610.f, 170.f, ctrl2, uiMesh);
	renderlogic::drawTexture(670.f, 170.f, sButton, uiMesh);
	renderlogic::drawTexture(570.f, 280.f, rightClick, uiMesh);
	renderlogic::drawTexture(570.f, 60.f, ctrl1, uiMesh);
	renderlogic::drawTexture(610.f, 60.f, ctrl2, uiMesh);
	renderlogic::drawTexture(670.f, 60.f, zButton, uiMesh);
	renderlogic::drawTexture(570.f, -60.f, ctrl1, uiMesh);
	renderlogic::drawTexture(610.f, -60.f, ctrl2, uiMesh);
	renderlogic::drawTexture(650.f, -60.f, button1, uiMesh);
	renderlogic::drawTexture(690.f, -60.f, button2, uiMesh);
	renderlogic::drawTexture(730.f, -60.f, button3, uiMesh);

	// UI TEXT
	f32 uiTextWidth, uiTextHeight;
	AEGfxGetPrintSize(font, strBuffer, 0.5f, &uiTextWidth, &uiTextHeight);
	sprintf_s(strBuffer, "Left Click to Set");
	AEGfxPrint(font, strBuffer, 0.67f, 0.76f, 0.4f, 1.f, 1.f, 1.f, 1.f);

	sprintf_s(strBuffer, "Right Click to Clear");
	AEGfxPrint(font, strBuffer, 0.67f, 0.5f, 0.4f, 1.f, 1.f, 1.f, 1.f);

	sprintf_s(strBuffer, "CTRL + S to Save");
	AEGfxPrint(font, strBuffer, 0.67f, 0.25f, 0.4f, 1.f, 1.f, 1.f, 1.f);

	sprintf_s(strBuffer, "CTRL + Z to Undo");
	AEGfxPrint(font, strBuffer, 0.67f, 0.f, 0.4f, 1.f, 1.f, 1.f, 1.f);

	sprintf_s(strBuffer, "CTRL + (Number)");
	AEGfxPrint(font, strBuffer, 0.67f, -0.3f, 0.4f, 1.f, 1.f, 1.f, 1.f);

	sprintf_s(strBuffer, "for Level");
	AEGfxPrint(font, strBuffer, 0.67f, -0.35f, 0.4f, 1.f, 1.f, 1.f, 1.f);

	// door prompt background box
	if (showDoorPrompt && doorPromptAlpha > 0.0f) {
		// Main prompt panel
		float boxWidth = 850.0f;
		float boxHeight = 400.0f;
		AEGfxSetColorToMultiply(0.1f, 0.1f, 0.3f, doorPromptAlpha); // dark blue background
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