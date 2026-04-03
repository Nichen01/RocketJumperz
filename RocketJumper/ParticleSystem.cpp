/* Start Header ************************************************************************/
/*!
\file         ParticleSystem.cpp
\brief        Implementation of the object-pool particle system.
              Uses a fixed-size static array for zero runtime allocations.

Copyright (C) 2026 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#include "pch.h"           // Brings in AEEngine.h, cmath, cstdlib, etc.
#include "ParticleSystem.h"

// -----------------------------------------------------------------------
// File-local (private) data -- only accessible inside this .cpp
// -----------------------------------------------------------------------

// The object pool: a fixed array of particles. No new/delete at runtime.
static Particle sParticles[MAX_PARTICLES];

// A small white quad mesh built once in Load() and freed in Unload().
// We manage this locally instead of adding an entry to AssetManager
// so the particle system stays fully self-contained.
static AEGfxVertexList* sParticleMesh = nullptr;

// -----------------------------------------------------------------------
// Helper: returns a random float in [0.0, 1.0]
// Uses the standard rand() seeded by the engine at startup.
// -----------------------------------------------------------------------
static float RandomFloat01() {
    return static_cast<float>(rand() % 10001) / 10000.0f;
}

// -----------------------------------------------------------------------
// Helper: returns a random float in [-1.0, 1.0]
// Useful for symmetric spread around a base velocity.
// -----------------------------------------------------------------------
static float RandomFloatNeg1To1() {
    return RandomFloat01() * 2.0f - 1.0f;
}

// -----------------------------------------------------------------------
// ParticleSystem::Load
// Resets the pool and builds the mesh used for all particle rendering.
// Call this once inside your Level_Load function.
// -----------------------------------------------------------------------
void ParticleSystem::Load() {
    // Mark every slot in the pool as inactive (available for reuse)
    for (int i = 0; i < MAX_PARTICLES; ++i) {
        sParticles[i].isActive = false;
    }

    // Build a 1x1 unit quad mesh with white vertex colour (0xFFFFFFFF).
    // White vertices let us control the final colour entirely through
    // AEGfxSetColorToMultiply / AEGfxSetColorToAdd at draw time.
    if (sParticleMesh != nullptr) {
        AEGfxMeshFree(sParticleMesh);
        sParticleMesh = nullptr;
    }

    AEGfxMeshStart();
    AEGfxTriAdd(
        -0.5f, -0.5f, 0xFFFFFFFF, 0.0f, 1.0f,   // bottom-left
         0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f,   // bottom-right
        -0.5f,  0.5f, 0xFFFFFFFF, 0.0f, 0.0f    // top-left
    );
    AEGfxTriAdd(
         0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f,   // bottom-right
         0.5f,  0.5f, 0xFFFFFFFF, 1.0f, 0.0f,   // top-right
        -0.5f,  0.5f, 0xFFFFFFFF, 0.0f, 0.0f    // top-left
    );
    sParticleMesh = AEGfxMeshEnd();
}

// -----------------------------------------------------------------------
// ParticleSystem::Emit
// Activates up to emitCount particles from the pool using the supplied
// emitter properties.  Skips slots that are already active.
// -----------------------------------------------------------------------
void ParticleSystem::Emit(const EmitterProps& props) {
    int spawned = 0;

    for (int i = 0; i < MAX_PARTICLES && spawned < props.emitCount; ++i) {
        // Skip slots that are already in use
        if (sParticles[i].isActive) {
            continue;
        }

        Particle& p = sParticles[i];
        p.isActive = true;

        // Position: spawn at the emitter centre
        p.x = props.spawnX;
        p.y = props.spawnY;

        // Velocity: base + random spread in [-spread, +spread]
        p.velX = props.velocityXBase + RandomFloatNeg1To1() * props.velocitySpreadX;
        p.velY = props.velocityYBase + RandomFloatNeg1To1() * props.velocitySpreadY;

        // Lifetime: base + random spread in [0, spread]
        p.lifetime    = props.lifetimeBase + RandomFloat01() * props.lifetimeSpread;
        p.maxLifetime = p.lifetime;

        // Scale: direct copy from emitter properties
        p.scale = props.scaleBase;

        // Colour: if spark mode is on, randomly pick white/yellow/orange.
        // Otherwise default to white so the Draw() colour multiply works as before.
        if (props.useSparkColors) {
            int colorPick = rand() % 3;
            if (colorPick == 0) {        // White
                p.r = 1.0f; p.g = 1.0f; p.b = 1.0f;
            } else if (colorPick == 1) { // Yellow
                p.r = 1.0f; p.g = 1.0f; p.b = 0.0f;
            } else {                     // Orange
                p.r = 1.0f; p.g = 0.5f; p.b = 0.0f;
            }
        } else {
            // Default to the original warm orange so existing effects (e.g. rocket
            // exhaust) look exactly the same as before this change.
            p.r = 1.0f; p.g = 0.7f; p.b = 0.2f;
        }
        ++spawned;
    }
}

// -----------------------------------------------------------------------
// ParticleSystem::Update
// Steps every active particle forward by dt seconds.
// Particles whose lifetime expires are deactivated (returned to the pool).
// -----------------------------------------------------------------------
void ParticleSystem::Update(float dt) {
    // Gravity acceleration applied to every particle each frame.
    // Negative Y is downward in AEEngine, so this pulls particles down.
    const float kGravity = -250.0f;

    for (int i = 0; i < MAX_PARTICLES; ++i) {
        if (!sParticles[i].isActive) {
            continue;
        }

        Particle& p = sParticles[i];

        // Count down the lifetime
        p.lifetime -= dt;

        if (p.lifetime <= 0.0f) {
            // Particle has expired -- return it to the pool
            p.isActive = false;
            continue;
        }

        // Apply gravity (accelerated motion) -- velocity changes each frame
        p.velY += kGravity * dt;

        // Update position (linear motion using the new velocity)
        p.x += p.velX * dt;
        p.y += p.velY * dt;
    }
}

// -----------------------------------------------------------------------
// ParticleSystem::Draw
// Renders every active particle as a small coloured square.
// Alpha fades from 1.0 down to 0.0 over the particle's lifetime
// so it smoothly disappears before being recycled.
// -----------------------------------------------------------------------
void ParticleSystem::Draw() {
    // Early out if the mesh was never built (safety check)
    if (sParticleMesh == nullptr) {
        return;
    }

    // Switch to colour-only rendering (no texture needed)
    AEGfxSetRenderMode(AE_GFX_RM_COLOR);
    AEGfxSetBlendMode(AE_GFX_BM_BLEND);

    for (int i = 0; i < MAX_PARTICLES; ++i) {
        if (!sParticles[i].isActive) {
            continue;
        }

        const Particle& p = sParticles[i];

        // Compute how much life remains as a 0-to-1 ratio.
        // Guard against division by zero (maxLifetime should always be > 0).
        float lifeRatio = (p.maxLifetime > 0.0f)
            ? (p.lifetime / p.maxLifetime)
            : 0.0f;

        // Fade alpha from fully opaque (1.0) to invisible (0.0)
        float alpha = lifeRatio;

        // Use the per-particle colour (set at spawn time) and fade with lifetime.
        // The vertex colour is white, so ColorToMultiply sets the final RGB+A.
        AEGfxSetColorToMultiply(p.r, p.g, p.b, alpha);
        AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
        AEGfxSetTransparency(alpha);

        // Build the Scale * Translate transform matrix
        AEMtx33 scale = { 0 };
        AEMtx33Scale(&scale, p.scale, p.scale);

        AEMtx33 translate = { 0 };
        AEMtx33Trans(&translate, p.x, p.y);

        AEMtx33 transform = { 0 };
        AEMtx33Concat(&transform, &translate, &scale);

        AEGfxSetTransform(transform.m);
        AEGfxMeshDraw(sParticleMesh, AE_GFX_MDM_TRIANGLES);
    }

    // Reset transparency so subsequent draw calls are not affected
    AEGfxSetTransparency(1.0f);
}

// -----------------------------------------------------------------------
// ParticleSystem::Free
// Placeholder for symmetry with the engine's Load/Init/Update/Draw/Free/Unload
// lifecycle.  Nothing to free that is not handled by Unload.
// -----------------------------------------------------------------------
void ParticleSystem::Free() {
    // Intentionally empty -- mesh is freed in Unload.
}

// -----------------------------------------------------------------------
// ParticleSystem::Unload
// Releases the mesh created in Load.  Call once in Level_Unload.
// -----------------------------------------------------------------------
void ParticleSystem::Unload() {
    if (sParticleMesh != nullptr) {
        AEGfxMeshFree(sParticleMesh);
        sParticleMesh = nullptr;
    }
}
