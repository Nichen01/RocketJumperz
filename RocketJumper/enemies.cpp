/* Start Header ************************************************************************/
/*!
\file		  enemies.cpp
\author	      Nicholas Chen (c.chen)
\date         January, 31, 2026
\brief        Enemy system with melee and ranged enemy types

Copyright (C) 2026 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header *********************************************************************/

#include "Enemies.h"
#include "Collision.h"
#include "Draw.h"
#include "Player.h"
#include "AssetManager.h"
#include <cmath>

namespace enemySystem {
    // Note: Audio resources are loaded and managed by Level files, not here.
    // They are passed in as parameters to updateEnemies().

    // -----------------------------------------------------------------------
    // Animation frame-rate constants for ranged enemy spritesheets
    // -----------------------------------------------------------------------
    const f32 kRangedFrameDelay = 0.12f;  // Seconds per frame (~8 fps)

    // Spritesheet dimensions (rows x cols) per state
    const int kRangedIdleCols   = 4;  // Reuses RangedMove.png, frozen on frame 0
    const int kRangedIdleRows   = 1;
    const int kRangedMoveCols   = 4;  // RangedMove.png   (1 row, 4 columns)
    const int kRangedMoveRows   = 1;
    const int kRangedAttackCols = 6;  // RangedAttack.png (1 row, 6 columns)
    const int kRangedAttackRows = 1;
    const int kRangedDeathCols  = 4;  // RangedDeath.png  (1 row, 4 columns)
    const int kRangedDeathRows  = 1;
    const int kRangedHurtCols   = 2;  // RangedHurt.png   (1 row, 2 columns)
    const int kRangedHurtRows   = 1;

    /*!*************************************************************************
     * SET ENEMY STATE
     * @brief Transitions a ranged enemy into a new animation state.
     *        Re-initializes the per-enemy SpriteAnimation when the state
     *        actually changes so the new spritesheet plays from frame 0.
     *
     * @param enemy       Reference to the enemy whose state is changing
     * @param newState    The target EnemyState to transition into
     * @return VOID
     ***************************************************************************/
    void SetEnemyState(Enemy& enemy, EnemyState newState)
    {
        // Early-return: do nothing if already in the requested state
        if (enemy.state == newState) return;

        enemy.state = newState;

        // Re-init the SpriteAnimation based on the new state's spritesheet
        switch (newState)
        {
        case STATE_IDLE:
            // Idle reuses the move sheet but stays on frame 0 (ANIM_IDLE)
            animSystem::init(enemy.anim,
                kRangedIdleCols, kRangedIdleRows,
                kRangedIdleCols * kRangedIdleRows,  // totalFrames = 4
                kRangedFrameDelay,
                ANIM_IDLE, 0);
            break;

        case STATE_MOVING:
            animSystem::init(enemy.anim,
                kRangedMoveCols, kRangedMoveRows,
                kRangedMoveCols * kRangedMoveRows,  // totalFrames = 4
                kRangedFrameDelay,
                ANIM_LOOP, 0);
            break;

        case STATE_ATTACK:
            animSystem::init(enemy.anim,
                kRangedAttackCols, kRangedAttackRows,
                kRangedAttackCols * kRangedAttackRows,  // totalFrames = 6
                kRangedFrameDelay,
                ANIM_PLAY_ONCE, 0);
            break;

        case STATE_DEATH:
            animSystem::init(enemy.anim,
                kRangedDeathCols, kRangedDeathRows,
                kRangedDeathCols * kRangedDeathRows,  // totalFrames = 4
                kRangedFrameDelay,
                ANIM_PLAY_ONCE, 0);
            break;
        }
    }

