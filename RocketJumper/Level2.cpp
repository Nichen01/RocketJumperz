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

#include "pch.h"
#include "Level2.h"


static s32* map = nullptr;
static int x = 16;
static int y = 9;
static int s = 80;

objectsquares objectinfo2[2] = { 0 };

static Projectile Projectiles[MAX_PROJECTILES];

// ENEMY DATA
static Enemy enemies[MAX_ENEMIES];
static Projectile enemyProjectiles[MAX_PROJECTILES];
static AEGfxTexture* meleeEnemyTexture = nullptr;
static AEGfxTexture* rangedEnemyTexture = nullptr;

// Font for HUD text (must be destroyed in Unload to avoid leak)
static s8 font = -1;

//==== sound and volume

static f32 bgVolume = 1.f;
static f32 bgmPitch = 1.f;

static AEAudio L2;
static AEAudio LaserBlast;
static AEAudio Punch;
static AEAudioGroup bgm;
static AEAudioGroup soundEffects;

// Font resource (must be destroyed in Unload to avoid leak)
static s8 fontLevel2 = -1;


// Note: characterPictest, base5test, and pMesh are defined in draw.cpp. access them through draw.h

void Level2_Load()
{
	
	// Load the music file once when the level loads
	L2 = AEAudioLoadMusic("Assets/Sounds/L1_bgm.mp3");
	// Create the audio group
	bgm = AEAudioCreateGroup();
	// Configure sound effects
	LaserBlast = AEAudioLoadSound("Assets/Sounds/LaserBlast.mp3");
	Punch = AEAudioLoadSound("Assets/Sounds/Punch.wav");
	soundEffects = AEAudioCreateGroup();
	
	// Load platform assets
	load::platform();

	// Load textures - these are defined in draw.cpp
	characterPictest = AEGfxTextureLoad("Assets/astronautRight.png");
	plasma = AEGfxTextureLoad("Assets/plasma.png");
}

void Level2_Initialize()
{
	AEAudioPlay(L2, bgm, 0.5f, 1.f, -1);

	// Create font for HUD text (stored so we can destroy it in Unload)
	font = AEGfxCreateFont("Assets/Fonts/gameover.ttf", 72);

	// Load textures, defined in draw.cpp
	characterPictest = AEGfxTextureLoad("Assets/astronautRight.png");
	plasma = AEGfxTextureLoad("Assets/plasma.png");

	// Initialize player movement system
	movement::initPlayerMovement(objectinfo2[player]);

	// Added after obstacle initialization:
	projectileSystem::initProjectiles(Projectiles, MAX_PROJECTILES);

	//=============CREATE TEXTURED MESH FOR WALLS==================//
	// This mesh is used by draw.cpp for rendering walls
	init::enemy();
	init::platform();
	init::player();
	init::projectile();

	if (!ImportMapDataFromFile("Assets/Map/Level2_Map.txt", 2)) {
		printf("Could not import file");
		return;
	}

	x = BINARY_MAP_WIDTH;
	y = BINARY_MAP_HEIGHT;

	map = new s32[x * y]{ 0 };

	for (int row{}; row < y; ++row) {
		for (int col{}; col < x; col++) {
			map[row * x + col] = MapData[row][col];
		}
	}

	objectinfo2[player].xPos = 0.0f;
	objectinfo2[player].yPos = 0.0f;
	objectinfo2[player].xScale = 60.0f;
	objectinfo2[player].yScale = 60.0f;

	// Initialize player health to 100 HP with no invincibility active
	InitPlayerHealth(objectinfo2[player]);

	objectinfo2[obstacle].xPos = -400.0f;
	objectinfo2[obstacle].yPos = 0.0f;
	objectinfo2[obstacle].xScale = 100.0f;
	objectinfo2[obstacle].yScale = 400.0f;

	//======== INIT ENEMIES DATA =======================//
	// Initialize enemy system
	enemySystem::initEnemies(enemies, MAX_ENEMIES);
	projectileSystem::initProjectiles(enemyProjectiles, MAX_PROJECTILES);

	// Load enemy textures (create these assets or use placeholder)
	meleeEnemyTexture = AEGfxTextureLoad("Assets/MeleeEnemy.png");
	rangedEnemyTexture = AEGfxTextureLoad("Assets/RangedEnemy.png");

	// SPAWN test enemies
	enemySystem::spawnEnemy(enemies, MAX_ENEMIES, ENEMY_RANGED, -200.0f, 100.0f);
	enemySystem::spawnEnemy(enemies, MAX_ENEMIES, ENEMY_RANGED, 300.0f, 50.0f);
	enemySystem::spawnEnemy(enemies, MAX_ENEMIES, ENEMY_MELEE, 300.0f, -200.0f);

	// DOOR
	animSystem::buildMesh(&doorMesh, doorFrameCount);
	doorTex = AEGfxTextureLoad("Assets/DoorOpen.png");
	if (!doorTex)
		printf("DOOR TEXTURE NOT FOUND!\n");
	else
		printf("DOOR OK\n");

	animSystem::init(doorAnim, doorFrameCount, doorFrameDelay, ANIM_IDLE, 0);
	doorIsOpen = false;

}

