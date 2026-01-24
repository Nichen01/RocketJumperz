#pragma once


#include "AEEngine.h"
#include "collision.h"

namespace movement {
    // Movement physics constants
    constexpr f32 THRUST_POWER = 20.0f;      // Jetpack acceleration force
    constexpr f32 DRAG_COEFFICIENT = 0.98f;  // Air drag (0.92 = 8% speed loss per frame)
    constexpr f32 MIN_VELOCITY = 0.1f;       // Velocity threshold to stop completely

    // Apply jetpack thrust toward mouse position
    void applyThrustTowardMouse(objectsquares& player, s32 mouseWorldX, s32 mouseWorldY);

    // Update player velocity with drag and position
    void updatePlayerPhysics(objectsquares& player);

    // Initialize player velocities (call in Load)
    void initPlayerMovement(objectsquares& player);
}