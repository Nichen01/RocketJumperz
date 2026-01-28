#pragma once

#include "AEEngine.h"

extern AEGfxVertexList* textureMesh;

extern AEGfxTexture* characterPic;
extern AEGfxTexture* base1;
extern AEGfxTexture* base2;
extern AEGfxTexture* base3;
extern AEGfxTexture* base4;
extern AEGfxTexture* base5;
extern AEGfxTexture* base6;
extern AEGfxTexture* base7;
extern AEGfxTexture* base8;
extern AEGfxTexture* base9;

extern AEMtx33 platformTransform1, platformTransform2, platformTransform2_1, platformTransform3;
extern AEMtx33 platformTransform4, platformTransform5, platformTransform5_1, platformTransform6;
extern AEMtx33 platformTransform7, platformTransform8, platformTransform8_1, platformTransform9;
extern AEMtx33 platformTransform10, platformTransform11, platformTransform11_1, platformTransform12;
extern AEMtx33 transformChar;

void initMesh(void);
void loadTextures(void);
void unloadTextures(void);
void initTransforms(void);
void drawFloors(void);
void drawCharacter(void);

