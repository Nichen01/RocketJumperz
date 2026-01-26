/* Start Header ************************************************************************/
/*!
\file		  Level1.cpp
\author       Ivan Chong, i.chong, 2503476
\par          i.chong@digipen.edu
\date         January, 16, 2026
\brief        Contain functions called by GameStateManager.cpp

Copyright (C) 2026 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#include <iostream>
#include "Level1.h"
#include "draw.h"
#include "collision.h"
#include "player.h"
#include "GameStateManager.h"




s32* map = new s32[144]{ 0 };
int x = 16;
int y = 9;
int s = 100;

objectsquares objectinfo[2] = { 0 };

AEGfxVertexList* pMesh = 0;


void Level1_Load()
{
	
}
void Level1_Initialize()
{	
	AEGfxMeshStart();

	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFF000000, 0.0f, 1.0f,
		0.5f, -0.5f, 0xFF000000, 1.0f, 1.0f,
		-0.5f, 0.5f, 0xFF000000, 0.0f, 0.0f);

	AEGfxTriAdd(
		0.5f, -0.5f, 0xFF000000, 1.0f, 1.0f,
		0.5f, 0.5f, 0xFF000000, 1.0f, 0.0f,
		-0.5f, 0.5f, 0xFF000000, 0.0f, 0.0f);

	pMesh = AEGfxMeshEnd();

	int x = 0, y = 0;
	for (y = 0; y < 9; y++) {
		if (y == 0 || y == 9 - 1) {
			for (x = 0; x < 16; x++) {
				map[(y * 16 + x)] = 1;
			}
		}
		else {
			for (x = 0; x < 16; x++) {
				if (x == 0 || x == 16 - 1) {
					map[(y * 16 + x)] = 1;
				}
				else {
					map[(y * 16 + x)] = 0;
				}
			}
		}
	}
	map[(4 * 16 + 6)] = 1;

	objectinfo[player].xPos = 0.0f;
	objectinfo[player].yPos = 0.0f;
	objectinfo[player].xScale = 70.0f;
	objectinfo[player].yScale = 70.0f;

	objectinfo[obstacle].xPos = -400.0f;
	objectinfo[obstacle].yPos = 0.0f;
	objectinfo[obstacle].xScale = 100.0f;
	objectinfo[obstacle].yScale = 400.0f;

	
}

void Level1_Update()
{
	gamelogic::Xcheck(map, x, s);
	gamelogic::Ycheck(map, x, s);
}

void Level1_Draw()
{
	AEGfxSetBackgroundColor(0.5f, 0.5f, 0.5f);

	AEGfxSetRenderMode(AE_GFX_RM_COLOR);

	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);

	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(1.0f);

	renderlogic::drawmap_Wall_floor(map, x, y, s);

	AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
	renderlogic::Drawsquare(objectinfo[player].xPos, objectinfo[player].yPos, objectinfo[player].xScale, objectinfo[player].yScale);
	AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
}

void Level1_Free()
{
	AEGfxMeshFree(pMesh);
	delete[] map;
}

void Level1_Unload()
{
	
}