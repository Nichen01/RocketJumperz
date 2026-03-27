
// External libraries are included in header file
#include "Level3.h"

static s32* map = nullptr;
static int x;
static int y;

// Player sprite render size in world units (half a tile -- proportional to 30x30 enemies)
const float PlayerScale = 80.0f;

extern objectsquares objectinfo3[2] = { 0 };
drop L3Drop[MAX_ENEMIES] = { 0 };

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
// The active font handle is fontLevel3 (declared below), which is
// created in Initialize and destroyed in Unload.

// Sound and volume
static f32 bgVolume = 1.f;

// Door variables (doorX, doorY, doorAnim, doorMesh, doorIsOpen, doorTex)
// are defined in draw.cpp and declared extern in draw.h.
// The constants below are Level1-specific door animation parameters.
static s32  DOOR_FRAME_COUNT = 7;
static f32  DOOR_FRAME_DELAY = 0.08f;   // ~12 fps

// Font resource (must be destroyed in Unload to avoid leak)
static s8 fontLevel3 = -1;

// bool for checking player proximity with door
static bool playerNear;
static bool playerNearBrokenDoor;

// bool for keycard in inventory
static bool healthCollected;
static bool keycardCollected;
static bool keycardCollectedAudio = false;

// Note: characterPictest, base5test, and pMesh are defined in draw.cpp. access them through draw.h

void Level3_Load()
{
	audio::loadsound();

	// Load wire item texture (world drop) and wire inventory textures
	AssetManager::LoadTexture(TEX_WIRE, "Assets/Items/wire.png");
	load::wireInventory();
	load::brokenDoor();	// Load broken door(s) for final door
	load::platform();	// Load platform tile textures
	load::ui();			// Load UI textures (eButton used by flashing door prompt in Draw)
	weaponSprite::Load();
	load::cooldownBar();
	load::background();

	// Load textures via AssetManager (prevents duplicate loads across level reloads)
	AssetManager::LoadTexture(TEX_PLAYER, "Assets/charactertest.png");
	AssetManager::LoadTexture(TEX_BASE5TEST, "Assets/Base5.png");
	AssetManager::LoadTexture(TEX_PLASMA, "Assets/plasma.png");
	AssetManager::LoadTexture(TEX_DOOR, "Assets/DoorOpen.png");
	AssetManager::LoadTexture(TEX_MUSHROOM_IDLE_SHEET, "Assets/Enemy/MushroomIdle/MushroomIdle.png");
	AssetManager::LoadTexture(TEX_RANGED_ENEMY, "Assets/RangedEnemy.png");
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
	fontLevel3 = AEGfxCreateFont("Assets/Fonts/gameover.ttf", 72);
	aiming::loadAiming();
}

