/* Start Header ************************************************************************/
/*!
\file		  Level1.cpp
\author       Ivan Chong, i.chong, 2503476
\par          i.chong@digipen.edu
\date         January, 16, 2026
\brief        Contain functions called by GameStateManager.cpp

Copyright (C) 2026 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#include <iostream>
#include "Level1.h"
#include "draw.h"
#include "collision.h"
#include "player.h"
#include "GameStateManager.h"
#include "projectile.h"
#include "Movement.h"
#include "render.h"
#include "enemies.h"
#include "binaryMap.h"

static s32* map = nullptr;
static int mapX{};
static int mapY{};
static int s = 100;

static objectsquares objectinfo[2] = { 0 };

// Local variables for projectile test level
static Projectile Projectiles[MAX_PROJECTILES];
static AEGfxVertexList* pTestMesh = nullptr;

// ENEMY DATA
static Enemy enemies[MAX_ENEMIES];
static Projectile enemyProjectiles[MAX_PROJECTILES];
static AEGfxTexture* meleeEnemyTexture = nullptr;
static AEGfxTexture* rangedEnemyTexture = nullptr;
static f32 playerHealth = 100.0f;


static f32 bgm_volume = 1.f;
static f32 bgm_pitch = 1.f;

static AEAudio L1;
static AEAudio LaserBlast;
static AEAudio Punch;
static AEAudioGroup bgm;
static AEAudioGroup soundEffects;

// Note: characterPictest, base5test, and pMesh are defined in draw.cpp. access them through draw.h

void Level1_Load()
{
	// Load the music file once when the level loads
	L1 = AEAudioLoadMusic("Assets/Sounds/L1_bgm.mp3");

	// Create the audio group
	bgm = AEAudioCreateGroup();

	// Configure sound effects
	LaserBlast = AEAudioLoadSound("Assets/Sounds/LaserBlast.mp3");
	Punch = AEAudioLoadSound("Assets/Sounds/Punch.wav");
	soundEffects = AEAudioCreateGroup();   // short for 'sound effect'

}

void Level1_Initialize()
{
	AEAudioPlay(L1, bgm, 0.5f, 1.f, -1);


	// Load textures - these are defined in draw.cpp
	characterPictest = AEGfxTextureLoad("Assets/CharacterRight.png");
	base5test = AEGfxTextureLoad("Assets/Base5.png");

	

	// Initialize player movement system
	movement::initPlayerMovement(objectinfo[player]);

	// Added after obstacle initialization:
	projectileSystem::initProjectiles(Projectiles, MAX_PROJECTILES);

	//=============CREATE TEXTURED MESH FOR WALLS==================//
	// This mesh is used by draw.cpp for rendering walls
	AEGfxMeshStart();
	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFFFFFFFF, 0.0f, 1.0f,
		0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f,
		-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f);

	AEGfxTriAdd(
		0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f,
		0.5f, 0.5f, 0xFFFFFFFF, 1.0f, 0.0f,
		-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f);
	pMesh = AEGfxMeshEnd();

	//=============CREATE SQUARE MESH FOR PROJECTILES==================//
	AEGfxMeshStart();
	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFFFFFFFF, 0.0f, 1.0f,
		0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f,
		-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f);
	AEGfxTriAdd(
		0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f,
		0.5f, 0.5f, 0xFFFFFFFF, 1.0f, 0.0f,
		-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f);
	pTestMesh = AEGfxMeshEnd();

	if (!ImportMapDataFromFile("Level1_Map.txt")) {
		printf("Could not import file");
		return;
	}

	mapX = BINARY_MAP_WIDTH;
	mapY = BINARY_MAP_HEIGHT;

	map = new s32[mapX * mapY];

	for (int row{}; row < mapY; ++row) {
		for (int col{}; col < mapX; col++) {
			map[row * mapX + col] = MapData[row][col];
		}
	}

	//objectinfo[player].xPos = 0.0f;
	//objectinfo[player].yPos = 0.0f;
	//objectinfo[player].xScale = 70.0f;
	//objectinfo[player].yScale = 70.0f;

	//objectinfo[obstacle].xPos = -400.0f;
	//objectinfo[obstacle].yPos = 0.0f;
	//objectinfo[obstacle].xScale = 100.0f;
	//objectinfo[obstacle].yScale = 400.0f;

	if (firstTimeLevel1) {
		objectinfo[player].xPos = 0.0f;
		objectinfo[player].yPos = 0.0f;
		firstTimeLevel1 = false;
	}
	else {
		objectinfo[player].xPos = 600.f;
		objectinfo[player].yPos = -300.f;
	}

	objectinfo[player].xScale = 70.0f;
	objectinfo[player].yScale = 70.0f;

	//======== INIT ENEMIES DATA =======================//
	// Initialize enemy system
	enemySystem::initEnemies(enemies, MAX_ENEMIES);
	projectileSystem::initProjectiles(enemyProjectiles, MAX_PROJECTILES);

	// Load enemy textures (create these assets or use placeholder)
	meleeEnemyTexture = AEGfxTextureLoad("Assets/MeleeEnemy.png");
	rangedEnemyTexture = AEGfxTextureLoad("Assets/RangedEnemy.png");

	// SPAWN test enemies
	enemySystem::spawnEnemy(enemies, MAX_ENEMIES, ENEMY_MELEE, -200.0f, 100.0f);
	enemySystem::spawnEnemy(enemies, MAX_ENEMIES, ENEMY_RANGED, 300.0f, -100.0f);
}

void Level1_Update()
{
	//=============== GET MOUSE INPUTS =====================//
	s32 mouseX, mouseY;
	AEInputGetCursorPosition(&mouseX, &mouseY);

	// Convert screen coordinates to world coordinates
	f32 worldMouseX = static_cast<f32>(mouseX) - static_cast<f32>(screenWidth / 2);
	f32 worldMouseY = static_cast<f32>(screenLength / 2) - static_cast<f32>(mouseY);

	//========== JETPACK MOVEMENT SYSTEM ===============//
	//Apply thrust when spacebar is pressed
	movement::physicsInput(objectinfo[player]);

	//===========  APPLY PHYSICS(DRAG)===================//
	// Update player physics (drag + position)
	movement::updatePlayerPhysics(objectinfo[player]);
	//===================================================//

	// ========== PROJECTILE SYSTEM UPDATE =============//
	projectileSystem::fireProjectiles(
		static_cast<s32>(worldMouseX),
		static_cast<s32>(worldMouseY),
		objectinfo[player],
		Projectiles,
		MAX_PROJECTILES);

	// Update all active projectiles
	projectileSystem::UpdateProjectiles(Projectiles, MAX_PROJECTILES);

	

	//============= UPDATE ENEMIES ===================/
	// Get delta time for enemy AI
	f32 dt = static_cast<f32>(AEFrameRateControllerGetFrameTime());

	// Update enemies
	enemySystem::updateEnemies(enemies, MAX_ENEMIES,
		objectinfo[player],
		enemyProjectiles, MAX_PROJECTILES,
		dt, LaserBlast, soundEffects);

	// Update enemy projectiles
	projectileSystem::UpdateProjectiles(enemyProjectiles, MAX_PROJECTILES);

	

	// Check player projectiles hitting enemies
	enemySystem::checkProjectileEnemyCollision(enemies, MAX_ENEMIES,
		Projectiles, MAX_PROJECTILES);

	// Check enemies damaging player
	f32 damageTaken = enemySystem::checkPlayerEnemyCollision(enemies, MAX_ENEMIES,
		objectinfo[player], Punch, soundEffects);
	if (damageTaken > 0.0f) {
		playerHealth -= damageTaken;
		printf("Player Health: %.1f\n", playerHealth);
	}
	gamelogic::OBJ_to_map(map, mapX, s, &enemies[0].shape,1);
	gamelogic::OBJ_to_map(map, mapX, s, &enemies[1].shape,1);
	gamelogic::OBJ_to_map(map, mapX, s, &objectinfo[player],1);

}

void Level1_Draw()
{
	AEGfxSetBackgroundColor(0.2f, 0.2f, 0.3f);  // Dark blue-gray background

	// Setup for textured rendering
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(1.0f);

	// Draw map walls and floor
	renderlogic::drawmap_Wall_floor(map, mapX, mapY, s);

	// ==== ENEMIES RENDER =======//
	// Render enemies
	enemySystem::renderEnemies(enemies, MAX_ENEMIES, pTestMesh,
		meleeEnemyTexture, rangedEnemyTexture);

	// Render enemy projectiles (red color)
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	AEGfxSetColorToAdd(1.0f, 0.0f, 0.0f, 1.0f);  // Red for enemy projectiles
	projectileSystem::renderProjectiles(enemyProjectiles, MAX_PROJECTILES, pTestMesh);

	//====== PLAYER RENDER =========//
	AEGfxTextureSet(characterPictest, 0, 0);
	renderlogic::Drawsquare(objectinfo[player].xPos, objectinfo[player].yPos,
		objectinfo[player].xScale, objectinfo[player].yScale);
	AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);

	// Render all active projectiles (YELLOW)
	AEGfxSetRenderMode(AE_GFX_RM_COLOR);
	AEGfxSetColorToAdd(1.0f, 1.0f, 0.0f, 1.0f);  // Yellow
	projectileSystem::renderProjectiles(Projectiles, MAX_PROJECTILES, pTestMesh);
}

void Level1_Free()
{
	if (pMesh) {
		AEGfxMeshFree(pMesh);
		pMesh = nullptr;
	}

	if (pTestMesh) {
		AEGfxMeshFree(pTestMesh);
		pTestMesh = nullptr;
	}

	delete[] map;
	map = nullptr;

	FreeMapData();
}

void Level1_Unload()
{
	if (characterPictest) AEGfxTextureUnload(characterPictest);
	if (base5test) AEGfxTextureUnload(base5test);
	if (meleeEnemyTexture) AEGfxTextureUnload(meleeEnemyTexture);
	if (rangedEnemyTexture) AEGfxTextureUnload(rangedEnemyTexture);

	AEAudioUnloadAudio(L1);
	AEAudioUnloadAudioGroup(bgm);
}