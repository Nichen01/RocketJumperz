#include "Movement.h"
#include "Sound.h"
#include "ParticleSystem.h"
//#include "physicsEngine.h"
#include <cmath>

int totalFrame = 40;
int numImages = 6;

namespace movement {
    // initialize the velocity 
    bool enableGravity, enableDrag;
    u8 jetPackCooldown;
    s32 mouseX,mouseY;
    f32 mouseDistance;
    AEVec2 directionVector;
    int bulletCount = 25;
    bool playerFacingLeft = false; // starts facing right

    void initPlayerMovement(objectsquares& player)
    {
        player.velocityX = 0.0f;
        player.velocityY = 0.0f;
        jetPackCooldown = 0;
        enableGravity = enableDrag = 1;
    }

    AEVec2 getMouse(objectsquares &player) {
        AEInputGetCursorPosition(&mouseX,&mouseY);
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

    // Determines which direction the player sprite should face based on the
    // mouse cursor position relative to the player. Call once per frame in Update.
    void UpdatePlayerFacing(objectsquares& player)
    {
        // getMouse() returns a normalized vector from the player toward the cursor.
        // We only care about the x-component: negative means the mouse is to the left.
        AEVec2 aimDir = getMouse(player);
        playerFacingLeft = (aimDir.x < 0.0f);
    }

    void physicsInput(objectsquares& player)
    {
        // Tick down cooldown each frame if active
        if (jetPackCooldown > 0) {
            jetPackCooldown--;
        }

        // Toggle gravity with G -- play sound effect on each toggle
        if (AEInputCheckTriggered(AEVK_G)) {
            enableGravity = !enableGravity;
            AEAudioPlay(GravityToggle, soundEffects, 1.0f, 1.0f, 0);
            printf("Gravity: %d\n", enableGravity);
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
            const float kExhaustOffset   = 25.0f;   // how far below/behind player to spawn
            const float kExhaustSpeed    = 350.0f;   // base speed of exhaust particles
            const float kExhaustSpreadX  = 120.0f;   // horizontal randomness
            const float kExhaustSpreadY  =  80.0f;   // vertical randomness
            const float kExhaustLifetime =   0.3f;   // seconds before particles fade out
            const float kExhaustScale    =   6.0f;   // particle render size
            const int   kExhaustCount    =  12;       // particles per burst

            EmitterProps exhaust;
            // Offset the spawn point opposite to the thrust direction
            exhaust.spawnX          = player.xPos - directionVector.x * kExhaustOffset;
            exhaust.spawnY          = player.yPos - directionVector.y * kExhaustOffset;
            // Base velocity is opposite to thrust (exhaust blows backward)
            exhaust.velocityXBase   = -directionVector.x * kExhaustSpeed;
            exhaust.velocityYBase   = -directionVector.y * kExhaustSpeed;
            exhaust.velocitySpreadX = kExhaustSpreadX;
            exhaust.velocitySpreadY = kExhaustSpreadY;
            exhaust.lifetimeBase    = kExhaustLifetime;
            exhaust.lifetimeSpread  = 0.1f;   // slight variation so particles don't all die at once
            exhaust.scaleBase       = kExhaustScale;
            exhaust.emitCount       = kExhaustCount;

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
        if (AEInputCheckTriggered(AEVK_RBUTTON) && bulletCount>0)
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