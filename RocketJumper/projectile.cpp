#include "Projectile.h"
#include "BinaryMap.h"  // BINARY_MAP_HEIGHT for tile-collision bounds check
#include <cmath>    // sinf, cosf, sqrtf
#include <cstdlib>  // rand, RAND_MAX

namespace projectileSystem {

    /*!*************************************************************************
     * INIT PROJECTILES
     * @brief Initializes an array of projectiles, setting their initial
     * positions, sizes, velocities, and marking them as inactive.
     * Usage: projectileSystem::initProjectiles(Projectiles, MAX_PROJECTILES);
     *
     * @param projectiles[]  Array containing projectile structs to initialize
     * @param maxCount       Maximum number of projectiles in the array
     * @return VOID
     ***************************************************************************/
    void initProjectiles(Projectile projectiles[], s32 maxCount)
    {
        // loop through all projectile slots to set data
        for (int i = 0; i < maxCount; i++)
        {
            projectiles[i].shape.xPos = 0.0f;
            projectiles[i].shape.yPos = 0.0f;
            projectiles[i].shape.xScale = 20.0f;
            projectiles[i].shape.yScale = 20.0f;
            projectiles[i].shape.velocityX = 0.0f;
            projectiles[i].shape.velocityY = 0.0f;
            projectiles[i].isActive = 0;
        }
    }

    /*!*************************************************************************
     * FIRE PROJECTILES
     * @brief Finds an inactive projectile slot and fires it from the player's
     * position. Calculates direction vector toward the mouse cursor
     * and sets the projectile's velocity.
     * Usage: projectileSystem::fireProjectiles(mouseX, mouseY, playerObj, Projectiles, MAX_PROJECTILES);
     *
     * @param worldMouseX    Mouse X coordinate in world space
     * @param worldMouseY    Mouse Y coordinate in world space
     * @param player         Reference to the player object to spawn projectiles from
     * @param projectiles[]  Array containing the projectile structs
     * @param maxCount       Maximum number of projectiles in the array
     * @return VOID
     ***************************************************************************/
    void fireProjectiles(s32 worldMouseX, 
        s32 worldMouseY, 
        objectsquares& player, 
        Projectile projectiles[], 
        s32 maxCount, 
        AEAudio attackSound, 
        AEAudioGroup sfxGroup)
    {
        if (AEInputCheckTriggered(AEVK_LBUTTON))
        {
            s32 foundSlot = -1;

            // Find first inactive projectile slot in projectiles array
            for (int i = 0; i < maxCount; i++)
            {
                if (projectiles[i].isActive == 0)
                {
                    foundSlot = i; // set potential slot to proj index
                    break;  // Exit loop early when we find a slot
                }
            }

            // if slot available, fire the projectile
            if (foundSlot != -1)
            {
                // Set projectile starting pos to player pos
                projectiles[foundSlot].shape.xPos = player.xPos;
                projectiles[foundSlot].shape.yPos = player.yPos + 5.0f;

                // Calculate direction vector TOWARD the mouse cursor
                f32 dx = static_cast<f32>(worldMouseX) - player.xPos;
                f32 dy = static_cast<f32>(worldMouseY) - player.yPos;

                // Normalize direction vector
                f32 length = sqrtf(dx * dx + dy * dy);
                if (length > 0.0f)
                {
                    dx /= length;
                    dy /= length;
                }

                // Set velocity
                f32 speed = 15.0f;
                projectiles[foundSlot].shape.velocityX = dx * speed;
                projectiles[foundSlot].shape.velocityY = dy * speed;

                // Activate projectile
                projectiles[foundSlot].isActive = 1;
                AEAudioPlay(attackSound, sfxGroup, 1.0f, 1.0f, 0);
                printf("Projectile slot %d fired!\n", foundSlot);
            }
            else
            {
                printf("No available projectile slots! Max: %d\n", maxCount);
            }
        }
        if (AEInputCheckTriggered(AEVK_RBUTTON))
        {
            s32 foundSlot = -1;

            // Find first inactive projectile slot in projectiles array
            for (int i = 0; i < maxCount; i++)
            {
                if (projectiles[i].isActive == 0)
                {
                    foundSlot = i; // set potential slot to proj index
                    break;  // Exit loop early when we find a slot
                }
            }

            // if slot available, fire the projectile
            if (foundSlot != -1)
            {
                // Set projectile starting pos to player pos
                projectiles[foundSlot].shape.xPos = player.xPos;
                projectiles[foundSlot].shape.yPos = player.yPos + 5.0f;

                // Calculate direction vector TOWARD the mouse cursor
                f32 dx = static_cast<f32>(worldMouseX) - player.xPos;
                f32 dy = static_cast<f32>(worldMouseY) - player.yPos;

                // Normalize direction vector
                f32 length = sqrtf(dx * dx + dy * dy);
                if (length > 0.0f)
                {
                    dx /= length;
                    dy /= length;
                }

                dx = -dx;
                dy = -dy;

                // Set velocity
                f32 speed = 15.0f;
                projectiles[foundSlot].shape.velocityX = dx * speed;
                projectiles[foundSlot].shape.velocityY = dy * speed;

                // Activate projectile
                projectiles[foundSlot].isActive = 1;
                AEAudioPlay(attackSound, sfxGroup, 1.0f, 1.0f, 0);
                printf("Projectile slot %d fired!\n", foundSlot);
            }
            else
            {
                printf("No available projectile slots! Max: %d\n", maxCount);
            }
        }
    }

