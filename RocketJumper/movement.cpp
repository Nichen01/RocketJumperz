#include "Movement.h"
//#include "physicsEngine.h"
#include <cmath>

namespace movement {
    // initialize the velocity 
    bool enableGravity, enableDrag;
    u8 bulletCount, jetPackCooldown;
    s32 mouseX,mouseY;
    f32 mouseDistance;
    AEVec2 directionVector;
    void initPlayerMovement(objectsquares& player)
    {
        player.velocityX = 0.0f;
        player.velocityY = 0.0f;
        bulletCount = 90;
        jetPackCooldown = 0;
        enableGravity = enableDrag = 1;
    }

    void getMouse(objectsquares &player) {
        AEInputGetCursorPosition(&mouseX,&mouseY);
        mouseX -= ((screenWidth / 2) + player.xPos + player.velocityX);
        mouseY = ((screenLength / 2) - mouseY - player.yPos - player.velocityY);
        mouseDistance = sqrtf(mouseX * mouseX + mouseY * mouseY);
        // Normalize direction vector (only if distance > 0 to avoid division by zero)
        if (mouseDistance > 0.0f)
        {
            directionVector.x = mouseX / mouseDistance;
            directionVector.y = mouseY / mouseDistance;
        }
    }

    void physicsInput(objectsquares& player)
    {
        (jetPackCooldown <= 0) ? jetPackCooldown-- : jetPackCooldown = 0;
        if (AEInputCheckTriggered(AEVK_G)) {
            enableGravity = (enableGravity) ? 0 : 1;
            printf("%d", enableGravity);
        }
        if (AEInputCheckTriggered(AEVK_D)) {
            enableDrag = (enableDrag) ? 0 : 1;
            printf("%d", enableDrag);
        }
        // Check if spacebar is pressed
        if (AEInputCheckTriggered(AEVK_SPACE)/* && (!jetPackCooldown)*/)
        {
            // Calculate direction vector from player to mouse
            getMouse(player);
            player.velocityX += directionVector.x * THRUST_POWER;
            player.velocityY += directionVector.y * THRUST_POWER;
            printf("Jetpack fired! Velocity: (%.2f, %.2f)\n", player.velocityX, player.velocityY);
            jetPackCooldown += 2;
            }
        if (AEInputCheckTriggered(AEVK_LBUTTON))
        {
            // Calculate direction vector from player to mouse
            getMouse(player);
            player.velocityX -= directionVector.x * ABSOLUTE_RECOIL;
            player.velocityY -= directionVector.y * ABSOLUTE_RECOIL;
            printf("Bullet fired! Velocity: (%.2f, %.2f)\n", player.velocityX, player.velocityY);
            bulletCount -= 1;
        }
    }

    void updatePlayerPhysics(objectsquares& player)
    {
        if (enableGravity) {
            player.velocityY -= GRAVITY;
        }
        // Apply drag to velocities (exponential decay)
        if (enableDrag) {
            player.velocityX *= DRAG_COEFFICIENT;
            player.velocityY *= DRAG_COEFFICIENT;
        }

        // Stop completely if velocity becomes negligible (prevents infinite drift)
        if (fabsf(player.velocityX) < MIN_VELOCITY)
        {
            player.velocityX = 0.0f;
        }
        if (fabsf(player.velocityY) < MIN_VELOCITY)
        {
            player.velocityY = 0.0f;
        }

        //Max Speed
        mouseDistance = static_cast<f32>(sqrt(player.velocityY * player.velocityY + player.velocityX + player.velocityX));
        if (mouseDistance > TERMINAL_VELOCITY) {
            player.velocityX = player.velocityX / mouseDistance * TERMINAL_VELOCITY;
            player.velocityY = player.velocityY / mouseDistance * TERMINAL_VELOCITY;
        }

        // Update position based on current velocity
        //player.xPos += player.velocityX;
       // player.yPos += player.velocityY;
    }
}