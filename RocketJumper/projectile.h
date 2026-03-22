#pragma once
#include "AEEngine.h"
#include "collision.h"  
#include "AEAudio.h"

// max number of projectiles that can be active at once
// Increased from 5 to 20 to support the shotgun (fires 5 pellets per shot)
constexpr s32 MAX_PROJECTILES = 20;

// Projectile structure containing all data for a single projectile
struct Projectile
{
    objectsquares shape;    // position and size info (from collision.h)         
    s8 isActive;           
};

namespace projectileSystem {

    // Initialize all projectiles to inactive state
    void initProjectiles(Projectile projectiles[], s32 maxCount);

    // Fire a new projectile toward mouse position
    void fireProjectiles(s32 worldMouseX,
        s32 worldMouseY,
        objectsquares& player,
        Projectile projectiles[],
        s32 maxCount,
        AEAudio attackSound,
        AEAudioGroup sfxGroup);

    // Update all active projectiles (movement, bounds checking)
    void UpdateProjectiles(Projectile projectiles[], s32 maxCount);

    // Render all active projectiles to screen
    void renderProjectiles(Projectile projectiles[], s32 maxCount, AEGfxTexture* texture,
        AEGfxVertexList* mesh);

    // Check if any projectiles hit the obstacle
    void checkProjectileCollisions(Projectile projectiles[], s32 maxCount,
        objectsquares& obstacle);

    // Fire a shotgun spread: 5 pellets in a cone toward the mouse position.
    // Each pellet is rotated slightly from the base direction, with random
    // speed variance so the spread looks organic.
    void FireShotgun(s32 worldMouseX,
        s32 worldMouseY,
        objectsquares& player,
        Projectile projectiles[],
        s32 maxCount,
        AEAudio attackSound,
        AEAudioGroup sfxGroup);
}