void Level2_Update()
{

	if (AEInputCheckTriggered(AEVK_L)) next = GS_LEVELEDITOR;
	//====== AUDIO CONTROLS ======
	if (AEInputCheckTriggered(AEVK_1)) {
		bgVolume -= 0.1f;
		if (bgVolume <= 0.f)
			bgVolume = 0.f;
		AEAudioSetGroupVolume(bgm, bgVolume);
	}
	if (AEInputCheckTriggered(AEVK_2)) {
		bgVolume += 0.1f;
		if (bgVolume <= 0.f)
			bgVolume = 0.f;
		AEAudioSetGroupVolume(bgm, bgVolume);
	}
	
	//=============== GET MOUSE INPUTS =====================//
	s32 mouseX, mouseY;
	AEInputGetCursorPosition(&mouseX, &mouseY);

	// Convert screen coordinates to world coordinates
	f32 worldMouseX = static_cast<f32>(mouseX) - static_cast<f32>(screenWidth / 2);
	f32 worldMouseY = static_cast<f32>(screenLength / 2) - static_cast<f32>(mouseY);

	//========== JETPACK MOVEMENT SYSTEM ===============//
	//Apply thrust when spacebar is pressed
	movement::physicsInput(objectinfo2[player]);

	if (AEInputCheckTriggered(AEVK_Q) || AEInputCheckTriggered(AEVK_ESCAPE)) {
		next = GS_QUIT;
	}

	//===========  APPLY PHYSICS(DRAG)===================//
	// Update player physics (drag + position)
	movement::updatePlayerPhysics(objectinfo2[player]);
	//===================================================//

	// ========== PROJECTILE SYSTEM UPDATE =============//
	projectileSystem::fireProjectiles(
		static_cast<s32>(worldMouseX),
		static_cast<s32>(worldMouseY),
		objectinfo2[player],
		Projectiles,
		MAX_PROJECTILES);

	// Update all active projectiles
	projectileSystem::UpdateProjectiles(Projectiles, MAX_PROJECTILES);



	//============= UPDATE ENEMIES ===================/
	// Get delta time for enemy AI
	f32 dt = static_cast<f32>(AEFrameRateControllerGetFrameTime());

	// Update enemies
	enemySystem::updateEnemies(enemies, MAX_ENEMIES,
		objectinfo2[player],
		enemyProjectiles, MAX_PROJECTILES,
		dt, LaserBlast, soundEffects);

	// Update enemy projectiles
	projectileSystem::UpdateProjectiles(enemyProjectiles, MAX_PROJECTILES);

	// Tick down the player's invincibility timer each frame
	UpdatePlayerInvincibility(objectinfo2[player], dt);

	// Check player projectiles hitting enemies
	enemySystem::checkProjectileEnemyCollision(enemies, MAX_ENEMIES,
		Projectiles, MAX_PROJECTILES);

	// Check melee enemies damaging player (uses PlayerTakeDamage internally)
	enemySystem::checkPlayerEnemyCollision(enemies, MAX_ENEMIES,
		objectinfo2[player], Punch, soundEffects);

	// Check ranged enemy projectiles hitting player (uses PlayerTakeDamage internally)
	enemySystem::checkEnemyPlayerProjectileCollision(
		enemyProjectiles, MAX_PROJECTILES, objectinfo2[player]);
	gamelogic::OBJ_to_map(map, x, s, &enemies[0].shape, 1);
	gamelogic::OBJ_to_map(map, x, s, &enemies[1].shape, 1);
	gamelogic::OBJ_to_map(map, x, s, &enemies[2].shape, 1);
	gamelogic::OBJ_to_map(map, x, s, &objectinfo2[player], 1);

}

