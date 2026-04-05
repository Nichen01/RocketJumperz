/* Start Header ************************************************************************/
/*!
\file		  Level2.cpp
\date         April, 01, 2026
\brief        Provides functions and resources for editing maps, placing tiles, doors, traps, enemies, and items. Handles input shortcuts, error prompts, and exporting map data.

			  Copyright (C) 2026 DigiPen Institute of Technology.
			  Reproduction or disclosure of this file or its contents
			  without the prior written consent of DigiPen Institute of
			  Technology is prohibited.
*/
/* End Header **************************************************************************/

// External libraries are included in header file
#include "Level2.h"
#include "traps.h"
#include "WeaponSprite.h"
#include "ParticleSystem.h"
#include "InstructionsMenu.h"

static s32* map = nullptr;
static int x;
static int y;

// Player sprite render size in world units (half a tile -- proportional to 30x30 enemies)
const float PlayerScale = 80.0f;

extern objectsquares objectinfo2[2] = { 0 };
drop L2Drop[MAX_ENEMIES] = { 0 };

// Wire drops -- one wire can drop per level from enemy deaths
static WireDrop wireDrops[MAX_ENEMIES] = { 0 };

// Local variables for projectile test level
static Projectile Projectiles[MAX_PROJECTILES];

static AEGfxVertexList* pTestMesh = nullptr;

// ENEMY DATA
static Projectile enemyProjectiles[MAX_PROJECTILES];
static Enemy enemies[MAX_ENEMIES];

// Mushroom animation state (mesh/textures owned by AssetManager)
static SpriteAnimation meleeAnim;

// Base texture loaded via AssetManager (no extern in draw.h, Level1-only)
static AEGfxTexture* base5test = nullptr;

// Font handle for in-game text -- UNUSED, kept for reference only.
// The active font handle is fontLevel2 (declared below), which is
// created in Initialize and destroyed in Unload.

// Sound and volume
static f32 bgVolume = 1.f;

// Door variables (doorX, doorY, doorAnim, doorMesh, doorIsOpen, doorTex)
// are defined in draw.cpp and declared extern in draw.h.
// The constants below are Level1-specific door animation parameters.
static s32  DOOR_FRAME_COUNT = 7;
static f32  DOOR_FRAME_DELAY = 0.08f;   // ~12 fps

// Font resource (must be destroyed in Unload to avoid leak)
static s8 fontLevel2 = -1;

// bool for checking player proximity with door
static bool playerNear;
bool playerEnteredDoor2 = false;

// bool for keycard in inventory
static bool healthCollected;
static bool keycardCollectedAudio = false;

//bool for checking if player previously cleared level
extern bool prevCleared2 = 0;

// Note: characterPictest, base5test, and pMesh are defined in draw.cpp. access them through draw.h

