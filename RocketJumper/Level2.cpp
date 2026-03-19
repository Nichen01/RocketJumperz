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

// External libraries included in header file
#include "Level2.h"

static s32* map = nullptr;
static int x = 16;
static int y = 9;
static int s = 80;

// Player sprite render size in world units (half a tile -- proportional to 30x30 enemies)
const float PlayerScale = 80.0f;

// Font handle for in-game text rendering 
static s8 font = -1;

objectsquares objectinfo2[2] = { 0 };
drop L2Drop[MAX_ENEMIES] = { 0 };

// Local variables for projectile test level
static Projectile Projectiles[MAX_PROJECTILES];

// ENEMY DATA
static Enemy enemies[MAX_ENEMIES];
static Projectile enemyProjectiles[MAX_PROJECTILES];
static AEGfxTexture* meleeEnemyTexture = nullptr;
static AEGfxTexture* rangedEnemyTexture = nullptr;
static AEGfxTexture* doorTexture = nullptr;

// Font resource (must be destroyed in Unload to avoid leak)
static s8 fontLevel1 = -1;

// Tracks whether the player is near a door (used in Draw for flashing E prompt)
static bool playerNear = false;

//==== sound and volume
static f32 bgVolume = 1.f;

// Door variables (doorX, doorY, doorAnim, doorMesh, doorIsOpen, doorTex)
// are defined in draw.cpp and declared extern in draw.h.
// The constants below are Level1-specific door animation parameters.
static s32  DOOR_FRAME_COUNT = 7;
static f32  DOOR_FRAME_DELAY = 0.08f;   // ~12 fps


//ANIMATION
SpriteAnimation meleeAnim;

// Note: characterPictest, base5test, and pMesh are defined in draw.cpp. access them through draw.h

void Level2_Load()
{
	aiming::loadAiming();
	audio::loadsound();

	// LOADING TEXTURES
	AssetManager::LoadTexture(TEX_PLAYER, "Assets/astronautRight.png");
	AssetManager::LoadTexture(TEX_PLASMA, "Assets/plasma.png");
	AssetManager::LoadTexture(TEX_MELEE_ENEMY, "Assets/Enemy/MushroomIdle/mushroomIdle.png");
	AssetManager::LoadTexture(TEX_RANGED_ENEMY, "Assets/RangedEnemy.png");
	AssetManager::LoadTexture(TEX_RANGED_MOVE,   "Assets/Enemy/RangedMove.png");
	AssetManager::LoadTexture(TEX_RANGED_ATTACK,  "Assets/Enemy/RangedAttack.png");
	AssetManager::LoadTexture(TEX_RANGED_DEATH,   "Assets/Enemy/RangedDeath.png");
	AssetManager::LoadTexture(TEX_RANGED_HURT,    "Assets/Enemy/RangedHurt.png");
	AssetManager::LoadTexture(TEX_DOOR, "Assets/DoorOpen.png");

	// Sync the extern pointers so other files can use them directly
	characterPictest  = AssetManager::GetTexture(TEX_PLAYER);
	plasma            = AssetManager::GetTexture(TEX_PLASMA);
	meleeEnemyTexture = AssetManager::GetTexture(TEX_MELEE_ENEMY);
	rangedEnemyTexture = AssetManager::GetTexture(TEX_RANGED_ENEMY);
	doorTex           = AssetManager::GetTexture(TEX_DOOR);

	load::platform();
	load::ui();

	// Create font for HUD text (stored so we can destroy it in Unload)
	fontLevel1 = AEGfxCreateFont("Assets/Fonts/gameover.ttf", 72);
}

