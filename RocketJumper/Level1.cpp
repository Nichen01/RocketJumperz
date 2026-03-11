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
#include "Level1.h"
#include "draw.h"
#include "collision.h"
#include "player.h"
#include "GameStateManager.h"
#include "GameStateList.h"
#include "projectile.h"
#include "Movement.h"
// render.h was removed in Joraye merge; Load.h (via Level1.h) provides
// load::platform() and unload::platform() which replaced render:: functions.
#include "enemies.h"
#include "binaryMap.h"
#include "animation.h"
#include "AssetManager.h"

static s32* map = nullptr;
static int x;
static int y;
static int s = 80;

extern objectsquares objectinfo[2] = { 0 };

// Local variables for projectile test level
static Projectile Projectiles[MAX_PROJECTILES];

// ENEMY DATA
static Enemy enemies[MAX_ENEMIES];
static Projectile enemyProjectiles[MAX_PROJECTILES];

// Mushroom animation state (mesh/textures owned by AssetManager)
static SpriteAnimation meleeAnim;

// Base texture loaded via AssetManager (no extern in draw.h, Level1-only)
static AEGfxTexture* base5test = nullptr;

// Font handle for in-game text -- UNUSED, kept for reference only.
// The active font handle is fontLevel1 (declared below), which is
// created in Initialize and destroyed in Unload.

/*
static AEGfxTexture* mushroomDieTexture[9] = { nullptr };
static AEGfxTexture* mushroomHitTexture[5] = { nullptr };
static AEGfxTexture* mushroomIdleTexture[9] = { nullptr };
*/

//==== sound and volume
static f32 bgVolume = 1.f;

static AEAudio L1;
static AEAudio LaserBlast;
static AEAudio Punch;
static AEAudioGroup bgm;
static AEAudioGroup soundEffects;

// Door variables (doorX, doorY, doorAnim, doorMesh, doorIsOpen, doorTex)
// are defined in draw.cpp and declared extern in draw.h.
// The constants below are Level1-specific door animation parameters.
static s32  DOOR_FRAME_COUNT = 7;
static f32  DOOR_FRAME_DELAY = 0.08f;   // ~12 fps

// Font resource (must be destroyed in Unload to avoid leak)
static s8 fontLevel1 = -1;

