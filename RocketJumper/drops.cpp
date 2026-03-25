#include "drops.h"
#include "collision.h"
#include "draw.h"
#include "movement.h"
#include "AssetManager.h"
#include "Load.h"
#include <cstdlib>   // rand()
#include <cmath>     // sinf()

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

	// ====================================================================
	// WIRE DROP SYSTEM
	// ====================================================================

	// Per-level tracking: how many enemies have died and whether we have
	// already dropped a wire on this level.  Reset when the level loads.
	static int  enemiesKilledThisLevel = 0;
	static bool wireDroppedThisLevel   = false;

	// Probability that the FIRST kill drops a wire (50%).
	// If the first kill misses, the second kill is guaranteed.
	static const int kWireDropChancePercent = 50;

	// Pi constant for the floating animation
	static const f32 kPi = 3.14159f;

	// ---- ResetWireDropTracker ----
	// Call at the start of every level Initialize so the chance logic
	// begins fresh for each level.
	void ResetWireDropTracker()
	{
		enemiesKilledThisLevel = 0;
		wireDroppedThisLevel   = false;
	}

	// ---- InitWireDrops ----
	// Zeroes out the wire drop array so none are active at level start.
	void InitWireDrops(WireDrop wireDrops[], int maxCount, f32 scale)
	{
		for (int i = 0; i < maxCount; ++i)
		{
			wireDrops[i].info.xPos   = 0.0f;
			wireDrops[i].info.yPos   = 0.0f;
			wireDrops[i].info.xScale = scale;
			wireDrops[i].info.yScale = scale;
			wireDrops[i].info.flag   = 0;   // 0 = inactive
		}
	}

	// ---- TrySpawnWireDrop ----
	// Called when an enemy dies.  Uses the first/second-kill guarantee
	// logic described in the design prompt.
	//
	// Returns true if a wire was actually placed on the ground.
	bool TrySpawnWireDrop(WireDrop wireDrops[], int maxCount,
	                      f32 xPos, f32 yPos)
	{
		// If we already dropped a wire on this level, skip
		if (wireDroppedThisLevel)
			return false;

		enemiesKilledThisLevel++;

		bool shouldDrop = false;

		if (enemiesKilledThisLevel == 1)
		{
			// First kill: 50% chance
			int roll = rand() % 100;
			shouldDrop = (roll < kWireDropChancePercent);
		}
		else if (enemiesKilledThisLevel == 2)
		{
			// Second kill: guaranteed if first didn't drop
			shouldDrop = true;
		}
		// After the 2nd kill, no more wire drops on this level

		if (!shouldDrop)
			return false;

		// Find an inactive slot in the wire drop array
		for (int i = 0; i < maxCount; ++i)
		{
			if (wireDrops[i].info.flag == 0)
			{
				wireDrops[i].info.xPos = xPos;
				wireDrops[i].info.yPos = yPos;
				wireDrops[i].info.flag = 1;  // 1 = active on ground
				wireDroppedThisLevel = true;
				printf("Wire dropped at (%.1f, %.1f)!\n", xPos, yPos);
				return true;
			}
		}

		return false;  // no free slot (should not happen normally)
	}

	// ---- UpdateWireDrops ----
	// Checks every active wire drop for overlap with the player.
	// On pickup: deactivate the drop, increment wireCount, cap at 3.
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
			}
		}
	}

	// ---- DrawWireDrops ----
	// Renders every active wire drop with a gentle up-and-down bob,
	// matching the keycard floating style from draw.cpp.
	void DrawWireDrops(WireDrop wireDrops[], int maxCount)
	{
		// Shared bob timer -- persists across frames
		static f32 bobTimer = 0.0f;
		f32 dt = static_cast<f32>(AEFrameRateControllerGetFrameTime());
		bobTimer += dt;

		// Bob parameters (same as keycard in drawMapWallFloor)
		const f32 bobPixels    = 5.0f;   // amplitude in pixels
		const f32 bobFrequency = 1.0f;   // cycles per second

		AEGfxTexture* wireTex = AssetManager::GetTexture(TEX_WIRE);
		if (!wireTex) return;  // safety: texture not loaded yet

		for (int i = 0; i < maxCount; ++i)
		{
			if (wireDrops[i].info.flag == 0) continue;

			// Calculate floating Y offset
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

		// Reset color state
		AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
	}
}
