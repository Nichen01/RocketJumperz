#pragma once
/* Start Header ************************************************************************/
/*!
\file		  enemy.h
\author       Nicholas Chen (c.chen)
\date         January, 31, 2026
\brief        Enemy system with melee and ranged enemy types

Copyright (C) 2026 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#include "AEEngine.h"
#include "player.h"
#include "projectile.h"

// Maximum number of enemies that can exist at once
#define MAX_ENEMIES 10

// Enemy types
enum EnemyType {
    ENEMY_MELEE,   // Chases player and damages on contact
    ENEMY_RANGED   // Shoots projectiles at player when in range
};

// Enemy structure
struct Enemy {
    objectsquares shape;     // Position, size, and collision data
    EnemyType type;          // Type of enemy (melee or ranged)
    f32 velocityX;           // Current X velocity
    f32 velocityY;           // Current Y velocity
    f32 health;              // Enemy health points
    f32 attackCooldown;      // Time until enemy can attack again
    f32 detectionRange;      // How far enemy can detect player
    f32 attackRange;         // How close enemy needs to be to attack
    f32 moveSpeed;           // How fast enemy moves
    s8 isActive;             // Is this enemy slot active?
    s8 hasLineOfSight;       // Can enemy see player?
};

namespace enemySystem {
    // Constants for enemy behavior
    constexpr f32 MELEE_DETECTION_RANGE = 900.0f;
    constexpr f32 MELEE_ATTACK_RANGE = 50.0f;
    constexpr f32 MELEE_MOVE_SPEED = 4.0f;
    constexpr f32 MELEE_HEALTH = 50.0f;
    constexpr f32 MELEE_ATTACK_COOLDOWN = 1.0f;  // Seconds between attacks
    constexpr f32 MELEE_DAMAGE = 10.0f;

    constexpr f32 RANGED_DETECTION_RANGE = 800.0f;
    constexpr f32 RANGED_ATTACK_RANGE = 800.0f;
    constexpr f32 RANGED_MOVE_SPEED = 1.5f;
    constexpr f32 RANGED_HEALTH = 30.0f;
    constexpr f32 RANGED_ATTACK_COOLDOWN = 2.0f;  // Seconds between shots
    constexpr f32 RANGED_DAMAGE = 5.0f;

    // Initialize all enemies to inactive state
    void initEnemies(Enemy enemies[], s32 maxCount);

    // Spawn a new enemy at specified position
    // Returns true if spawn successful, false if no slots available
    s8 spawnEnemy(Enemy enemies[], s32 maxCount,
        EnemyType type, f32 xPos, f32 yPos);

    // Update all active enemies (AI, movement, attacks)
    void updateEnemies(Enemy enemies[], s32 maxCount,
        objectsquares& player,
        Projectile enemyProjectiles[], s32 maxProjectiles,
        f32 deltaTime, AEAudio attackSound, AEAudioGroup sfxGroup);

    // Render all active enemies
    void renderEnemies(Enemy enemies[], s32 maxCount,
        AEGfxVertexList* mesh,
        AEGfxTexture* meleeTexture,
        AEGfxTexture* rangedTexture);

    // Check if player collided with any melee enemies
    // Returns damage dealt to player
    f32 checkPlayerEnemyCollision(Enemy enemies[], s32 maxCount,
        objectsquares& player, AEAudio attackSound, AEAudioGroup sfxGroup);

    // Check if player projectiles hit any enemies
    void checkProjectileEnemyCollision(Enemy enemies[], s32 maxCount,
        Projectile projectiles[], s32 maxProjectiles);

    // Helper: Calculate distance between two points
    f32 getDistance(f32 x1, f32 y1, f32 x2, f32 y2);

    // Helper: Check if enemy has line of sight to player (no walls blocking)
    s8 hasLineOfSight(Enemy& enemy, objectsquares& player,
        int map[], int mapX, int mapY, int mapS);
}