void Level2_Initialize()
{
	currentGameLevel = 2;

	AEAudioPlay(Level, bgm, 0.5f, 1.f, -1);


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
	for (auto& door : doors) {
		if (door.id == playerEnteredDoorId) {
			objectinfo2[player].xPos = door.worldX;
			objectinfo2[player].yPos = door.worldY; // slight offset so player isn't inside door
			spawnSet = true;
			break;
		}
	}
	// fallback if no door found (first time loading)
	if (!spawnSet) {
		objectinfo2[player].xPos = 0.f;
		objectinfo2[player].yPos = 0.f;
	}
	objectinfo2[player].xScale = PlayerScale;
	objectinfo2[player].yScale = PlayerScale;

	// Initialize player health to 100 HP with no invincibility active
	InitPlayerHealth(objectinfo2[player]);

	//======== INIT ENEMIES DATA =======================//
	// Initialize enemy system
	enemySystem::initEnemies(enemies, MAX_ENEMIES);
	projectileSystem::initProjectiles(enemyProjectiles, MAX_PROJECTILES);

	

	// SPAWN test enemies
	enemySystem::spawnEnemy(enemies, MAX_ENEMIES, ENEMY_MELEE, -200.0f, 100.0f);
	enemySystem::spawnEnemy(enemies, MAX_ENEMIES, ENEMY_RANGED, 300.0f, -100.0f);

	// Initialize melee enemy animation (3 cols, 2 rows, 6 frames at 10 fps, looping)
	animSystem::init(meleeAnim, 3, 2, 6, 0.1f, ANIM_LOOP, 0);

	// Build spritesheet meshes for ranged enemy states (rows, cols)
	AssetManager::BuildSqrMesh(MESH_RANGED_MOVE,   1, 4);
	AssetManager::BuildSqrMesh(MESH_RANGED_ATTACK,  1, 6);
	AssetManager::BuildSqrMesh(MESH_RANGED_DEATH,   1, 4);
	AssetManager::BuildSqrMesh(MESH_RANGED_HURT,    1, 2);

	// Custom door mesh via AssetManager
	AssetManager::BuildSqrMesh(MESH_DOOR, 1, 7);
	doorMesh = AssetManager::GetMesh(MESH_DOOR);
	pickup::initDrops(L2Drop, MAX_ENEMIES, PlayerScale);
}

