
#include "levelEditor.h"
#include "GameStateManager.h"

// GLOBAL VARIABLES
static AEGfxTexture* door;
static AEGfxVertexList* levelTileMesh = nullptr;
static AEGfxTexture* tileTextures[9];
static const char* pText1{ "Level 1" };
static const char* pText2{ "Level 2" };
static s8 font;
int level{ 1 };
s32 currentTileIndex{};
static std::vector<TileAction> actionHistory;


void levelEditor_Load() {

	font = AEGfxCreateFont("Assets/Fonts/PressStart2P-Regular.ttf", 50);
	door = AEGfxTextureLoad("Assets/Platform/staticDoor.jng");



	tileTextures[0] = AEGfxTextureLoad("Assets/Platform/platform1.png");
	tileTextures[1] = AEGfxTextureLoad("Assets/Platform/platform2.png");
	tileTextures[2] = AEGfxTextureLoad("Assets/Platform/platform3.png");
	tileTextures[3] = AEGfxTextureLoad("Assets/Platform/platform4.png");
	tileTextures[4] = AEGfxTextureLoad("Assets/Platform/platform5.png");
	tileTextures[5] = AEGfxTextureLoad("Assets/Platform/platform6.png");
	tileTextures[6] = AEGfxTextureLoad("Assets/Platform/platform7.png");
	tileTextures[7] = AEGfxTextureLoad("Assets/Platform/platform8.png");
	tileTextures[8] = AEGfxTextureLoad("Assets/Platform/platform9.png");
}

void levelEditor_Initialize() {

	AEGfxMeshStart();
	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFFFFFFFF, 0.0f, 1.0f,
		0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f,
		-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f);
	AEGfxTriAdd(
		0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f,
		0.5f, 0.5f, 0xFFFFFFFF, 1.0f, 0.0f,
		-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f);
	levelTileMesh = AEGfxMeshEnd();

	//ideally should be separated into loading the imported file, and initialising the map from the file
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

void levelEditor_Update() {
	if (AEInputCheckTriggered(AEVK_1)) {
		level = 1;
		next=GS_RESTART;
	}
	else if (AEInputCheckTriggered(AEVK_2)) {
		level = 2;
		next=GS_RESTART;
	}
	if (AEInputCheckTriggered(AEVK_L)) next = (level==1)?GS_LEVEL1:GS_LEVEL2;

	if (AEInputCheckTriggered(AEVK_RIGHT)) {
		currentTileIndex++;
		if (currentTileIndex >= 9) currentTileIndex = 0;
	}
	else if (AEInputCheckTriggered(AEVK_LEFT)) {
		currentTileIndex--;
		if (currentTileIndex < 0) currentTileIndex = 8;
	}

	if (AEInputCheckCurr(AEVK_LCTRL) && AEInputCheckTriggered(AEVK_Z)) {
		if (!actionHistory.empty()) {
			TileAction last = actionHistory.back();
			actionHistory.pop_back();

			MapData[last.row][last.col] = last.prevValue;
		}
	}
}

