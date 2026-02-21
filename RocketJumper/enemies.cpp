#pragma once
/* Start Header ************************************************************************/
/*!
\file		  enemy.h
\author	      Nicholas Chen (c.chen)
\date         January, 31, 2026
\brief        Enemy system with melee and ranged enemy types

Copyright (C) 2026 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header *********************************************************************/

#include "enemies.h"
#include "collision.h"
#include "render.h"
#include <cmath>

namespace enemySystem {
    // Audio
    AEAudio LaserBlaster = AEAudioLoadSound("Assets/ore.mp3");
    AEAudioGroup se = AEAudioCreateGroup();

    // Initialize all enemies to inactive state
    void initEnemies(Enemy enemies[], s32 maxCount)
    {
        for (int i = 0; i < maxCount; i++)
        {
            enemies[i].shape.xPos = 0.0f;
            enemies[i].shape.yPos = 0.0f;
            enemies[i].shape.xScale = 35.0f;  // Default enemy size
            enemies[i].shape.yScale = 35.0f;
            enemies[i].type = ENEMY_MELEE;
            enemies[i].shape.velocityX = 0.0f;
            enemies[i].shape.velocityY = 0.0f;
            enemies[i].health = 0.0f;
            enemies[i].attackCooldown = 0.0f;
            enemies[i].detectionRange = 0.0f;
            enemies[i].attackRange = 0.0f;
            enemies[i].moveSpeed = 0.0f;
            enemies[i].isActive = 0;
            enemies[i].hasLineOfSight = 0;
        }
    }

    // Spawn a new enemy at specified position
    s8 spawnEnemy(Enemy enemies[], s32 maxCount,
        EnemyType type, f32 xPos, f32 yPos)
    {
        // Find first inactive enemy slot
        s32 foundSlot = -1;
        for (int i = 0; i < maxCount; i++)
        {
            if (enemies[i].isActive == 0) // if the enemy in array is inactive, assign it a slot
            {
                foundSlot = i;
                break;
            }
        }

        // If no slots available, return failure
        if (foundSlot == -1)
        {
            printf("Cannot spawn enemy: No available slots (max: %d)\n", maxCount);
            return 0;
        }

        // Initialize enemy based on type
        enemies[foundSlot].shape.xPos = xPos;
        enemies[foundSlot].shape.yPos = yPos;
        enemies[foundSlot].type = type;
        enemies[foundSlot].shape.velocityX = 0.0f;
        enemies[foundSlot].shape.velocityY = 0.0f;
        enemies[foundSlot].attackCooldown = 0.0f;
        enemies[foundSlot].hasLineOfSight = 0;
        enemies[foundSlot].isActive = 1;

        if (type == ENEMY_MELEE)
        {
            enemies[foundSlot].health = MELEE_HEALTH;
            enemies[foundSlot].detectionRange = MELEE_DETECTION_RANGE;
            enemies[foundSlot].attackRange = MELEE_ATTACK_RANGE;
            enemies[foundSlot].moveSpeed = MELEE_MOVE_SPEED;
            enemies[foundSlot].shape.xScale = 30.0f;
            enemies[foundSlot].shape.yScale = 30.0f;
        }
        else if (type == ENEMY_RANGED)
        {
            enemies[foundSlot].health = RANGED_HEALTH;
            enemies[foundSlot].detectionRange = RANGED_DETECTION_RANGE;
            enemies[foundSlot].attackRange = RANGED_ATTACK_RANGE;
            enemies[foundSlot].moveSpeed = RANGED_MOVE_SPEED;
            enemies[foundSlot].shape.xScale = 30.0f;
            enemies[foundSlot].shape.yScale = 30.0f;
        }

        // Debugging
        printf("Enemy spawned at (%.1f, %.1f) - Type: %s\n",
            xPos, yPos, type == ENEMY_MELEE ? "MELEE" : "RANGED");
        return 1;
    }

    // Helper function: Calculate distance between two points
    f32 getDistance(f32 x1, f32 y1, f32 x2, f32 y2)
    {
        f32 dx = x2 - x1;
        f32 dy = y2 - y1;
        return sqrtf(dx * dx + dy * dy);
    }

    // Helper function: Check line of sight (basic version, doesn't check walls yet)
    s8 hasLineOfSight(Enemy& enemy, objectsquares& player,
        int map[], int mapX, int mapY, int mapS)
    {
        // For now, just check distance
        // TODO: Add raycast to check for walls between enemy and player
        f32 distance = getDistance(enemy.shape.xPos, enemy.shape.yPos,
            player.xPos, player.yPos);
        return (distance <= enemy.detectionRange) ? 1 : 0;
    }