    /*!*************************************************************************
     * INIT ENEMIES
     * @brief Initializes all enemies to an inactive state with default values
     *
     * @param enemies[]    Array containing all the enemies
     * @param maxCount     Maximum number of enemies
     * @return VOID
     ***************************************************************************/
    void initEnemies(Enemy enemies[], s32 maxCount)
    {
        // loop through the array of enemies and set all properties to 0
        for (int i = 0; i < maxCount; i++)
        {
            enemies[i].shape.xPos = 0.0f;
            enemies[i].shape.yPos = 0.0f;
            enemies[i].shape.xScale = 80.0f;  // Default enemy size
            enemies[i].shape.yScale = 80.0f;
            enemies[i].type = ENEMY_MELEE;
            enemies[i].shape.velocityX = 0.0f;
            enemies[i].shape.velocityY = 0.0f;
            enemies[i].health = 0.0f;
            enemies[i].attackCooldown = 0.0f;
            enemies[i].detectionRange = 0.0f;
            enemies[i].attackRange = 0.0f;
            enemies[i].moveSpeed = 0.0f;
            enemies[i].isActive = 0; // so to inactive so it wont update/render unless instructed to
            enemies[i].hasLineOfSight = 0;

            // Zero-init the per-enemy animation state
            enemies[i].state = STATE_IDLE;
            enemies[i].facingRight = 1;  // default facing right
            animSystem::reset(enemies[i].anim);
        }
    }

    /*!*************************************************************************
     * SPAWN ENEMY
     * @brief Spawns a new enemy of a specific type at the given coordinates
     *
     * @param enemies[]    Array containing all the enemies
     * @param maxCount     Maximum number of enemies
     * @param type         Enum specifying if it is a MELEE or RANGED enemy
     * @param xPos         X coordinate to spawn the enemy at
     * @param yPos         Y coordinate to spawn the enemy at
     * @return s8          Returns 1 if spawn is successful, 0 if no slots available
     ***************************************************************************/
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

        // If no slots available(all slots active), return failure
        if (foundSlot == -1)
        {
            printf("Cannot spawn enemy: No available slots (max: %d)\n", maxCount);
            return 0;
        }

        // Initialize new enemy based on type
        enemies[foundSlot].shape.xPos = xPos;
        enemies[foundSlot].shape.yPos = yPos;
        enemies[foundSlot].type = type;
        enemies[foundSlot].shape.velocityX = 0.0f;
        enemies[foundSlot].shape.velocityY = 0.0f;
        enemies[foundSlot].attackCooldown = 0.0f;
        enemies[foundSlot].hasLineOfSight = 0;
        enemies[foundSlot].isActive = 1; // set active to update and render

