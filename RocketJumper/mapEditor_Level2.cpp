#include "mapEditor_Level1.h"
#include "GameStateManager.h"
#include "pch.h"

// GLOBALS
static int* map = nullptr;
static AEMtx33 platformTransform;
static AEGfxTexture* baseTextures[12];

static AEMtx33 scale, translate, transform;

static float tileSize = 100.f;
static float offsetX = -800.f; // half screen width 
static float offsetY = 450.f; // half screen height

static AEGfxTexture* character = NULL;
static AEGfxTexture* enemy = NULL;
static AEGfxTexture* base1 = NULL;
static AEGfxTexture* base2 = NULL;
static AEGfxTexture* base3 = NULL;
static AEGfxTexture* base4 = NULL;
static AEGfxTexture* base5 = NULL;
static AEGfxTexture* base6 = NULL;
static AEGfxTexture* base7 = NULL;
static AEGfxTexture* base8 = NULL;
static AEGfxTexture* base9 = NULL;
static AEGfxVertexList* pMesh = nullptr;

void Level2_Load()
{
    map = new int[144];
    std::ifstream ifs("Assets/Level2_Map.txt");
    for (int i = 0; i < 144; i++) {
        if (ifs >> map[i]) {
        }
        else {
            map[i] = 0;
        }
    }
    ifs.close();
}

void Level2_Initialize()
{
    character = AEGfxTextureLoad("Assets/astronautRight.png");
    enemy = AEGfxTextureLoad("Assets/AlienRight.png");
    base1 = AEGfxTextureLoad("Assets/Base1.png");
    base2 = AEGfxTextureLoad("Assets/Base2.png");
    base3 = AEGfxTextureLoad("Assets/Base3.png");
    base4 = AEGfxTextureLoad("Assets/Base4.png");
    base5 = AEGfxTextureLoad("Assets/Base5.png");
    base6 = AEGfxTextureLoad("Assets/Base6.png");
    base7 = AEGfxTextureLoad("Assets/Base7.png");
    base8 = AEGfxTextureLoad("Assets/Base8.png");
    base9 = AEGfxTextureLoad("Assets/Base9.png");

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
    baseTextures[10] = character;
    baseTextures[11] = enemy;

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
}

void Level2_Update()
{
    if (AEInputCheckCurr(AEVK_ESCAPE))
    {
        next = GS_QUIT;
    }
    if (AEInputCheckCurr(AEVK_1))
    {
        next = GS_LEVEL1;
    }
}

void Level2_Draw()
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

            if (tile == 10) {
                AEMtx33Scale(&scale, 150.f, 150.f);
                AEMtx33Trans(&translate, j * tileSize + offsetX + 50.f, (offsetY - 50.f) - i * tileSize);
            }
            else if (tile == 11) {
                AEMtx33Scale(&scale, 180.f, 180.f);
                AEMtx33Trans(&translate, j * tileSize + offsetX + 50.f, (offsetY - 55.f) - i * tileSize);
            }
            else {
                AEMtx33Scale(&scale, 100.f, 100.f);
                AEMtx33Trans(&translate, j * tileSize + offsetX + 50.f, (offsetY - 50.f) - i * tileSize);
            }
            AEMtx33Concat(&transform, &translate, &scale);
            AEGfxSetTransform(transform.m);
            AEGfxTextureSet(tex, 0, 0);
            AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
        }
    }
}

void Level2_Free()
{
    AEGfxMeshFree(pMesh);
    if (map) {
        delete[] map;
        map = nullptr;
    }
}

void Level2_Unload() {
    for (int i = 1; i <= 11; ++i) {
        if (baseTextures[i]) AEGfxTextureUnload(baseTextures[i]);
    }
}
