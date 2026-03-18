
// External libraries are included in header file
#include "Level3.h"

static s32* map = nullptr;
static int x;
static int y;

// Player sprite render size in world units (half a tile -- proportional to 30x30 enemies)
const float PlayerScale = 80.0f;

extern objectsquares objectinfo3[2] = { 0 };
drop L3Drop[MAX_ENEMIES] = { 0 };

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
// The active font handle is fontLevel1 (declared below), which is
// created in Initialize and destroyed in Unload.

// Sound and volume
static f32 bgVolume = 1.f;

// Door variables (doorX, doorY, doorAnim, doorMesh, doorIsOpen, doorTex)
// are defined in draw.cpp and declared extern in draw.h.
// The constants below are Level1-specific door animation parameters.
static s32  DOOR_FRAME_COUNT = 7;
static f32  DOOR_FRAME_DELAY = 0.08f;   // ~12 fps

// Font resource (must be destroyed in Unload to avoid leak)
static s8 fontLevel1 = -1;

// bool for checking player proximity with door
static bool playerNear;
static bool playerNearBrokenDoor;

// bool for keycard in inventory
static bool keycardCollected;
static bool keycardCollectedAudio = false;

// Note: characterPictest, base5test, and pMesh are defined in draw.cpp. access them through draw.h

void Level3_Load()
{
	audio::loadsound();

	load::brokenDoor();	// Load broken door(s) for final door
	load::platform();	// Load platform tile textures
	load::ui();			// Load UI textures (eButton used by flashing door prompt in Draw)

	// Load textures via AssetManager (prevents duplicate loads across level reloads)
	AssetManager::LoadTexture(TEX_PLAYER, "Assets/astronautRight.png");
	AssetManager::LoadTexture(TEX_BASE5TEST, "Assets/Base5.png");
	AssetManager::LoadTexture(TEX_PLASMA, "Assets/plasma.png");
	AssetManager::LoadTexture(TEX_DOOR, "Assets/DoorOpen.png");
	AssetManager::LoadTexture(TEX_MUSHROOM_IDLE_SHEET, "Assets/Enemy/MushroomIdle/MushroomIdle.png");
	AssetManager::LoadTexture(TEX_RANGED_ENEMY, "Assets/RangedEnemy.png");
	AssetManager::LoadTexture(TEX_KEYCARD, "Assets/Items/keycard.png");

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
	fontLevel1 = AEGfxCreateFont("Assets/Fonts/gameover.ttf", 72);
	aiming::loadAiming();
}

void Level3_Initialize()
{
	characterPictest = AssetManager::GetTexture(TEX_PLAYER);
	base5test = AssetManager::GetTexture(TEX_BASE5TEST);
	plasma = AssetManager::GetTexture(TEX_PLASMA);
	doorTex = AssetManager::GetTexture(TEX_DOOR);
	keyTexture = AssetManager::GetTexture(TEX_KEYCARD);
	currentGameLevel = 1;

	AEAudioPlay(Level, bgm, 0.5f, 1.f, -1);

	// Create font for HUD text (stored so we can destroy it in Unload)
	fontLevel1 = AEGfxCreateFont("Assets/Fonts/gameover.ttf", 72);

	// Initialize player movement system
	movement::initPlayerMovement(objectinfo3[player]);

	// Added after obstacle initialization:
	projectileSystem::initProjectiles(Projectiles, MAX_PROJECTILES);

	//=============CREATE TEXTURED MESH FOR PLAYER==================//
	AssetManager::BuildSqrMesh(MESH_PLAYER);
	AssetManager::BuildSqrMesh(MESH_PLATFORM);
	AssetManager::BuildSqrMesh(MESH_TEST);
	AssetManager::BuildSqrMesh(MESH_UI);
	pMesh = AssetManager::GetMesh(MESH_PLAYER);
	platformMesh = AssetManager::GetMesh(MESH_PLATFORM);
	pTestMesh = AssetManager::GetMesh(MESH_TEST);
	uiMesh = AssetManager::GetMesh(MESH_UI);

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

	//======== INIT ENEMIES DATA =======================//
	// Initialize enemy system
	enemySystem::initEnemies(enemies, MAX_ENEMIES);
	projectileSystem::initProjectiles(enemyProjectiles, MAX_PROJECTILES);

	// SPAWN test enemies
	enemySystem::spawnEnemy(enemies, MAX_ENEMIES, ENEMY_MELEE, -200.0f, 100.0f);
	enemySystem::spawnEnemy(enemies, MAX_ENEMIES, ENEMY_RANGED, -100.0f, -200.0f);

	//MUSHROOM ANIM TEST
	{
		AEGfxVertexList* meleeEnemyMesh = nullptr;
		animSystem::buildMesh(&meleeEnemyMesh, 2, 3);
		AssetManager::StoreMesh(MESH_MELEE_ENEMY, meleeEnemyMesh);
	}
	animSystem::init(meleeAnim, 3, 2, 6, 0.1f, ANIM_LOOP, 0);

	// DOOR
	animSystem::buildMesh(&doorMesh, 1, 7);
	AssetManager::StoreMesh(MESH_DOOR, doorMesh);

	animSystem::init(doorAnim, 7, 1, DOOR_FRAME_COUNT, DOOR_FRAME_DELAY, ANIM_IDLE, 0);
	doorIsOpen = false;
	pickup::initDrops(L3Drop, MAX_ENEMIES, PlayerScale);
}

