#pragma once
#include "AEEngine.h"

struct BB
{
	AEVec2	min;
	AEVec2	max;
};

struct objectsquares {
	f32 xPos;
	f32 yPos;

	f32 xScale;
	f32 yScale;

	u8 flag;

	AEVec2				scale;
	//AEVec2				posCurr;

	AEVec2				posPrev;

	AEVec2				velCurr;
	BB				boundingBox;

	int gridCollisionFlag;

	int leftX;
	int rightX;
	int topY;
	int bottomY;
	int TR;
	int TL;
	int BR;
	int BL;

	f32 speed;
	f32 velocityX;
	f32 velocityY;

	f32 dx;
	f32 dy;

	// ---- Player Health System ----
	int health;                  // Current health points (0 to maxHealth)
	f32 invincibilityTimer;      // Seconds of invincibility remaining after a hit
};

// Maximum health the player can have
constexpr int kPlayerMaxHealth    = 1000;

// How long the player is invincible after taking a hit (seconds)
constexpr f32 kHitInvincibilityDuration = 0.5f;

enum objecttype {
	player,
	obstacle,
	enemy
};

// Counter for the WIRE to fix FINAL DOOR
extern int wireCount;

extern objectsquares objectinfo2[2];

// ---- Player Health Functions ----
// Call once in Initialize to set health to max and reset invincibility
void InitPlayerHealth(objectsquares& playerObj);

// Reduce player health by 'amount'. Respects invincibility timer so the
// player cannot take damage every frame from sustained contact. Also usable
// by environmental traps -- just call PlayerTakeDamage(player, trapDamage).
// Returns true if damage was actually applied (false if still invincible).
bool PlayerTakeDamage(objectsquares& playerObj, int amount);

// Call once per frame in Update to tick down the invincibility timer.
// Pass the frame's delta time (seconds).
void UpdatePlayerInvincibility(objectsquares& playerObj, f32 dt);