void Level2_Load()
{
	audio::loadsound();

	InstructionsMenu::Load();

	// Load platform tile textures
	load::platform();

	// Load UI textures (eButton used by flashing door prompt in Draw)
	load::ui();
	load::cooldownBar();

	// Load wire item texture (world drop) and wire inventory textures
	AssetManager::LoadTexture(TEX_WIRE, "Assets/Items/wire.png");
	load::wireInventory();
	load::background2();

	// Load textures via AssetManager (prevents duplicate loads across level reloads)
	AssetManager::LoadTexture(TEX_PLAYER, "Assets/Enemy/Character.png");
	AssetManager::LoadTexture(TEX_BASE5TEST, "Assets/Base5.png");
	AssetManager::LoadTexture(TEX_PLASMA, "Assets/Enemy/plasma.png");
	AssetManager::LoadTexture(TEX_DOOR, "Assets/Platform/DoorOpen.png");
	AssetManager::LoadTexture(TEX_MUSHROOM_IDLE_SHEET, "Assets/Enemy/MushroomIdle/MushroomIdle.png");
	AssetManager::LoadTexture(TEX_RANGED_ENEMY, "Assets/Enemy/RangedEnemy.png");
	AssetManager::LoadTexture(TEX_KEYCARD, "Assets/Items/keycard.png");

	// Ranged enemy state spritesheets (1 row each, variable columns)
	AssetManager::LoadTexture(TEX_RANGED_MOVE,   "Assets/Enemy/RangedMove.png");
	AssetManager::LoadTexture(TEX_RANGED_ATTACK,  "Assets/Enemy/RangedAttack.png");
	AssetManager::LoadTexture(TEX_RANGED_DEATH,   "Assets/Enemy/RangedDeath.png");
	AssetManager::LoadTexture(TEX_RANGED_HURT,    "Assets/Enemy/RangedHurt.png");

	// Sync the extern pointers so other files (draw.cpp etc.) can use them directly
	characterPictest = AssetManager::GetTexture(TEX_PLAYER);
	base5test = AssetManager::GetTexture(TEX_BASE5TEST);
	plasma = AssetManager::GetTexture(TEX_PLASMA);
	doorTex = AssetManager::GetTexture(TEX_DOOR);

	// Loading of assets for mushroomDie using AssetManager
	{
		const TextureID dieIDs[9] = {
			TEX_MUSHROOM_DIE0, TEX_MUSHROOM_DIE1, TEX_MUSHROOM_DIE2,
			TEX_MUSHROOM_DIE3, TEX_MUSHROOM_DIE4, TEX_MUSHROOM_DIE5,
			TEX_MUSHROOM_DIE6, TEX_MUSHROOM_DIE7, TEX_MUSHROOM_DIE8
		};
		for (int i = 0; i < 9; ++i) {
			char path[128];
			sprintf_s(path, "Assets/Enemy/MushroomDie/MushroomDie%d.png", i);
			AssetManager::LoadTexture(dieIDs[i], path);
			mushroomDieTexture[i] = AssetManager::GetTexture(dieIDs[i]);
		}
	}

	// Loading of assets for mushroomHit using AssetManager
	{
		const TextureID hitIDs[5] = {
			TEX_MUSHROOM_HIT0, TEX_MUSHROOM_HIT1, TEX_MUSHROOM_HIT2,
			TEX_MUSHROOM_HIT3, TEX_MUSHROOM_HIT4
		};
		for (int i = 0; i < 5; ++i) {
			char path[128];
			sprintf_s(path, "Assets/Enemy/MushroomHit/MushroomHit%d.png", i);
			AssetManager::LoadTexture(hitIDs[i], path);
			mushroomHitTexture[i] = AssetManager::GetTexture(hitIDs[i]);
		}
	}

	// Loading of assets for mushroomIdle using AssetManager
	{
		const TextureID idleIDs[7] = {
			TEX_MUSHROOM_IDLE0, TEX_MUSHROOM_IDLE1, TEX_MUSHROOM_IDLE2,
			TEX_MUSHROOM_IDLE3, TEX_MUSHROOM_IDLE4, TEX_MUSHROOM_IDLE5,
			TEX_MUSHROOM_IDLE6
		};
		for (int i = 0; i < 7; ++i) {
			char path[128];
			sprintf_s(path, "Assets/Enemy/MushroomIdle/MushroomIdle%d.png", i);
			AssetManager::LoadTexture(idleIDs[i], path);
			mushroomIdleTexture[i] = AssetManager::GetTexture(idleIDs[i]);
		}
	}

	// Create font for gameover text (stored so we can destroy it in Unload)
	fontLevel2 = AEGfxCreateFont("Assets/Fonts/gameover.ttf", 72);
	weaponSprite::Load();

	// Build the particle system mesh and reset the pool
	ParticleSystem::Load();
}

