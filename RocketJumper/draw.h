#pragma once
#include "pch.h"
#include "AEEngine.h"
#include "Animation.h"
#include "Level1.h"
#include "AEGraphics.h"
#include "BinaryMap.h"
#include "Enemies.h"
#include "Movement.h"

namespace renderlogic {
	void drawSquare(f32 xPos, f32 yPos, f32 xsize, f32 ysize);
	void drawMapWallFloor(int map[], int mapX, int mapY, int mapS);
	void drawTileArray();
	void drawTexture(f32 x, f32 y, AEGfxTexture* textureAsset, AEGfxVertexList* mesh, f32 scaleX = 40.f, f32 scaleY = 40.f);
	void flashingTexture(f32 x, f32 y, AEGfxTexture* textureAsset, f32 scale);
	void drawGlass(int row, int col, int mapS, AEGfxVertexList* platformMesh);
	void drawUITexture(f32 x, f32 y, AEGfxTexture* textureAsset, f32 scale);
	void drawWireInventory(int);
	void drawCooldownHUD(f32, f32);
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