static bool playerNear;

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
	//soundEffects = AEAudioCreateGroup();
	soundEffects = AEAudioCreateGroup();   // short for 'sound effect'

	// Load platform tile textures
	load::platform();

	// Load UI textures (eButton used by flashing door prompt in Draw)
	load::ui();

	// Load textures via AssetManager (prevents duplicate loads across level reloads)
	characterPictest = AssetManager::LoadTexture("characterPictest", "Assets/astronautRight.png");
	base5test        = AssetManager::LoadTexture("base5test",        "Assets/Base5.png");
	plasma           = AssetManager::LoadTexture("plasma",           "Assets/plasma.png");
	doorTex          = AssetManager::LoadTexture("doorTex",          "Assets/DoorOpen.png");
	AssetManager::LoadTexture("meleeEnemy",  "Assets/Enemy/MushroomIdle/MushroomIdleSheet.png");
	AssetManager::LoadTexture("rangedEnemy", "Assets/RangedEnemy.png");

	// Loading of assets for mushroomDie
	mushroomDieTexture[0] = AEGfxTextureLoad("Assets/Enemy/MushroomDie/MushroomDie0.png");
	mushroomDieTexture[1] = AEGfxTextureLoad("Assets/Enemy/MushroomDie/MushroomDie1.png");
	mushroomDieTexture[2] = AEGfxTextureLoad("Assets/Enemy/MushroomDie/MushroomDie2.png");
	mushroomDieTexture[3] = AEGfxTextureLoad("Assets/Enemy/MushroomDie/MushroomDie3.png");
	mushroomDieTexture[4] = AEGfxTextureLoad("Assets/Enemy/MushroomDie/MushroomDie4.png");
	mushroomDieTexture[5] = AEGfxTextureLoad("Assets/Enemy/MushroomDie/MushroomDie5.png");
	mushroomDieTexture[6] = AEGfxTextureLoad("Assets/Enemy/MushroomDie/MushroomDie6.png");
	mushroomDieTexture[7] = AEGfxTextureLoad("Assets/Enemy/MushroomDie/MushroomDie7.png");
	mushroomDieTexture[8] = AEGfxTextureLoad("Assets/Enemy/MushroomDie/MushroomDie8.png");

	// Loading of assets for mushroomHit
	mushroomHitTexture[0] = AEGfxTextureLoad("Assets/Enemy/MushroomHit/MushroomHit0.png");
	mushroomHitTexture[1] = AEGfxTextureLoad("Assets/Enemy/MushroomHit/MushroomHit1.png");
	mushroomHitTexture[2] = AEGfxTextureLoad("Assets/Enemy/MushroomHit/MushroomHit2.png");
	mushroomHitTexture[3] = AEGfxTextureLoad("Assets/Enemy/MushroomHit/MushroomHit3.png");
	mushroomHitTexture[4] = AEGfxTextureLoad("Assets/Enemy/MushroomHit/MushroomHit4.png");

	// Loading of assets for mushroomIdle
	mushroomIdleTexture[0] = AEGfxTextureLoad("Assets/Enemy/MushroomIdle/MushroomIdle0.png");
	mushroomIdleTexture[1] = AEGfxTextureLoad("Assets/Enemy/MushroomIdle/MushroomIdle1.png");
	mushroomIdleTexture[2] = AEGfxTextureLoad("Assets/Enemy/MushroomIdle/MushroomIdle2.png");
	mushroomIdleTexture[3] = AEGfxTextureLoad("Assets/Enemy/MushroomIdle/MushroomIdle3.png");
	mushroomIdleTexture[4] = AEGfxTextureLoad("Assets/Enemy/MushroomIdle/MushroomIdle4.png");
	mushroomIdleTexture[5] = AEGfxTextureLoad("Assets/Enemy/MushroomIdle/MushroomIdle5.png");
	mushroomIdleTexture[6] = AEGfxTextureLoad("Assets/Enemy/MushroomIdle/MushroomIdle6.png");
	/*mushroomIdleTexture[7] = AEGfxTextureLoad("Assets/Enemy/MushroomIdle/MushroomIdle7.png");
	mushroomIdleTexture[8] = AEGfxTextureLoad("Assets/Enemy/MushroomIdle/MushroomIdle8.png");*/

	// Create font for gameover text (stored so we can destroy it in Unload)
	fontLevel1 = AEGfxCreateFont("Assets/Fonts/gameover.ttf", 72);
}

void Level1_Initialize()
{
	currentGameLevel = 1;

	AEAudioPlay(L1, bgm, 0.5f, 1.f, -1);

	// Create font for HUD text (stored so we can destroy it in Unload)
	fontLevel1 = AEGfxCreateFont("Assets/Fonts/gameover.ttf", 72);

	// Initialize player movement system
	movement::initPlayerMovement(objectinfo[player]);

	// Added after obstacle initialization:
	projectileSystem::initProjectiles(Projectiles, MAX_PROJECTILES);

	//=============CREATE TEXTURED MESH FOR PLAYER==================//
	// A full quad (2 triangles) is required for the player sprite to render
	// correctly.  The previous code was missing AEGfxMeshStart() and only
	// had one triangle, which caused the player to appear as a half-sprite.
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
	AssetManager::StoreMesh("pMesh", pMesh);

	AEGfxMeshStart();
	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFFFFFFFF, 0.0f, 1.0f,
		0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f,
		-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f);

	AEGfxTriAdd(
		0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f,
		0.5f, 0.5f, 0xFFFFFFFF, 1.0f, 0.0f,
		-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f);
	platformMesh = AEGfxMeshEnd();
	AssetManager::StoreMesh("platformMesh", platformMesh);

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
	AssetManager::StoreMesh("pTestMesh", AEGfxMeshEnd());

	if (!ImportMapDataFromFile("Assets/Map/Level1_Map.txt")) {
		printf("Could not import file");
		return;
	}

	x = BINARY_MAP_WIDTH;
	y = BINARY_MAP_HEIGHT;

	map = new s32[x * y]{ 0 };

	for (int row{}; row < y; ++row) {
		for (int col{}; col < x; col++) {
			map[row * x + col] = BinaryCollisionArray[row][col];
		}
	}

	// Spawn player at the door they came from
	bool spawnSet = false;
	for (auto& door : doors) {
		if (door.id == playerEnteredDoorId) {
			objectinfo[player].xPos = door.worldX;
			objectinfo[player].yPos = door.worldY + 40.f; // slight offset so player isn't inside door
			spawnSet = true;
			break;
		}
	}
	// fallback if no door found (first time loading)
	if (!spawnSet) {
		objectinfo[player].xPos = 0.f;
		objectinfo[player].yPos = 0.f;
	}
	objectinfo[player].xScale = 60.0f;
	objectinfo[player].yScale = 60.0f;

	// Initialize player health to 100 HP with no invincibility active
	InitPlayerHealth(objectinfo[player]);

	//======== INIT ENEMIES DATA =======================//
	// Initialize enemy system
	enemySystem::initEnemies(enemies, MAX_ENEMIES);
	projectileSystem::initProjectiles(enemyProjectiles, MAX_PROJECTILES);



	// SPAWN test enemies
	enemySystem::spawnEnemy(enemies, MAX_ENEMIES, ENEMY_MELEE, -200.0f, 100.0f);
	enemySystem::spawnEnemy(enemies, MAX_ENEMIES, ENEMY_RANGED, 300.0f, -100.0f);

	//MUSHROOM ANIM TEST
	{
		AEGfxVertexList* meleeEnemyMesh = nullptr;
		animSystem::buildMesh(&meleeEnemyMesh, 2, 3);
		AssetManager::StoreMesh("meleeEnemyMesh", meleeEnemyMesh);
	}
	animSystem::init(meleeAnim, 3, 2, 6, 0.1f, ANIM_LOOP, 0);

	// DOOR
	animSystem::buildMesh(&doorMesh, 1, 7);
	AssetManager::StoreMesh("doorMesh", doorMesh);
	
	if (!doorTex)
		printf("DOOR TEXTURE NOT FOUND!\n");
	else
		printf("DOOR OK\n");

	animSystem::init(doorAnim, 7, 1, DOOR_FRAME_COUNT, DOOR_FRAME_DELAY, ANIM_IDLE, 0);
	doorIsOpen = false;
}

