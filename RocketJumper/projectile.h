#pragma once
#include "AEEngine.h"
#include "collision.h"  

// max number of projectiles that can be active at once
#define MAX_PROJECTILES 5

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
    void fireProjectiles(s32 worldMouseX, s32 worldMouseY,
        objectsquares& player,
        Projectile projectiles[],
        s32 maxCount);

    // Update all active projectiles (movement, bounds checking)
    void UpdateProjectiles(Projectile projectiles[], s32 maxCount);

    // Render all active projectiles to screen
    void renderProjectiles(Projectile projectiles[], s32 maxCount,
        AEGfxVertexList* mesh);

    // Check if any projectiles hit the obstacle
    void checkProjectileCollisions(Projectile projectiles[], s32 maxCount,
        objectsquares& obstacle);
}