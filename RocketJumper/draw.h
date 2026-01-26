#pragma once
#include "AEEngine.h"

namespace renderlogic {
	void Drawsquare(f32 xPos, f32 yPos, f32 xsize, f32 ysize);
	void drawmap_Wall_floor(int map[], int mapX, int mapY, int mapS);
}

extern AEGfxVertexList* pMesh;