void Level3_Initialize()
{
	characterPictest = AssetManager::GetTexture(TEX_PLAYER);
	base5test = AssetManager::GetTexture(TEX_BASE5TEST);
	plasma = AssetManager::GetTexture(TEX_PLASMA);
	doorTex = AssetManager::GetTexture(TEX_DOOR);
	keyTexture = AssetManager::GetTexture(TEX_KEYCARD);
	currentGameLevel = 3;

	AEAudioPlay(Level, bgm, 0.5f, 1.f, -1);

	// Font is already created in Level3_Load -- do NOT recreate here.
	// Recreating would leak the previous font handle each time the level reinitializes.

	// Initialize player movement system
	movement::initPlayerMovement(objectinfo3[player]);

	// Added after obstacle initialization:
	projectileSystem::initProjectiles(Projectiles, MAX_PROJECTILES);

	//=============CREATE TEXTURED MESH FOR ALL STANDARD OBJECTS==================//
	AssetManager::BuildSqrMesh(MESH_QUAD);
	pMesh        = AssetManager::GetMesh(MESH_QUAD);
	platformMesh = AssetManager::GetMesh(MESH_QUAD);
	pTestMesh    = AssetManager::GetMesh(MESH_QUAD);
	uiMesh       = AssetManager::GetMesh(MESH_QUAD);

	if (!ImportMapDataFromFile("Assets/Map/Level3_Map.txt")) {
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
		// Lock all doors except for Tutorial -> Level 1
		door.isLocked = true;
		if (door.id == 21) {
			door.isLocked = false;
		}

		if (door.id == playerEnteredDoorId) {
			objectinfo3[player].xPos = door.worldX;
			objectinfo3[player].yPos = door.worldY;
			spawnSet = true;
			break;
		}
	}
	// fallback if no door found (first time loading)
	if (!spawnSet) {
		objectinfo3[player].xPos = 0.f;
		objectinfo3[player].yPos = 0.f;
	}
	objectinfo3[player].xScale = PlayerScale;
	objectinfo3[player].yScale = PlayerScale;

	// Initialize player health to 100 HP with no invincibility active
	InitPlayerHealth(objectinfo3[player]);

	// Start with the plasma gun equipped (default weapon)
	objectinfo3[player].currentWeapon = WEAPON_PLASMA;

	//======== INIT ENEMIES DATA =======================//
	// Initialize enemy system
	enemySystem::initEnemies(enemies, MAX_ENEMIES);
	projectileSystem::initProjectiles(enemyProjectiles, MAX_PROJECTILES);

	// SPAWN test enemies
	enemySystem::spawnEnemy(enemies, MAX_ENEMIES, ENEMY_MELEE, enemy1X, enemy1Y);
	enemySystem::spawnEnemy(enemies, MAX_ENEMIES, ENEMY_RANGED, enemy2X, enemy2Y);

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
	pickup::initDrops(L3Drop, MAX_ENEMIES, PlayerScale);

	// Wire drops: reset per-level tracker and initialize wire drop array
	pickup::ResetWireDropTracker();
	pickup::InitWireDrops(wireDrops, MAX_ENEMIES, PlayerScale);
}