    /*!*************************************************************************
     * UPDATE ENEMIES
     * @brief Update all active enemies (AI, movement, attacks)
     *
     * @param enemies[]    Array containing all the enemies 
     * @param maxCount     Maximum number of enemies
     * @return VOID
     ***************************************************************************/
    void updateEnemies(Enemy enemies[], s32 maxCount,
        objectsquares& player,
        Projectile enemyProjectiles[], s32 maxProjectiles,
        f32 deltaTime, AEAudio attackSound, AEAudioGroup sfxGroup)
    {
        for (int i = 0; i < maxCount; i++)
        {
            if (enemies[i].isActive == 0) continue;

            // Decrease attack cooldown
            if (enemies[i].attackCooldown > 0.0f)
            {
                enemies[i].attackCooldown -= deltaTime;
            }

            // Calculate distance to player
            f32 distanceToPlayer = getDistance(
                enemies[i].shape.xPos, enemies[i].shape.yPos,
                player.xPos, player.yPos
            );

            // Check if player is in detection range
            if (distanceToPlayer <= enemies[i].detectionRange)
            {
                enemies[i].hasLineOfSight = 1;

                // Calculate direction to player
                f32 dx = player.xPos - enemies[i].shape.xPos;
                f32 dy = player.yPos - enemies[i].shape.yPos;

                // Normalize direction
                f32 length = sqrtf(dx * dx + dy * dy);
                if (length > 0.0f)
                {
                    dx /= length;
                    dy /= length;
                }

                // === MELEE ENEMY BEHAVIOR === //
                if (enemies[i].type == ENEMY_MELEE)
                {
                    // Move towards player if not in attack range
                    if (distanceToPlayer > enemies[i].attackRange)
                    {
                        enemies[i].shape.velocityX = dx * enemies[i].moveSpeed;
                        enemies[i].shape.velocityY = dy * enemies[i].moveSpeed;

                        // Apply movement
                        //enemies[i].shape.xPos += enemies[i].velocityX;
                        //enemies[i].shape.yPos += enemies[i].velocityY;
                    }
                    else
                    {
                        
                        // Stop moving when in attack range
                        //enemies[i].shape.velocityX = 0.0f;
                        //enemies[i].shape.velocityY = 0.0f;
                    }
                }
                // === RANGED ENEMY BEHAVIOR === //
                else if (enemies[i].type == ENEMY_RANGED)
                {
                    // Maintain distance from player
                    f32 optimalDistance = enemies[i].attackRange * 0.7f;

                    // Too close, move away
                    if (distanceToPlayer < optimalDistance)
                    {
                        // opp direction * speed = move away
                        enemies[i].shape.velocityX = -dx * enemies[i].moveSpeed;
                        enemies[i].shape.velocityY = -dy * enemies[i].moveSpeed;

                        //enemies[i].shape.xPos += enemies[i].velocityX;
                        //enemies[i].shape.yPos += enemies[i].velocityY;
                    }
                    // Too far,  move closer
                    else if (distanceToPlayer > enemies[i].attackRange)
                    {
                        // direction * move speed = move towards
                        enemies[i].shape.velocityX = dx * enemies[i].moveSpeed;
                        enemies[i].shape.velocityY = dy * enemies[i].moveSpeed;

                        //enemies[i].shape.xPos += enemies[i].velocityX;
                        //enemies[i].shape.yPos += enemies[i].velocityY;
                    }
                    else
                    {
                        // In range, stop
                        //enemies[i].velocityX = 0.0f;
                        //enemies[i].velocityY = 0.0f;
                    }

                    // If in range and cooldown is ready = shoot at player
                    if (distanceToPlayer <= enemies[i].attackRange &&
                        enemies[i].attackCooldown <= 0.0f)
                    {
                        // Find available projectile slot
                        for (int p = 0; p < maxProjectiles; p++)
                        {
                            if (enemyProjectiles[p].isActive == 0) // if projectile is not active(available)
                            {
                                

                                // Fire projectile towards player
                                // init projectile position at enemy position and init size
                                enemyProjectiles[p].shape.xPos = enemies[i].shape.xPos;
                                enemyProjectiles[p].shape.yPos = enemies[i].shape.yPos;
                                enemyProjectiles[p].shape.xScale = 10.0f;
                                enemyProjectiles[p].shape.yScale = 10.0f;

                                f32 projectileSpeed = 8.0f;
                                enemyProjectiles[p].shape.velocityX = dx * projectileSpeed;
                                enemyProjectiles[p].shape.velocityY = dy * projectileSpeed;
                                enemyProjectiles[p].isActive = 1;

                                // Reset cooldown back up
                                enemies[i].attackCooldown = RANGED_ATTACK_COOLDOWN;

                                // Play the sound passed in from Level 1
                                AEAudioPlay(attackSound, sfxGroup, 1.0f, 1.0f, 0);

                                printf("Enemy %d fired projectile!\n", i);
                                break;
                            }
                        }
                    }
                }
            }
            // Player NOT in detection range. No movement 
            else
            {
                enemies[i].hasLineOfSight = 0;
                enemies[i].shape.velocityX = 0.0f;
                enemies[i].shape.velocityY = 0.0f;
            }

            // Check if enemy is dead
            if (enemies[i].health <= 0.0f)
            {
                enemies[i].isActive = 0;
                printf("Enemy %d defeated!\n", i);
            }
        }
    }