void Level2_Initialize()
{
	InstructionsMenu::Init();

	// Spawn card only if not yet collected and not already progressed
	if (!keycardCollected2 && !playerEnteredDoor2) {
		key.active = true;
	}
	else {
		key.active = false;
	}

	characterPictest = AssetManager::GetTexture(TEX_PLAYER);
	base5test = AssetManager::GetTexture(TEX_BASE5TEST);
	plasma = AssetManager::GetTexture(TEX_PLASMA);
	doorTex = AssetManager::GetTexture(TEX_DOOR);
	keyTexture = AssetManager::GetTexture(TEX_KEYCARD);
	currentGameLevel = 2;

	AEAudioPlay(Level, bgm, MainVolume, 1.0f, -1);

	// Font is already created in Level2_Load -- do NOT recreate here.
	// Recreating would leak the previous font handle each time the level reinitializes.

	// Initialize player movement system
	movement::initPlayerMovement(objectinfo2[player]);

	// Added after obstacle initialization:
	projectileSystem::initProjectiles(Projectiles, MAX_PROJECTILES);

	//=============CREATE TEXTURED MESHES==================//
	AssetManager::BuildSqrMesh(MESH_QUAD);
	AssetManager::BuildSqrMesh(MESH_MELEE_ENEMY, 2, 3);
	enemyMesh      = AssetManager::GetMesh(MESH_QUAD);
	platformMesh   = AssetManager::GetMesh(MESH_QUAD);
	pMesh          = AssetManager::GetMesh(MESH_QUAD);
	projectileMesh = AssetManager::GetMesh(MESH_QUAD);
	uiMesh         = AssetManager::GetMesh(MESH_QUAD);

	if (!ImportMapDataFromFile("Assets/Map/Level2_Map.txt")) {
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
	if (playerEnteredDoorId != -1) {
		for (auto& door : doors) {
			door.isLocked = true;
			if (door.id == 22) door.isLocked = false;
			if (keycardCollected2) door.isLocked = false; // keep unlocked if card collected
			if (door.id == playerEnteredDoorId) {
				objectinfo2[player].xPos = door.worldX;
				objectinfo2[player].yPos = door.worldY;
				spawnSet = true;
			}
		}
	}

	// fallback if no door found (first time loading)
	if (!spawnSet) {
		objectinfo2[player].xPos = 0.f;
		objectinfo2[player].yPos = 0.f;
	}
	objectinfo2[player].xScale = PlayerScale;
	objectinfo2[player].yScale = PlayerScale;

	// Initialize player health from saved checkpoint (max on fresh game)
	InitPlayerHealth(objectinfo2[player]);
	objectinfo2[player].health = savedHealth;

	// Load saved checkpoint stats so progress from previous levels is preserved
	movement::bulletCount = savedAmmo;
	wireCount             = savedWireCount;

	// Start with the plasma gun equipped (default weapon)
	objectinfo2[player].currentWeapon = WEAPON_PLASMA;

	//======== INIT ENEMIES DATA =======================//
	// Initialize enemy system
	enemySystem::initEnemies(enemies, MAX_ENEMIES);
	projectileSystem::initProjectiles(enemyProjectiles, MAX_PROJECTILES);

	// SPAWN test enemies
	if (!prevCleared2) {
		enemySystem::spawnEnemy(enemies, MAX_ENEMIES, ENEMY_MELEE, enemy2X, enemy2Y);
		enemySystem::spawnEnemy(enemies, MAX_ENEMIES, ENEMY_RANGED, enemy1X, enemy1Y);
	}

	//MUSHROOM ANIM TEST
	AssetManager::BuildSqrMesh(MESH_MELEE_ENEMY, 2, 3);
	animSystem::init(meleeAnim, 3, 2, 6, 0.1f, ANIM_LOOP, 0);

	// Build spritesheet meshes for ranged enemy states (rows, cols)
	AssetManager::BuildSqrMesh(MESH_RANGED_MOVE,   1, 4);
	AssetManager::BuildSqrMesh(MESH_RANGED_ATTACK,  1, 6);
	AssetManager::BuildSqrMesh(MESH_RANGED_DEATH,   1, 4);
	AssetManager::BuildSqrMesh(MESH_RANGED_HURT,    1, 2);
	// DOOR
	AssetManager::BuildSqrMesh(MESH_DOOR, 1, 7);
	doorMesh = AssetManager::GetMesh(MESH_DOOR);

	animSystem::init(doorAnim, 7, 1, DOOR_FRAME_COUNT, DOOR_FRAME_DELAY, ANIM_IDLE, 0);
	doorIsOpen = false;

	pickup::initDrops(L2Drop, MAX_ENEMIES, PlayerScale-20);

	// Wire drops: reset per-level tracker and initialize wire drop array
	pickup::ResetWireDropTracker();
	pickup::InitWireDrops(wireDrops, MAX_ENEMIES, PlayerScale);

	// Ammo pool: spawns 80px to the right of the player's start position
	pickup::InitAmmoPool(objectinfo2[player].xPos + 80.0f, objectinfo2[player].yPos, 60.0f);

	traps::initTraps();

}

void Level2_Update()
{
	// DEBUGGING FEATURE TO TRANSIT TO DIFFERENT LEVELS
	if (AEInputCheckCurr(AEVK_1)) next = GS_TUTORIAL;
	if (AEInputCheckCurr(AEVK_2)) next = GS_LEVEL1;
	if (AEInputCheckCurr(AEVK_4)) next = GS_LEVEL3;

	// If the instructions overlay is open, skip all gameplay logic (pause)
	if (InstructionsMenu::Update()) return;

	//====== TOGGLE LEVEL EDITOR GAME STATE ======//
	if (AEInputCheckTriggered(AEVK_L)) {
		currentGameLevel = 2;
		next = GS_LEVELEDITOR;
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
	movement::physicsInput(objectinfo2[player]);

	// ---- Weapon Toggle (Q key) ----
	if (AEInputCheckTriggered(AEVK_Q)) {
		if (objectinfo2[player].currentWeapon == WEAPON_PLASMA) {
			objectinfo2[player].currentWeapon = WEAPON_SHOTGUN;
			printf("Weapon switched to: SHOTGUN\n");
		}
		else {
			objectinfo2[player].currentWeapon = WEAPON_PLASMA;
			printf("Weapon switched to: PLASMA\n");
		}
	}

	//===========  APPLY PHYSICS(DRAG)===================//
	// Update player physics (drag + position)
	movement::updatePlayerPhysics(objectinfo2[player]);
	movement::UpdatePlayerFacing(objectinfo2[player]);
	weaponSprite::Update(objectinfo2[player]);
	pickup::updateDrops(L2Drop, MAX_ENEMIES, objectinfo2[player]);
	pickup::UpdateWireDrops(wireDrops, MAX_ENEMIES, objectinfo2[player]);
	pickup::UpdateAmmoPool(objectinfo2[player]);
	//===================================================//

	// ========== PROJECTILE SYSTEM UPDATE =============//
	// Fire using the currently equipped weapon (toggled with Q)
	if (movement::bulletCount>0) {
		if (objectinfo2[player].currentWeapon == WEAPON_SHOTGUN && movement::bulletCount > 2) {
			projectileSystem::FireShotgun(
				static_cast<s32>(worldMouseX),
				static_cast<s32>(worldMouseY),
				objectinfo2[player],
				Projectiles,
				MAX_PROJECTILES,
				LaserBlast,
				soundEffects);
		}
		else if (objectinfo2[player].currentWeapon != WEAPON_SHOTGUN) {
			projectileSystem::fireProjectiles(
				static_cast<s32>(worldMouseX),
				static_cast<s32>(worldMouseY),
				objectinfo2[player],
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

	// Step all active particles forward
	ParticleSystem::Update(dt);

	// Update enemies (pass wireDrops so dead enemies can spawn wire items)
	enemySystem::updateEnemies(enemies, MAX_ENEMIES,
		objectinfo2[player], L2Drop,
		enemyProjectiles, MAX_PROJECTILES,
		dt, LaserBlast, soundEffects, prevCleared2,
		wireDrops, MAX_ENEMIES);

	// Update enemy projectiles
	projectileSystem::UpdateProjectiles(enemyProjectiles, MAX_PROJECTILES, map, x, static_cast<int>(tileSize));

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

	// If player health < 0, go to death screen
	if (objectinfo2[player].health <= 0) {
		next = GS_DEATH;
	}

	traps::updateTraps(enemies, objectinfo2, map, x,y, tileSize);
	traps::UpdateSawAnim(dt);
	gamelogic::Collision_movement(&enemies[0].shape, map, x, static_cast<int>(tileSize), 1);
	gamelogic::Collision_movement(&enemies[1].shape, map, x, static_cast<int>(tileSize), 1);
	gamelogic::Collision_movement(&objectinfo2[player], map, x, static_cast<int>(tileSize), 1);

	// -----------------------------------------------------------------------
	// Door animation
	// -----------------------------------------------------------------------

	bool nearAnyDoor = false; // track if player is near at least one door

	for (auto& door : doors) {
		// Only process doors connected to this level
		if (door.entranceLevel != currentGameLevel && door.exitLevel != currentGameLevel)
			continue;

		// --- Spark Emission Logic for Locked Doors ---
		if (door.isLocked) {
			door.sparkTimer += dt;
			if (door.sparkTimer >= 4.0f) {
				door.sparkTimer = 0.0f;

				EmitterProps sparkProps;
				sparkProps.spawnX = door.worldX;
				sparkProps.spawnY = door.worldY + 10.0f;
				sparkProps.velocityXBase = 0.0f;
				sparkProps.velocityYBase = 200.0f;    // Shoot upwards; gravity pulls them down
				sparkProps.velocitySpreadX = 100.0f;  // Wide X spread for cone shape
				sparkProps.velocitySpreadY = 40.0f;
				sparkProps.lifetimeBase = 1.0f;        // Longer life to show the arc
				sparkProps.lifetimeSpread = 0.7f;
				sparkProps.scaleBase = 5.0f;
				sparkProps.emitCount = 18;
				sparkProps.useSparkColors = true;   // Random white/yellow/orange sparks

				ParticleSystem::Emit(sparkProps);
			}
		}

		f32 dx = objectinfo2[player].xPos - door.worldX;
		f32 dy = objectinfo2[player].yPos - door.worldY;
		f32 dist = sqrtf(dx * dx + dy * dy);

		bool nearThisDoor = (dist <= doorTriggerRadius);

		if (nearThisDoor) {
			nearAnyDoor = true; // accumulate result

			// Handle door animation when player approaches/leaves
			if (!door.isLocked && !door.isOpen && door.anim.playMode == ANIM_IDLE) {
				animSystem::play(door.anim, ANIM_PLAY_ONCE);
			}

			// Handle E key transition
			if (nearThisDoor && AEInputCheckTriggered(AEVK_E)) {
				if (door.isLocked && !keycardCollected2) {
					std::cout << "Door is locked!" << std::endl;
					AEAudioPlay(Error, soundEffects, 1.f, 1.f, 0);
				}
				else {
					int toLevel = (currentGameLevel == door.entranceLevel) ? door.exitLevel : door.entranceLevel;

					// Save current stats as a checkpoint before leaving the level
					savedAmmo      = movement::bulletCount;
					savedWireCount = wireCount;
					savedHealth    = objectinfo2[player].health;

					playerEnteredDoor2 = true;
					playerEnteredDoorId = door.id; // remember which door was used
					switch (toLevel) {
					case 0: next = GS_TUTORIAL; break;
					case 1: next = GS_LEVEL1;   break;
					case 2: next = GS_LEVEL2;   break;
					case 3: next = GS_LEVEL3;   break;
					}
				}
			}
		}
		else {
			if (door.isOpen && door.anim.playMode == ANIM_IDLE)
				animSystem::play(door.anim, ANIM_PLAY_REVERSE);
		}

		// Always update animation state
		animSystem::update(door.anim, dt);

		if (door.anim.justFinished)
			door.isOpen = (door.anim.currentFrame != 0);
	}

	// Collect health pack
	objectsquares healthObj;
	healthObj.xPos = hp.worldX;
	healthObj.yPos = hp.worldY;
	healthObj.xScale = hp.size; healthObj.yScale = hp.size;

	if (hp.active && gamelogic::static_collision(&objectinfo2[player], &healthObj)) {
		hp.active = false;
		hp.collected = true;
		objectinfo2[player].health += rand() % 31;
		AEAudioPlay(Pickup, soundEffects, 1, 1, 0);
	}

	// After loop, set global flag for rendering
	playerNear = nearAnyDoor;


	// justFinished is true for one frame when a one-shot completes
	if (doorAnim.justFinished)
		doorIsOpen = (doorAnim.currentFrame == DOOR_FRAME_COUNT - 1);
	// -----------------------------------------------------------------------

	// MUSHROOM ANIMATION
	animSystem::update(meleeAnim, dt);

	// Checks if player is on the keycard
	objectsquares keyObj;
	keyObj.xPos = key.worldX;
	keyObj.yPos = key.worldY;
	keyObj.xScale = key.size;
	keyObj.yScale = key.size;

	if (key.active && gamelogic::static_collision(&objectinfo2[player], &keyObj)) {
		key.active = false;
		keycardCollected2 = true;
		AEAudioPlay(Pickup, soundEffects, 1, 1, 0);

		for (auto& door : doors) {
			door.isLocked = false;
		}
	}

	// Ensure doors stay unlocked once the keycard is collected
	if (keycardCollected2) {
		for (auto& door : doors) {
			door.isLocked = false;
		}
	}
}

void Level2_Draw()
{
	//AEGfxSetBackgroundColor(0.2f, 0.2f, 0.3f);  // Dark blue-gray background
	renderlogic::drawTexture(0.f, 0.f, backgroundTex, uiMesh, static_cast<f32>(AEGfxGetWindowWidth()), static_cast<f32>(AEGfxGetWindowHeight()));

	// Setup for textured rendering
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(1.0f);

	// ===== RENDER WALLS ======= //
	renderlogic::drawMapWallFloor(map, x, y, static_cast<int>(tileSize));

	// ====== RENDERING PADLOCK ====== //
	for (auto& door : doors) {
		if (door.isLocked) {
			// Draw padlock texture at the door�s position
			renderlogic::drawTexture(door.worldX, door.worldY, padlock, uiMesh, 50.f, 50.f);
		}
	}

	// ==== ENEMIES RENDER =======//
	enemySystem::renderEnemies(enemies,
		MAX_ENEMIES,
		AssetManager::GetMesh(MESH_MELEE_ENEMY),
		AssetManager::GetMesh(MESH_QUAD),
		AssetManager::GetTexture(TEX_MUSHROOM_IDLE_SHEET),
		AssetManager::GetTexture(TEX_RANGED_ENEMY),
		animSystem::getUOffset(meleeAnim),
		animSystem::getVOffset(meleeAnim));

	// Render enemy projectiles with plasma texture
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
	projectileSystem::renderProjectiles(enemyProjectiles, MAX_PROJECTILES, plasma, AssetManager::GetMesh(MESH_QUAD));

	pickup::drawDrops(L2Drop, MAX_ENEMIES);
	pickup::DrawWireDrops(wireDrops, MAX_ENEMIES);
	pickup::DrawAmmoPool();

	//====== PLAYER RENDER =========//
	// Reset render state so leftover color tints from enemies/projectiles don't affect the player
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxTextureSet(characterPictest, 0, 0);
	// Flip sprite horizontally when the player is aiming left.
	f32 playerDrawScaleX = movement::playerFacingLeft
		? -objectinfo2[player].xScale
		:  objectinfo2[player].xScale;
	renderlogic::drawSquare(objectinfo2[player].xPos, objectinfo2[player].yPos,
		playerDrawScaleX, objectinfo2[player].yScale);
	AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
	weaponSprite::Draw();

	// Render player projectiles with plasma texture
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
	projectileSystem::renderProjectiles(Projectiles, MAX_PROJECTILES, plasma, AssetManager::GetMesh(MESH_QUAD));

	// ====== PARTICLE SYSTEM RENDER ====== //
	// Draw particles behind UI elements but in front of projectiles
	ParticleSystem::Draw();

	// Reset render state after particles (they use RM_COLOR) so the
	// cooldown bar and HUD text render correctly with textures.
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);

	//====== PLAYER THRUST COOLDOWN BAR RENDER =========//
	renderlogic::drawCooldownHUD(objectinfo2[player].xPos, objectinfo2[player].yPos - 40.f);

	// ====== HUD: Health, Ammo, Weapon, Gravity ======//
	// Drawn last so it appears on top of all world geometry.
	// AEGfxPrint uses normalized coords: (-1,-1) = bottom-left, (1,1) = top-right.
	if (fontLevel2 >= 0)
	{
		// ---- Health icon + number (top-left) ----
		f32 halfW = static_cast<f32>(AEGfxGetWindowWidth())  * 0.5f;
		f32 halfH = static_cast<f32>(AEGfxGetWindowHeight()) * 0.5f;

		// Health icon at top-left (world coords from normalized -0.93, 0.88)
		f32 healthIconX = -halfW + 40.f;   // 40px from left edge
		f32 healthIconY = halfH - 40.f;   // 40px from top edge
		AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
		AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
		renderlogic::drawTexture(healthIconX, healthIconY, healthDrop, uiMesh, 70.f, 70.f);

		// Health number text right of the icon
		char healthText[32];
		snprintf(healthText, sizeof(healthText), "%d", objectinfo2[player].health);
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
		AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		AEGfxPrint(fontLevel2, healthText, -0.88f, 0.88f, 0.8f, 0.0f, 1.0f, 0.0f, 1.0f);

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
		AEGfxPrint(fontLevel2, ammoText, -0.59f, 0.88f, 0.8f, 1.0f, 1.0f, 0.0f, 1.0f);

		// ---- Current weapon sprite (below health row) ----
		// Show the sprite of the currently equipped weapon
		AEGfxTexture* weaponIcon = (objectinfo2[player].currentWeapon == WEAPON_SHOTGUN)
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
				AEGfxPrint(fontLevel2, gravityBuf, -0.25f, 0.90f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f);
			}
			else {
				sprintf_s(gravityBuf, sizeof(gravityBuf), "Gravity: ON");
				AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
				AEGfxPrint(fontLevel2, gravityBuf, -0.12f, 0.90f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f);
			}
		}
	}

	// ====== HARDCODED TILES AT THE BOTTOM ====== //
	renderlogic::drawTileArray();

	// ====== FLASHING E BUTTON WHEN PLAYER IS NEAR A DOOR ====== //
	if (playerNear) {
		renderlogic::flashingTexture(objectinfo2[player].xPos, objectinfo2[player].yPos + 60.f, eButton, 50.f);
	}
	// ====== WIRE INVENTORY (shows wire count 0-3) ====== //
	renderlogic::drawWireInventory(wireCount);

	// ====== DISPLAY KEYCARD IN INVENTORY ====== //
	if (playerEnteredDoor2) {
		renderlogic::drawTexture(-750.f, -400.f, inventory, uiMesh, 100.f, 100.f);
	}
	else {
		if (keycardCollected2) {
			renderlogic::drawTexture(-750.f, -400.f, keycardInventory, uiMesh, 100.f, 100.f);
			for (auto& door : doors) {
				door.isLocked = false;
			}
		}
		else {
			renderlogic::drawTexture(-750.f, -400.f, inventory, uiMesh, 100.f, 100.f);
		}
	}

	// Draw the "?" icon (or the full overlay if it is open) on top of everything
	InstructionsMenu::Draw();
	traps::drawTraps();
}

void Level2_Free()
{
	// Unload all AssetManager-tracked textures
	AssetManager::FreeAllMeshes();

	if (map) {
		delete[] map;
		map = nullptr;
	}

	FreeMapData();
}

void Level2_Unload()
{
	InstructionsMenu::Unload();

	// Unload all AssetManager-tracked textures (auto-nulls internal sTextures[]).
	AssetManager::UnloadAllTextures();

	// Safely null out mushroom textures (AssetManager already freed the memory)
	for (int i = 0; i < 9; ++i) { mushroomDieTexture[i] = nullptr; }
	for (int i = 0; i < 5; ++i) { mushroomHitTexture[i] = nullptr; }
	for (int i = 0; i < 9; ++i) { mushroomIdleTexture[i] = nullptr; }
	weaponSprite::Unload();
	// Platform and UI textures are already freed by AssetManager::UnloadAllTextures() above.

	if (glassMap) {
		for (int i = 0; i < BINARY_MAP_HEIGHT; ++i) delete[] glassMap[i];
		delete[] glassMap;
		glassMap = nullptr;
	}

	// Destroy the font created in Load
	if (fontLevel2 != -1) { AEGfxDestroyFont(fontLevel2); fontLevel2 = -1; }

	// Free the particle system mesh
	ParticleSystem::Unload();

	// Unload ALL audio resources that were loaded in Load

	audio::unloadsound();
}
