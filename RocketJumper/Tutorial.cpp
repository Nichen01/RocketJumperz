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
#include "AssetManager.h"

static s32* map = nullptr;
static int x = 16;
static int y = 9;
static int s = 80;

// Player sprite render size in world units (half a tile -- proportional to 30x30 enemies)
const float PlayerScale = 40.0f;

static s8 font = -1;

objectsquares objectinfoTut[2] = { 0 };

// Local variables for projectile test level
static Projectile Projectiles[MAX_PROJECTILES];

// ENEMY DATA
static Enemy enemies[MAX_ENEMIES];
static Projectile enemyProjectiles[MAX_PROJECTILES];
static AEGfxTexture* meleeEnemyTexture = nullptr;
static AEGfxTexture* rangedEnemyTexture = nullptr;

//==== sound and volume
static f32 bgVolume = 1.f;

static char strBuffer[100];

static bool playerNear;

// Font resource (must be destroyed in Unload to avoid leak)
static s8 fontLevel1 = -1;

// Note: characterPictest, base5test, and pMesh are defined in draw.cpp. access them through draw.h

void Tutorial_Load()
{
	font = AEGfxCreateFont("Assets/Fonts/PressStart2P-Regular.ttf", 50);

	audio::loadsound();

	// Load textures via AssetManager (enum-based IDs)
	AssetManager::LoadTexture(TEX_PLAYER, "Assets/astronautRight.png");
	AssetManager::LoadTexture(TEX_PLASMA, "Assets/plasma.png");
	AssetManager::LoadTexture(TEX_DOOR, "Assets/DoorOpen.png");
	AssetManager::LoadTexture(TEX_MELEE_ENEMY, "Assets/MeleeEnemy.png");
	AssetManager::LoadTexture(TEX_RANGED_ENEMY, "Assets/RangedEnemy.png");

	// Sync the extern pointers so other files can use them directly
	characterPictest  = AssetManager::GetTexture(TEX_PLAYER);
	plasma            = AssetManager::GetTexture(TEX_PLASMA);
	doorTex           = AssetManager::GetTexture(TEX_DOOR);
	meleeEnemyTexture = AssetManager::GetTexture(TEX_MELEE_ENEMY);
	rangedEnemyTexture = AssetManager::GetTexture(TEX_RANGED_ENEMY);

	// Load platform assets
	load::platform();
	load::ui();

	// Create font for gameover text (stored so we can destroy it in Unload)
	fontLevel1 = AEGfxCreateFont("Assets/Fonts/gameover.ttf", 72);
}