    // UPDATE ALL ACTIVE PROJECTILES (MOVEMENT, BOUNDING, AND WALL COLLISION)
    void UpdateProjectiles(Projectile projectiles[], s32 maxCount,
        int map[], int mapX, int mapS)
    {
        for (int i = 0; i < maxCount; i++)
        {
            if (projectiles[i].isActive == 1)
            {
                // Move projectile along its velocity vector (toward mouse)
                projectiles[i].shape.xPos += projectiles[i].shape.velocityX;
                projectiles[i].shape.yPos += projectiles[i].shape.velocityY;

                // Check if projectile is off-screen
                if (projectiles[i].shape.xPos > 800.0f || projectiles[i].shape.xPos < -800.0f ||
                    projectiles[i].shape.yPos > 450.0f || projectiles[i].shape.yPos < -450.0f)
                {
                    projectiles[i].isActive = 0;  // Deactivate projectile
                    printf("Projectile %d went off-screen\n", i);
                    continue; // Already deactivated, skip wall check
                }

                // --- Tile collision check ---
                // Convert world position to map-grid indices using the same
                // helper functions that Collision_movement uses.
                float tileS = static_cast<float>(mapS);
                float gridX = gamelogic::posX_to_index(projectiles[i].shape.xPos, tileS);
                float gridY = gamelogic::posY_to_index(projectiles[i].shape.yPos, tileS);

                // Convert to integer tile coordinates
                int tileCol = static_cast<int>(gridX);
                int tileRow = static_cast<int>(gridY);

                // Bounds check before accessing the map array
                if (tileCol >= 0 && tileCol < mapX &&
                    tileRow >= 0 && tileRow < BINARY_MAP_HEIGHT)
                {
                    int tileValue = map[tileRow * mapX + tileCol];

                    // Any non-zero tile is solid (matches BinaryCollisionArray logic)
                    if (tileValue >= 1)
                    {
                        projectiles[i].isActive = 0;
                        printf("Projectile %d hit wall at tile (%d, %d)\n",
                            i, tileCol, tileRow);
                    }
                }
            }
        }
    }

    //======================RENDER ALL ACTIVE PROJECTILES====================================//
    void renderProjectiles(Projectile projectiles[], s32 maxCount, AEGfxTexture* texture, AEGfxVertexList* mesh)
    {
        if (!mesh)
        {
            printf("MESH IS NULL!\n");
            return;
        }

        if (!texture)
        {
            printf("TEXTURE IS NULL!\n");
        }

        for (int i = 0; i < maxCount; i++)
        {
            if (projectiles[i].isActive == 1)
            {
                // Set up transformation matrix
                AEMtx33 scale = { 0 };
                AEMtx33Scale(&scale, projectiles[i].shape.xScale, projectiles[i].shape.yScale);

                AEMtx33 rotate = { 0 };
                AEMtx33Rot(&rotate, 0.0f);

                AEMtx33 translate = { 0 };
                AEMtx33Trans(&translate, projectiles[i].shape.xPos, projectiles[i].shape.yPos);

                AEMtx33 transform = { 0 };
                AEMtx33Concat(&transform, &rotate, &scale);
                AEMtx33Concat(&transform, &translate, &transform);

                AEGfxSetTransform(transform.m);

                // Set texture if provided
                if (texture)
                {
                    AEGfxTextureSet(texture, 0.0f, 0.0f);
                }

                // Draw the projectile
                AEGfxMeshDraw(mesh, AE_GFX_MDM_TRIANGLES);
            }
        }
    }

    // Check projectile collisions with an obstacle using existing collision function
    void checkProjectileCollisions(Projectile projectiles[], s32 maxCount, objectsquares& obstacle)
    {
        for (int i = 0; i < maxCount; i++)
        {
            if (projectiles[i].isActive == 1)
            {
                // Use the existing collision detection from collision.h
                if (gamelogic::collision(&projectiles[i].shape, &obstacle))
                {
                    projectiles[i].isActive = 0;  // Destroy projectile on hit
                    printf("Projectile %d hit obstacle!\n", i);
                }
            }
        }
    }

