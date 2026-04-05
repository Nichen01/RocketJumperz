/* Start Header ************************************************************************/
/*!
\file		  Tutorial.cpp
\date         April, 01, 2026
\brief        Provides functions and resources for editing maps, placing tiles, doors, traps, enemies, and items. Handles input shortcuts, error prompts, and exporting map data.

			  Copyright (C) 2026 DigiPen Institute of Technology.
			  Reproduction or disclosure of this file or its contents
			  without the prior written consent of DigiPen Institute of
			  Technology is prohibited.
*/
/* End Header **************************************************************************/

#include "pch.h"
#include "AssetManager.h"
#include "Level1.h"
#include "WeaponSprite.h"
#include "Drops.h"
#include "InstructionsMenu.h"

static s32* map = nullptr;
static int x = 16;
static int y = 9;
static int s = 80;

// Player sprite render size in world units (half a tile -- proportional to 30x30 enemies)
const float PlayerScale = 80.0f;

static s8 font = -1;

objectsquares objectinfoTut[2] = { 0 };
drop TutDrop[MAX_ENEMIES] = { 0 };

// Local variables for projectile test level
static Projectile Projectiles[MAX_PROJECTILES];

// ENEMY DATA
static Enemy enemies[MAX_ENEMIES];
static Projectile enemyProjectiles[MAX_PROJECTILES];
static AEGfxTexture* meleeEnemyTexture = nullptr;
static AEGfxTexture* rangedEnemyTexture = nullptr;

// Mushroom animation state (mesh/textures owned by AssetManager)
static SpriteAnimation meleeAnim;

//==== sound and volume
static f32 bgVolume = 1.f;

static char strBuffer[100];

static bool playerNear;
bool playerEnteredDoor0 = false;

static bool keycardCollectedAudio = false;

// Font resource (must be destroyed in Unload to avoid leak)
static s8 fontLevel1 = -1;

bool prevClearedTut = 0;

// Note: characterPictest, base5test, and pMesh are defined in draw.cpp. access them through draw.h

void Tutorial_Load()
{
	font = AEGfxCreateFont("Assets/Fonts/gameover.ttf", 50);
	audio::loadsound();

	InstructionsMenu::Load();

	// Load textures via AssetManager (enum-based IDs)
	AssetManager::LoadTexture(TEX_PLAYER, "Assets/Enemy/Character.png");
	AssetManager::LoadTexture(TEX_PLASMA, "Assets/Enemy/plasma.png");
	AssetManager::LoadTexture(TEX_DOOR, "Assets/Platform/DoorOpen.png");
	//AssetManager::LoadTexture(TEX_MELEE_ENEMY, "Assets/Enemy/MushroomIdle/mushroomIdle.png");
	AssetManager::LoadTexture(TEX_RANGED_ENEMY, "Assets/Enemy/RangedEnemy.png");
	AssetManager::LoadTexture(TEX_RANGED_MOVE,   "Assets/Enemy/RangedMove.png");
	AssetManager::LoadTexture(TEX_RANGED_ATTACK,  "Assets/Enemy/RangedAttack.png");
	AssetManager::LoadTexture(TEX_RANGED_DEATH,   "Assets/Enemy/RangedDeath.png");
	AssetManager::LoadTexture(TEX_RANGED_HURT,    "Assets/Enemy/RangedHurt.png");

	// Sync the extern pointers so other files can use them directly
	characterPictest  = AssetManager::GetTexture(TEX_PLAYER);
	plasma            = AssetManager::GetTexture(TEX_PLASMA);
	doorTex           = AssetManager::GetTexture(TEX_DOOR);
	
	rangedEnemyTexture = AssetManager::GetTexture(TEX_RANGED_ENEMY);

	// Load platform assets
	load::platform();
	load::ui();
	load::cooldownBar();
	load::background2();
	load::wireInventory();
	load::key();

	// Create font for gameover text (stored so we can destroy it in Unload)
	fontLevel1 = AEGfxCreateFont("Assets/Fonts/gameover.ttf", 72);
	weaponSprite::Load();
}

