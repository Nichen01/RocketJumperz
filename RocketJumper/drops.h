#pragma once
#include "player.h"

extern int wireDropsSpawned;

enum DropType {
	DROP_AMMO,
	DROP_HEALTH
};

struct drop {
	objectsquares info;
	DropType type;
};

// ---- Wire Drop System ----
// Wires are dropped by enemies on death.  The first kill in a level has
// a 50 % chance to drop a wire.  If the first kill does NOT drop one,
// the second kill is guaranteed to drop it.  wireCount persists across
// all levels (declared extern in player.h).

struct WireDrop {
	objectsquares info;    // position, scale, flag (1 = active on ground)
};

namespace pickup {
	void initDrops(drop[],int,f32);
	void updateDrops(drop[],int,objectsquares&);
	void drawDrops(drop[],int);

	// Wire-specific helpers ------------------------------------------------

	// Call once in Level Initialize to zero-out the wire drop array
	// and reset the per-level kill/drop tracker.
	void InitWireDrops(WireDrop wireDrops[], int maxCount, f32 scale);

	// Called from the enemy death path.  Decides whether this kill should
	// produce a wire drop and, if so, activates it at (xPos, yPos).
	// Returns true if a wire was spawned.
	bool TrySpawnWireDrop(WireDrop wireDrops[], int maxCount,
	                      f32 xPos, f32 yPos);

	// Call once per frame in Update.  Checks player overlap with every
	// active wire drop.  On pickup, increments wireCount and plays sound.
	void UpdateWireDrops(WireDrop wireDrops[], int maxCount,
	                     objectsquares& playerObj);

	// Renders all active wire drops with a floating bob animation,
	// using the TEX_WIRE texture.
	void DrawWireDrops(WireDrop wireDrops[], int maxCount);

	// Call in Level Initialize to reset the per-level kill tracker
	// so the first/second-kill guarantee logic starts fresh.
	void ResetWireDropTracker();

	// ---- Infinite Ammo Pool ----
	// Spawns beside the level door when the player runs out of ammo.
	// Picking it up restores bulletCount to 50.

	struct InfiniteAmmoPool {
		objectsquares shape;
		bool isActive;
	};

	extern InfiniteAmmoPool ammoPool;

	void InitAmmoPool(f32 spawnX, f32 spawnY, f32 scale);
	void UpdateAmmoPool(objectsquares& player);
	void DrawAmmoPool();
}
