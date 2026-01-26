#ifndef DRAWWALLSLEVEL2_H
#define DRAWWALLSLEVEL2_H

#include "AEEngine.h"

extern AEGfxVertexList* textureMesh;

extern AEGfxTexture* characterPic;
extern AEGfxTexture* enemyPic;
extern AEGfxTexture* base1;
extern AEGfxTexture* base2;
extern AEGfxTexture* base3;
extern AEGfxTexture* base4;
extern AEGfxTexture* base5;
extern AEGfxTexture* base6;
extern AEGfxTexture* base7;
extern AEGfxTexture* base8;
extern AEGfxTexture* base9;
extern AEGfxTexture* base10;
extern AEGfxTexture* base11;
extern AEGfxTexture* base12;

extern AEMtx33 platformTransform65, platformTransform66, platformTransform67, platformTransform68;

void initMesh2(void);
void loadTextures2(void);
void unloadTextures2(void);
void initTransform2(void);
void drawWalls2(void);

#endif // DRAWWALLSLEVEL2_H