        // assign stats based on enemy type
        if (type == ENEMY_MELEE)
        {
            enemies[foundSlot].health = MELEE_HEALTH;
            enemies[foundSlot].detectionRange = MELEE_DETECTION_RANGE;
            enemies[foundSlot].attackRange = MELEE_ATTACK_RANGE;
            enemies[foundSlot].moveSpeed = MELEE_MOVE_SPEED;
            enemies[foundSlot].shape.xScale = 80.0f;
            enemies[foundSlot].shape.yScale = 80.0f;
        } 
        else if (type == ENEMY_RANGED)
        {
            enemies[foundSlot].health = RANGED_HEALTH;
            enemies[foundSlot].detectionRange = RANGED_DETECTION_RANGE;
            enemies[foundSlot].attackRange = RANGED_ATTACK_RANGE;
            enemies[foundSlot].moveSpeed = RANGED_MOVE_SPEED;

            // Default ranged enemies to idle animation state
            enemies[foundSlot].facingRight = 1;
            enemies[foundSlot].state = STATE_IDLE;
            // Init with idle sheet (move sheet, frozen on frame 0)
            animSystem::init(enemies[foundSlot].anim,
                kRangedIdleCols, kRangedIdleRows,
                kRangedIdleCols * kRangedIdleRows,
                kRangedFrameDelay,
                ANIM_IDLE, 0);
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

    /*!*************************************************************************
     * HAS LINE OF SIGHT
     * @brief Checks if the enemy has a clear line of sight to the player
     *
     * @param enemy        Reference to the enemy checking line of sight
     * @param player       Reference to the player object
     * @param map[]        Array representing the collision map (unused currently)
     * @param mapX         Width of the map (unused currently)
     * @param mapY         Height of the map (unused currently)
     * @param mapS         Scale/Size of map tiles (unused currently)
     * @return s8          Returns 1 if the player is within sight, 0 otherwise
     ***************************************************************************/
    s8 hasLineOfSight(Enemy& enemy, objectsquares& player,
        int map[], int mapX, int mapY, int mapS)
    {
        map = map;
        mapX = mapX;
        mapS = mapS;
        mapY = mapY;
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
     * @param enemies[]          Array containing all the enemies
     * @param maxCount           Maximum number of enemies
     * @param player             Reference to the player to track position
     * @param enemyProjectiles[] Array for enemy projectiles
     * @param maxProjectiles     Maximum number of projectiles allowed
     * @param deltaTime          Time elapsed since last frame
     * @param attackSound        Sound to play when an enemy attacks
     * @param sfxGroup           Audio group for volume control
     * @return VOID
     ***************************************************************************/
    void updateEnemies(Enemy enemies[], s32 maxCount,
        objectsquares& player, drop loot[],
        Projectile enemyProjectiles[], s32 maxProjectiles,
        f32 deltaTime, AEAudio attackSound, AEAudioGroup sfxGroup,
        WireDrop wireDrops[], s32 wireDropMax)
    {
        // iterate through enemy slots 
        for (int i = 0; i < maxCount; i++)
        {
            // skip inactive enemies
            if (enemies[i].isActive == 0) continue;

            // Decrease attack cooldown using delta time to be framerate independent 
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

                // Calculate direction vector(dx, dy) to player
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
                    }
                    // Too far,  move closer
                    else if (distanceToPlayer > enemies[i].attackRange)
                    {
                        // direction * move speed = move towards
                        enemies[i].shape.velocityX = dx * enemies[i].moveSpeed;
                        enemies[i].shape.velocityY = dy * enemies[i].moveSpeed;
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
                                enemyProjectiles[p].shape.xScale = 20.0f;
                                enemyProjectiles[p].shape.yScale = 20.0f;

                                // set projectile velocity using normalized direction vector to player
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

            // ==============================================================
            // STATE MACHINE + ANIMATION UPDATE (ranged enemies only)
            // ==============================================================
            if (enemies[i].type == ENEMY_RANGED)
            {
                // --- Update facing direction based on horizontal velocity ---
                if (enemies[i].shape.velocityX > 0.1f)
                    enemies[i].facingRight = 1;
                else if (enemies[i].shape.velocityX < -0.1f)
                    enemies[i].facingRight = 0;

                // --- State transitions (highest priority first) ---
                if (enemies[i].health <= 0.0f)
                {
                    // Transition to death state, stop all movement
                    SetEnemyState(enemies[i], STATE_DEATH);
                    enemies[i].shape.velocityX = 0.0f;
                    enemies[i].shape.velocityY = 0.0f;

                    // Only despawn once the death animation finishes
                    if (enemies[i].anim.justFinished)
                    {
                        enemies[i].isActive = 0;
                        loot[i].info.xPos = enemies[i].shape.xPos;
                        loot[i].info.yPos = enemies[i].shape.yPos;
                        loot[i].info.flag = 1;

                        // Attempt to spawn a wire drop at the enemy's death position.
                        // The wire drop tracker handles the first-kill chance /
                        // second-kill guarantee logic internally.
                        if (wireDrops && wireDropMax > 0)
                            pickup::TrySpawnWireDrop(wireDrops, wireDropMax,
                                enemies[i].shape.xPos, enemies[i].shape.yPos);

                        printf("Enemy %d defeated!\n", i);
                    }
                }
                else if (enemies[i].attackCooldown > (RANGED_ATTACK_COOLDOWN - kRangedFrameDelay * kRangedAttackCols))
                {
                    // Enemy just fired -- play attack anim for attack-anim duration
                    SetEnemyState(enemies[i], STATE_ATTACK);
                }
                else if (fabsf(enemies[i].shape.velocityX) > 0.1f)
                {
                    SetEnemyState(enemies[i], STATE_MOVING);
                }
                else
                {
                    SetEnemyState(enemies[i], STATE_IDLE);
                }

                // Advance this enemy's individual animation each frame
                animSystem::update(enemies[i].anim, deltaTime);
            }
            // ==============================================================
            // NON-RANGED (melee) death check -- unchanged behavior
            // ==============================================================
            else if (enemies[i].health <= 0.0f)
            {
                enemies[i].isActive = 0;
                loot[i].info.xPos = enemies[i].shape.xPos;
                loot[i].info.yPos = enemies[i].shape.yPos;
                loot[i].info.flag = 1;

                // Attempt to spawn a wire drop at the melee enemy's death position.
                if (wireDrops && wireDropMax > 0)
                    pickup::TrySpawnWireDrop(wireDrops, wireDropMax,
                        enemies[i].shape.xPos, enemies[i].shape.yPos);

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
     * @param meleeMesh         VertexList containing mesh for MELEE enemy
     * @param rangedMesh        VertexList containing mesh for RANGED enemy
     * @param meleeTexture      Texture set containing texture for MELEE enemy
     * @param rangedTexture     Texture set containing texture for RANGED enemy
     * @param meleeUOffset      U offset for melee texture animation
     * @param meleeVoffset      V offset for melee texture animation
     * @return VOID
     ***************************************************************************/
     // Render all active enemies
    void renderEnemies(Enemy enemies[],
        s32 maxCount,
        AEGfxVertexList* meleeMesh,
        AEGfxVertexList* rangedMesh,
        AEGfxTexture* meleeTexture,
        AEGfxTexture* rangedTexture,
        f32 meleeUOffset,
        f32 meleeVoffset)
    {
        // loop through enemies array
        for (int i = 0; i < maxCount; i++)
        {
            if (enemies[i].isActive == 1)
            {
                // Choose texture, mesh, and UV offsets based on enemy type
                AEGfxTexture* texture = nullptr;
                AEGfxVertexList* mesh = nullptr;
                f32 uOffset = 0.0f;
                f32 vOffset = 0.0f;

                // Scale values -- may be negated for sprite flipping
                f32 renderScaleX = enemies[i].shape.xScale;
                f32 renderScaleY = enemies[i].shape.yScale;

                if (enemies[i].type == ENEMY_MELEE)
                {
                    // Melee enemies keep their original single-texture behavior
                    texture = meleeTexture;
                    mesh = meleeMesh;
                    uOffset = meleeUOffset;
                    vOffset = meleeVoffset;
                }
                else if (enemies[i].type == ENEMY_RANGED)
                {
                    // -------------------------------------------------------
                    // RANGED: pick texture + mesh based on current state
                    // -------------------------------------------------------
                    switch (enemies[i].state)
                    {
                    case STATE_ATTACK:
                        texture = AssetManager::GetTexture(TEX_RANGED_ATTACK);
                        mesh    = AssetManager::GetMesh(MESH_RANGED_ATTACK);
                        break;
                    case STATE_DEATH:
                        texture = AssetManager::GetTexture(TEX_RANGED_DEATH);
                        mesh    = AssetManager::GetMesh(MESH_RANGED_DEATH);
                        break;
                    case STATE_MOVING:
                        texture = AssetManager::GetTexture(TEX_RANGED_MOVE);
                        mesh    = AssetManager::GetMesh(MESH_RANGED_MOVE);
                        break;
                    case STATE_IDLE:
                    default:
                        // Idle reuses the move sheet frozen on frame 0
                        texture = AssetManager::GetTexture(TEX_RANGED_MOVE);
                        mesh    = AssetManager::GetMesh(MESH_RANGED_MOVE);
                        break;
                    }

                    // UV offsets come from the enemy's own SpriteAnimation
                    uOffset = animSystem::getUOffset(enemies[i].anim);
                    vOffset = animSystem::getVOffset(enemies[i].anim);

                    // Sprite flipping: if the default sprite faces RIGHT and
                    // the enemy is facing LEFT, negate X scale to mirror it.
                    if (!enemies[i].facingRight)
                    {
                        renderScaleX = -enemies[i].shape.xScale;
                    }

                    // Fallback: if state-specific assets aren't loaded yet,
                    // fall back to the legacy ranged texture passed in.
                    if (!texture)
                    {
                        texture = rangedTexture;
                        mesh    = rangedMesh;
                        uOffset = 0.0f;
                        vOffset = 0.0f;
                    }
                }
                // prevent crash if mesh isn't loaded
                if (!mesh) continue;

                // prevent crash if mesh isn't loaded
                if (!mesh) continue;

                // Texture available ==> set render mode and texture set
                if (texture)
                {
                    AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
                    AEGfxTextureSet(texture, uOffset, vOffset);
                }
                else { // no texture fall back to solid color
                    AEGfxSetRenderMode(AE_GFX_RM_COLOR);
                    AEGfxTextureSet(nullptr, 0.0f, 0.0f);
                    AEGfxSetColorToAdd(1.0f, 0.0f, 0.0f, 1.0f);
                }

                // Draw enemy -- renderScaleX may be negative for flipped sprites
                renderlogic::drawSquare(enemies[i].shape.xPos, enemies[i].shape.yPos,
                    renderScaleX, renderScaleY);
                AEGfxMeshDraw(mesh, AE_GFX_MDM_TRIANGLES);
            }
        }

        // Reset color state so subsequent draw calls are not tinted
        AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
    }

    /*!*************************************************************************
     * CHECK PLAYER ENEMY COLLISION
     * @brief Checks if there is collision between player and enemy sprites
     *
     * @param enemies[]    Array containing all the enemies
     * @param maxCount     Maximum number of enemies
     * @param player       Struct data for the player square
     * @param attackSound  Audio for attack impact
     * @param sfxGroup     Audio group for the sound
     * @return f32         Total damage done to player
     ***************************************************************************/
    f32 checkPlayerEnemyCollision(Enemy enemies[], s32 maxCount,
        objectsquares& player, AEAudio attackSound, AEAudioGroup sfxGroup)
    {
        f32 totalDamage = 0.0f;

        // For every enemy
        for (int i = 0; i < maxCount; i++)
        {
            // only active melee enemies do contact damage
            if (enemies[i].isActive == 1 && enemies[i].type == ENEMY_MELEE)
            {
                // Check collision using existing collision system
                if (gamelogic::collision(&player, &enemies[i].shape))
                {
                    // Only deal damage if enemy attack cooldown is ready
                    if (enemies[i].attackCooldown <= 0.0f)
                    {
                        // Try to apply damage through the central health system.
                        // PlayerTakeDamage checks the player's invincibility timer.
                        int meleeDmg = static_cast<int>(MELEE_DAMAGE);
                        if (PlayerTakeDamage(player, meleeDmg))
                        {
                            AEAudioPlay(attackSound, sfxGroup, 1.0f, 1.0f, 0);
                            totalDamage += MELEE_DAMAGE;

                            //reset the attack cooldown, to prevent damage every frame
                            enemies[i].attackCooldown = MELEE_ATTACK_COOLDOWN;
                        }
                    }
                }
            }
        }
        return totalDamage;
    }

    /*!*************************************************************************
     * CHECK PROJECTILE ENEMY COLLISION
     * @brief Checks if player projectile hits enemy
     *
     * @param enemies[]            Array containing all the enemies data
     * @param maxCount             Maximum number of enemies
     * @param projectiles[]        Struct data each player projectile
     * @param maxProjectiles       Maximum number of projectiles allowed
     * @return VOID
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
                            break;  // break, so only one projectile hits one enemy
                        }
                    }
                }
            }
        }
    }

    /*!*************************************************************************
     * CHECK ENEMY PLAYER PROJECTILE COLLISION
     * @brief Checks whether any enemy projectile has hit the player
     *
     * @param enemyprojectiles[]   Array containing all enemy projectiles
     * @param maxProjectiles       Maximum number of enemy projectiles
     * @param player               Reference to the player object
     * @return f32                 Total damage dealt to the player
     ***************************************************************************/
    f32 checkEnemyPlayerProjectileCollision(Projectile enemyprojectiles[], s32 maxProjectiles, objectsquares& player)
    {
        f32 totalDamage = 0.0f;

        for (int i{}; i < maxProjectiles; ++i)
        {
            if (enemyprojectiles[i].isActive == 1)
            {
                // check if there is a collision between projectile and player
                if (gamelogic::collision(&enemyprojectiles[i].shape, &player))
                {
                    int rangedDmg = static_cast<int>(RANGED_DAMAGE);
                    if (PlayerTakeDamage(player, rangedDmg))
                    {
                        totalDamage += RANGED_DAMAGE;
                    }
                    // Destroy the projectile on contact regardless of invincibility
                    enemyprojectiles[i].isActive = 0;
                }
            }
        }
        return totalDamage;
    }
}