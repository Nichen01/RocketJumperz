#include "draw.h"
#include "AEGraphics.h"
#include "binaryMap.h"

// Global texture pointers -- declared extern in draw.h, defined here
AEGfxTexture* characterPictest = NULL;
AEGfxTexture* base5test = NULL;
AEGfxTexture* plasma = NULL;

// Different types of platforms
AEGfxTexture* platform1 = NULL;
AEGfxTexture* platform2 = NULL;
AEGfxTexture* platform3 = NULL;
AEGfxTexture* platform4 = NULL;
AEGfxTexture* platform5 = NULL;
AEGfxTexture* platform6 = NULL;
AEGfxTexture* platform7 = NULL;
AEGfxTexture* platform8 = NULL;
AEGfxTexture* platform9 = NULL;
AEGfxTexture* glass = NULL;

AEGfxVertexList* pMesh = nullptr;
AEGfxVertexList * platformMesh = nullptr;
namespace renderlogic {
	void Drawsquare(f32 xPos, f32 yPos, f32 xsize, f32 ysize) {
		AEMtx33 scale = { 0 };
		AEMtx33Scale(&scale, xsize, ysize);

		AEMtx33 rotate = { 0 };
		AEMtx33Rot(&rotate, 0.0f);

		AEMtx33 translate = { 0 };
		AEMtx33Trans(&translate, xPos, yPos);

		AEMtx33 transform = { 0 };
		AEMtx33Concat(&transform, &rotate, &scale);
		AEMtx33Concat(&transform, &translate, &transform);

		AEGfxSetTransform(transform.m);

	}
	void drawmap_Wall_floor(int /*map*/[], int /*mapX*/, int /*mapY*/, int mapS) {
		int xo, yo;
		//iterate and print map in 2D map array
		for (s32 row = 0; row < BINARY_MAP_HEIGHT; row++) {	  // check for row
			for (s32 col = 0; col < BINARY_MAP_WIDTH; col++) { // check for column

				xo = (col * mapS);
				yo = (row * mapS);

				//default background
				
				switch (MapData[row][col]) {
				case 0: //air
					AEGfxTextureSet(glass, 0, 0);
					AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
					renderlogic::Drawsquare(((float)xo + mapS / 2) - 800.0f, 450.0f - ((float)yo + mapS / 2), (float)mapS, (float)mapS);
					AEGfxMeshDraw(platformMesh, AE_GFX_MDM_TRIANGLES);
					break;
				case 11: // platform1
					AEGfxTextureSet(platform1, 0, 0);
					AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
					renderlogic::Drawsquare(((float)xo + mapS / 2)-800.0f, 450.0f-((float)yo + mapS / 2), (float)mapS, (float)mapS);
					AEGfxMeshDraw(platformMesh, AE_GFX_MDM_TRIANGLES);
					break;
				case 12: // platform2
					AEGfxTextureSet(platform2, 0, 0);
					AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
					renderlogic::Drawsquare(((float)xo + mapS / 2) - 800.0f, 450.0f - ((float)yo + mapS / 2), (float)mapS, (float)mapS);
					AEGfxMeshDraw(platformMesh, AE_GFX_MDM_TRIANGLES);
					break;
				case 13: // platform3
					AEGfxTextureSet(platform3, 0, 0);
					AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
					renderlogic::Drawsquare(((float)xo + mapS / 2) - 800.0f, 450.0f - ((float)yo + mapS / 2), (float)mapS, (float)mapS);
					AEGfxMeshDraw(platformMesh, AE_GFX_MDM_TRIANGLES);
					break;
				case 14: // platform4
					AEGfxTextureSet(platform4, 0, 0);
					AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
					renderlogic::Drawsquare(((float)xo + mapS / 2) - 800.0f, 450.0f - ((float)yo + mapS / 2), (float)mapS, (float)mapS);
					AEGfxMeshDraw(platformMesh, AE_GFX_MDM_TRIANGLES);
					break;
				case 15: // platform5
					AEGfxTextureSet(platform5, 0, 0);
					AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
					renderlogic::Drawsquare(((float)xo + mapS / 2) - 800.0f, 450.0f - ((float)yo + mapS / 2), (float)mapS, (float)mapS);
					AEGfxMeshDraw(platformMesh, AE_GFX_MDM_TRIANGLES);
					break;
				case 16: // platform6
					AEGfxTextureSet(platform6, 0, 0);
					AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
					renderlogic::Drawsquare(((float)xo + mapS / 2) - 800.0f, 450.0f - ((float)yo + mapS / 2), (float)mapS, (float)mapS);
					AEGfxMeshDraw(platformMesh, AE_GFX_MDM_TRIANGLES);
					break;
				case 17: // platform7
					AEGfxTextureSet(platform7, 0, 0);
					AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
					renderlogic::Drawsquare(((float)xo + mapS / 2) - 800.0f, 450.0f - ((float)yo + mapS / 2), (float)mapS, (float)mapS);
					AEGfxMeshDraw(platformMesh, AE_GFX_MDM_TRIANGLES);
					break;
				case 18: // platform8
					AEGfxTextureSet(platform8, 0, 0);
					AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
					renderlogic::Drawsquare(((float)xo + mapS / 2) - 800.0f, 450.0f - ((float)yo + mapS / 2), (float)mapS, (float)mapS);
					AEGfxMeshDraw(platformMesh, AE_GFX_MDM_TRIANGLES);
					break;
				case 19: // platform9
					AEGfxTextureSet(platform9, 0, 0);
					AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
					renderlogic::Drawsquare(((float)xo + mapS / 2) - 800.0f, 450.0f - ((float)yo + mapS / 2), (float)mapS, (float)mapS);
					AEGfxMeshDraw(platformMesh, AE_GFX_MDM_TRIANGLES);
					break;
				case 21: // door tile 
					AEGfxTextureSet(doorTex, animSystem::getUOffset(doorAnim), 0.f); 
					renderlogic::Drawsquare(((float)xo + mapS / 2) - 800.0f, 450.0f - ((float)yo + mapS / 2), (float)mapS, (float)mapS); 
					AEGfxMeshDraw(doorMesh, AE_GFX_MDM_TRIANGLES); 
					break;
				default://defaults to playable area
					break;
				}

			}

		}

	}
}


