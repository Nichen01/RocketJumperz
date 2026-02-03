#include "draw.h"
#include "AEGraphics.h"

extern AEGfxTexture* characterPictest = NULL;
extern AEGfxTexture* base5test = NULL;

AEGfxVertexList* pMesh = nullptr;
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
	void drawmap_Wall_floor(int map[], int mapX, int mapY, int mapS) {
		int x, y, xo, yo;
		//iterate and print map in 2D map array
		for (y = 0; y < mapY; y++) {	  // check for row
			for (x = 0; x < mapX; x++) { // check for column

				xo = (x * mapS);
				yo = (y * mapS);
				//default background
				
				switch (map[(y * mapX + x)]) {
				case 0: //playable area
					break;
				case 1: //wall
					AEGfxTextureSet(base5test, 0, 0);
					AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 1.0f);
					renderlogic::Drawsquare(((float)xo + mapS / 2)-800.0f, 450.0f-((float)yo + mapS / 2), (float)mapS, (float)mapS);
					AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
					break;
				case 10: //goal,green
					AEGfxSetColorToAdd(0.0f, 1.0f, 0.0f, 1.0f);
					renderlogic::Drawsquare(((float)xo + mapS / 2) - 800.0f, 450.0f - ((float)yo + mapS / 2), (float)mapS, (float)mapS);
					AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
					break;
				default://defaults to playable area
					break;
				}

			}

		}

	}
}