void Tutorial_Initialize()
{
	InstructionsMenu::Init();

	currentGameLevel = 0;
	AEAudioPlay(Level, bgm, MainVolume, 1.0f, -1);
	// Initialize player movement system
	movement::initPlayerMovement(objectinfoTut[player]);
	projectileSystem::initProjectiles(Projectiles, MAX_PROJECTILES);

	// Load saved checkpoint stats so progress from previous levels is preserved.
	// On a fresh game these are the defaults (ammo=50, wires=0, health=100).
	movement::bulletCount = savedAmmo;
	wireCount             = savedWireCount;

	//=============CREATE TEXTURED MESH FOR WALLS==================//
	// This mesh is used by draw.cpp for rendering walls

	AssetManager::BuildSqrMesh(MESH_QUAD);
	enemyMesh      = AssetManager::GetMesh(MESH_QUAD);
	platformMesh   = AssetManager::GetMesh(MESH_QUAD);
	pMesh          = AssetManager::GetMesh(MESH_QUAD);
	projectileMesh = AssetManager::GetMesh(MESH_QUAD);
	uiMesh         = AssetManager::GetMesh(MESH_QUAD);

	if (!ImportMapDataFromFile("Assets/Map/Tutorial.txt")) {
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
		door.isLocked = true;
		if (door.id == playerEnteredDoorId) {
			objectinfoTut[player].xPos = door.worldX;
			objectinfoTut[player].yPos = door.worldY;
			spawnSet = true;
			break;
		}
	}

	// fallback if no door found (first time loading)
	if (!spawnSet) {
		objectinfoTut[player].xPos = 600.f;
		objectinfoTut[player].yPos = -300.f;
	}
	objectinfoTut[player].xScale = PlayerScale;
	objectinfoTut[player].yScale = PlayerScale;

	// Initialize player health from saved checkpoint (max on fresh game)
	InitPlayerHealth(objectinfoTut[player]);
	objectinfoTut[player].health = savedHealth;

	// Start with the plasma gun equipped (default weapon)
	objectinfoTut[player].currentWeapon = WEAPON_PLASMA;

	//======== INIT ENEMIES DATA =======================//
	// Initialize enemy system
	enemySystem::initEnemies(enemies, MAX_ENEMIES);
	projectileSystem::initProjectiles(enemyProjectiles, MAX_PROJECTILES);

	// SPAWN test enemies
	enemySystem::spawnEnemy(enemies, MAX_ENEMIES, ENEMY_RANGED, enemy1X, enemy1Y);
	
	// Build spritesheet meshes for ranged enemy states (rows, cols)
	AssetManager::BuildSqrMesh(MESH_RANGED_MOVE,   1, 4);
	AssetManager::BuildSqrMesh(MESH_RANGED_ATTACK,  1, 6);
	AssetManager::BuildSqrMesh(MESH_RANGED_DEATH,   1, 4);
	AssetManager::BuildSqrMesh(MESH_RANGED_HURT,    1, 2);

	// DOOR

	// Build door animation mesh: 1 row, 7 columns (7 frames in a horizontal strip).
	AssetManager::BuildSqrMesh(MESH_DOOR, 1, 7);
	doorMesh = AssetManager::GetMesh(MESH_DOOR);

	if (!doorTex) printf("DOOR TEXTURE NOT FOUND!\n");
	else printf("DOOR OK\n");
	pickup::initDrops(TutDrop, MAX_ENEMIES, PlayerScale);

}

