#pragma once
#include "AEEngine.h"
#include "animation.h"

namespace renderlogic {
	void Drawsquare(f32 xPos, f32 yPos, f32 xsize, f32 ysize);
	void drawmap_Wall_floor(int map[], int mapX, int mapY, int mapS);
}

// Declare these as extern - they will be defined in Level1.cpp
extern AEGfxVertexList* pMesh;
extern AEGfxVertexList* platformMesh;
extern AEGfxTexture* characterPictest;
extern AEGfxTexture* base5test;
extern AEGfxTexture* plasma;

// DoorOpen.jpg: 224 x 32 pixels, single row of 7 frames (32x32 each)
// ---------------------------------------------------------------------------
extern int  DOOR_FRAME_COUNT;
extern f32  DOOR_FRAME_DELAY;   // ~12 fps
extern f32  doorX;  // adjust to match your map
extern f32  doorY;
extern f32  DOOR_WIDTH;   // matches tile size s
extern f32  DOOR_HEIGHT;
extern f32  DOOR_TRIGGER_RADIUS;  // px from door centre
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
extern AEGfxTexture* glass;