void Level3_Update()
{
	//====== TOGGLE LEVEL EDITOR GAME STATE ======//
	if (AEInputCheckTriggered(AEVK_L)) {
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

	if (AEInputCheckTriggered(AEVK_Q) || AEInputCheckTriggered(AEVK_ESCAPE)) {
		next = GS_QUIT;
	}

	//===========  APPLY PHYSICS(DRAG)===================//
	// Update player physics (drag + position)
	movement::updatePlayerPhysics(objectinfo3[player]);
	aiming::updateAiming(objectinfo3[player]);
	pickup::updateDrops(L3Drop, MAX_ENEMIES, objectinfo3[player]);
	//===================================================//

	// ========== PROJECTILE SYSTEM UPDATE =============//
	if (movement::bulletCount) {
		projectileSystem::fireProjectiles(
			static_cast<s32>(worldMouseX),
			static_cast<s32>(worldMouseY),
			objectinfo3[player],
			Projectiles,
			MAX_PROJECTILES,
			LaserBlast,
			soundEffects);
	}

	// Update all active projectiles
	projectileSystem::UpdateProjectiles(Projectiles, MAX_PROJECTILES);

	//============= UPDATE ENEMIES ===================/
	// Get delta time for enemy AI
	f32 dt = static_cast<f32>(AEFrameRateControllerGetFrameTime());

	// Update enemies
	enemySystem::updateEnemies(enemies, MAX_ENEMIES,
		objectinfo3[player], L3Drop,
		enemyProjectiles, MAX_PROJECTILES,
		dt, LaserBlast, soundEffects);

	// Update enemy projectiles
	projectileSystem::UpdateProjectiles(enemyProjectiles, MAX_PROJECTILES);

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

	// -----------------------------------------------------------------------
	// Door animation
	// -----------------------------------------------------------------------

	bool nearAnyDoor = false; // track if player is near at least one door

	for (auto& door : doors) {
		// Only process doors connected to this level
		if (door.entranceLevel != 1 && door.exitLevel != 1)
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

			if (door.isOpen && door.anim.playMode == ANIM_IDLE)
				animSystem::play(door.anim, ANIM_PLAY_REVERSE);

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

	//========== DOOR RENDERING BASED ON STATE ==========//
	if (finalDoor.state == 0) {
		renderlogic::drawTexture(finalDoor.worldX, finalDoor.worldY, brokenDoor0, uiMesh, tileSize, tileSize);
	}
	else if (finalDoor.state == 1) {
		renderlogic::drawTexture(finalDoor.worldX, finalDoor.worldY, brokenDoor1, uiMesh, tileSize, tileSize);
	}
	else {
		renderlogic::drawTexture(finalDoor.worldX, finalDoor.worldY, brokenDoor2, uiMesh, tileSize, tileSize);
	}

	//========== PLAYER FIXING BROKEN DOOR ==========//
	if (AEInputCheckTriggered(AEVK_E) && playerNearBrokenDoor && wireCount > 0) {
		finalDoor.state = wireCount - 1;
		wireCount--;
	}

	if (playerNearBrokenDoor && wireCount <= 0) {
		AEAudioPlay(Error, soundEffects, 1.f, 1.f, 0);
	}
}

void Level3_Draw()
{
	AEGfxSetBackgroundColor(0.2f, 0.2f, 0.3f);  // Dark blue-gray background

	// Setup for textured rendering
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(1.0f);

	// ===== RENDER WALLS ======= //
	renderlogic::drawMapWallFloor(map, x, y, static_cast<int>(tileSize));

	// ==== ENEMIES RENDER =======//
	enemySystem::renderEnemies(enemies,
		MAX_ENEMIES,
		AssetManager::GetMesh(MESH_MELEE_ENEMY),
		AssetManager::GetMesh(MESH_TEST),
		AssetManager::GetTexture(TEX_MUSHROOM_IDLE_SHEET),
		AssetManager::GetTexture(TEX_RANGED_ENEMY),
		animSystem::getUOffset(meleeAnim),
		animSystem::getVOffset(meleeAnim));

	// Render enemy projectiles with plasma texture
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
	projectileSystem::renderProjectiles(enemyProjectiles, MAX_PROJECTILES, plasma, AssetManager::GetMesh(MESH_TEST));

	pickup::drawDrops(L3Drop, MAX_ENEMIES);

	//====== PLAYER RENDER =========//
	// Reset render state so leftover color tints from enemies/projectiles don't affect the player
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxTextureSet(characterPictest, 0, 0);
	renderlogic::drawSquare(objectinfo3[player].xPos, objectinfo3[player].yPos,
		objectinfo3[player].xScale, objectinfo3[player].yScale);
	AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
	aiming::drawAiming();

	// Render player projectiles with plasma texture
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
	projectileSystem::renderProjectiles(Projectiles, MAX_PROJECTILES, plasma, AssetManager::GetMesh(MESH_TEST));

	// ====== HUD: Player Health Display ======//
	// Drawn last so it appears on top of all world geometry.
	// AEGfxPrint uses normalized coords: (-1,-1) = bottom-left, (1,1) = top-right.
	if (fontLevel1 >= 0)
	{
		char healthText[32];
		snprintf(healthText, sizeof(healthText), "Health: %d", objectinfo3[player].health);

		// Prepare render state for font (font uses a glyph texture atlas)
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
		AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);

		// Print at top-left corner of the screen (white text)
		AEGfxPrint(fontLevel1, healthText, -0.95f, 0.85f, 0.8f, 1.0f, 1.0f, 1.0f, 1.0f);

	}

	// ====== HARDCODED TILES AT THE BOTTOM ====== //
	renderlogic::drawTileArray();

	// ====== FLASHING E BUTTON WHEN PLAYER IS NEAR A DOOR ====== //
	if (playerNear) {
		renderlogic::flashingTexture(objectinfo3[player].xPos, objectinfo3[player].yPos + 60.f, eButton, 50.f);
	}

	// ====== PLACEHOLDER INVENTORY FOR WIRES ====== //
	renderlogic::drawWireInventory(wireCount);

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
	// Unload all AssetManager-tracked textures, then null the shared extern pointers.
	AssetManager::UnloadAllTextures();


	// Safely null out mushroom textures (AssetManager already freed the memory)
	for (int i = 0; i < 9; ++i) { mushroomDieTexture[i] = nullptr; }
	for (int i = 0; i < 5; ++i) { mushroomHitTexture[i] = nullptr; }
	for (int i = 0; i < 9; ++i) { mushroomIdleTexture[i] = nullptr; }
	aiming::unloadAiming();
	// Platform and UI textures are already freed by AssetManager::UnloadAllTextures() above.

	if (glassMap) {
		for (int i = 0; i < BINARY_MAP_HEIGHT; ++i) delete[] glassMap[i];
		delete[] glassMap;
		glassMap = nullptr;
	}

	// Destroy the font created in Load
	if (fontLevel1 != -1) { AEGfxDestroyFont(fontLevel1); fontLevel1 = -1; }

	// Unload ALL audio resources that were loaded in Load

	audio::unloadsound();
}
