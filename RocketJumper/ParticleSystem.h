/* Start Header ************************************************************************/
/*!
\file         ParticleSystem.h
\brief        Object-pool particle system for Rocket Jumperz.
              All logic is isolated inside the ParticleSystem namespace.
              Uses a fixed-size static array (no dynamic allocation at runtime).

Copyright (C) 2026 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#pragma once

// -----------------------------------------------------------------------
// Particle -- one individual particle in the pool.
// Active particles update every frame; inactive ones are skipped.
// -----------------------------------------------------------------------
struct Particle {
    float x;            // world-space X position
    float y;            // world-space Y position
    float velX;         // horizontal velocity (pixels per second)
    float velY;         // vertical velocity  (pixels per second)
    float lifetime;     // remaining seconds before this particle dies
    float maxLifetime;  // original lifetime (used to compute fade alpha)
    float scale;        // render size in world units
    bool  isActive;     // true = alive and updating, false = free slot
};

// -----------------------------------------------------------------------
// EmitterProps -- configuration passed to Emit() that describes HOW
// to spawn a burst of particles (position, velocity spread, etc.).
// -----------------------------------------------------------------------
struct EmitterProps {
    float spawnX;           // centre X of the emission point
    float spawnY;           // centre Y of the emission point
    float velocityXBase;    // base horizontal speed
    float velocityYBase;    // base vertical speed
    float velocitySpreadX;  // random spread added/subtracted on X axis
    float velocitySpreadY;  // random spread added/subtracted on Y axis
    float lifetimeBase;     // base lifetime in seconds
    float lifetimeSpread;   // random spread added to base lifetime
    float scaleBase;        // render scale for each particle
    int   emitCount;        // how many particles to spawn per Emit call
};

// Maximum number of particles alive at any time (object pool size)
const int MAX_PARTICLES = 500;

// -----------------------------------------------------------------------
// ParticleSystem namespace -- all public functions for the system.
// Call Load/Unload once per level load cycle; Update/Draw every frame.
// -----------------------------------------------------------------------
namespace ParticleSystem {

    // Initialise the pool (marks all particles inactive) and
    // builds the internal mesh used for drawing.
    void Load();

    // Step all active particles forward by dt seconds.
    // Decreases lifetime, kills expired particles, updates positions.
    void Update(float dt);

    // Render every active particle using AEEngine color-mode drawing.
    // Particles fade out as their lifetime approaches zero.
    void Draw();

    // No-op placeholder kept for symmetry with the engine state pattern.
    void Free();

    // Frees the internal mesh. Call once in Level_Unload.
    void Unload();

    // Spawn a burst of particles using the given emitter properties.
    // Searches the pool for inactive slots; stops after emitCount
    // particles have been activated (or the pool is full).
    void Emit(const EmitterProps& props);
}
