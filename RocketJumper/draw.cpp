#include "Draw.h"


// Global texture pointers -- declared extern in draw.h, defined here
AEGfxTexture* characterPictest = NULL;
AEGfxTexture* plasma = NULL;

Enemy enemies[MAX_ENEMIES];

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

	void drawSquare(f32 xPos, f32 yPos, f32 xsize, f32 ysize) {
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

	void drawMapWallFloor(int map[], int mapX, int mapY, int mapS) {
		int xo, yo;
		map = map;
		mapY = mapY;
		mapX = mapX;
		//iterate and print map in 2D map array
		for (s32 row = 0; row < BINARY_MAP_HEIGHT; row++) {	  // check for row
			for (s32 col = 0; col < BINARY_MAP_WIDTH; col++) { // check for column

				xo = (col * mapS);
				yo = (row * mapS);

				//float enemyWorldX = ((float)xo + mapS / 2) - 800.0f;
				//float enemyWorldY = 450.0f - ((float)yo + mapS / 2);

				//default background

				switch (MapData[row][col]) {
				case 0: // air tile
					drawGlass(row, col, mapS, platformMesh);
				break;
				case 11: // platform1
					AEGfxTextureSet(platform1, 0, 0);
					AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
					renderlogic::drawSquare(((float)xo + mapS / 2)-800.0f, 450.0f-((float)yo + mapS / 2), (float)mapS, (float)mapS);
					AEGfxMeshDraw(platformMesh, AE_GFX_MDM_TRIANGLES);
					break;
				case 12: // platform2
					AEGfxTextureSet(platform2, 0, 0);
					AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
					renderlogic::drawSquare(((float)xo + mapS / 2) - 800.0f, 450.0f - ((float)yo + mapS / 2), (float)mapS, (float)mapS);
					AEGfxMeshDraw(platformMesh, AE_GFX_MDM_TRIANGLES);
					break;
				case 13: // platform3
					AEGfxTextureSet(platform3, 0, 0);
					AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
					renderlogic::drawSquare(((float)xo + mapS / 2) - 800.0f, 450.0f - ((float)yo + mapS / 2), (float)mapS, (float)mapS);
					AEGfxMeshDraw(platformMesh, AE_GFX_MDM_TRIANGLES);
					break;
				case 14: // platform4
					AEGfxTextureSet(platform4, 0, 0);
					AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
					renderlogic::drawSquare(((float)xo + mapS / 2) - 800.0f, 450.0f - ((float)yo + mapS / 2), (float)mapS, (float)mapS);
					AEGfxMeshDraw(platformMesh, AE_GFX_MDM_TRIANGLES);
					break;
				case 15: // platform5
					AEGfxTextureSet(platform5, 0, 0);
					AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
					renderlogic::drawSquare(((float)xo + mapS / 2) - 800.0f, 450.0f - ((float)yo + mapS / 2), (float)mapS, (float)mapS);
					AEGfxMeshDraw(platformMesh, AE_GFX_MDM_TRIANGLES);
					break;
				case 16: // platform6
					AEGfxTextureSet(platform6, 0, 0);
					AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
					renderlogic::drawSquare(((float)xo + mapS / 2) - 800.0f, 450.0f - ((float)yo + mapS / 2), (float)mapS, (float)mapS);
					AEGfxMeshDraw(platformMesh, AE_GFX_MDM_TRIANGLES);
					break;
				case 17: // platform7
					AEGfxTextureSet(platform7, 0, 0);
					AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
					renderlogic::drawSquare(((float)xo + mapS / 2) - 800.0f, 450.0f - ((float)yo + mapS / 2), (float)mapS, (float)mapS);
					AEGfxMeshDraw(platformMesh, AE_GFX_MDM_TRIANGLES);
					break;
				case 18: // platform8
					AEGfxTextureSet(platform8, 0, 0);
					AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
					renderlogic::drawSquare(((float)xo + mapS / 2) - 800.0f, 450.0f - ((float)yo + mapS / 2), (float)mapS, (float)mapS);
					AEGfxMeshDraw(platformMesh, AE_GFX_MDM_TRIANGLES);
					break;
				case 19: // platform9
					AEGfxTextureSet(platform9, 0, 0);
					AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
					renderlogic::drawSquare(((float)xo + mapS / 2) - 800.0f, 450.0f - ((float)yo + mapS / 2), (float)mapS, (float)mapS);
					AEGfxMeshDraw(platformMesh, AE_GFX_MDM_TRIANGLES);
					break;
				case 21: case 22: case 23: case 24: case 25: case 26: case 27: case 28: case 29:
				{
					if (!doorMesh || !doorTex) break;  // safety guard
					// Find the matching door for this tile position
					float tileWorldX = ((float)xo + mapS / 2) - 800.0f;
					float tileWorldY = 450.0f - ((float)yo + mapS / 2);

					f32 uOffset = 0.f; // default to frame 0
					for (auto& door : doors) {
						// Match this tile to the correct door by position,
						// using currentGameLevel so it works on every level.
						if ((door.entranceLevel == currentGameLevel || door.exitLevel == currentGameLevel) &&
							fabsf(door.worldX - tileWorldX) < 1.f &&
							fabsf(door.worldY - tileWorldY) < 1.f)
						{
							uOffset = animSystem::getUOffset(door.anim);
							break;
						}
					}

					AEGfxTextureSet(doorTex, uOffset, 0.f);
					renderlogic::drawSquare(tileWorldX, tileWorldY, (float)mapS, (float)mapS);
					AEGfxMeshDraw(doorMesh, AE_GFX_MDM_TRIANGLES);
					break;
				}
				case 67: // key
					drawGlass(row, col, mapS, platformMesh);

					if (key.active) {
						AEGfxTextureSet(keyTexture, 0, 0);
						renderlogic::drawSquare(
							((float)xo + mapS / 2) - AEGfxGetWindowWidth() / 2,
							AEGfxGetWindowHeight() / 2 - ((float)yo + mapS / 2),
							(float)mapS,
							(float)mapS
						);
						AEGfxMeshDraw(platformMesh, AE_GFX_MDM_TRIANGLES);
					}
					break;
				case 69: // final door
					AEGfxTextureSet(platform9, 0, 0);
					AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
					renderlogic::drawSquare(((float)xo + mapS / 2) - 800.0f, 450.0f - ((float)yo + mapS / 2), (float)mapS, (float)mapS);
					AEGfxMeshDraw(platformMesh, AE_GFX_MDM_TRIANGLES);
					break;
				default: //defaults to playable area
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

	void drawTexture(f32 x, f32 y, AEGfxTexture* textureAsset, AEGfxVertexList* mesh, f32 scaleX, f32 scaleY) {
		AEMtx33 uiScl, uiRot, uiTransl, uiTransf;
		AEMtx33Scale(&uiScl, scaleX, scaleY);
		AEMtx33Rot(&uiRot, 0);
		
		AEMtx33Trans(&uiTransl, x, y);
		AEMtx33Concat(&uiTransf, &uiRot, &uiScl);
		AEMtx33Concat(&uiTransf, &uiTransl, &uiTransf);
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxTextureSet(textureAsset, 0, 0);
		AEGfxSetTransform(uiTransf.m);
		AEGfxMeshDraw(mesh, AE_GFX_MDM_TRIANGLES);
	}

	void flashingTexture(f32 x, f32 y, AEGfxTexture* textureAsset, f32 scale = 40.f) {
		static f32 timer = 0.f;
		f32 dt = static_cast<f32>(AEFrameRateControllerGetFrameTime());
		timer += dt;

		f32 alpha = (sinf(timer * 6.f) + 1.f) / 2.f;
		alpha = 0.4f + alpha * 0.6f;

		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxSetColorToMultiply(1.f, 1.f, 1.f, 1.f);
		AEGfxSetColorToAdd(0.f, 0.f, 0.f, 0.f);
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		AEGfxSetTransparency(alpha);
		AEGfxTextureSet(textureAsset, 0.f, 0.f);
		drawSquare(x, y, scale, scale);
		AEGfxMeshDraw(platformMesh, AE_GFX_MDM_TRIANGLES);
		AEGfxSetTransparency(1.f);  // reset transparency after drawing
	}

	// Draw glass only if the current cell has a valid glass type
	void drawGlass(int row, int col, int mapS, AEGfxVertexList* mesh) {
		int glassType = glassMap[row][col];
		if (glassType < 0) return; // skip if no glass assigned

		// Semi-transparent overlay
		AEGfxSetTransparency(0.4f);

		// Select texture based on type
		switch (glassType) {
		case 0: AEGfxTextureSet(glass0, 0, 0); break;
		case 1: AEGfxTextureSet(glass1, 0, 0); break;
		case 2: AEGfxTextureSet(glass2, 0, 0); break;
		case 3: AEGfxTextureSet(glass3, 0, 0); break;
		case 4: AEGfxTextureSet(glass4, 0, 0); break;
		}

		// Position square in world space
		float xPos = (col * mapS + mapS / 2.0f) - 800.0f;
		float yPos = 450.0f - (row * mapS + mapS / 2.0f);

		renderlogic::drawSquare(xPos, yPos, (float)mapS, (float)mapS);
		AEGfxMeshDraw(mesh, AE_GFX_MDM_TRIANGLES);

		// Reset transparency so later draws aren’t affected
		AEGfxSetTransparency(1.0f);
	}

	void drawUITexture(f32 x, f32 y, AEGfxTexture* textureAsset, f32 scale) {
		AEMtx33 uiScl, uiRot, uiTransl, uiTransf;
		AEMtx33Scale(&uiScl, scale, scale);
		AEMtx33Rot(&uiRot, 0);

		AEMtx33Trans(&uiTransl, x, y);
		AEMtx33Concat(&uiTransf, &uiRot, &uiScl);
		AEMtx33Concat(&uiTransf, &uiTransl, &uiTransf);
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxTextureSet(textureAsset, 0, 0);
		AEGfxSetTransform(uiTransf.m);
		AEGfxMeshDraw(uiMesh, AE_GFX_MDM_TRIANGLES);
	}

	void drawWireInventory(int wire) {
		AEMtx33 uiScl, uiRot, uiTransl, uiTransf;
		AEMtx33Scale(&uiScl, 100.f, 100.f);
		AEMtx33Rot(&uiRot, 0);

		AEMtx33Trans(&uiTransl, -650.f, -400.f);
		AEMtx33Concat(&uiTransf, &uiRot, &uiScl);
		AEMtx33Concat(&uiTransf, &uiTransl, &uiTransf);
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);

		if (wire == 0) {
			AEGfxTextureSet(inventory, 0, 0);
		}
		else if (wire == 1) {
			AEGfxTextureSet(wireInventory0, 0, 0);
		}
		else if (wire == 2) {
			AEGfxTextureSet(wireInventory1, 0, 0);
		}
		else if (wire == 3) {
			AEGfxTextureSet(wireInventory2, 0, 0);
		}
		
		AEGfxSetTransform(uiTransf.m);
		AEGfxMeshDraw(uiMesh, AE_GFX_MDM_TRIANGLES);
	}

}


