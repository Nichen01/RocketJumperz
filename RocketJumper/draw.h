#pragma once
#include "AEEngine.h"
#include "Animation.h"
#include "Level1.h"

namespace renderlogic {
	void drawSquare(f32 xPos, f32 yPos, f32 xsize, f32 ysize);
	void drawMapWallFloor(int map[], int mapX, int mapY, int mapS);
	void drawTileArray();
}

// Declare these as extern - they will be defined in Level1.cpp
extern AEGfxVertexList* pMesh;

extern AEGfxTexture* characterPictest;
extern AEGfxTexture* platformTex1;
extern AEGfxTexture* platformTex2;
extern AEGfxTexture* platformTex3;
extern AEGfxTexture* plasma;

// DoorOpen.jpg: 224 x 32 pixels, single row of 7 frames (32x32 each)
// ---------------------------------------------------------------------------
extern int  doorFrameCount;
extern f32  doorFrameDelay;   // ~12 fps
extern f32  doorX;  // adjust to match your map
extern f32  doorY;
extern f32  doorWidth;   // matches tile size s
extern f32  doorHeight;
extern f32  doorTriggerRadius;  // px from door centre
extern SpriteAnimation  doorAnim;
extern AEGfxVertexList* doorMesh;
extern bool doorIsOpen; // tracks fully-open state
extern AEGfxTexture* doorTex;

// ---------------------------------------------------------------------------

extern AEGfxTexture* platform1;
extern AEGfxTexture* platform2;
extern AEGfxTexture* platform3;
extern AEGfxTexture* platform4;
extern AEGfxTexture* platform5;
extern AEGfxTexture* platform6;
extern AEGfxTexture* platform7;
extern AEGfxTexture* platform8;
extern AEGfxTexture* platform9;
extern AEGfxTexture* glass0, *glass1, *glass2, *glass3, *glass4;


