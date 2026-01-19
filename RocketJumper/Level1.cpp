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
#include <fstream>
#include "Level1.h"
#include "collision.h"
#include "GameStateManager.h"

namespace renderlogic {
	void Drawsquare(f32 xpos, f32 ypos, f32 xsize, f32 ysize) {
		AEMtx33 scale = { 0 };
		AEMtx33Scale(&scale, xsize, ysize);

		AEMtx33 rotate = { 0 };
		AEMtx33Rot(&rotate, 0.0f);

		AEMtx33 translate = { 0 };
		AEMtx33Trans(&translate, xpos, ypos);

		AEMtx33 transform = { 0 };
		AEMtx33Concat(&transform, &rotate, &scale);
		AEMtx33Concat(&transform, &translate, &transform);

		AEGfxSetTransform(transform.m);

	}
}

objectsquares objectinfo[2] = { 0 };

AEGfxVertexList* pMesh;
AEGfxVertexList* HMesh;

void Level1_Load()
{
	objectinfo[player].xpos = 0.0f;
	objectinfo[player].ypos = 0.0f;
	objectinfo[player].xscale = 100.0f;
	objectinfo[player].yscale = 100.0f;

	objectinfo[obstacle].xpos = -400.0f;
	objectinfo[obstacle].ypos = 0.0f;
	objectinfo[obstacle].xscale = 100.0f;
	objectinfo[obstacle].yscale = 400.0f;
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

	AEGfxTriAdd(
		0.0f, -0.5f, 0xFF000000, 0.0f, 1.0f,
		1.0f, -0.5f, 0xFF000000, 1.0f, 1.0f,
		0.0f, 0.5f, 0xFF000000, 0.0f, 0.0f);

	AEGfxTriAdd(
		1.0f, -0.5f, 0xFF000000, 1.0f, 1.0f,
		1.0f, 0.5f, 0xFF000000, 1.0f, 0.0f,
		0.0f, 0.5f, 0xFF000000, 0.0f, 0.0f);

	HMesh = AEGfxMeshEnd();
}
void Level1_Initialize()
{
	

}

void Level1_Update()
{	
	if (gamelogic::collision(&objectinfo[player], &objectinfo[obstacle])) {
		printf("collision");
	}
}

void Level1_Draw()
{
	

	AEGfxSetBackgroundColor(0.5f, 0.5f, 0.5f);

	AEGfxSetRenderMode(AE_GFX_RM_COLOR);

	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);

	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(1.0f);

	AEGfxSetColorToAdd(1.0f, 0.0f, 0.0f, 0.0f);
	renderlogic::Drawsquare(objectinfo[obstacle].xpos, objectinfo[obstacle].ypos, objectinfo[obstacle].xscale, objectinfo[obstacle].yscale);
	AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);

	AEGfxSetColorToAdd(1.0f, 1.0f, 1.0f, 1.0f);
	renderlogic::Drawsquare(objectinfo[player].xpos, objectinfo[player].ypos, objectinfo[player].xscale, objectinfo[player].yscale);
	AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
}

void Level1_Free()
{
	AEGfxMeshFree(HMesh);
	AEGfxMeshFree(pMesh);
}

void Level1_Unload()
{
	
}