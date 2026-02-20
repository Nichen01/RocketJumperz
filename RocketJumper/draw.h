#pragma once
#include "AEEngine.h"

namespace renderlogic {
	void Drawsquare(f32 xPos, f32 yPos, f32 xsize, f32 ysize);
	void drawmap_Wall_floor(int map[], int mapX, int mapY, int mapS);
}

// Declare these as extern - they will be defined in Level1.cpp
extern AEGfxVertexList* pMesh;
extern AEGfxTexture* characterPictest;
extern AEGfxTexture* base5test;
extern AEGfxTexture* platformTex1;
extern AEGfxTexture* platformTex2;
extern AEGfxTexture* platformTex3;