void Level1_Update()
{
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
	movement::physicsInput(objectinfo[player]);

	if (AEInputCheckTriggered(AEVK_Q)|| AEInputCheckTriggered(AEVK_ESCAPE)) {
		next = GS_QUIT;
	}

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

	// Tick down the player's invincibility timer each frame
	UpdatePlayerInvincibility(objectinfo[player], dt);

	// Check player projectiles hitting enemies
	enemySystem::checkProjectileEnemyCollision(enemies, MAX_ENEMIES,
		Projectiles, MAX_PROJECTILES);

	// Check melee enemies damaging player (uses PlayerTakeDamage internally)
	enemySystem::checkPlayerEnemyCollision(enemies, MAX_ENEMIES,
		objectinfo[player], Punch, soundEffects);

	// Check ranged enemy projectiles hitting player (uses PlayerTakeDamage internally)
	enemySystem::checkEnemyPlayerProjectileCollision(
		enemyProjectiles, MAX_PROJECTILES, objectinfo[player]);
	gamelogic::OBJ_to_map(map, x, s, &enemies[0].shape, 1);
	gamelogic::OBJ_to_map(map, x, s, &enemies[1].shape, 1);
	gamelogic::OBJ_to_map(map, x, s, &objectinfo[player], 1);

	// -----------------------------------------------------------------------
	// Door animation
	// -----------------------------------------------------------------------

	for (auto& door : doors) {

		if (door.firstLevel != 1 && door.secondLevel != 1) continue;
		f32 dx = objectinfo[player].xPos - door.worldX;
		f32 dy = objectinfo[player].yPos - door.worldY;
		f32 dist = sqrtf(dx * dx + dy * dy);
		// Assign to the file-scope static so Level1_Draw can read it
		playerNear = (dist <= doorTriggerRadius);

		if (playerNear && !door.isOpen && door.anim.playMode == ANIM_IDLE)
			animSystem::play(door.anim, ANIM_PLAY_ONCE);

		if (!playerNear && door.isOpen && door.anim.playMode == ANIM_IDLE)
			animSystem::play(door.anim, ANIM_PLAY_REVERSE);

		animSystem::update(door.anim, dt);

		if (door.anim.justFinished)
			door.isOpen = (door.anim.currentFrame != 0);

		// E key transition -- inside the loop so door and playerNear are in scope
		if (playerNear && door.isOpen && AEInputCheckTriggered(AEVK_E)) {
			int toLevel = (currentGameLevel == door.firstLevel) ? door.secondLevel : door.firstLevel;
			playerEnteredDoorId = door.id; // remember which door was used
			switch (toLevel) {
			case 0: next = GS_TUTORIAL; break;
			case 1: next = GS_LEVEL1; break;
			case 2: next = GS_LEVEL2; break;
			}
		}
	}

	// justFinished is true for one frame when a one-shot completes
	if (doorAnim.justFinished)
		doorIsOpen = (doorAnim.currentFrame == DOOR_FRAME_COUNT - 1);
	// -----------------------------------------------------------------------

	// MUSHROOM ANIMATION
	animSystem::update(meleeAnim, dt);

	//need fix animation
	/*
	static float frameTimer{ 0.0f };
	frameTimer += dt;
	if (frameTimer >= 0.1f) {
		static int currentFrame{};
		currentFrame = (currentFrame + 1) % 9; // 9 mushroom idle frames
		meleeEnemyTexture = mushroomIdleTexture[currentFrame];
		frameTimer = 0.0f;
	}
	*/

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

	// ===== RENDER WALLS ======= //
	renderlogic::drawMapWallFloor(map, x, y, s);

	// ==== ENEMIES RENDER =======//
	enemySystem::renderEnemies(enemies,
		MAX_ENEMIES,
		AssetManager::GetMesh("meleeEnemyMesh"),
		AssetManager::GetMesh("pTestMesh"),
		AssetManager::GetTexture("meleeEnemy"),
		AssetManager::GetTexture("rangedEnemy"),
		animSystem::getUOffset(meleeAnim),
		animSystem::getVOffset(meleeAnim));

	// Render enemy projectiles with plasma texture
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
	projectileSystem::renderProjectiles(enemyProjectiles, MAX_PROJECTILES, plasma, AssetManager::GetMesh("pTestMesh"));

	//====== PLAYER RENDER =========//
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxTextureSet(characterPictest, 0, 0);
	renderlogic::drawSquare(objectinfo[player].xPos, objectinfo[player].yPos,
		objectinfo[player].xScale, objectinfo[player].yScale);
	AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);

	// Render player projectiles with plasma texture
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
	projectileSystem::renderProjectiles(Projectiles, MAX_PROJECTILES, plasma, AssetManager::GetMesh("pTestMesh"));

	// ====== HUD: Player Health Display ======
	// Drawn last so it appears on top of all world geometry.
	// AEGfxPrint uses normalized coords: (-1,-1) = bottom-left, (1,1) = top-right.
	if (fontLevel1 >= 0)
	{
		char healthText[32];
		snprintf(healthText, sizeof(healthText), "Health: %d", objectinfo[player].health);

		// Prepare render state for font (font uses a glyph texture atlas)
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
		AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);

		// Print at top-left corner of the screen (white text)
		AEGfxPrint(fontLevel1, healthText, -0.95f, 0.85f, 0.8f, 1.0f, 1.0f, 1.0f, 1.0f);

	}
	renderlogic::drawTileArray();

	if (playerNear) {
		renderlogic::flashingTexture(objectinfo[player].xPos, objectinfo[player].yPos + 60.f, eButton, 50.f);
	}
}