void Level3_Update()
{
	//====== TOGGLE LEVEL EDITOR GAME STATE ======//
	if (AEInputCheckTriggered(AEVK_L)) {
		level = 3;
		next = GS_LEVELEDITOR;
	}

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

	//========== JETPACK MOVEMENT SYSTEM ===============//
	//Apply thrust when spacebar is pressed
	movement::physicsInput(objectinfo3[player]);

	if (AEInputCheckTriggered(AEVK_ESCAPE)) {
		next = GS_QUIT;
	}

	// ---- Weapon Toggle (Q key) ----
	// Press Q to switch between Plasma (single shot) and Shotgun (spread).
	if (AEInputCheckTriggered(AEVK_Q)) {
		if (objectinfo3[player].currentWeapon == WEAPON_PLASMA) {
			objectinfo3[player].currentWeapon = WEAPON_SHOTGUN;
			printf("Weapon switched to: SHOTGUN\n");
		}
		else {
			objectinfo3[player].currentWeapon = WEAPON_PLASMA;
			printf("Weapon switched to: PLASMA\n");
		}
	}

	//===========  APPLY PHYSICS(DRAG)===================//
	// Update player physics (drag + position)
	movement::updatePlayerPhysics(objectinfo3[player]);
	movement::UpdatePlayerFacing(objectinfo3[player]);
	aiming::updateAiming(objectinfo3[player]);
	weaponSprite::Update(objectinfo3[player]);
	pickup::updateDrops(L3Drop, MAX_ENEMIES, objectinfo3[player]);
	pickup::UpdateWireDrops(wireDrops, MAX_ENEMIES, objectinfo3[player]);
	//===================================================//

	// ========== PROJECTILE SYSTEM UPDATE =============//
	// Fire using the currently equipped weapon (toggled with Q)
	if (movement::bulletCount) {
		if (objectinfo3[player].currentWeapon == WEAPON_SHOTGUN) {
			// Shotgun: 5-pellet cone spread
			projectileSystem::FireShotgun(
				static_cast<s32>(worldMouseX),
				static_cast<s32>(worldMouseY),
				objectinfo3[player],
				Projectiles,
				MAX_PROJECTILES,
				LaserBlast,
				soundEffects);
		}
		else {
			// Plasma (default): single shot toward mouse
			projectileSystem::fireProjectiles(
				static_cast<s32>(worldMouseX),
				static_cast<s32>(worldMouseY),
				objectinfo3[player],
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

	// Update enemies (pass wireDrops so dead enemies can spawn wire items)
	enemySystem::updateEnemies(enemies, MAX_ENEMIES,
		objectinfo3[player], L3Drop,
		enemyProjectiles, MAX_PROJECTILES,
		dt, LaserBlast, soundEffects,
		wireDrops, MAX_ENEMIES);

	// Update enemy projectiles
	projectileSystem::UpdateProjectiles(enemyProjectiles, MAX_PROJECTILES, map, x, static_cast<int>(tileSize));

	// Tick down the player's invincibility timer each frame
	UpdatePlayerInvincibility(objectinfo3[player], dt);

	// Check player projectiles hitting enemies
	enemySystem::checkProjectileEnemyCollision(enemies, MAX_ENEMIES,
		Projectiles, MAX_PROJECTILES);

	// Check melee enemies damaging player (uses PlayerTakeDamage internally)
	enemySystem::checkPlayerEnemyCollision(enemies, MAX_ENEMIES,
		objectinfo3[player], Punch, soundEffects);

	// Check ranged enemy projectiles hitting player (uses PlayerTakeDamage internally)
	enemySystem::checkEnemyPlayerProjectileCollision(
		enemyProjectiles, MAX_PROJECTILES, objectinfo3[player]);

	// If player health < 0, go to death screen
	if (objectinfo3[player].health <= 0) {
		next = GS_DEATH;
	}

	gamelogic::Collision_movement(&enemies[0].shape, map, x, static_cast<int>(tileSize), 1);
	gamelogic::Collision_movement(&enemies[1].shape, map, x, static_cast<int>(tileSize), 1);
	gamelogic::Collision_movement(&objectinfo3[player], map, x, static_cast<int>(tileSize), 1);

	if (AEInputCheckCurr(AEVK_3)) wireCount = 3;

	// -----------------------------------------------------------------------
	// Door animation
	// -----------------------------------------------------------------------

	bool nearAnyDoor = false; // track if player is near at least one door

	for (auto& door : doors) {
		// Only process doors connected to this level
		if (door.entranceLevel != currentGameLevel && door.exitLevel != currentGameLevel)
			continue;

		f32 dx = objectinfo3[player].xPos - door.worldX;
		f32 dy = objectinfo3[player].yPos - door.worldY;
		f32 dist = sqrtf(dx * dx + dy * dy);

		bool nearThisDoor = (dist <= doorTriggerRadius);

		if (nearThisDoor) {
			nearAnyDoor = true; // accumulate result

			// Handle door animation when player approaches/leaves
			if (!door.isOpen && door.anim.playMode == ANIM_IDLE)
				animSystem::play(door.anim, ANIM_PLAY_ONCE);

			// Handle E key transition
			if (door.isOpen && AEInputCheckTriggered(AEVK_E)) {
				if (door.isLocked && !keycardCollected) {
					std::cout << "Door is locked!" << std::endl;
					AEAudioPlay(Error, soundEffects, 1.f, 1.f, 0);
				}
				else {
					int toLevel = (currentGameLevel == door.entranceLevel) ? door.exitLevel : door.entranceLevel;
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

	// Check if player is near the broken door
		f32 dx = objectinfo3[player].xPos - finalDoor.worldX;
		f32 dy = objectinfo3[player].yPos - finalDoor.worldY;
		f32 dist = sqrtf(dx * dx + dy * dy);

		playerNearBrokenDoor = (dist <= doorTriggerRadius);


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

	if (key.active && gamelogic::static_collision(&objectinfo3[player], &keyObj)) {
		key.active = false;
		keycardCollected = true;
		AEAudioPlay(Pickup, soundEffects, 1, 1, 0);
	}

	//========== DOOR RENDERING BASED ON WIRE COUNT ==========//
	// Show progressively repaired door as the player collects wires
	if (wireCount <= 0) {
		renderlogic::drawTexture(finalDoor.worldX, finalDoor.worldY, brokenDoor0, uiMesh, tileSize, tileSize);
	}
	else if (wireCount == 1) {
		renderlogic::drawTexture(finalDoor.worldX, finalDoor.worldY, brokenDoor1, uiMesh, tileSize, tileSize);
	}
	else {
		// 2 or 3 wires: show most-repaired door texture
		renderlogic::drawTexture(finalDoor.worldX, finalDoor.worldY, brokenDoor2, uiMesh, tileSize, tileSize);
	}

	// Player tries to use the final broken door.
	// The door requires exactly 3 wires to be accessible.
	// If the player has fewer than 3, play an error sound.
	if (AEInputCheckTriggered(AEVK_E) && playerNearBrokenDoor) {
		if (wireCount >= 3) {
			// Door is accessible -- transition to victory
			next = GS_VICTORY;
		}
		else {
			// Not enough wires -- play error sound to inform the player
			AEAudioPlay(Error, soundEffects, 1.f, 1.f, 0);
			printf("Need 3 wires to open the final door! Current: %d\n", wireCount);
		}
	}


}

void Level3_Draw()
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

	// ====== DRAW FINAL DOOR BASED ON WIRE COUNT ====== //
	// Visual repair progress tied to global wireCount
	if (wireCount <= 0) {
		renderlogic::drawTexture(finalDoor.worldX, finalDoor.worldY, brokenDoor0, uiMesh, tileSize, tileSize);
	}
	else if (wireCount == 1) {
		renderlogic::drawTexture(finalDoor.worldX, finalDoor.worldY, brokenDoor1, uiMesh, tileSize, tileSize);
	}
	else {
		renderlogic::drawTexture(finalDoor.worldX, finalDoor.worldY, brokenDoor2, uiMesh, tileSize, tileSize);
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

	pickup::drawDrops(L3Drop, MAX_ENEMIES);
	pickup::DrawWireDrops(wireDrops, MAX_ENEMIES);

	//====== PLAYER RENDER =========//
	// Reset render state so leftover color tints from enemies/projectiles don't affect the player
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxTextureSet(characterPictest, 0, 0);
	// Flip sprite horizontally when the player is aiming left.
	f32 playerDrawScaleX = movement::playerFacingLeft
		? -objectinfo3[player].xScale
		: objectinfo3[player].xScale;
	renderlogic::drawSquare(objectinfo3[player].xPos, objectinfo3[player].yPos,
		playerDrawScaleX, objectinfo3[player].yScale);
	AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
	//aiming::drawAiming();
	weaponSprite::Draw();

	// Render player projectiles with plasma texture
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
	projectileSystem::renderProjectiles(Projectiles, MAX_PROJECTILES, plasma, AssetManager::GetMesh(MESH_QUAD));

	//====== PLAYER THRUST COOLDOWN BAR RENDER =========//
	renderlogic::drawCooldownHUD(objectinfo3[player].xPos, objectinfo3[player].yPos - 40.f);

	// ====== HUD: Health, Ammo, Weapon, Gravity ======//
	// Drawn last so it appears on top of all world geometry.
	// AEGfxPrint uses normalized coords: (-1,-1) = bottom-left, (1,1) = top-right.
	if (fontLevel3 >= 0)
	{
		// ---- Health icon + number (top-left) ----
		f32 halfW = static_cast<f32>(AEGfxGetWindowWidth())  * 0.5f;
		f32 halfH = static_cast<f32>(AEGfxGetWindowHeight()) * 0.5f;

		f32 healthIconX = -halfW + 40.f;
		f32 healthIconY =  halfH - 40.f;
		AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
		AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
		renderlogic::drawTexture(healthIconX, healthIconY, healthDrop, uiMesh, 30.f, 30.f);

		char healthText[32];
		snprintf(healthText, sizeof(healthText), "%d", objectinfo3[player].health);
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
		AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		AEGfxPrint(fontLevel3, healthText, -0.88f, 0.88f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f);

		// ---- Ammo icon + count (right beside health) ----
		f32 ammoIconX = healthIconX + 130.f;
		f32 ammoIconY = healthIconY;
		AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
		AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
		renderlogic::drawTexture(ammoIconX, ammoIconY, ammoDrop, uiMesh, 30.f, 30.f);

		char ammoText[32];
		snprintf(ammoText, sizeof(ammoText), "%d", movement::bulletCount);
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
		AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		AEGfxPrint(fontLevel3, ammoText, -0.72f, 0.88f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f);

		// ---- Current weapon sprite (below health row) ----
		AEGfxTexture* weaponIcon = (objectinfo3[player].currentWeapon == WEAPON_SHOTGUN)
			? AssetManager::GetTexture(TEX_SHOTGUN)
			: AssetManager::GetTexture(TEX_PLASMA_GUN);
		f32 weaponIconX = healthIconX;
		f32 weaponIconY = healthIconY - 40.f;
		AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
		AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
		renderlogic::drawTexture(weaponIconX, weaponIconY, weaponIcon, uiMesh, 50.f, 30.f);

		// ---- Gravity indicator (top center) ----
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		if (movement::enableGravity) {
			AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
			AEGfxPrint(fontLevel3, "Gravity", -0.12f, 0.90f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f);
		}
		else {
			AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
			AEGfxPrint(fontLevel3, "Gravity", -0.12f, 0.90f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f);
		}
	}

	// ====== HARDCODED TILES AT THE BOTTOM ====== //
	renderlogic::drawTileArray();

	// ====== FLASHING E BUTTON WHEN PLAYER IS NEAR A DOOR ====== //
	if (playerNear) {
		renderlogic::flashingTexture(objectinfo3[player].xPos, objectinfo3[player].yPos + 60.f, eButton, 50.f);
	}

	// ====== WIRE INVENTORY (shows wire count 0-3) ====== //
	renderlogic::drawWireInventory(wireCount);

	// Draw wire count text at the bottom-right of the wire inventory box
	if (fontLevel3 >= 0) {
		char wireText[16];
		snprintf(wireText, sizeof(wireText), "%d/3", wireCount);
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
		AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		AEGfxPrint(fontLevel3, wireText, -0.84f, -0.97f, 0.6f, 1.0f, 1.0f, 1.0f, 1.0f);
	}

	// ====== DISPLAY KEYCARD IN INVENTORY ====== //
	if (keycardCollected) {
		renderlogic::drawTexture(-750.f, -400.f, keycardInventory, uiMesh, 100.f, 100.f);
		for (auto& door : doors) {
			door.isLocked = false;
		}	
	}
	else {
		renderlogic::drawTexture(-750.f, -400.f, inventory, uiMesh, 100.f, 100.f);
	}

	if (playerNearBrokenDoor) {
		renderlogic::flashingTexture(finalDoor.worldX, finalDoor.worldY + 60.f, eButton, 50.f);
	}
}

void Level3_Free()
{
	// Unload all AssetManager-tracked textures
	AssetManager::FreeAllMeshes();

	if (map) {
		delete[] map;
		map = nullptr;
	}

	FreeMapData();
}

void Level3_Unload()
{
	// Unload all AssetManager-tracked textures (auto-nulls internal sTextures[]).
	AssetManager::UnloadAllTextures();

	// Safely null out mushroom textures (AssetManager already freed the memory)
	for (int i = 0; i < 9; ++i) { mushroomDieTexture[i] = nullptr; }
	for (int i = 0; i < 5; ++i) { mushroomHitTexture[i] = nullptr; }
	for (int i = 0; i < 9; ++i) { mushroomIdleTexture[i] = nullptr; }
	aiming::unloadAiming();
	weaponSprite::Unload();
	// Platform and UI textures are already freed by AssetManager::UnloadAllTextures() above.

	if (glassMap) {
		for (int i = 0; i < BINARY_MAP_HEIGHT; ++i) delete[] glassMap[i];
		delete[] glassMap;
		glassMap = nullptr;
	}

	// Destroy the font created in Load
	if (fontLevel3 != -1) { AEGfxDestroyFont(fontLevel3); fontLevel3 = -1; }

	// Unload ALL audio resources that were loaded in Load

	audio::unloadsound();
}
