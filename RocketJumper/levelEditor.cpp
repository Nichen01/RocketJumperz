
#include "levelEditor.h"

// GLOBAL VARIABLES
static AEGfxTexture* button;
static AEGfxVertexList* levelTileMesh = nullptr;
static s8 buttonFont;
int level{ 1 };


void levelEditor_Load() {

	button = AEGfxTextureLoad("Assets/UI/Button.png");
	buttonFont = AEGfxCreateFont("Assets/Fonts/PressStart2P-Regular.ttf", 50);

	switch (level) {
	case 1:
		ImportMapDataFromFile("Assets/Map/Level1_Map.txt");
		std::cout << "File 1 read";
		break;
	case 2:
		ImportMapDataFromFile("Assets/Map/Level2_Map.txt");
		break;
	}
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
}

void levelEditor_Update() {

	if (AEInputCheckTriggered(AEVK_1)) level = 1;
	else if (AEInputCheckTriggered(AEVK_2)) level = 2;


}

void levelEditor_Draw() {

    AEGfxSetRenderMode(AE_GFX_RM_COLOR);
    AEGfxSetBlendMode(AE_GFX_BM_NONE);
	AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);

	// getting mouse coordinates
	s32 mouseX, mouseY;
	AEInputGetCursorPosition(&mouseX, &mouseY);
	float worldMouseX = static_cast<f32>(mouseX) - AEGfxGetWindowWidth() / 2.f;
	float worldMouseY = AEGfxGetWindowHeight() / 2.f - static_cast<f32>(mouseY);

	// display level in a grid system
    const float tileSize = 65.f;
	const float gap = 3.f;
    for (s32 row = 0; row < BINARY_MAP_HEIGHT; ++row) {
        for (s32 col = 0; col < BINARY_MAP_WIDTH; ++col) {

			float xPos = -col * tileSize + 480.f;
			float yPos = -row * tileSize + 410.f;
			float halfSize = (tileSize - gap) / 2.f;

			bool isGridHovered = (worldMouseX >= xPos - halfSize && worldMouseX <= xPos + halfSize && worldMouseY >= yPos - halfSize && worldMouseY <= yPos + halfSize);
			if (isGridHovered) {
				AEGfxSetColorToMultiply(1.0f, 1.0f, 0.f, 1.0f);
				if (AEInputCheckTriggered(AEVK_LBUTTON)) {
					s32 clickedRow = row;
					s32 clickedCol = col;
					MapData[clickedRow][clickedCol] = (MapData[clickedRow][clickedCol] == 0) ? 1 : 0;
				}
				else if (AEInputCheckCurr(AEVK_LCTRL) && (AEInputCheckTriggered(AEVK_S))) {
					switch (level) {
					case 1: ExportMapDataToFile("Assets/Map/Level1_Map.txt"); break;
					case 2: ExportMapDataToFile("Assets/Map/Level2_Map.txt"); break;
					}
				}
			}
			else if (MapData[row][col] == 11) AEGfxSetColorToMultiply(0.2f, 0.2f, 0.2f, 1.0f);
			else AEGfxSetColorToMultiply(0.8f, 0.8f, 0.8f, 1.0f);

            AEMtx33 scl, rot, transl, transf;
            AEMtx33Scale(&scl, tileSize - gap, tileSize - gap);
            AEMtx33Rot(&rot, 0);
			AEMtx33Trans(&transl, xPos, yPos);

            AEMtx33Concat(&transf, &rot, &scl);
            AEMtx33Concat(&transf, &transl, &transf);

            AEGfxSetTransform(transf.m);
            AEGfxMeshDraw(levelTileMesh, AE_GFX_MDM_TRIANGLES);
        }
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
	if (button) AEGfxTextureUnload(button);
	AEGfxDestroyFont(buttonFont);
}