void Level1_Free()
{
	// Free all registered meshes, then null the shared extern pointers draw.cpp holds
	AssetManager::FreeAllMeshes();
	pMesh        = nullptr;
	platformMesh = nullptr;
	doorMesh     = nullptr;

	if (map) {
		delete[] map;
		map = nullptr;
	}

	FreeMapData();
}

void Level1_Unload()
{
	// Unload all registered textures, then null the shared extern pointers draw.cpp holds.
	// Unload all AssetManager-tracked textures, then null the shared extern pointers.
	AssetManager::UnloadAllTextures();
	characterPictest = nullptr;
	base5test        = nullptr;
	plasma           = nullptr;
	doorTex          = nullptr;

	unload::platform();
	unload::ui();

	if (glassMap) {
		for (int i = 0; i < BINARY_MAP_HEIGHT; ++i) delete[] glassMap[i];
		delete[] glassMap;
		glassMap = nullptr;
	}


	// Destroy the font created in Initialize
	if (fontLevel1 != -1) { AEGfxDestroyFont(fontLevel1); fontLevel1 = -1; }

	// Unload ALL audio resources that were loaded in Load
	AEAudioUnloadAudio(L1);
	AEAudioUnloadAudio(LaserBlast);
	AEAudioUnloadAudio(Punch);
	AEAudioUnloadAudioGroup(bgm);
	AEAudioUnloadAudioGroup(soundEffects);
}
