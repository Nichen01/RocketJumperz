
#include "Draw.h"
#include "AEGraphics.h"
#include "BinaryMap.h"

// Global texture pointers -- declared extern in draw.h, defined here
AEGfxTexture* characterPictest = NULL;
AEGfxTexture* base5test = NULL;
AEGfxTexture* plasma = NULL;

// Different types of platforms
AEGfxTexture* platform1 = NULL, * platform2 = NULL, * platform3 = NULL, * platform4 = NULL, * platform5 = NULL, * platform6 = NULL, * platform7 = NULL, * platform8 = NULL, * platform9 = NULL;
AEGfxTexture* glass0 = NULL, *glass1 = NULL, *glass2 = NULL, *glass3 = NULL, *glass4 = NULL;

AEGfxVertexList* pMesh = nullptr;
AEGfxVertexList * platformMesh = nullptr;

f32 doorX, doorY;
s32  doorFrameCount = 7;
f32  doorFrameDelay = 0.08f;   // ~12 fps
f32  doorWidth = 80.f;   // matches tile size s
f32  doorHeight = 80.f;
f32  doorTriggerRadius = 150.0f;  // px from door centre
SpriteAnimation  doorAnim;
AEGfxVertexList* doorMesh;
bool doorIsOpen = false; // tracks fully-open state
AEGfxTexture* doorTex;

namespace renderlogic {
	void initPlatformMesh() {
		AEGfxMeshStart();
		AEGfxTriAdd(
			-0.5f, -0.5f, 0xFFFFFFFF, 0.0f, 1.0f,
			0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f,
			-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f);

		AEGfxTriAdd(
			0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f,
			0.5f, 0.5f, 0xFFFFFFFF, 1.0f, 0.0f,
			-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f);

		platformMesh = AEGfxMeshEnd();
	}

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

	void drawmap_Wall_floor(int map[], int mapX, int mapY, int mapS) {
		int xo, yo;
		//iterate and print map in 2D map array
		for (s32 row = 0; row < BINARY_MAP_HEIGHT; row++) {	  // check for row
			for (s32 col = 0; col < BINARY_MAP_WIDTH; col++) { // check for column

				xo = (col * mapS);
				yo = (row * mapS);

				//default background

				switch (MapData[row][col]) {
				case 0: // air tile
				{
					int glassType = glassMap[row][col];
					if (glassType >= 0) {
						AEGfxSetTransparency(0.4f);
						switch (glassType) {
						case 0: AEGfxTextureSet(glass0, 0, 0); break;
						case 1: AEGfxTextureSet(glass1, 0, 0); break;
						case 2: AEGfxTextureSet(glass2, 0, 0); break;
						case 3: AEGfxTextureSet(glass3, 0, 0); break;
						case 4: AEGfxTextureSet(glass4, 0, 0); break;
						}
						renderlogic::Drawsquare(((float)xo + mapS / 2) - 800.0f,
							450.0f - ((float)yo + mapS / 2),
							(float)mapS, (float)mapS);
						AEGfxMeshDraw(platformMesh, AE_GFX_MDM_TRIANGLES);
						AEGfxSetTransparency(1.f);
					}
				}
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

	void drawTileArray() {

		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxTextureSet(platform5, 0, 0);
		AEMtx33 scl, rot, trans, transf;
		AEMtx33Scale(&scl, 80.f, 80.f);
		AEMtx33Rot(&rot, 0);
		f32 coordsX{};

		for (int j{}; j < 20; j++) {
			AEMtx33Trans(&trans, -760.f + coordsX, -467.f);
			AEMtx33Concat(&transf, &rot, &scl);
			AEMtx33Concat(&transf, &trans, &transf);
			AEGfxSetTransform(transf.m);
			AEGfxMeshDraw(platformMesh, AE_GFX_MDM_TRIANGLES);
			coordsX += 80.f;
		}

	}
}