    /*!*************************************************************************
     * RENDER ENEMIES
     * @brief Renders and prepares the enemy textures to be drawn
     *
     * @param enemies[]         Array containing all the enemies
     * @param maxCount          Maximum number of enemies
     * @param mesh              VertexList containing mesh for each enemy
     * @param meleeTexture      Texture set containing texture for MELEE enemy
     * @param rangedTexture     Texture set containing texture for RANGED enemy
     * @return VOID
     ***************************************************************************/
    // Render all active enemies
    void renderEnemies(Enemy enemies[], s32 maxCount,
        AEGfxVertexList* mesh,
        AEGfxTexture* meleeTexture,
        AEGfxTexture* rangedTexture)
    {
        for (int i = 0; i < maxCount; i++)
        {
            if (enemies[i].isActive == 1)
            {
                // Choose texture based on enemy type and if the texture is not NULL
                AEGfxTexture* texture = nullptr;
                if (enemies[i].type == ENEMY_MELEE && meleeTexture)
                {
                    texture = meleeTexture;
                }
                else if (enemies[i].type == ENEMY_RANGED && rangedTexture)
                {
                    texture = rangedTexture;
                }

                // Texture available ==> set render mode and texture set
                if (texture)
                {
                    AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
                    AEGfxTextureSet(texture, 0, 0);
                }
                else
                {
                    // Fallback to colored squares if no texture
                    AEGfxSetRenderMode(AE_GFX_RM_COLOR);
                    if (enemies[i].type == ENEMY_MELEE)
                    {
                        AEGfxSetColorToAdd(1.0f, 0.3f, 0.3f, 1.0f);  // Red for melee
                    }
                    else
                    {
                        AEGfxSetColorToAdd(0.3f, 0.3f, 1.0f, 1.0f);  // Blue for ranged
                    }
                }

                // Draw enemy
                render::Drawsquare(enemies[i].shape.xPos, enemies[i].shape.yPos,
                    enemies[i].shape.xScale, enemies[i].shape.yScale);
                AEGfxMeshDraw(mesh, AE_GFX_MDM_TRIANGLES);
            }
        }
    }

    /*!*************************************************************************
     * CHECK PLAYER ENEMY COLLISION
     * @brief Checks if there is collision between player and enemy sprites
     *
     * @param enemies[]    Array containing all the enemies
     * @param maxCount     Maximum number of enemies
     * @param player       Struct data for the player square
     * @return             (f32) Total damage done to player 
     ***************************************************************************/
    f32 checkPlayerEnemyCollision(Enemy enemies[], s32 maxCount,
        objectsquares& player, AEAudio attackSound, AEAudioGroup sfxGroup)
    {
        f32 totalDamage = 0.0f;

        // For every enemy
        for (int i = 0; i < maxCount; i++)
        {
            if (enemies[i].isActive == 1 && enemies[i].type == ENEMY_MELEE)
            {
                // Check collision using existing collision system
                if (gamelogic::collision(&player, &enemies[i].shape))
                {
                    // Only deal damage if cooldown is ready
                    if (enemies[i].attackCooldown <= 0.0f)
                    {
                        // Play the sound passed in from Level 1
                        AEAudioPlay(attackSound, sfxGroup, 1.0f, 1.0f, 0);
                        totalDamage += MELEE_DAMAGE; 
                        enemies[i].attackCooldown = MELEE_ATTACK_COOLDOWN; // Reset cooldown
                        printf("Player hit by melee enemy %d! Damage: %.1f\n", i, MELEE_DAMAGE);
                    }
                }
            }
        }
        return totalDamage;
    }

    // Check if player projectiles hit any enemies
    /*!*************************************************************************
     * CHECK PROJECTILE ENEMY COLLISION
     * @brief Checks if player projectile hits enemy
     *
     * @param enemies[]            Array containing all the enemies data
     * @param maxCount             Maximum number of enemies
     * @param projectiles[]        Struct data each player projectile
     * @return                     VOID
     ***************************************************************************/
    void checkProjectileEnemyCollision(Enemy enemies[], s32 maxCount,
        Projectile projectiles[], s32 maxProjectiles)
    {
        // For every enemy, check if enemy is active
        for (int e = 0; e < maxCount; e++)
        {
            if (enemies[e].isActive == 1)
            {
                // For every projectile, check if projectile is active
                for (int p = 0; p < maxProjectiles; p++)
                {
                    if (projectiles[p].isActive == 1)
                    {
                        // if collision between projectile and enemy
                        if (gamelogic::collision(&projectiles[p].shape, &enemies[e].shape))
                        {
                            // Deal damage to enemy
                            enemies[e].health -= 10.0f;  // reduce enemy health

                            // Destroy projectile on hit 
                            projectiles[p].isActive = 0;

                            printf("Enemy %d hit! Health remaining: %.1f\n",
                                e, enemies[e].health);
                            break;  // One projectile can only hit one enemy
                        }
                    }
                }
            }
        }
    }
}