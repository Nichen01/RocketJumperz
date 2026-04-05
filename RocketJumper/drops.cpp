/* Start Header ************************************************************************/
/*!
\file         drops.cpp
\author       Jeremiah He Shuo (jeremiahhe.shuo), Nicholas Chen (c.chen)
\brief        Code regarding drop mechanics and types
Copyright (C) 2026 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#include "Drops.h"
#include "Collision.h"
#include "Draw.h"
#include "Movement.h"
#include "AssetManager.h"
#include "Load.h"
#include "Sound.h"
#include <cstdlib>   // rand()
#include <cmath>     // sinf()

int wireDropsSpawned{};

namespace pickup {
	AEMtx33 dropTransform = { 0 };
	static const f32 kPi = 3.14159f;

	// ----------------------------------------------------------
	/*!
	\brief Initializes drop instances.

	\param instance Array of drop objects.
	\param max Maximum number of drops.
	\param scale Scale applied to each drop.

	Sets default scale, type, and inactive state.
	*/
	// ----------------------------------------------------------
	void initDrops(drop instance[], int max, f32 scale) {
		for (int i{};i < max;i++) {
			instance[i].info.xScale = scale;
			instance[i].info.yScale = scale;
			instance[i].info.flag = 0;
			instance[i].type = DROP_AMMO;
		}
	}

	// ----------------------------------------------------------
	/*!
	\brief Updates drop interactions with the player.

	\param instance Array of drop objects.
	\param max Maximum number of drops.
	\param player Reference to player object.

	Handles pickup logic:
	- Health drops increase player health.
	- Ammo drops increase bullet count.
	*/
	// ----------------------------------------------------------
	void updateDrops(drop instance[], int max, objectsquares& player) {
		for (int i{};i < max;i++) {
			if (instance[i].info.flag == 0) continue;
			else if (gamelogic::static_collision(&player, &instance[i].info)) {
				instance[i].info.flag = 0;
				if (instance[i].type == DROP_HEALTH) {
					player.health += 50;
				}
				else {
					movement::bulletCount += 50;
				}
				AEAudioPlay(Pickup, soundEffects, 1.0f, 1.0f, 0);
			}
		}
	}

	// ----------------------------------------------------------
	/*!
	\brief Renders all active drops with a bobbing animation.

	\param instance Array of drop objects.
	\param max Maximum number of drops.

	Applies a sinusoidal vertical offset for visual feedback.
	*/
	// ----------------------------------------------------------
	void drawDrops(drop instance[], int max) {
		static f32 bobTimer = 0.0f;
		f32 dt = static_cast<f32>(AEFrameRateControllerGetFrameTime());
		bobTimer += dt;

		const f32 bobPixels = 5.0f;
		const f32 bobFrequency = 1.0f;

		AEGfxTexture* ammoTex = AssetManager::GetTexture(TEX_DROP);
		AEGfxTexture* healthTex = AssetManager::GetTexture(TEX_HEALTH);

		for (int i{}; i < max; i++) {
			if (instance[i].info.flag == 0) continue;

			AEGfxTexture* tex = (instance[i].type == DROP_HEALTH) ? healthTex : ammoTex;
			if (!tex) continue;

			f32 yOffset = sinf(bobTimer * bobFrequency * 2.0f * kPi) * bobPixels;

			AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
			AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
			AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
			AEGfxSetBlendMode(AE_GFX_BM_BLEND);
			AEGfxTextureSet(tex, 0.0f, 0.0f);

			renderlogic::drawSquare(
				instance[i].info.xPos,
				instance[i].info.yPos + yOffset,
				instance[i].info.xScale,
				instance[i].info.yScale);
			AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
		}

		AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
	}

	// ====================================================================
	// WIRE DROP SYSTEM
	// ====================================================================

	static int  enemiesKilledThisLevel = 0;
	static bool wireDroppedThisLevel = false;

	static const int kWireDropChancePercent = 50;

	// ----------------------------------------------------------
	/*!
	\brief Resets wire drop tracking for a new level.

	Resets:
	- Enemy kill counter
	- Drop occurrence flag
	*/
	// ----------------------------------------------------------
	void ResetWireDropTracker()
	{
		enemiesKilledThisLevel = 0;
		wireDroppedThisLevel = false;
	}

	// ----------------------------------------------------------
	/*!
	\brief Initializes wire drop array.

	\param wireDrops Array of wire drop objects.
	\param maxCount Maximum number of drops.
	\param scale Scale applied to each drop.

	Sets all drops to inactive state.
	*/
	// ----------------------------------------------------------
	void InitWireDrops(WireDrop wireDrops[], int maxCount, f32 scale)
	{
		for (int i = 0; i < maxCount; ++i)
		{
			wireDrops[i].info.xPos = 0.0f;
			wireDrops[i].info.yPos = 0.0f;
			wireDrops[i].info.xScale = scale;
			wireDrops[i].info.yScale = scale;
			wireDrops[i].info.flag = 0;   // 0 = inactive
		}
	}

	// ----------------------------------------------------------
	/*!
	\brief Attempts to spawn a wire drop when an enemy dies.

	\param wireDrops Array of wire drops.
	\param maxCount Maximum number of drops.
	\param xPos Spawn X position.
	\param yPos Spawn Y position.
	\return True if a wire was spawned.

	Uses first/second kill probability logic and caps total wires.
	*/
	// ----------------------------------------------------------
	bool TrySpawnWireDrop(WireDrop wireDrops[], int maxCount,
		f32 xPos, f32 yPos)
	{
		if (wireDropsSpawned >= 3)
			return false;

		if (wireDroppedThisLevel)
			return false;

		enemiesKilledThisLevel++;

		bool shouldDrop = false;

		if (enemiesKilledThisLevel == 1)
		{
			int roll = rand() % 100;
			shouldDrop = (roll < kWireDropChancePercent);
		}
		else if (enemiesKilledThisLevel == 2)
		{
			shouldDrop = true;
		}

		if (!shouldDrop)
			return false;

		for (int i = 0; i < maxCount; ++i)
		{
			if (wireDrops[i].info.flag == 0)
			{
				wireDrops[i].info.xPos = xPos;
				wireDrops[i].info.yPos = yPos;
				wireDrops[i].info.flag = 1;
				wireDroppedThisLevel = true;

				wireDropsSpawned++;  // increment global counter
				printf("Wire dropped at (%.1f, %.1f)! Total spawned: %d\n",
					xPos, yPos, wireDropsSpawned);

				return true;
			}
		}

		return false;
	}

	// ----------------------------------------------------------
	/*!
	\brief Updates wire drops and handles player pickup.

	\param wireDrops Array of wire drops.
	\param maxCount Maximum number of drops.
	\param playerObj Reference to player object.

	On pickup:
	- Deactivates drop
	- Increments wire count (capped)
	*/
	// ----------------------------------------------------------
	void UpdateWireDrops(WireDrop wireDrops[], int maxCount,
		objectsquares& playerObj)
	{
		for (int i = 0; i < maxCount; ++i)
		{
			if (wireDrops[i].info.flag == 0) continue;

			if (gamelogic::static_collision(&playerObj, &wireDrops[i].info))
			{
				wireDrops[i].info.flag = 0;  // deactivate

				// Increment global wire count (capped at 3 for the final door)
				if (wireCount < 3)
				{
					wireCount++;
					printf("Wire collected! Total wires: %d / 3\n", wireCount);
				}
				AEAudioPlay(Pickup, soundEffects, 1.0f, 1.0f, 0);
			}
		}
	}

	// ----------------------------------------------------------
	/*!
	\brief Draws all active wire drops.

	\param wireDrops Array of wire drops.
	\param maxCount Maximum number of drops.

	Renders with vertical bob animation.
	*/
	// ----------------------------------------------------------
	void DrawWireDrops(WireDrop wireDrops[], int maxCount)
	{
		static f32 bobTimer = 0.0f;
		f32 dt = static_cast<f32>(AEFrameRateControllerGetFrameTime());
		bobTimer += dt;

		const f32 bobPixels = 5.0f;
		const f32 bobFrequency = 1.0f;

		AEGfxTexture* wireTex = AssetManager::GetTexture(TEX_WIRE);
		if (!wireTex) return;

		for (int i = 0; i < maxCount; ++i)
		{
			if (wireDrops[i].info.flag == 0) continue;

			f32 yOffset = sinf(bobTimer * bobFrequency * 2.0f * kPi) * bobPixels;

			AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
			AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
			AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
			AEGfxSetBlendMode(AE_GFX_BM_BLEND);
			AEGfxTextureSet(wireTex, 0.0f, 0.0f);

			renderlogic::drawSquare(
				wireDrops[i].info.xPos,
				wireDrops[i].info.yPos + yOffset,
				wireDrops[i].info.xScale,
				wireDrops[i].info.yScale);

			AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
		}

		AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
	}

	// ====================================================================
	// INFINITE AMMO POOL
	// ====================================================================

	InfiniteAmmoPool ammoPool;

	// ----------------------------------------------------------
	/*!
	\brief Initializes infinite ammo pool.

	\param spawnX X position.
	\param spawnY Y position.
	\param scale Scale of pool.

	Starts inactive until triggered.
	*/
	// ----------------------------------------------------------
	void InitAmmoPool(f32 spawnX, f32 spawnY, f32 scale)
	{
		ammoPool.shape.xPos = spawnX;
		ammoPool.shape.yPos = spawnY;
		ammoPool.shape.xScale = scale;
		ammoPool.shape.yScale = scale;
		ammoPool.isActive = false;
	}

	// ----------------------------------------------------------
	/*!
	\brief Updates ammo pool state and handles pickup.

	\param player Reference to player object.

	Activates when ammo reaches zero and refills on collision.
	*/
	// ----------------------------------------------------------
	void UpdateAmmoPool(objectsquares& player)
	{
		if (movement::bulletCount <= 0)
			ammoPool.isActive = true;

		if (!ammoPool.isActive)
			return;

		if (gamelogic::static_collision(&player, &ammoPool.shape))
		{
			movement::bulletCount = 50;
			ammoPool.isActive = false;
			AEAudioPlay(Pickup, soundEffects, 1.0f, 1.0f, 0);
		}
	}

	// ----------------------------------------------------------
	/*!
	\brief Draws infinite ammo pool when active.

	Applies vertical bobbing animation.
	*/
	// ----------------------------------------------------------
	void DrawAmmoPool()
	{
		if (!ammoPool.isActive)
			return;

		static f32 bobTimer = 0.0f;
		bobTimer += static_cast<f32>(AEFrameRateControllerGetFrameTime());

		f32 yOffset = sinf(bobTimer * 1.0f * 2.0f * kPi) * 5.0f;

		AEGfxTexture* ammoTex = AssetManager::GetTexture(TEX_DROP);
		if (!ammoTex) return;

		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
		AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		AEGfxTextureSet(ammoTex, 0.0f, 0.0f);

		renderlogic::drawSquare(
			ammoPool.shape.xPos,
			ammoPool.shape.yPos + yOffset,
			ammoPool.shape.xScale,
			ammoPool.shape.yScale);
		AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
	}
}