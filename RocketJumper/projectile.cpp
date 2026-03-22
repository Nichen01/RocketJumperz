#include "Projectile.h"

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
     * position. Calculates direction vector away from the mouse cursor
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

                // Calculate direction vector AWAY from mouse (rocket-jumper: fire opposite to aim)
                f32 dx = player.xPos - static_cast<f32>(worldMouseX);
                f32 dy = player.yPos - static_cast<f32>(worldMouseY);

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
    }

    // UPDATE ALL ACTIVE PROJECTILES (MOVEMENT AND BOUNDING)
    void UpdateProjectiles(Projectile projectiles[], s32 maxCount)
    {
        for (int i = 0; i < maxCount; i++)
        {
            if (projectiles[i].isActive == 1)
            {
                // Move projectile along its velocity vector (away from mouse)
                projectiles[i].shape.xPos -= projectiles[i].shape.velocityX;
                projectiles[i].shape.yPos -= projectiles[i].shape.velocityY;

                // Check if projectile is off-screen
                if (projectiles[i].shape.xPos > 800.0f || projectiles[i].shape.xPos < -800.0f ||
                    projectiles[i].shape.yPos > 450.0f || projectiles[i].shape.yPos < -450.0f)
                {
                    projectiles[i].isActive = 0;  // Deactivate projectile
                    printf("Projectile %d went off-screen\n", i);
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
}