    /*!*************************************************************************
     * FIRE SHOTGUN
     * @brief Fires 5 pellets in a cone spread toward the mouse cursor.
     *        Uses 2D vector rotation to fan the pellets out from the base
     *        direction, and adds +/- 10% random speed variance per pellet.
     *
     * @param worldMouseX    Mouse X coordinate in world space
     * @param worldMouseY    Mouse Y coordinate in world space
     * @param player         Reference to the player object
     * @param projectiles[]  Array containing the projectile structs
     * @param maxCount       Maximum number of projectiles in the array
     * @param attackSound    Sound effect to play on fire
     * @param sfxGroup       Audio group for the sound effect
     ***************************************************************************/
    void FireShotgun(s32 worldMouseX,
        s32 worldMouseY,
        objectsquares& player,
        Projectile projectiles[],
        s32 maxCount,
        AEAudio attackSound,
        AEAudioGroup sfxGroup)
    {
        if (!(AEInputCheckTriggered(AEVK_LBUTTON)||AEInputCheckTriggered(AEVK_RBUTTON)))
            return;

        // ---- Constants for the shotgun spread ----
        const int   kPelletCount    = 5;       // Number of pellets per shot
        const f32   kBaseSpeed      = 15.0f;   // Same base speed as plasma
        const f32   kSpeedVariance  = 0.10f;   // +/- 10% random variance
        const f32   kSpreadAngle    = 0.35f;   // Total cone half-angle in radians (~20 degrees)

        // Calculate base direction vector from player to mouse
        f32 dx = static_cast<f32>(worldMouseX) - player.xPos;
        f32 dy = static_cast<f32>(worldMouseY) - player.yPos;

        // Normalize direction
        f32 length = sqrtf(dx * dx + dy * dy);
        if (length > 0.0f)
        {
            dx /= length;
            dy /= length;
        }
        else
        {
            // If mouse is exactly on the player, default to shooting right
            dx = 1.0f;
            dy = 0.0f;
        }
        if (AEInputCheckTriggered(AEVK_RBUTTON)) {
            dx=-dx;
            dy=-dy;
        }
        bool playedSound = false;

        // Spawn each pellet with a slightly different angle
        for (int pellet = 0; pellet < kPelletCount; ++pellet)
        {
            // Find an inactive projectile slot
            s32 foundSlot = -1;
            for (int i = 0; i < maxCount; ++i)
            {
                if (projectiles[i].isActive == 0)
                {
                    foundSlot = i;
                    break;
                }
            }

            // If no slot available, stop spawning (don't waste iteration)
            if (foundSlot == -1)
            {
                printf("Shotgun: ran out of projectile slots at pellet %d\n", pellet);
                break;
            }

            // ---- Calculate this pellet's rotation angle ----
            // Spread the pellets evenly across the cone, from -kSpreadAngle to +kSpreadAngle.
            // For 5 pellets: angles are -0.35, -0.175, 0, +0.175, +0.35
            // Map pellet index [0..4] to angle [-kSpreadAngle..+kSpreadAngle]
            f32 theta = -kSpreadAngle + (2.0f * kSpreadAngle)
                * (static_cast<f32>(pellet) / static_cast<f32>(kPelletCount - 1));

            // ---- 2D vector rotation ----
            // Rotated direction = (x*cos(theta) - y*sin(theta),
            //                      x*sin(theta) + y*cos(theta))
            f32 cosTheta = cosf(theta);
            f32 sinTheta = sinf(theta);
            f32 rotatedDx = dx * cosTheta - dy * sinTheta;
            f32 rotatedDy = dx * sinTheta + dy * cosTheta;

            // ---- Random speed variance (+/- 10%) ----
            // rand() / RAND_MAX gives [0, 1], scale to [-variance, +variance]
            f32 randomFactor = 1.0f + kSpeedVariance
                * (2.0f * (static_cast<f32>(rand()) / static_cast<f32>(RAND_MAX)) - 1.0f);
            f32 pelletSpeed = kBaseSpeed * randomFactor;

            // ---- Set up the projectile ----
            projectiles[foundSlot].shape.xPos = player.xPos;
            projectiles[foundSlot].shape.yPos = player.yPos + 5.0f;
            projectiles[foundSlot].shape.velocityX = rotatedDx * pelletSpeed;
            projectiles[foundSlot].shape.velocityY = rotatedDy * pelletSpeed;
            projectiles[foundSlot].isActive = 1;

            printf("Shotgun pellet %d fired in slot %d (angle=%.2f, speed=%.2f)\n",
                pellet, foundSlot, theta, pelletSpeed);

            // Play the sound only once per shot, not per pellet
            if (!playedSound)
            {
                AEAudioPlay(attackSound, sfxGroup, 1.0f, 1.0f, 0);
                playedSound = true;
            }
        }
    }
}