#pragma once


#include "AEEngine.h"
#include "collision.h"

extern s32 screenWidth, screenLength;
namespace movement {
    extern u8 jetPackCooldown;
    // Movement physics constants
    constexpr f32 THRUST_POWER = 17.0f;      // Jetpack acceleration force
    constexpr f32 ABSOLUTE_RECOIL = 10.f;      // Bullet Recoil force (plasma gun baseline)
    constexpr f32 SHOTGUN_RECOIL_MULTIPLIER = 4.f; // Shotgun recoil is 1.6x the plasma gun
    constexpr f32 DRAG_COEFFICIENT = 0.97f;  // Air drag (0.92 = 8% speed loss per frame)
    constexpr f32 MIN_VELOCITY = 0.1f;       // Velocity threshold to stop completely
    constexpr f32 TERMINAL_VELOCITY = 10.f;    // Velocity will not increase pass this
    constexpr f32 GRAVITY = 0.4f;
    extern int bulletCount;

    // True when gravity is currently active (toggled with G key).
    // Used by the HUD to display red/green gravity indicator.
    extern bool enableGravity;

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