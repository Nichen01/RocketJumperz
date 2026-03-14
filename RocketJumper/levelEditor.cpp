
#include "LevelEditor.h"
#include "GameStateManager.h"
#include "AssetManager.h"

// GLOBAL VARIABLES
static AEGfxTexture* door;
static AEGfxTexture* tileTextures[10];
static const char* pText1{ "Level 1" };
static const char* pText2{ "Level 2" };
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

void LevelEditor_Load() {

	font = AEGfxCreateFont("Assets/Fonts/PressStart2P-Regular.ttf", 50);

	load::ui();

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

	// Fill the local tileTextures array from the AssetManager cache
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

}

void LevelEditor_Initialize() {

	AssetManager::BuildSqrMesh(MESH_PLATFORM);
	AssetManager::BuildSqrMesh(MESH_UI);
	platformMesh = AssetManager::GetMesh(MESH_PLATFORM);
	uiMesh       = AssetManager::GetMesh(MESH_UI);

	// ideally should be separated into loading the imported file, and initialising the map from the file
	switch (level) {
	case 1:
		ImportMapDataFromFile("Assets/Map/Level1_Map.txt");
		std::cout << "File 1 read";
		break;
	case 2:
		ImportMapDataFromFile("Assets/Map/Level2_Map.txt");
		std::cout << "File 2 read";
		break;
	}
}

