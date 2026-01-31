#include "projectile.h"

namespace projectileSystem {

    // Initialize all projectiles to inactive state
    void initProjectiles(Projectile projectiles[], s32 maxCount)
    {
        for (int i = 0; i < maxCount; i++)
        {
            projectiles[i].shape.xPos = 0.0f;
            projectiles[i].shape.yPos = 0.0f;
            projectiles[i].shape.xScale = 10.0f;
            projectiles[i].shape.yScale = 10.0f;
            projectiles[i].velocityX = 0.0f;
            projectiles[i].velocityY = 0.0f;
            projectiles[i].isActive = 0;
        }
    }

    // FIRE A NEW PROJECTILE
    void fireProjectiles(s32 worldMouseX, s32 worldMouseY, objectsquares& player, Projectile projectiles[], s32 maxCount)
    {
        if (AEInputCheckTriggered(AEVK_LBUTTON))
        {
            // Find first inactive projectile slot
            s32 foundSlot = -1;
            for (int i = 0; i < maxCount; i++)
            {
                if (projectiles[i].isActive == 0)
                {
                    foundSlot = i;
                    break;  // Exit loop early when we find a slot
                }
            }

            // if found an available slot, fire the projectile
            if (foundSlot != -1)
            {
                // Set projectile starting position to player position
                projectiles[foundSlot].shape.xPos = player.xPos;
                projectiles[foundSlot].shape.yPos = player.yPos;

                // Calculate direction from player to mouse
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
                projectiles[foundSlot].velocityX = dx * speed;
                projectiles[foundSlot].velocityY = dy * speed;

                // Activate projectile
                projectiles[foundSlot].isActive = 1;

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
                // Move projectile
                projectiles[i].shape.xPos += projectiles[i].velocityX;
                projectiles[i].shape.yPos += projectiles[i].velocityY;

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
    void renderProjectiles(Projectile projectiles[], s32 maxCount, AEGfxVertexList* mesh)
    {
        if (!mesh)
        {
            printf("MESH IS NULL!");
            return;
        }
        for (int i = 0; i < maxCount; i++)
        {
            if (projectiles[i].isActive == 1)
            {
                // Set projectile color (yellow for visibility)
                AEGfxSetColorToAdd(1.0f, 1.0f, 0.0f, 1.0f);

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