/* Start Header ************************************************************************/
/*!
\file         movement.cpp
\author       Jeremiah He Shuo (jeremiahhe.shuo), Nicholas Chen (c.chen)
\brief        Code regarding movement from the jetpack and the guns
Copyright (C) 2026 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/


#include "Movement.h"
#include "Sound.h"
#include "ParticleSystem.h"
#include <cmath>

int totalFrame = 40;
int numImages = 6;

namespace movement {
    // initialize the velocity
    bool enableGravity, enableDrag;
    u8 jetPackCooldown;
    s32 mouseX, mouseY;
    f32 mouseDistance;
    AEVec2 directionVector;
    int bulletCount = 25;
    bool playerFacingLeft = false; // starts facing right

    // LShift gravity-disable state -- when true, gravity is temporarily off
    bool isGravityDisabled = false;
    // Countdown in seconds; starts at 7.0 when the disable is activated
    f32 gravityTimer = 0.0f;
    // Cooldown after gravity auto-reverts; blocks LShift re-toggle while > 0
    f32 gravityCooldown = 0.0f;

    // ----------------------------------------------------------
    /*!
    \brief Handles the LShift gravity-disable mechanic.

    - Pressing LShift toggles gravity on/off.
    - When disabled, a 7-second timer starts.
    - When timer expires, gravity is automatically restored.
    - A cooldown prevents immediate reuse after auto-reset.

    \note Call once per frame in each level's Update function.
    */
    // ----------------------------------------------------------
    void UpdateGravityToggle()
    {
        // Get how much time passed since the last frame (in seconds)
        f32 dt = static_cast<f32>(AEFrameRateControllerGetFrameTime());

        // Tick down the post-revert cooldown each frame
        if (gravityCooldown > 0.0f)
        {
            gravityCooldown -= dt;
        }

        // LShift toggles the gravity-disable state
        // Only allow toggling OFF if the cooldown has expired
        if (AEInputCheckTriggered(AEVK_LSHIFT))
        {
            if (!isGravityDisabled && gravityCooldown <= 0.0f)
            {
                // Turn OFF gravity -- start the 7-second countdown
                isGravityDisabled = true;
                gravityTimer = 7.0f;
                AEAudioPlay(GravityToggle, soundEffects, 1.0f, 1.0f, 0);
            }
            else if (isGravityDisabled)
            {
                // Manually re-enabled gravity before the timer expired
                // No cooldown penalty for manual re-enable
                isGravityDisabled = false;
                gravityTimer = 0.0f;
                AEAudioPlay(GravityToggle, soundEffects, 1.0f, 1.0f, 0);
            }
        }

        // If gravity is disabled, tick down the timer each frame
        if (isGravityDisabled)
        {
            gravityTimer -= dt;

            // Timer expired -- automatically re-enable gravity and
            // start a 3-second cooldown before LShift can be used again
            if (gravityTimer <= 0.0f)
            {
                isGravityDisabled = false;
                gravityTimer = 0.0f;
                gravityCooldown = 3.0f;
                AEAudioPlay(GravityToggle, soundEffects, 1.0f, 1.0f, 0);
            }
        }
    }

    // ----------------------------------------------------------
    /*!
    \brief Initializes player movement state.

    \param player Reference to the player object.

    Resets velocity, cooldowns, and enables physics flags.
    */
    // ----------------------------------------------------------
    void initPlayerMovement(objectsquares& player)
    {
        player.velocityX = 0.0f;
        player.velocityY = 0.0f;
        jetPackCooldown = 0;
        enableGravity = enableDrag = 1;
    }

    // ----------------------------------------------------------
    /*!
    \brief Computes direction vector from player to mouse.

    \param player Reference to the player object.
    \return Normalized direction vector toward cursor.
    */
    // ----------------------------------------------------------
    AEVec2 getMouse(objectsquares& player) {
        AEInputGetCursorPosition(&mouseX, &mouseY);
        mouseX -= static_cast<s32>((screenWidth / 2) + player.xPos + player.velocityX);
        mouseY = static_cast<s32>((screenLength / 2) - mouseY - player.yPos - player.velocityY);
        mouseDistance = sqrtf(static_cast<f32>(mouseX * mouseX + mouseY * mouseY));
        // Normalize direction vector (only if distance > 0 to avoid division by zero)
        if (mouseDistance > 0.0f)
        {
            directionVector.x = mouseX / mouseDistance;
            directionVector.y = mouseY / mouseDistance;
        }
        return directionVector;
    }

    // ----------------------------------------------------------
    /*!
    \brief Updates player facing direction based on mouse position.

    \param player Reference to the player object.
    */
    // ----------------------------------------------------------
    void UpdatePlayerFacing(objectsquares& player)
    {
        // getMouse() returns a normalized vector from the player toward the cursor.
        // We only care about the x-component: negative means the mouse is to the left.
        AEVec2 aimDir = getMouse(player);
        playerFacingLeft = (aimDir.x < 0.0f);
    }

    // ----------------------------------------------------------
    /*!
    \brief Handles player input for movement and combat.

    \param player Reference to the player object.
    */
    // ----------------------------------------------------------
    void physicsInput(objectsquares& player)
    {
        // Tick down cooldown each frame if active
        if (jetPackCooldown > 0) {
            jetPackCooldown--;
        }

        // Toggle drag with D
        if (AEInputCheckTriggered(AEVK_D)) {
            enableDrag = !enableDrag;
            printf("Drag: %d\n", enableDrag);
        }

        // Reload bullets with R
        if (AEInputCheckTriggered(AEVK_R)) {
            bulletCount += 10;
        }

        // Jetpack thrust with SPACE (only if cooldown is 0)
        if (AEInputCheckTriggered(AEVK_SPACE) && jetPackCooldown == 0)
        {
            // Calculate direction vector from player to mouse
            getMouse(player);

            player.velocityX += directionVector.x * THRUST_POWER * static_cast<f32>(screenLength) / screenWidth;
            player.velocityY += directionVector.y * THRUST_POWER * static_cast<f32>(screenWidth) / screenLength;

            printf("Jetpack fired! Velocity: (%.2f, %.2f)\n", player.velocityX, player.velocityY);

            // Set cooldown in frames (e.g. 30 = ~0.5s at 60 FPS)
            jetPackCooldown = 30;

            // --- Jetpack exhaust particles ---
            // Spawn particles opposite to thrust direction so they look
            // like hot gas shooting out behind the player.
            const float kExhaustOffset = 25.0f;   // how far below/behind player to spawn
            const float kExhaustSpeed = 350.0f;   // base speed of exhaust particles
            const float kExhaustSpreadX = 120.0f;   // horizontal randomness
            const float kExhaustSpreadY = 80.0f;   // vertical randomness
            const float kExhaustLifetime = 0.3f;   // seconds before particles fade out
            const float kExhaustScale = 6.0f;   // particle render size
            const int   kExhaustCount = 12;       // particles per burst

            EmitterProps exhaust;
            // Offset the spawn point opposite to the thrust direction
            exhaust.spawnX = player.xPos - directionVector.x * kExhaustOffset;
            exhaust.spawnY = player.yPos - directionVector.y * kExhaustOffset;
            // Base velocity is opposite to thrust (exhaust blows backward)
            exhaust.velocityXBase = -directionVector.x * kExhaustSpeed;
            exhaust.velocityYBase = -directionVector.y * kExhaustSpeed;
            exhaust.velocitySpreadX = kExhaustSpreadX;
            exhaust.velocitySpreadY = kExhaustSpreadY;
            exhaust.lifetimeBase = kExhaustLifetime;
            exhaust.lifetimeSpread = 0.1f;   // slight variation so particles don't all die at once
            exhaust.scaleBase = kExhaustScale;
            exhaust.emitCount = kExhaustCount;

            ParticleSystem::Emit(exhaust);
        }

        // Fire bullet with left mouse button
        if (AEInputCheckTriggered(AEVK_LBUTTON) && bulletCount > 0)
        {
            if (player.currentWeapon == WEAPON_SHOTGUN && bulletCount < 3) return;
            // Calculate direction vector from player to mouse
            getMouse(player);

            // Shotgun kicks harder than the plasma gun (1.6x recoil)
            f32 recoil = ABSOLUTE_RECOIL;
            if (player.currentWeapon == WEAPON_SHOTGUN)
                recoil *= SHOTGUN_RECOIL_MULTIPLIER;

            player.velocityX -= directionVector.x * recoil * static_cast<f32>(screenLength) / screenWidth;
            player.velocityY -= directionVector.y * recoil * static_cast<f32>(screenWidth) / screenLength;
            bulletCount -= 1;
            if (player.currentWeapon == WEAPON_SHOTGUN) bulletCount -= 2;
        }
        if (AEInputCheckTriggered(AEVK_RBUTTON) && bulletCount > 0)
        {
            if (player.currentWeapon == WEAPON_SHOTGUN && bulletCount < 3) return;
            // Calculate direction vector from player to mouse
            getMouse(player);

            // Shotgun kicks harder than the plasma gun (1.6x recoil)
            f32 recoil = ABSOLUTE_RECOIL;
            if (player.currentWeapon == WEAPON_SHOTGUN)
                recoil *= SHOTGUN_RECOIL_MULTIPLIER;

            player.velocityX += directionVector.x * recoil * static_cast<f32>(screenLength) / screenWidth;
            player.velocityY += directionVector.y * recoil * static_cast<f32>(screenWidth) / screenLength;

            bulletCount -= 1;
            if (player.currentWeapon == WEAPON_SHOTGUN) bulletCount -= 2;
        }
    }

    // ----------------------------------------------------------
    /*!
    \brief Updates player physics including gravity, drag, and velocity limits.

    \param player Reference to the player object.
    */
    // ----------------------------------------------------------
    void updatePlayerPhysics(objectsquares& player)
    {
        // Only apply gravity when the standard toggle is on AND the
        // temporary LShift disable is not active.
        if (enableGravity && !isGravityDisabled) {
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
    }
}