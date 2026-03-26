#include "traps.h"
#include "collision.h"
#include "player.h"
#include "enemies.h"
#include "binaryMap.h"
#include "AssetManager.h"

// ---------------------------------------------------------------------------
// Saw animation constants
// ---------------------------------------------------------------------------
static const int   kSawCols       = 6;     // 6 frames in a single row (192px / 32px per frame)
static const int   kSawRows       = 1;
static const int   kSawTotalFrames = 6;
static const f32   kSawFrameDelay  = 0.08f; // ~12 fps spin speed

namespace traps {

	// Saw animation instance -- all saw tiles share the same playback state
	SpriteAnimation sawAnim;

	s8 trapDamage = 5;
	s8 trapInstanceCooldown = 0;
	s8 trapRange = 3;
	bool nearTrap = 0;
	AEVec2 nearestTrap = {};
	f32 angle;
	AEMtx33 trapTransform;

	void initTraps() {
		nearTrap = 0;
		AEVec2Zero(&nearestTrap);

		// Initialize the saw animation to loop continuously from frame 0
		animSystem::init(sawAnim, kSawCols, kSawRows, kSawTotalFrames,
			kSawFrameDelay, ANIM_LOOP, 0);

		// Build the spritesheet mesh for the saw (1 row, 7 columns)
		AssetManager::BuildSqrMesh(MESH_SAW, kSawRows, kSawCols);
	}
	AEVec2 checkNearestTrap(float NposX,float NposY, int* map, int x,int y) {
		(void)map;
		nearTrap = 0;
		AEVec2 temp = {};
		for (int dx = -trapRange; dx <= trapRange; ++dx) {
			for (int dy = -trapRange; dy <= trapRange; ++dy) {
				int checkX = static_cast<int>(NposX + dx);
				int checkY = static_cast<int>(NposY + dy);
				if (checkX < 0) checkX = 0;
				if (checkY < 0) checkY = 0;
				if (checkX >= x) checkX = x-1;
				if (checkY >= y) checkY = y-1;
				if (MapData[checkY][checkX] == 50) {
					nearTrap = 1;
					AEVec2Set(&temp, static_cast<f32>(checkX), static_cast<f32>(checkY));
					return temp;
				}
			}
		}
		return temp;
	}
	void suckPlayer(objectsquares& player, AEVec2 trap,float s) {
		float trapX = gamelogic::index_to_posX(static_cast<f32>(trap.x+0.5), s);
		float trapY = gamelogic::index_to_posY(static_cast<f32>(trap.y+0.5), s);
		f32 dx = player.xPos - trapX;
		f32 dy = player.yPos - trapY;
		f32 dist = sqrtf(dx * dx + dy * dy);
		player.velocityX -= dx / dist * (trapSuction * 0.2f);
		player.velocityY -= dy / dist * (trapSuction * 0.2f);
		
	}
	void updateTraps(Enemy enemies[],objectsquares objectinfo[],int* map,int x, int y, float s) {
		enemies = enemies;
		trapInstanceCooldown -= 1;
		float NposX = gamelogic::posX_to_index(objectinfo[player].xPos+ objectinfo[player].velocityX, s);
		float NposY = gamelogic::posY_to_index(objectinfo[player].yPos + objectinfo[player].velocityY, s);
		float NScaleX = objectinfo[player].xScale / s;
		float NScaleY = objectinfo[player].yScale / s;
		nearestTrap = checkNearestTrap(NposX, NposY, map, x,y);
		if (nearTrap) {
			suckPlayer(objectinfo[player], nearestTrap, s);
			/*
			//Drawing tentacle
			AEMtx33 scale = { 0 };
			AEMtx33Scale(&scale, objectinfo[player].xScale * 3, objectinfo[player].yScale * 3);

			AEMtx33 rotate = { 0 };
			//angle = movement::getMouse(player).y/ movement::getMouse(player).x;
			angle = static_cast<f32>(atan2(movement::getMouse(objectinfo[player]).y, movement::getMouse(objectinfo[player]).x));
			angle += 5 * PI / 4;
			AEMtx33Rot(&rotate, angle);

			AEMtx33 translate = { 0 };
			AEMtx33Trans(&translate, objectinfo[player].xPos, objectinfo[player].yPos);

			trapTransform = { 0 };
			AEMtx33Concat(&trapTransform, &rotate, &scale);
			AEMtx33Concat(&trapTransform, &translate, &trapTransform);*/
		}
		if (gamelogic::CheckInstanceBinaryMapCollision(NposX, NposY, NScaleX, NScaleY, map, 2, x)) {
			if (trapInstanceCooldown <= 0.0f)
			{
				if (PlayerTakeDamage(objectinfo[player], trapDamage))
				{
					trapInstanceCooldown = trapCooldown;
				}
			}
			// Trap-tile collision resolution is handled by the level's own
			// Collision_movement calls (index=1 for walls already keeps entities
			// in bounds).  Calling Collision_movement here with index=2 caused a
			// double-move per frame (velocity applied twice), which could push
			// entities out of the map and trigger access violations.
		}
	}
	/*void drawTraps() {
		AEGfxTextureSet(trapBeam, 0, 0);
		AEGfxSetTransform(trapTransform.m);
		AEGfxSetColorToMultiply(1, 1, 1, 1);
		AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
	}*/

	// Advance the saw spin animation by one frame's worth of delta time
	void UpdateSawAnim(f32 deltaTime) {
		animSystem::update(sawAnim, deltaTime);
	}

	// Return the current horizontal UV offset for the saw spritesheet
	f32 GetSawUOffset() {
		return animSystem::getUOffset(sawAnim);
	}
}