void levelEditor_Draw() {

    AEGfxSetRenderMode(AE_GFX_RM_COLOR);
    AEGfxSetBlendMode(AE_GFX_BM_NONE);
	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f); // full white so texture shows correctly


	//// text legends
	//for (s32 i{}; i < 7; i++) {
	//	AEMtx33 scl, rot, transl, transf;
	//	AEMtx33Scale(&scl, 30.f, 30.f);
	//	AEMtx33Rot(&rot, 0);
	//	AEMtx33Trans(&transl, 500.f, static_cast<f32>(500.f + 30 * i));

	//	AEMtx33Concat(&transf, &rot, &scl);
	//	AEMtx33Concat(&transf, &transl, &transf);

	//	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	//	AEGfxTextureSet(door, 0, 0);

	//	AEGfxSetTransform(transf.m);
	//	AEGfxMeshDraw(levelTileMesh, AE_GFX_MDM_TRIANGLES);

	//	AEGfxSetRenderMode(AE_GFX_RM_COLOR);

	//}

	// getting mouse coordinates
	s32 mouseX, mouseY;
	AEInputGetCursorPosition(&mouseX, &mouseY);
	float worldMouseX = static_cast<f32>(mouseX) - AEGfxGetWindowWidth() / 2.f;
	float worldMouseY = AEGfxGetWindowHeight() / 2.f - static_cast<f32>(mouseY);

	// text to print
	f32 levelWidth, levelHeight, tileWidth, tileHeight;
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
					TileAction action;
					action.row = row;
					action.col = col;
					action.prevValue = MapData[row][col];
					action.newValue = 11 + currentTileIndex;

					MapData[row][col] = action.newValue;
					actionHistory.push_back(action);
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
			else if (MapData[row][col] == 100) {
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
			AEGfxMeshDraw(levelTileMesh, AE_GFX_MDM_TRIANGLES);

			if (isGridHovered) {
				if (AEInputCheckCurr(AEVK_LCTRL) && AEInputCheckTriggered(AEVK_S)) {
					switch (level) {
					case 1: ExportMapDataToFile("Assets/Map/Level1_Map.txt"); break;
					case 2: ExportMapDataToFile("Assets/Map/Level2_Map.txt"); break;
					}
				}
			}
			if (MapData[row][col] == 11) {
				float normX = xPos / (AEGfxGetWindowWidth() / 2.0f);
				float normY = yPos / (AEGfxGetWindowHeight() / 2.0f);
				AEGfxPrint(font, "1", normX, normY, 0.5f, 1, 1, 1, 1);
			} 
			else if (MapData[row][col] == 12) {
				float normX = xPos / (AEGfxGetWindowWidth() / 2.0f);
				float normY = yPos / (AEGfxGetWindowHeight() / 2.0f);
				AEGfxPrint(font, "2", normX, normY, 0.5f, 1, 1, 1, 1);
			}
			else if (MapData[row][col] == 13) {
				float normX = xPos / (AEGfxGetWindowWidth() / 2.0f);
				float normY = yPos / (AEGfxGetWindowHeight() / 2.0f);
				AEGfxPrint(font, "3", normX, normY, 0.5f, 1, 1, 1, 1);
			}
			else if (MapData[row][col] == 14) {
				float normX = xPos / (AEGfxGetWindowWidth() / 2.0f);
				float normY = yPos / (AEGfxGetWindowHeight() / 2.0f);
				AEGfxPrint(font, "4", normX, normY, 0.5f, 1, 1, 1, 1);
			}
			else if (MapData[row][col] == 15) {
				float normX = xPos / (AEGfxGetWindowWidth() / 2.0f);
				float normY = yPos / (AEGfxGetWindowHeight() / 2.0f);
				AEGfxPrint(font, "5", normX, normY, 0.5f, 1, 1, 1, 1);
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
	AEGfxTextureSet(tileTextures[currentTileIndex], 0, 0);

	AEGfxSetTransform(tileTransf.m);
	AEGfxMeshDraw(levelTileMesh, AE_GFX_MDM_TRIANGLES);

	AEGfxSetRenderMode(AE_GFX_RM_COLOR); 

	// to print the text below the asset
	switch (currentTileIndex) {
		char strBuffer[100];
		memset(strBuffer, 0, 100 * sizeof(char));
		f32 tileTextWidth, tileTextHeight;
		AEGfxGetPrintSize(font, strBuffer, 0.15f, &tileTextWidth, &tileTextHeight);
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
	}

}

void levelEditor_Free() {

	if (levelTileMesh) {
		AEGfxMeshFree(levelTileMesh);
		levelTileMesh = nullptr;
	}
	FreeMapData();
}

void levelEditor_Unload() {
	AEGfxDestroyFont(font);
}