void Tutorial_Update()
{
	// If the instructions overlay is open, skip all gameplay logic (pause)
	if (InstructionsMenu::Update()) return;

	if (AEInputCheckCurr(AEVK_1)) next = GS_LEVEL1;

	//====== AUDIO CONTROLS ======//
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

	//========== GRAVITY TOGGLE (LShift) ===============//
	movement::UpdateGravityToggle();

	//========== JETPACK MOVEMENT SYSTEM ===============//
	//Apply thrust when spacebar is pressed
	movement::physicsInput(objectinfoTut[player]);

	// ---- Weapon Toggle (Q key) ----
	// Press Q to switch between Plasma (single shot) and Shotgun (spread).
	if (AEInputCheckTriggered(AEVK_Q)) {
		if (objectinfoTut[player].currentWeapon == WEAPON_PLASMA) {
			objectinfoTut[player].currentWeapon = WEAPON_SHOTGUN;
			printf("Weapon switched to: SHOTGUN\n");
		}
		else {
			objectinfoTut[player].currentWeapon = WEAPON_PLASMA;
			printf("Weapon switched to: PLASMA\n");
		}
	}


	//===========  APPLY PHYSICS(DRAG)===================//
	// Update player physics (drag + position)
	movement::updatePlayerPhysics(objectinfoTut[player]);
	movement::UpdatePlayerFacing(objectinfoTut[player]);
	pickup::updateDrops(TutDrop, MAX_ENEMIES, objectinfoTut[player]);
	//===================================================//

	// ========== PROJECTILE SYSTEM UPDATE =============//
	// Fire using the currently equipped weapon (toggled with Q)
	if (movement::bulletCount>0) {
		if (objectinfoTut[player].currentWeapon == WEAPON_SHOTGUN && movement::bulletCount>2) {
			// Shotgun: 5-pellet cone spread
			projectileSystem::FireShotgun(
				static_cast<s32>(worldMouseX),
				static_cast<s32>(worldMouseY),
				objectinfoTut[player],
				Projectiles,
				MAX_PROJECTILES,
				LaserBlast,
				soundEffects);
		}
		else if (objectinfoTut[player].currentWeapon != WEAPON_SHOTGUN) {
			// Plasma (default): single shot toward mouse
			projectileSystem::fireProjectiles(
				static_cast<s32>(worldMouseX),
				static_cast<s32>(worldMouseY),
				objectinfoTut[player],
				Projectiles,
				MAX_PROJECTILES,
				LaserBlast,
				soundEffects);
		}
	}

	// Update all active projectiles
	projectileSystem::UpdateProjectiles(Projectiles, MAX_PROJECTILES, map, x, static_cast<int>(tileSize));

	//============= UPDATE ENEMIES ===================/
	// Get delta time for enemy AI
	f32 dt = static_cast<f32>(AEFrameRateControllerGetFrameTime());

	// Update enemies
	enemySystem::updateEnemies(enemies, MAX_ENEMIES,
		objectinfoTut[player],TutDrop,
		enemyProjectiles, MAX_PROJECTILES,
		dt, LaserBlast, soundEffects,prevClearedTut);

	// Update enemy projectiles
	projectileSystem::UpdateProjectiles(enemyProjectiles, MAX_PROJECTILES, map, x, static_cast<int>(tileSize));

	// Tick down the player's invincibility timer each frame
	UpdatePlayerInvincibility(objectinfoTut[player], dt);

	// Check player projectiles hitting enemies
	enemySystem::checkProjectileEnemyCollision(enemies, MAX_ENEMIES,
		Projectiles, MAX_PROJECTILES);

	// Check ranged enemy projectiles hitting player (uses PlayerTakeDamage internally)
	enemySystem::checkEnemyPlayerProjectileCollision(
		enemyProjectiles, MAX_PROJECTILES, objectinfoTut[player]);

	// If player health < 0, go to death screen
	if (objectinfoTut[player].health <= 0) {
		next = GS_DEATH;
	}

	gamelogic::Collision_movement(&enemies[0].shape, map, x, s, 1);
	gamelogic::Collision_movement(&enemies[1].shape, map, x, s, 1);
	gamelogic::Collision_movement(&objectinfoTut[player], map, x, s, 1);

	// -----------------------------------------------------------------------
	// Door animation
	// -----------------------------------------------------------------------

	for (auto& door : doors) {
		f32 dx = objectinfoTut[player].xPos - door.worldX;
		f32 dy = objectinfoTut[player].yPos - door.worldY;
		f32 dist = sqrtf(dx * dx + dy * dy);
		playerNear = (dist <= doorTriggerRadius);

		// Only animate if door is not locked
		if (!door.isLocked) {
			if (playerNear && !door.isOpen && door.anim.playMode == ANIM_IDLE)
				animSystem::play(door.anim, ANIM_PLAY_ONCE);

			if (!playerNear && door.isOpen && door.anim.playMode == ANIM_IDLE)
				animSystem::play(door.anim, ANIM_PLAY_REVERSE);
		}

		animSystem::update(door.anim, dt);

		if (door.anim.justFinished)
			door.isOpen = (door.anim.currentFrame != 0);

		// Transition only if unlocked
		if (playerNear && door.isOpen && !door.isLocked && AEInputCheckTriggered(AEVK_E)) {
			int toLevel = (currentGameLevel == door.entranceLevel) ? door.exitLevel : door.entranceLevel;

			// Save current stats as a checkpoint before leaving the level.
			// If the player dies in the next level, these values are restored.
			savedAmmo      = movement::bulletCount;
			savedWireCount = wireCount;
			savedHealth    = objectinfoTut[player].health;

			playerEnteredDoor0 = true;
			playerEnteredDoorId = door.id;
			switch (toLevel) {
			case 0: next = GS_TUTORIAL; break;
			case 1: next = GS_LEVEL1; break;
			case 2: next = GS_LEVEL2; break;
			}
		}
	}

	// Collect health pack
	objectsquares keyObj;
	keyObj.xPos = key.worldX;
	keyObj.yPos = key.worldY;
	keyObj.xScale = key.size;
	keyObj.yScale = key.size;

	if (key.active && gamelogic::static_collision(&objectinfoTut[player], &keyObj)) {
		key.active = false;
		keycardCollected0 = true;
		AEAudioPlay(Pickup, soundEffects, 1, 1, 0);

		for (auto& door : doors) {
			door.isLocked = false;
		}
	}


	weaponSprite::Update(objectinfoTut[player]);

	objectsquares healthObj;
	healthObj.xPos = hp.worldX;
	healthObj.yPos = hp.worldY;
	healthObj.xScale = hp.size; healthObj.yScale = hp.size;

	if (hp.active && gamelogic::static_collision(&objectinfoTut[player], &healthObj)) {
		hp.active = false;
		hp.collected = true;
		objectinfoTut[player].health += rand() % 31;
		AEAudioPlay(Pickup, soundEffects, 1, 1, 0);
	}

	// Ensure doors stay unlocked once the keycard is collected
	if (keycardCollected0) {
		for (auto& door : doors) {
			door.isLocked = false;
		}
	}
}

