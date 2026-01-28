#include <iostream>
#include "mapEditor.h"
#include "GameStateManager.h"
#include "pch.h"

// Globals for this file
int* map = new int[144]{ 0 };
AEMtx33 platformTransform1;
AEGfxTexture* baseTextures[14]; // enough for IDs 0–13


float tileSize = 100.f; 
float offsetX = -800.f; // half screen width 
float offsetY = 450.f; // half screen height

AEGfxTexture* characterPic = NULL;
AEGfxTexture* base1 = NULL;
AEGfxTexture* base2 = NULL;
AEGfxTexture* base3 = NULL;
AEGfxTexture* base4 = NULL;
AEGfxTexture* base5 = NULL;
AEGfxTexture* base6 = NULL;
AEGfxTexture* base7 = NULL;
AEGfxTexture* base8 = NULL;
AEGfxTexture* base9 = NULL;
AEGfxTexture* base10 = NULL;
AEGfxTexture* base11 = NULL;
AEGfxTexture* base12 = NULL;
AEGfxVertexList* pMesh = nullptr;


// ----------------------------------------------------------------------------
// Load assets (textures)
// ----------------------------------------------------------------------------
void Level1_Load()
{
    std::ifstream ifs("Assets/Level1_Map.txt");
    if (!ifs.is_open()) {
        std::cout << "Failed to open map file!\n";
    }

    for (int i = 0; i < 144; i++) {
        if (ifs >> map[i]) {
        }
        else {
            map[i] = 0; // default if read fails
        }
    }
    std::cout << "Loading map from Assets/Level1_Map.txt\n";

    ifs.close();
}

// ----------------------------------------------------------------------------
// Initialize assets (textures + mesh)
// ----------------------------------------------------------------------------
void Level1_Initialize()
{
    // --- Load textures ---
    characterPic = AEGfxTextureLoad("Assets/CharacterRight.png");
    base1 = AEGfxTextureLoad("Assets/Base1.png");
    base2 = AEGfxTextureLoad("Assets/Base2.png");
    base3 = AEGfxTextureLoad("Assets/Base3.png");
    base4 = AEGfxTextureLoad("Assets/Base4.png");
    base5 = AEGfxTextureLoad("Assets/Base5.png");
    base6 = AEGfxTextureLoad("Assets/Base6.png");
    base7 = AEGfxTextureLoad("Assets/Base7.png");
    base8 = AEGfxTextureLoad("Assets/Base8.png");
    base9 = AEGfxTextureLoad("Assets/Base9.png");
    base10 = AEGfxTextureLoad("Assets/Base10.png");
    base11 = AEGfxTextureLoad("Assets/Base11.png");
    base12 = AEGfxTextureLoad("Assets/Base12.png");

    // --- Assign textures into array for easy lookup ---
    baseTextures[0] = nullptr;
    baseTextures[1] = base1;
    baseTextures[2] = base2;
    baseTextures[3] = base3;
    baseTextures[4] = base4;
    baseTextures[5] = base5;
    baseTextures[6] = base6;
    baseTextures[7] = base7;
    baseTextures[8] = base8;
    baseTextures[9] = base9;
    baseTextures[10] = base10;
    baseTextures[11] = base11;
    baseTextures[12] = base12;
    baseTextures[13] = characterPic;

    AEMtx33 scale, rotate, translate;
    AEMtx33Scale(&scale, 100.f, 100.f);
    AEMtx33Rot(&rotate, 0);
    AEMtx33Trans(&translate, -750.f, -280.f);

    AEMtx33Concat(&platformTransform1, &rotate, &scale);
    AEMtx33Concat(&platformTransform1, &translate, &platformTransform1);

    AEGfxMeshStart();

    AEGfxTriAdd(
        -0.5f, -0.5f, 0xFFFFFFFF, 0.0f, 1.0f,
        0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f,
        -0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f);

    AEGfxTriAdd(
        0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f,
        0.5f, 0.5f, 0xFFFFFFFF, 1.0f, 0.0f,
        -0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f);

    pMesh = AEGfxMeshEnd();

    for (int i = 0; i < 144; ++i) {
        std::cout << map[i] << " ";
        if ((i + 1) % 16 == 0) std::cout << "\n";
    }

}


void Level1_Update()
{
    // Example: press ESC to quit
    if (AEInputCheckCurr(AEVK_ESCAPE))
    {
        next = GS_QUIT;
    }
    if (AEInputCheckTriggered(AEVK_R)) {
        Level1_Load();
        std::cout << "Map reloaded!\n";
    }

}

void Level1_Draw()
{

    AEGfxSetBackgroundColor(0.2f, 0.2f, 0.2f);
    AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
    AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
    AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
    AEGfxSetBlendMode(AE_GFX_BM_BLEND);
    AEGfxSetTransparency(1.0f);

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 16; j++) {
            int tile = map[i * 16 + j];
            if (tile == 0) continue; // skip empty

            AEGfxTexture* tex = baseTextures[tile];
            if (!tex) continue;

            AEMtx33 scale, translate, transform;
            AEMtx33Scale(&scale, 100.f, 100.f);
            AEMtx33Trans(&translate,
                j * tileSize + offsetX + 50.f,
                (offsetY - 50.f) - i * tileSize);
            AEMtx33Concat(&transform, &translate, &scale);

            AEGfxSetTransform(transform.m);
            AEGfxTextureSet(tex, 0, 0);
            AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
        }
    }

}

void Level1_Free()
{
    printf("Level1: Free\n");
    AEGfxMeshFree(pMesh);
    delete[] map;
}

void Level1_Unload() { 
    for (int i = 1; i <= 13; ++i) { 
        if (baseTextures[i]) AEGfxTextureUnload(baseTextures[i]); 
    } 
}
