#include "Player.h"
#include "Collision.h"
#include <cstdio>

int wireCount = 0;

// ---- Checkpoint Variables ----
// Saved at each level transition so the player keeps progress from prior levels.
// Reset to defaults when starting a new game from the main menu.
int savedWireCount  = 0;
int savedAmmo       = 500;
int savedHealth     = 1000;

bool keycardCollected0 = false;
bool keycardCollected1 = false;
bool keycardCollected2 = false;
bool keycardCollected3 = false;
int doorState = 0;

// ---------------------------------------------------------------------------
// InitPlayerHealth
// Sets the player's health to the maximum and clears the invincibility timer.
// Call this once during Level Initialize, after creating the player object.
// ---------------------------------------------------------------------------
void InitPlayerHealth(objectsquares& playerObj)
{
	playerObj.health = kPlayerMaxHealth;
	playerObj.invincibilityTimer = 0.0f;
}

// ---------------------------------------------------------------------------
// PlayerTakeDamage
// Reduces player health by 'amount'. If the invincibility timer is still
// active (from a recent hit), no damage is applied and the function returns
// false. Otherwise, damage is applied, health is clamped to [0, maxHealth],
// and the invincibility timer is started.
//
// Can be used for any damage source -- enemies, traps,
// projectiles, environmental hazards, etc. Just pass the amount.
// ---------------------------------------------------------------------------
bool PlayerTakeDamage(objectsquares& playerObj, int amount)
{
	// If the player is still invincible from a recent hit, skip damage
	if (playerObj.invincibilityTimer > 0.0f)
		return false;

	// Apply damage, clamping health to [0, maxHealth]
	playerObj.health -= amount;

	if (playerObj.health < 0)
		playerObj.health = 0;
	if (playerObj.health > kPlayerMaxHealth)
		playerObj.health = kPlayerMaxHealth;

	// Start the invincibility window so the player is not hit every frame
	playerObj.invincibilityTimer = kHitInvincibilityDuration;

	printf("Player took %d damage! Health: %d / %d\n",
		amount, playerObj.health, kPlayerMaxHealth);

	return true;
}

// ---------------------------------------------------------------------------
// UpdatePlayerInvincibility
// Ticks down the invincibility timer each frame. Call this once per frame
// in the Level Update function, passing the frame delta time.
// ---------------------------------------------------------------------------
void UpdatePlayerInvincibility(objectsquares& playerObj, f32 dt)
{
	if (playerObj.invincibilityTimer > 0.0f)
	{
		playerObj.invincibilityTimer -= dt;
		if (playerObj.invincibilityTimer < 0.0f)
			playerObj.invincibilityTimer = 0.0f;
	}
}