void Tutorial_Draw()
{
	renderlogic::drawTexture(0.f, 0.f, backgroundTex, uiMesh, static_cast<f32>(AEGfxGetWindowWidth()), static_cast<f32>(AEGfxGetWindowHeight()));

	AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);

	memset(strBuffer, 0, 100 * sizeof(char));
	f32 textWidth, textHeight;
	AEGfxGetPrintSize(font, strBuffer, 0.6f, &textWidth, &textHeight);
	sprintf_s(strBuffer, "Left Click to Shoot");
	AEGfxPrint(font, strBuffer, 0.52f, -0.6f, 0.5f, 1.f, 1.f, 1.f, 1.f);

	sprintf_s(strBuffer, "towards Mouse Cursor");
	AEGfxPrint(font, strBuffer, 0.5f, -0.67f, 0.5f, 1.f, 1.f, 1.f, 1.f);

	sprintf_s(strBuffer, "Right Click to Advance");
	AEGfxPrint(font, strBuffer, 0.f, -0.6f, 0.5f, 1.f, 1.f, 1.f, 1.f);

	sprintf_s(strBuffer, "towards Mouse Cursor");
	AEGfxPrint(font, strBuffer, 0.f, -0.67f, 0.5f, 1.f, 1.f, 1.f, 1.f);

	sprintf_s(strBuffer, "Spacebar to Jump");
	AEGfxPrint(font, strBuffer, -0.7f, -0.0f, 0.5f, 1.f, 1.f, 1.f, 1.f);

	sprintf_s(strBuffer, "Left Shift To Toggle Gravity");
	AEGfxPrint(font, strBuffer, 0.3f, 0.1f, 0.5f, 1.f, 1.f, 1.f, 1.f);

	sprintf_s(strBuffer, "Q to Switch Weapon");
	AEGfxPrint(font, strBuffer, -0.7f, -0.65f, 0.6f, 1.f, 1.f, 1.f, 1.f);

	sprintf_s(strBuffer, "E to Enter");
	AEGfxPrint(font, strBuffer, 0.42f, 0.53f, 0.5f, 1.f, 1.f, 1.f, 1.f);

	// ===== RENDER WALLS ======= //
	renderlogic::drawMapWallFloor(map, x, y, s);

	// ==== RENDER UI FOR TUTORIAL ==== //
	renderlogic::drawTileArray();
	renderlogic::drawUITexture(350.f, -280.f, leftClick, 55.f);
	renderlogic::drawUITexture(-50.f, -280.f, rightClick, 55.f);
	renderlogic::drawUITexture(200.f, 50.f, shiftButton, 55.f);
	renderlogic::drawUITexture(-600.f, -280.f, qButton, 55.f);
	renderlogic::drawUITexture(-600.f, 10.f, spacebar, 55.f);
	renderlogic::drawUITexture(280.f, 250.f, eButton, 55.f);

	// ====== RENDERING PADLOCK ====== //
	for (auto& door : doors) {
		if (door.isLocked) {
			// Draw padlock texture at the door�s position
			renderlogic::drawTexture(door.worldX, door.worldY, padlock, uiMesh, 50.f, 50.f);
		}
	}

	// ==== ENEMIES RENDER =======//
	enemySystem::renderEnemies(enemies, MAX_ENEMIES,
		AssetManager::GetMesh(MESH_MELEE_ENEMY),
		projectileMesh,
		meleeEnemyTexture, rangedEnemyTexture,
		animSystem::getUOffset(meleeAnim),
		animSystem::getVOffset(meleeAnim));

	// Render enemy projectiles with plasma texture
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
	projectileSystem::renderProjectiles(enemyProjectiles, MAX_PROJECTILES, plasma, projectileMesh);

	//====== PLAYER RENDER =========//
	// Reset render state so leftover color tints from enemies/projectiles don't affect the player
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);

	if (objectinfoTut[player].invincibilityTimer > 0.0f) {
		// Rapidly blink red and normal while invincible (damage flash)
		if (sinf(objectinfoTut[player].invincibilityTimer * 40.0f) > 0.0f) {
			AEGfxSetColorToMultiply(1.0f, 0.2f, 0.2f, 1.0f);
		} else {
			AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
		}
	}
	else {
		// Normal color
		AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
	}

	AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxTextureSet(characterPictest, 0, 0);
	// Flip sprite horizontally when the player is aiming left.
	f32 playerDrawScaleX = movement::playerFacingLeft
		? -objectinfoTut[player].xScale
		:  objectinfoTut[player].xScale;
	renderlogic::drawSquare(objectinfoTut[player].xPos, objectinfoTut[player].yPos,
		playerDrawScaleX, objectinfoTut[player].yScale);
	AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);

	// Render player projectiles with plasma texture
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
	projectileSystem::renderProjectiles(Projectiles, MAX_PROJECTILES, plasma, projectileMesh);

	//====== PLAYER THRUST COOLDOWN BAR RENDER =========//
	renderlogic::drawCooldownHUD(objectinfoTut[player].xPos, objectinfoTut[player].yPos - 50.f);

	// ====== HUD: Health, Ammo, Weapon, Gravity ======//
	// Drawn last so it appears on top of all world geometry.
	// AEGfxPrint uses normalized coords: (-1,-1) = bottom-left, (1,1) = top-right.
	if (fontLevel1 >= 0)
	{
		// ---- Health icon + number (top-left) ----
		// Render a small health sprite to the left of the health number
		f32 halfW = static_cast<f32>(AEGfxGetWindowWidth())  * 0.5f;
		f32 halfH = static_cast<f32>(AEGfxGetWindowHeight()) * 0.5f;

		// Health icon at top-left (world coords from normalized -0.93, 0.88)
		f32 healthIconX = -halfW + 40.f;   // 40px from left edge
		f32 healthIconY =  halfH - 40.f;   // 40px from top edge
		AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
		AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
		renderlogic::drawTexture(healthIconX, healthIconY, healthDrop, uiMesh, 70.f, 70.f);

		// Health number text right of the icon
		char healthText[32];
		snprintf(healthText, sizeof(healthText), "%d", objectinfoTut[player].health);
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
		AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		AEGfxPrint(fontLevel1, healthText, -0.88f, 0.88f, 0.8f, 0.0f, 1.0f, 0.0f, 1.0f);

		// ---- Ammo icon + count (right beside health) ----
		f32 ammoIconX = healthIconX + 225.f;  // offset to the right of health (increased for readability)
		f32 ammoIconY = healthIconY;
		AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
		AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
		renderlogic::drawTexture(ammoIconX, ammoIconY, ammoDrop, uiMesh, 60.f, 60.f);

		// Ammo count text right of the ammo icon
		char ammoText[32];
		snprintf(ammoText, sizeof(ammoText), "%d", movement::bulletCount);
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
		AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		AEGfxPrint(fontLevel1, ammoText, -0.59f, 0.88f, 0.8f, 1.0f, 1.0f, 0.0f, 1.0f);

		// ---- Current weapon sprite (below health row) ----
		// Show the sprite of the currently equipped weapon
		AEGfxTexture* weaponIcon = (objectinfoTut[player].currentWeapon == WEAPON_SHOTGUN)
			? AssetManager::GetTexture(TEX_SHOTGUN)
			: AssetManager::GetTexture(TEX_PLASMA_GUN);
		f32 weaponIconX = healthIconX + 475.f;
		f32 weaponIconY = healthIconY;
		AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
		AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
		renderlogic::drawTexture(weaponIconX, weaponIconY, weaponIcon, uiMesh, 100.f, 50.f);

		// ---- Gravity indicator (top center) ----
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		{
			char gravityBuf[64];
			if (movement::isGravityDisabled) {
				sprintf_s(gravityBuf, sizeof(gravityBuf),
					"Gravity: OFF | Timer: %.1fs", movement::gravityTimer);
				AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
				AEGfxPrint(fontLevel1, gravityBuf, -0.25f, 0.90f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f);
			}
			else {
				sprintf_s(gravityBuf, sizeof(gravityBuf), "Gravity: ON");
				AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
				AEGfxPrint(fontLevel1, gravityBuf, -0.12f, 0.90f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f);
			}
		}
	}
	
	// ===== RENDERING OF THE FLASHING 'E' ===== //
	if (playerNear) {
		renderlogic::flashingTexture(objectinfoTut[player].xPos, objectinfoTut[player].yPos + 60.f, eButton, 50.f);
	}
	weaponSprite::Draw();
	pickup::drawDrops(TutDrop, MAX_ENEMIES);

	// ====== DISPLAY KEYCARD IN INVENTORY ====== //
	if (playerEnteredDoor0) {
		renderlogic::drawTexture(-750.f, -400.f, inventory, uiMesh, 100.f, 100.f);
	}
	else {
		// Otherwise, show keycard if collected, empty if not
		if (keycardCollected0) {
			renderlogic::drawTexture(-750.f, -400.f, keycardInventory, uiMesh, 100.f, 100.f);
			for (auto& door : doors) {
				door.isLocked = false;
			}
		}
		else {
			renderlogic::drawTexture(-750.f, -400.f, inventory, uiMesh, 100.f, 100.f);
		}
	}

	// ====== WIRE INVENTORY (shows wire count 0-3) ====== //
	renderlogic::drawWireInventory(wireCount);

	// Draw the "?" icon (or the full overlay if it is open) on top of everything
	InstructionsMenu::Draw();
}

void Tutorial_Free()
{
	AssetManager::FreeAllMeshes();

	if (map) {
		delete[] map;
		map = nullptr;
	}

	FreeMapData();
}

void Tutorial_Unload()
{
	InstructionsMenu::Unload();

	AssetManager::UnloadAllTextures();

	if (glassMap) {
		for (int i = 0; i < BINARY_MAP_HEIGHT; ++i) delete[] glassMap[i];
		delete[] glassMap;
		glassMap = nullptr;
	}
	weaponSprite::Unload();
	// Destroy the font created in Load (tutorial text labels)
	if (font != -1) { AEGfxDestroyFont(font); font = -1; }

	// Destroy the font created in Initialize (HUD health text)
	if (fontLevel1 != -1) { AEGfxDestroyFont(fontLevel1); fontLevel1 = -1; }

	// Unload ALL audio resources that were loaded in Load
	audio::unloadsound();
}