void LevelEditor_Update() {
	if (AEInputCheckCurr(AEVK_LCTRL) && AEInputCheckTriggered(AEVK_1)) {
		level = 1;
		next = GS_RESTART;
	}
	else if (AEInputCheckCurr(AEVK_LCTRL) && AEInputCheckTriggered(AEVK_2)) {
		level = 2;
		next = GS_RESTART;
	}
	if (AEInputCheckTriggered(AEVK_L)) next = (level == 1) ? GS_LEVEL1 : GS_LEVEL2;

	// selecting asset
	if (AEInputCheckTriggered(AEVK_RIGHT)) {
		currentTileIndex++;
		if (currentTileIndex >= 10) currentTileIndex = 0;
	}
	else if (AEInputCheckTriggered(AEVK_LEFT)) {
		currentTileIndex--;
		if (currentTileIndex < 0) currentTileIndex = 9;
	}

	// undo
	if (AEInputCheckCurr(AEVK_LCTRL) && AEInputCheckTriggered(AEVK_Z)) {
		if (!actionHistory.empty()) {
			TileAction last = actionHistory.back();
			actionHistory.pop_back();

			MapData[last.row][last.col] = last.prevValue;
		}
	}

	// door prompt
	if (currentTileIndex == 9 && AEInputCheckTriggered(AEVK_LBUTTON)) {
		showDoorPrompt = true;
		doorPromptAlpha = 1.0f;
	}

	if (showDoorPrompt) {
		if (AEInputCheckTriggered(AEVK_1)) {
			doorID = 1;
			TileAction action{};
			action.row = promptRow;
			action.col = promptCol;
			action.prevValue = MapData[promptRow][promptCol];
			action.newValue = 20 + doorID;

			MapData[promptRow][promptCol] = action.newValue;
			actionHistory.push_back(action);

			showDoorPrompt = false;
			doorPromptAlpha = 0.f;
		}
		else if (AEInputCheckTriggered(AEVK_2)) {
			doorID = 2;
			TileAction action{};
			action.row = promptRow;
			action.col = promptCol;
			action.prevValue = MapData[promptRow][promptCol];
			action.newValue = 20 + doorID;

			MapData[promptRow][promptCol] = action.newValue;
			actionHistory.push_back(action);

			showDoorPrompt = false;
			doorPromptAlpha = 0.f;
		}
		else if (AEInputCheckTriggered(AEVK_3)) {
			doorID = 3;
			TileAction action{};
			action.row = promptRow;
			action.col = promptCol;
			action.prevValue = MapData[promptRow][promptCol];
			action.newValue = 20 + doorID;

			MapData[promptRow][promptCol] = action.newValue;
			actionHistory.push_back(action);

			showDoorPrompt = false;
			doorPromptAlpha = 0.f;
		}
		else if (AEInputCheckTriggered(AEVK_4)) {
			doorID = 4;
			TileAction action{};
			action.row = promptRow;
			action.col = promptCol;
			action.prevValue = MapData[promptRow][promptCol];
			action.newValue = 20 + doorID;

			MapData[promptRow][promptCol] = action.newValue;
			actionHistory.push_back(action);

			showDoorPrompt = false;
			doorPromptAlpha = 0.f;
		}
		else if (AEInputCheckTriggered(AEVK_5)) {
			doorID = 5;
			TileAction action{};
			action.row = promptRow;
			action.col = promptCol;
			action.prevValue = MapData[promptRow][promptCol];
			action.newValue = 20 + doorID;

			MapData[promptRow][promptCol] = action.newValue;
			actionHistory.push_back(action);

			showDoorPrompt = false;
			doorPromptAlpha = 0.f;
		}
		else if (AEInputCheckTriggered(AEVK_6)) {
			doorID = 6;
			TileAction action{};
			action.row = promptRow;
			action.col = promptCol;
			action.prevValue = MapData[promptRow][promptCol];
			action.newValue = 20 + doorID;

			MapData[promptRow][promptCol] = action.newValue;
			actionHistory.push_back(action);

			showDoorPrompt = false;
			doorPromptAlpha = 0.f;
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
				AEGfxSetColorToMultiply(0.95f, 0.95f, 0.5f, 1.0f); // yellow highlight
				if (AEInputCheckTriggered(AEVK_LBUTTON)) {
					if (currentTileIndex == 9) {
						promptRow = row;
						promptCol = col;
						showDoorPrompt = true;
						doorPromptAlpha = 1.0f;
					}
					else {
						TileAction action;
						action.row = row;
						action.col = col;
						action.prevValue = MapData[row][col];
						action.newValue = 11 + currentTileIndex;

						MapData[row][col] = action.newValue;
						actionHistory.push_back(action);
					}
				}
				if (AEInputCheckTriggered(AEVK_RBUTTON)) {
					TileAction action;
					action.row = row;
					action.col = col;
					action.prevValue = MapData[row][col];
					action.newValue = 0;

					MapData[row][col] = action.newValue;
					actionHistory.push_back(action);
				}
			}                                                           
			else if (MapData[row][col] >= 11 && MapData[row][col] <= 19) {
				AEGfxSetColorToMultiply(0.2f, 0.2f, 0.2f, 1.0f); // dark gray
			}
			else if (MapData[row][col] <= 29 && MapData[row][col] >= 21) {
				AEGfxSetColorToMultiply(0.5f, 0.8f, 0.5f, 1.f);
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

			if (isGridHovered) {
				if (AEInputCheckCurr(AEVK_LCTRL) && AEInputCheckTriggered(AEVK_S)) {
					switch (level) {
					case 1: ExportMapDataToFile("Assets/Map/Level1_Map.txt"); break;
					case 2: ExportMapDataToFile("Assets/Map/Level2_Map.txt"); break;
					}
				}
			}
			if (MapData[row][col] == 11 || MapData[row][col] == 21) {
				float normX = xPos / (AEGfxGetWindowWidth() / 2.0f);
				float normY = yPos / (AEGfxGetWindowHeight() / 2.0f);
				AEGfxPrint(font, "1", normX, normY, 0.5f, 1, 1, 1, 1);
			} 
			else if (MapData[row][col] == 12 || MapData[row][col] == 22) {
				float normX = xPos / (AEGfxGetWindowWidth() / 2.0f);
				float normY = yPos / (AEGfxGetWindowHeight() / 2.0f);
				AEGfxPrint(font, "2", normX, normY, 0.5f, 1, 1, 1, 1);
			}
			else if (MapData[row][col] == 13 || MapData[row][col] == 23) {
				float normX = xPos / (AEGfxGetWindowWidth() / 2.0f);
				float normY = yPos / (AEGfxGetWindowHeight() / 2.0f);
				AEGfxPrint(font, "3", normX, normY, 0.5f, 1, 1, 1, 1);
			}
			else if (MapData[row][col] == 14 || MapData[row][col] == 24) {
				float normX = xPos / (AEGfxGetWindowWidth() / 2.0f);
				float normY = yPos / (AEGfxGetWindowHeight() / 2.0f);
				AEGfxPrint(font, "4", normX, normY, 0.5f, 1, 1, 1, 1);
			}
			else if (MapData[row][col] == 15 || MapData[row][col] == 25) {
				float normX = xPos / (AEGfxGetWindowWidth() / 2.0f);
				float normY = yPos / (AEGfxGetWindowHeight() / 2.0f);
				AEGfxPrint(font, "5", normX, normY, 0.5f, 1, 1, 1, 1);
			}
			else if (MapData[row][col] == 16 || MapData[row][col] == 26) {
				float normX = xPos / (AEGfxGetWindowWidth() / 2.0f);
				float normY = yPos / (AEGfxGetWindowHeight() / 2.0f);
				AEGfxPrint(font, "6", normX, normY, 0.5f, 1, 1, 1, 1);
			}
			else if (MapData[row][col] == 17 || MapData[row][col] == 27) {
				float normX = xPos / (AEGfxGetWindowWidth() / 2.0f);
				float normY = yPos / (AEGfxGetWindowHeight() / 2.0f);
				AEGfxPrint(font, "7", normX, normY, 0.5f, 1, 1, 1, 1);
			}
			else if (MapData[row][col] == 18 || MapData[row][col] == 28) {
				float normX = xPos / (AEGfxGetWindowWidth() / 2.0f);
				float normY = yPos / (AEGfxGetWindowHeight() / 2.0f);
				AEGfxPrint(font, "8", normX, normY, 0.5f, 1, 1, 1, 1);
			}
			else if (MapData[row][col] == 19 || MapData[row][col] == 29) {
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
	if (currentTileIndex >= 0 && currentTileIndex < 10 && tileTextures[currentTileIndex]) {
		AEGfxTextureSet(tileTextures[currentTileIndex], 0, 0);
	}

	AEGfxSetTransform(tileTransf.m);
	AEGfxMeshDraw(platformMesh, AE_GFX_MDM_TRIANGLES);

	memset(strBuffer, 0, 100 * sizeof(char));
	f32 tileTextWidth, tileTextHeight;
	AEGfxGetPrintSize(font, strBuffer, 0.15f, &tileTextWidth, &tileTextHeight);
	// to print the text below the asset
	switch (currentTileIndex) {
	case 0:
		sprintf_s(strBuffer, "Platform: Top Left Corner");
		AEGfxPrint(font, strBuffer, -0.5f, -0.9f, 0.5f, 1.f, 1.f, 1.f, 1.f);
		break;
	case 1:
		sprintf_s(strBuffer, "Platform: Top Middle");
		AEGfxPrint(font, strBuffer, -0.5f, -0.9f, 0.5f, 1.f, 1.f, 1.f, 1.f);
		break;
	case 2:
		sprintf_s(strBuffer, "Platform: Top Right Corner");
		AEGfxPrint(font, strBuffer, -0.5f, -0.9f, 0.5f, 1.f, 1.f, 1.f, 1.f);
		break;
	case 3:
		sprintf_s(strBuffer, "Platform: Middle Left");
		AEGfxPrint(font, strBuffer, -0.5f, -0.9f, 0.5f, 1.f, 1.f, 1.f, 1.f);
		break;
	case 4:
		sprintf_s(strBuffer, "Platform: Middle");
		AEGfxPrint(font, strBuffer, -0.5f, -0.9f, 0.5f, 1.f, 1.f, 1.f, 1.f);
		break;
	case 5:
		sprintf_s(strBuffer, "Platform: Middle Right");
		AEGfxPrint(font, strBuffer, -0.5f, -0.9f, 0.5f, 1.f, 1.f, 1.f, 1.f);
		break;
	case 6:
		sprintf_s(strBuffer, "Platform: Bottom Left Corner");
		AEGfxPrint(font, strBuffer, -0.5f, -0.9f, 0.5f, 1.f, 1.f, 1.f, 1.f);
		break;
	case 7:
		sprintf_s(strBuffer, "Platform: Bottom Middle");
		AEGfxPrint(font, strBuffer, -0.5f, -0.9f, 0.5f, 1.f, 1.f, 1.f, 1.f);
		break;
	case 8:
		sprintf_s(strBuffer, "Platform: Bottom Right Corner");
		AEGfxPrint(font, strBuffer, -0.5f, -0.9f, 0.5f, 1.f, 1.f, 1.f, 1.f);
		break;
	case 9:
		sprintf_s(strBuffer, "Platform: Door");
		AEGfxPrint(font, strBuffer, -0.5f, -0.9f, 0.5f, 1.f, 1.f, 1.f, 1.f);
		break;
	}

	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	
	renderlogic::drawUITexture(-170.f, -330.f, leftArrow);
	renderlogic::drawUITexture(-20.f, -330.f, rightArrow);
	renderlogic::drawUITexture(570.f, 400.f, leftClick);
	renderlogic::drawUITexture(570.f, 280.f, rightClick);
	renderlogic::drawUITexture(570.f, 170.f, ctrl1);
	renderlogic::drawUITexture(610.f, 170.f, ctrl2);
	renderlogic::drawUITexture(670.f, 170.f, sButton);
	renderlogic::drawUITexture(570.f, 280.f, rightClick);
	renderlogic::drawUITexture(570.f, 60.f, ctrl1);
	renderlogic::drawUITexture(610.f, 600.f, ctrl2);
	renderlogic::drawUITexture(670.f, 60.f, zButton);
	renderlogic::drawUITexture(570.f, -60.f, ctrl1);
	renderlogic::drawUITexture(610.f, -60.f, ctrl2);
	renderlogic::drawUITexture(650.f, -60.f, button1);
	renderlogic::drawUITexture(690.f, -60.f, button2);
	renderlogic::drawUITexture(730.f, -60.f, button3);
	renderlogic::drawUITexture(770.f, -60.f, button4);

	// UI TEXT
	f32 uiTextWidth, uiTextHeight;
	AEGfxGetPrintSize(font, strBuffer, 0.5f, &uiTextWidth, &uiTextHeight);
	sprintf_s(strBuffer, "Left Click to Set");
	AEGfxPrint(font, strBuffer, 0.67f, 0.76f, 0.3f, 1.f, 1.f, 1.f, 1.f);

	sprintf_s(strBuffer, "Right Click to Clear");
	AEGfxPrint(font, strBuffer, 0.67f, 0.5f, 0.25f, 1.f, 1.f, 1.f, 1.f);

	sprintf_s(strBuffer, "CTRL + S to Save");
	AEGfxPrint(font, strBuffer, 0.67f, 0.25f, 0.3f, 1.f, 1.f, 1.f, 1.f);

	sprintf_s(strBuffer, "CTRL + Z to Undo");
	AEGfxPrint(font, strBuffer, 0.67f, 0.f, 0.3f, 1.f, 1.f, 1.f, 1.f);

	sprintf_s(strBuffer, "CTRL + (Number)");
	AEGfxPrint(font, strBuffer, 0.67f, -0.3f, 0.3f, 1.f, 1.f, 1.f, 1.f);
	sprintf_s(strBuffer, "for Level");
	AEGfxPrint(font, strBuffer, 0.67f, -0.35f, 0.3f, 1.f, 1.f, 1.f, 1.f);

	// door prompt background box
	if (showDoorPrompt && doorPromptAlpha > 0.0f) {
		float boxWidth = 1100.0f;
		float boxHeight = 100.0f;
		float boxX = 0.05f;
		float boxY = 0.05f;

		// set color (black with alpha)
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		AEGfxSetColorToMultiply(0.5f, 0.5f, 0.5f, doorPromptAlpha);

		// draw rectangle behind text
		renderlogic::drawSquare(boxX * AEGfxGetWindowWidth() / 2.0f,
			boxY * AEGfxGetWindowHeight() / 2.0f,
			boxWidth, boxHeight);
		AEGfxMeshDraw(platformMesh, AE_GFX_MDM_TRIANGLES);

		memset(strBuffer, 0, sizeof(strBuffer));
		f32 doorTextWidth, doorTextHeight;
		AEGfxGetPrintSize(font, strBuffer, 0.2f, &doorTextWidth, &doorTextHeight);
		AEGfxPrint(font, "Assign a number to the door!", -0.55f, 0.0f, 0.7f, 1.0f, 1.0f, 1.0f, doorPromptAlpha);
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
}