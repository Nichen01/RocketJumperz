#pragma once


#include "AEEngine.h"
#include "collision.h"

extern s32 screenWidth, screenLength;
namespace movement {
    // Movement physics constants
    constexpr f32 THRUST_POWER = 25.0f;      // Jetpack acceleration force
    constexpr f32 ABSOLUTE_RECOIL = 10.f;      // Bullet Recoil force
    constexpr f32 DRAG_COEFFICIENT = 0.97f;  // Air drag (0.92 = 8% speed loss per frame)
    constexpr f32 MIN_VELOCITY = 0.1f;       // Velocity threshold to stop completely
    constexpr f32 TERMINAL_VELOCITY = 5.f;    // Velocity will not increase pass this
    constexpr f32 GRAVITY = 0.2f;
    extern int bulletCount;

    // True when the player's aim direction points to the left (mouse is left of player).
    // Updated each frame by UpdatePlayerFacing(). Used by draw code to flip the sprite.
    extern bool playerFacingLeft;

    // Initialize player velocities (call in Load)
    void initPlayerMovement(objectsquares& player);

    //updates directionVector with unit vector based on mouse position from player
    AEVec2 getMouse(objectsquares& player);

    // Recalculate which way the player is facing based on current mouse position.
    // Call once per frame in the Update function, before Draw.
    void UpdatePlayerFacing(objectsquares& player);

    // update physics based on character input
    void physicsInput(objectsquares& player);

    // Update player velocity with drag and position
    void updatePlayerPhysics(objectsquares& player);
}