#pragma once


#include "AEEngine.h"
#include "collision.h"

extern s32 screenWidth, screenLength;
namespace movement {
    // Movement physics constants
    constexpr f32 THRUST_POWER = 5.0f;      // Jetpack acceleration force
    constexpr f32 ABSOLUTE_RECOIL = 5;      // Bullet Recoil force
    constexpr f32 DRAG_COEFFICIENT = 0.98f;  // Air drag (0.92 = 8% speed loss per frame)
    constexpr f32 MIN_VELOCITY = 0.1f;       // Velocity threshold to stop completely
    constexpr f32 TERMINAL_VELOCITY = 10;    // Velocity will not increase pass this

    // Initialize player velocities (call in Load)
    void initPlayerMovement(objectsquares& player);

    // update physics based on character input
    void physicsInput(objectsquares& player);

    // Update player velocity with drag and position
    void updatePlayerPhysics(objectsquares& player);
}