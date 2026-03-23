#include "drops.h"
#include "collision.h"
#include "draw.h"
#include "movement.h"

namespace pickup {
	AEMtx33 dropTransform = { 0 };
	void initDrops(drop instance[], int max,f32 scale) {
		for (int i{};i < max;i++) {
			instance[i].info.xScale = scale;
			instance[i].info.yScale = scale;
			instance[i].info.flag = 0;

		}
	}
	void updateDrops(drop instance[],int max,objectsquares& player) {
		for (int i{};i < max;i++) {
			if (instance[i].info.flag == 0) continue;
			else if (gamelogic::static_collision(&player, &instance[i].info)) {
				instance[i].info.flag = 0;
				movement::bulletCount += 25;
				player.health += 50;

			}
		}
	}
	void drawDrops(drop instance[], int max) {
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		for (int i{};i < max;i++) {
			if (instance[i].info.flag == 0) continue;
			else {
				renderlogic::drawSquare(instance[i].info.xPos, instance[i].info.yPos, instance[i].info.xScale, instance[i].info.xScale);
				AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
				//printf("\n\ndrawn drop:%d\n\n", i);
			}
		}
		// Reset color state so subsequent draw calls are not tinted green
		AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxSetColorToAdd(0, 0, 0, 0);
	}
}