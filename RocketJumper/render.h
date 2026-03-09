#pragma once
#include "AEEngine.h" 

extern AEGfxTexture* platformTileArray[20];

namespace render {
	void Drawsquare(f32 xPos, f32 yPos, f32 xsize, f32 ysize);
	void drawPlatform();
	void unloadPlatform();
}