void Tutorial_Initialize()
{
	currentGameLevel = 0;
	AEAudioPlay(Level, bgm, 0.5f, 1.f, -1);

	// Initialize player movement system
	movement::initPlayerMovement(objectinfoTut[player]);
	projectileSystem::initProjectiles(Projectiles, MAX_PROJECTILES);

	//=============CREATE TEXTURED MESH FOR WALLS==================//
	// This mesh is used by draw.cpp for rendering walls

	AssetManager::BuildSqrMesh(MESH_ENEMY);
	AssetManager::BuildSqrMesh(MESH_PLATFORM);
	AssetManager::BuildSqrMesh(MESH_PLAYER);
	AssetManager::BuildSqrMesh(MESH_PROJECTILE);
	AssetManager::BuildSqrMesh(MESH_UI);
	enemyMesh      = AssetManager::GetMesh(MESH_ENEMY);
	platformMesh   = AssetManager::GetMesh(MESH_PLATFORM);
	pMesh          = AssetManager::GetMesh(MESH_PLAYER);
	projectileMesh = AssetManager::GetMesh(MESH_PROJECTILE);
	uiMesh         = AssetManager::GetMesh(MESH_UI);

	if (!ImportMapDataFromFile("Assets/Map/Tutorial.txt")) {
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

	// Spawn player at the door they came from
	bool spawnSet = false;
	for (auto& door : doors) {
		if (door.id == playerEnteredDoorId) {
			objectinfoTut[player].xPos = door.worldX;
			objectinfoTut[player].yPos = door.worldY + 40.f; // slight offset so player isn't inside door
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

	// Initialize player health to 100 HP with no invincibility active
	InitPlayerHealth(objectinfoTut[player]);

	//======== INIT ENEMIES DATA =======================//
	// Initialize enemy system
	enemySystem::initEnemies(enemies, MAX_ENEMIES);
	projectileSystem::initProjectiles(enemyProjectiles, MAX_PROJECTILES);

	

	// SPAWN test enemies
	enemySystem::spawnEnemy(enemies, MAX_ENEMIES, ENEMY_MELEE, -200.0f, 100.0f);
	enemySystem::spawnEnemy(enemies, MAX_ENEMIES, ENEMY_RANGED, 300.0f, -100.0f);

	// DOOR

	// Build door animation mesh: 1 row, 7 columns (7 frames in a horizontal strip).
	// Free any existing doorMesh first to avoid leaking if re-initialized.
	AEGfxVertexList* tempDoorMesh = nullptr;
	animSystem::buildMesh(&tempDoorMesh, 1, 7);
	AssetManager::StoreMesh(MESH_DOOR, tempDoorMesh);
	doorMesh = tempDoorMesh;

	if (!doorTex) printf("DOOR TEXTURE NOT FOUND!\n");
	else printf("DOOR OK\n");
}

void Tutorial_Update()
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
	movement::physicsInput(objectinfoTut[player]);

	if (AEInputCheckTriggered(AEVK_Q) || AEInputCheckTriggered(AEVK_ESCAPE)) {
		next = GS_QUIT;
	}

	//===========  APPLY PHYSICS(DRAG)===================//
	// Update player physics (drag + position)
	movement::updatePlayerPhysics(objectinfoTut[player]);
	//===================================================//

	// ========== PROJECTILE SYSTEM UPDATE =============//
	projectileSystem::fireProjectiles(
		static_cast<s32>(worldMouseX),
		static_cast<s32>(worldMouseY),
		objectinfoTut[player],
		Projectiles,
		MAX_PROJECTILES);

	// Update all active projectiles
	projectileSystem::UpdateProjectiles(Projectiles, MAX_PROJECTILES);

	//============= UPDATE ENEMIES ===================/
	// Get delta time for enemy AI
	f32 dt = static_cast<f32>(AEFrameRateControllerGetFrameTime());

	// Update enemies
	enemySystem::updateEnemies(enemies, MAX_ENEMIES,
		objectinfoTut[player],
		enemyProjectiles, MAX_PROJECTILES,
		dt, LaserBlast, soundEffects);

	// Update enemy projectiles
	projectileSystem::UpdateProjectiles(enemyProjectiles, MAX_PROJECTILES);

	// Tick down the player's invincibility timer each frame
	UpdatePlayerInvincibility(objectinfoTut[player], dt);

	// Check player projectiles hitting enemies
	enemySystem::checkProjectileEnemyCollision(enemies, MAX_ENEMIES,
		Projectiles, MAX_PROJECTILES);

	// Check melee enemies damaging player (uses PlayerTakeDamage internally)
	enemySystem::checkPlayerEnemyCollision(enemies, MAX_ENEMIES,
		objectinfoTut[player], Punch, soundEffects);

	// Check ranged enemy projectiles hitting player (uses PlayerTakeDamage internally)
	enemySystem::checkEnemyPlayerProjectileCollision(
		enemyProjectiles, MAX_PROJECTILES, objectinfoTut[player]);
	/*gamelogic::OBJ_to_map(map, x, s, &enemies[0].shape, 15);
	gamelogic::OBJ_to_map(map, x, s, &enemies[1].shape, 15);
	gamelogic::OBJ_to_map(map, x, s, &objectinfoTut[player], 15);*/

	gamelogic::Collision_movement(&enemies[0].shape, map, x, s, 15);
	gamelogic::Collision_movement(&enemies[1].shape, map, x, s, 15);
	gamelogic::Collision_movement(&objectinfoTut[player], map, x, s, 15);

	// -----------------------------------------------------------------------
	// Door animation
	// -----------------------------------------------------------------------

	for (auto& door : doors) {

		if (door.firstLevel != 0 && door.secondLevel != 0) continue;
		f32 dx = objectinfoTut[player].xPos - door.worldX;
		f32 dy = objectinfoTut[player].yPos - door.worldY;
		f32 dist = sqrtf(dx * dx + dy * dy);
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
			playerEnteredDoorId = door.id;  // remember which door was used
			switch (toLevel) {
			case 0: next = GS_TUTORIAL; break;
			case 1: next = GS_LEVEL1; break;
			case 2: next = GS_LEVEL2; break;
			}
		}
	}
}

void Tutorial_Draw()
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

	// ==== RENDER UI FOR TUTORIAL ==== //
	renderlogic::drawTileArray();
	renderlogic::drawUITexture(350.f, -280.f, leftClick, 50.f);
	renderlogic::drawUITexture(-50.f, -280.f, gButton, 50.f);
	renderlogic::drawUITexture(-600.f, 10.f, spacebar, 50.f);
	renderlogic::drawUITexture(280.f, 250.f, eButton, 50.f);

	memset(strBuffer, 0, 100 * sizeof(char));
	f32 textWidth, textHeight;
	AEGfxGetPrintSize(font, strBuffer, 0.15f, &textWidth, &textHeight);
	sprintf_s(strBuffer, "Left Click to Propel");
	AEGfxPrint(font, strBuffer, 0.5f, -0.6f, 0.3f, 1.f, 1.f, 1.f, 1.f);

	sprintf_s(strBuffer, "towards Mouse Cursor");
	AEGfxPrint(font, strBuffer, 0.5f, -0.67f, 0.3f, 1.f, 1.f, 1.f, 1.f);

	sprintf_s(strBuffer, "G To Toggle Gravity");
	AEGfxPrint(font, strBuffer, 0.0f, -0.63f, 0.3f, 1.f, 1.f, 1.f, 1.f);

	sprintf_s(strBuffer, "Spacebar to Jump");
	AEGfxPrint(font, strBuffer, -0.7f, -0.0f, 0.4f, 1.f, 1.f, 1.f, 1.f);

	sprintf_s(strBuffer, "E to Enter");
	AEGfxPrint(font, strBuffer, 0.42f, 0.53f, 0.4f, 1.f, 1.f, 1.f, 1.f);

	// ==== ENEMIES RENDER =======//
	enemySystem::renderEnemies(enemies, MAX_ENEMIES, enemyMesh, projectileMesh,
		meleeEnemyTexture, rangedEnemyTexture);

	// Render enemy projectiles with plasma texture
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
	projectileSystem::renderProjectiles(enemyProjectiles, MAX_PROJECTILES, plasma, projectileMesh);

	//====== PLAYER RENDER =========//
	// Reset render state so leftover color tints from enemies/projectiles don't affect the player
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxTextureSet(characterPictest, 0, 0);
	renderlogic::drawSquare(objectinfoTut[player].xPos, objectinfoTut[player].yPos,
		objectinfoTut[player].xScale, objectinfoTut[player].yScale);
	AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);

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
		snprintf(healthText, sizeof(healthText), "Health: %d", objectinfoTut[player].health);

		// Prepare render state for font (font uses a glyph texture atlas)
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
		AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);

		// Print at top-left corner of the screen (white text)
		AEGfxPrint(fontLevel1, healthText, -0.95f, 0.85f, 0.8f, 1.0f, 1.0f, 1.0f, 1.0f);
	}
	
	// ===== RENDERING OF THE FLASHING 'E' ===== //
	if (playerNear) {
		renderlogic::flashingTexture(objectinfoTut[player].xPos, objectinfoTut[player].yPos + 60.f, eButton, 50.f);
	}

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
	AssetManager::UnloadAllTextures();
	
	if (glassMap) {
		for (int i = 0; i < BINARY_MAP_HEIGHT; ++i) delete[] glassMap[i];
		delete[] glassMap;
		glassMap = nullptr;
	}

	// Destroy the font created in Load (tutorial text labels)
	if (font != -1) { AEGfxDestroyFont(font); font = -1; }

	// Destroy the font created in Initialize (HUD health text)
	if (fontLevel1 != -1) { AEGfxDestroyFont(fontLevel1); fontLevel1 = -1; }

	// Unload ALL audio resources that were loaded in Load
	audio::unloadsound();
}