void Level2_Update()
{
	//====== TOGGLE LEVEL EDITOR GAME STATE ======//
	if (AEInputCheckTriggered(AEVK_L)) {
		next = GS_LEVELEDITOR;
	}

	// ====== AUDIO CONTROLS ====== //
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
	aiming::updateAiming(objectinfo2[player]);
	pickup::updateDrops(L2Drop, MAX_ENEMIES, objectinfo2[player]);
	//===================================================//

	// ========== PROJECTILE SYSTEM UPDATE =============//
	if (movement::bulletCount) {
		projectileSystem::fireProjectiles(
			static_cast<s32>(worldMouseX),
			static_cast<s32>(worldMouseY),
			objectinfo2[player],
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
		objectinfo2[player],L2Drop,
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

	// If player health < 0, go to death screen
	if (objectinfo2[player].health <= 0) {
		next = GS_DEATH;
	}


	gamelogic::Collision_movement(&enemies[0].shape, map, x, s, 1);
	gamelogic::Collision_movement(&enemies[1].shape, map, x, s, 1);
	gamelogic::Collision_movement(&objectinfo2[player], map, x, s, 1);

	// -----------------------------------------------------------------------
	// Door animation
	// -----------------------------------------------------------------------

	bool nearAnyDoor = false; // track if player is near at least one door

	for (auto& door : doors) {
		// Only process doors connected to this level
		if (door.firstLevel != 1 && door.secondLevel != 1)
			continue;

		f32 dx = objectinfo2[player].xPos - door.worldX;
		f32 dy = objectinfo2[player].yPos - door.worldY;
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
				int toLevel = (currentGameLevel == door.firstLevel) ? door.secondLevel : door.firstLevel;
				playerEnteredDoorId = door.id; // remember which door was used
				switch (toLevel) {
				case 0: next = GS_TUTORIAL; break;
				case 1: next = GS_LEVEL1;   break;
				case 2: next = GS_LEVEL2;   break;
				}
			}
		}

		// Always update animation state
		animSystem::update(door.anim, dt);

		if (door.anim.justFinished)
			door.isOpen = (door.anim.currentFrame != 0);
	}

	// After loop, set global flag for rendering
	playerNear = nearAnyDoor;


	// justFinished is true for one frame when a one-shot completes
	if (doorAnim.justFinished)
		doorIsOpen = (doorAnim.currentFrame == DOOR_FRAME_COUNT - 1);
	// -----------------------------------------------------------------------

	// MUSHROOM ANIMATION
	animSystem::update(meleeAnim, dt);
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

	// ===== RENDER WALLS ======= //
	renderlogic::drawMapWallFloor(map, x, y, s);

	// ==== ENEMIES RENDER =======//
	// Render enemies
	enemySystem::renderEnemies(enemies,
		MAX_ENEMIES,
		AssetManager::GetMesh(MESH_MELEE_ENEMY),
		projectileMesh,
		meleeEnemyTexture,
		rangedEnemyTexture,
		animSystem::getUOffset(meleeAnim),
		animSystem::getVOffset(meleeAnim));

	// Render enemy projectiles with plasma texture
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
	projectileSystem::renderProjectiles(enemyProjectiles, MAX_PROJECTILES, plasma, projectileMesh);

	pickup::drawDrops(L2Drop, MAX_ENEMIES);

	//====== PLAYER RENDER =========//
	// Reset render state so leftover color tints from enemies/projectiles don't affect the player
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxTextureSet(characterPictest, 0, 0);
	renderlogic::drawSquare(objectinfo2[player].xPos, objectinfo2[player].yPos,
		objectinfo2[player].xScale, objectinfo2[player].yScale);
	AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
	aiming::drawAiming();

	// Render player projectiles with plasma texture
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
	projectileSystem::renderProjectiles(Projectiles, MAX_PROJECTILES, plasma, projectileMesh);

	// ====== HUD: Player Health Display ======
	// Drawn last so it appears on top of all world geometry.
	// AEGfxPrint uses normalized coords: (-1,-1) = bottom-left, (1,1) = top-right.
	if (fontLevel1 >= 0)
	{
		char healthText[32];
		snprintf(healthText, sizeof(healthText), "Health: %d", objectinfo2[player].health);

		// Prepare render state for font (font uses a glyph texture atlas)
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
		AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);

		// Print at top-left corner of the screen (white text)
		AEGfxPrint(fontLevel1, healthText, -0.95f, 0.85f, 0.8f, 1.0f, 1.0f, 1.0f, 1.0f);

	}
	renderlogic::drawTileArray();

	// ===== RENDERING OF THE FLASHING 'E' ===== //
	if (playerNear) {
		renderlogic::flashingTexture(objectinfo2[player].xPos, objectinfo2[player].yPos + 60.f, eButton, 50.f);
	}
}

void Level2_Free()
{
	AssetManager::FreeAllMeshes();

	doorMesh = nullptr;
	pMesh = nullptr;
	platformMesh = nullptr;
	enemyMesh = nullptr;
	projectileMesh = nullptr;

	if (map) {
		delete[] map;
		map = nullptr;
	}

	FreeMapData();
}

void Level2_Unload()
{
	// Unload ALL textures that were loaded in Initialize
	AssetManager::UnloadAllTextures();

	characterPictest = nullptr;
	plasma = nullptr;
	meleeEnemyTexture = nullptr;
	rangedEnemyTexture = nullptr;
	doorTex = nullptr;
	

	if (glassMap) {
		for (int i = 0; i < BINARY_MAP_HEIGHT; ++i) delete[] glassMap[i];
		delete[] glassMap;
		glassMap = nullptr;
	}


	// Destroy the font created in Initialize
	if (fontLevel1 != -1) { AEGfxDestroyFont(fontLevel1); fontLevel1 = -1; }

	// Unload ALL audio resources that were loaded in Load
	audio::unloadsound();
	aiming::unloadAiming();
}