void Level2_Draw()
{
	AEGfxSetBackgroundColor(0.2f, 0.2f, 0.3f);  // Dark blue-gray background

	// Setup for textured rendering
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(1.0f);

	// Draw map walls and floor
	renderlogic::drawMapWallFloor(map, x, y, s);

	// ==== ENEMIES RENDER =======//
	// Render enemies
	enemySystem::renderEnemies(enemies, MAX_ENEMIES, enemyMesh,
		meleeEnemyTexture, rangedEnemyTexture);

	// Render enemy projectiles (red color)
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);  // Changed from COLOR to TEXTURE
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
	projectileSystem::renderProjectiles(enemyProjectiles, MAX_PROJECTILES, plasma, projectileMesh);

	//====== PLAYER RENDER =========//
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxTextureSet(characterPictest, 0, 0);
	renderlogic::drawSquare(objectinfo2[player].xPos, objectinfo2[player].yPos,
		objectinfo2[player].xScale, objectinfo2[player].yScale);
	AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);

	// Render all active projectiles (YELLOW)
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
	projectileSystem::renderProjectiles(Projectiles, MAX_PROJECTILES, plasma, projectileMesh);

	// ====== HUD: Player Health Display ======
	if (font >= 0)
	{
		char healthText[32];
		snprintf(healthText, sizeof(healthText), "Health: %d", objectinfo2[player].health);

		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
		AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);

		AEGfxPrint(font, healthText, -0.95f, 0.85f, 0.8f, 1.0f, 1.0f, 1.0f, 1.0f);
	}

	renderlogic::drawTileArray();
}

void Level2_Free()
{
	freeAsset::platform();
	freeAsset::door();
	freeAsset::enemy();
	freeAsset::player();
	freeAsset::projectile();

	if (map) {
		delete[] map;
		map = nullptr;
	}
}

void Level2_Unload()
{
	// Unload ALL textures that were loaded in Initialize
	if (characterPictest) { AEGfxTextureUnload(characterPictest); characterPictest = nullptr; }
	if (plasma) { AEGfxTextureUnload(plasma); plasma = nullptr; }
	unload::platform();
	if (meleeEnemyTexture) { AEGfxTextureUnload(meleeEnemyTexture); meleeEnemyTexture = nullptr; }
	if (rangedEnemyTexture) { AEGfxTextureUnload(rangedEnemyTexture); rangedEnemyTexture = nullptr; }

	// Destroy the font created in Initialize
	if (fontLevel2 != -1) { AEGfxDestroyFont(fontLevel2); fontLevel2 = -1; }

	// Unload ALL audio resources that were loaded in Load
	AEAudioUnloadAudio(L2);
	AEAudioUnloadAudio(LaserBlast);
	AEAudioUnloadAudio(Punch);
	AEAudioUnloadAudioGroup(bgm);
	AEAudioUnloadAudioGroup(soundEffects);
}