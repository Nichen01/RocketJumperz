#include "Movement.h"
//#include "physicsEngine.h"
#include <cmath>

namespace movement {
    // initialize the velocity 
    void initPlayerMovement(objectsquares& player)
    {
        player.velocityX = 0.0f;
        player.velocityY = 0.0f;
    }

    void applyThrustTowardMouse(objectsquares& player, s32 mouseWorldX, s32 mouseWorldY)
    {
        // Check if spacebar is pressed
        if (AEInputCheckTriggered(AEVK_SPACE))
        {
            // Calculate direction vector from player to mouse
            f32 dx = static_cast<f32>(mouseWorldX) - player.xPos;
            f32 dy = static_cast<f32>(mouseWorldY) - player.yPos;

            // Calculate distance (magnitude of direction vector)
            f32 distance = sqrtf(dx * dx + dy * dy);

            // Normalize direction vector (only if distance > 0 to avoid division by zero)
            if (distance > 0.0f)
            {
                dx /= distance;
                dy /= distance;

                // Apply thrust in the normalized direction
                player.velocityX += dx * THRUST_POWER;
                player.velocityY += dy * THRUST_POWER;

                printf("Jetpack fired! Velocity: (%.2f, %.2f)\n", player.velocityX, player.velocityY);
            }
        }
    }

    void updatePlayerPhysics(objectsquares& player)
    {
        // Apply drag to velocities (exponential decay)
        player.velocityX *= DRAG_COEFFICIENT;
        player.velocityY *= DRAG_COEFFICIENT;

        // Stop completely if velocity becomes negligible (prevents infinite drift)
        if (fabsf(player.velocityX) < MIN_VELOCITY)
        {
            player.velocityX = 0.0f;
        }
        if (fabsf(player.velocityY) < MIN_VELOCITY)
        {
            player.velocityY = 0.0f;
        }

        // Update position based on current velocity
        player.xPos += player.velocityX;
        player.yPos += player.velocityY;
    }
}