#ifndef DRAWWALLSLEVEL1_H
#define DRAWWALLSLEVEL1_H

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
extern AEGfxTexture* base10;
extern AEGfxTexture* base11;
extern AEGfxTexture* base12;

extern AEMtx33 platformTransform1, platformTransform2, platformTransform3, platformTransform4;
extern AEMtx33 platformTransform5, platformTransform6, platformTransform7, platformTransform8;
extern AEMtx33 platformTransform9, platformTransform10, platformTransform11, platformTransform12;
extern AEMtx33 platformTransform13, platformTransform14, platformTransform15;
extern AEMtx33 platformTransform16, platformTransform17, platformTransform18;
extern AEMtx33 platformTransform19, platformTransform20, platformTransform21, platformTransform22, platformTransform23, platformTransform24, platformTransform25, platformTransform26, platformTransform27, platformTransform28, platformTransform29, platformTransform30, platformTransform31;
extern AEMtx33 platformTransform32, platformTransform33, platformTransform34, platformTransform35, platformTransform36, platformTransform37, platformTransform38, platformTransform39, platformTransform40, platformTransform41, platformTransform42, platformTransform43, platformTransform44;
extern AEMtx33 transformChar, transformDoor;
extern AEMtx33 platformTransform45, platformTransform46, platformTransform47, platformTransform48, platformTransform49, platformTransform50, platformTransform51, platformTransform52, platformTransform53, platformTransform54, platformTransform55;
extern AEMtx33 platformTransform56, platformTransform57, platformTransform58, platformTransform59, platformTransform60, platformTransform61, platformTransform62, platformTransform63, platformTransform64;

void initMesh1(void);
void loadTextures1(void);
void unloadTextures1(void);
void initTransform1(void);
void drawWalls1(void);

#endif // DRAWWALLSLEVEL1_H
