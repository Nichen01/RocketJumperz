/* Start Header ************************************************************************/
/*!
\file         traps.cpp
\author       Jeremiah He Shuo (jeremiahhe.shuo)
\brief        Code regarding traps: trap initiation, trap physics calculation, trap rendering and helper functions
Copyright (C) 2026 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#include "traps.h"
#include "collision.h"
#include "player.h"
#include "enemies.h"
#include "binaryMap.h"
#include "AssetManager.h"
#include "Sound.h"

// ---------------------------------------------------------------------------
// Saw animation constants
// ---------------------------------------------------------------------------
static const int   kSawCols = 6;     // 6 frames in a single row (192px / 32px per frame)
static const int   kSawRows = 1;
static const int   kSawTotalFrames = 6;
static const f32   kSawFrameDelay = 0.08f; // ~12 fps spin speed


namespace traps {
	//for testing, compiles faster in cpp
	//f32 trapSuction = 0.55f;

	// Saw animation instance, all saw tiles share the same playback state
	SpriteAnimation sawAnim;

	s8 trapDamage = 5;
	s8 trapInstanceCooldown = 0;
	s8 trapRange = 2;
	bool nearTrap = 0;
	bool wasNearTrap = false;   // tracks previous frame's nearTrap state for one-shot sound
	AEVec2 nearestTrap = {};
	f32 angle, tentacleMagnifier;
	AEMtx33 trapTransform;

	// Volume for the suction trap hum (0.0 to 1.0).
	const f32 SuctionHumVolume = 0.3f;

	// ----------------------------------------------------------
	/*!
	\brief Initializes trap-related systems and animation.

	Resets nearest trap tracking and initializes:
	- Saw animation playback
	- Saw spritesheet mesh
	*/
	// ----------------------------------------------------------
	void initTraps() {
		nearTrap = 0;
		wasNearTrap = false;
		AEVec2Zero(&nearestTrap);

		// Initialize the saw animation to loop continuously from frame 0
		animSystem::init(sawAnim, kSawCols, kSawRows, kSawTotalFrames,
			kSawFrameDelay, ANIM_LOOP, 0);

		// Build the spritesheet mesh for the saw (1 row, 7 columns)
		AssetManager::BuildSqrMesh(MESH_SAW, kSawRows, kSawCols);
	}

	// ----------------------------------------------------------
	/*!
	\brief Finds the nearest trap tile within range.

	\param NposX Player X position in map index space.
	\param NposY Player Y position in map index space.
	\param map Pointer to map data (unused).
	\param x Map width.
	\param y Map height.
	\return Position of nearest trap in index space.

	Searches a square region defined by trapRange.
	*/
	// ----------------------------------------------------------
	AEVec2 checkNearestTrap(float NposX, float NposY, int* map, int x, int y) {
		(void)map;
		nearTrap = 0;
		AEVec2 temp = {};
		for (int dx = -trapRange; dx <= trapRange; ++dx) {
			for (int dy = -trapRange; dy <= trapRange; ++dy) {
				int checkX = static_cast<int>(NposX + dx);
				int checkY = static_cast<int>(NposY + dy);
				if (checkX < 0) checkX = 0;
				if (checkY < 0) checkY = 0;
				if (checkX >= x) checkX = x - 1;
				if (checkY >= y) checkY = y - 1;
				if (MapData[checkY][checkX] == 50) {
					nearTrap = 1;
					AEVec2Set(&temp, static_cast<f32>(checkX), static_cast<f32>(checkY));
					return temp;
				}
			}
		}
		return temp;
	}

	// ----------------------------------------------------------
	/*!
	\brief Applies suction force from trap to player.

	\param player Reference to player object.
	\param trap Trap position in world space.

	Applies force toward trap based on distance vector.
	*/
	// ----------------------------------------------------------
	void suckPlayer(objectsquares& player, AEVec2 trap) {
		f32 dx = player.xPos - trap.x;
		f32 dy = player.yPos - trap.y;
		f32 dist = sqrtf(dx * dx + dy * dy);
		player.velocityX -= dx / dist * trapSuction;
		player.velocityY -= dy / dist * trapSuction;

	}

	// ----------------------------------------------------------
	/*!
	\brief Updates trap interactions and effects.

	\param enemies Array of enemies.
	\param objectinfo Array of game objects.
	\param map Pointer to map data.
	\param x Map width.
	\param y Map height.
	\param s Tile scale.

	Handles:
	- Trap detection
	- Player suction
	- Tentacle rendering transform
	- Damage application
	*/
	// ----------------------------------------------------------
	void updateTraps(Enemy enemies[], objectsquares objectinfo[], int* map, int x, int y, float s) {
		enemies = enemies;
		trapInstanceCooldown -= 1;
		float NposX = gamelogic::posX_to_index(objectinfo[player].xPos + objectinfo[player].velocityX, s);
		float NposY = gamelogic::posY_to_index(objectinfo[player].yPos + objectinfo[player].velocityY, s);
		float NScaleX = objectinfo[player].xScale / s;
		float NScaleY = objectinfo[player].yScale / s;
		nearestTrap = checkNearestTrap(NposX, NposY, map, x, y);
		nearestTrap.x = gamelogic::index_to_posX(static_cast<f32>(nearestTrap.x + 0.5), s);
		nearestTrap.y = gamelogic::index_to_posY(static_cast<f32>(nearestTrap.y + 0.5), s);
		if (nearTrap) {
			// Play energyHum sound ONCE when suction first begins (not every frame)
			if (!wasNearTrap) {
				AEAudioPlay(EnergyHum, soundEffects, SuctionHumVolume, 1.0f, 0);
			}
			suckPlayer(objectinfo[player], nearestTrap);
			f32 relativeX = objectinfo[player].xPos - nearestTrap.x;
			f32 relativeY = objectinfo[player].yPos - nearestTrap.y;

			//Drawing tentacle
			//scale to distance between player and trap
			AEMtx33 scale = { 0 };
			tentacleMagnifier = static_cast<f32>(sqrt(relativeY * relativeY + relativeX * relativeX) / sqrt(2));
			AEMtx33Scale(&scale, tentacleMagnifier, tentacleMagnifier);

			AEMtx33 rotate = { 0 };
			angle = static_cast<f32>(atan2(relativeY, relativeX));
			angle -= PI / 4;
			AEMtx33Rot(&rotate, angle);

			AEMtx33 translate = { 0 };
			AEMtx33Trans(&translate, nearestTrap.x + relativeX / 2, nearestTrap.y + relativeY / 2);

			trapTransform = { 0 };
			AEMtx33Concat(&trapTransform, &rotate, &scale);
			AEMtx33Concat(&trapTransform, &translate, &trapTransform);
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
			// in bounds). 
		}

		// Remember this frame's suction state so we can detect the
		// rising edge (not-near -> near) on the next frame.
		wasNearTrap = nearTrap;
	}

	// ----------------------------------------------------------
	/*!
	\brief Draws trap visuals (tentacle beam).

	Renders only when a trap is nearby.
	*/
	// ----------------------------------------------------------
	void drawTraps() {
		if (nearTrap) {
			AEGfxTextureSet(trapBeam, 0, 0);
			AEGfxSetTransform(trapTransform.m);
			AEGfxSetColorToMultiply(1, 1, 1, 1);
			AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
		}
	}

	// ----------------------------------------------------------
	/*!
	\brief Updates saw animation.

	\param deltaTime Time elapsed since last frame.
	*/
	// ----------------------------------------------------------
	void UpdateSawAnim(f32 deltaTime) {
		animSystem::update(sawAnim, deltaTime);
	}

	// ----------------------------------------------------------
	/*!
	\brief Gets current horizontal UV offset for saw animation.

	\return U offset for spritesheet sampling.
	*/
	// ----------------------------------------------------------
	f32 GetSawUOffset() {
		return animSystem::getUOffset(sawAnim);
	}
}