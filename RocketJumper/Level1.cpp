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

#include <cmath>
#include <cstdio>
#include "Level1.h"
#include "draw.h"
#include "collision.h"
#include "player.h"
#include "GameStateManager.h"
#include "GameStateList.h"
#include "projectile.h"
#include "Movement.h"
#include "render.h"
#include "enemies.h"
#include "binaryMap.h"
#include "animation.h"

s32* map = nullptr;
int x;
int y;
int s = 80;

objectsquares objectinfo[2] = { 0 };

// Local variables for projectile test level
static Projectile Projectiles[MAX_PROJECTILES];
static AEGfxVertexList* pTestMesh = nullptr;

// ENEMY DATA
static Enemy enemies[MAX_ENEMIES];
static Projectile enemyProjectiles[MAX_PROJECTILES];
static AEGfxTexture* meleeEnemyTexture = nullptr;
static AEGfxTexture* rangedEnemyTexture = nullptr;


static AEGfxTexture* mushroomDieTexture[9] = { nullptr };
static AEGfxTexture* mushroomHitTexture[5] = { nullptr };
static AEGfxTexture* mushroomIdleTexture[9] = { nullptr };

//==== sound and volume
static f32 bgVolume = 1.f;

static AEAudio L1;
static AEAudio LaserBlast;
static AEAudio Punch;
static AEAudioGroup bgm;
static AEAudioGroup soundEffects;

f32 doorX, doorY;
s32  DOOR_FRAME_COUNT = 7;
f32  DOOR_FRAME_DELAY = 0.08f;   // ~12 fps
f32  DOOR_WIDTH = s;   // matches tile size s
f32  DOOR_HEIGHT = s;
f32  DOOR_TRIGGER_RADIUS = 150.0f;  // px from door centre
SpriteAnimation  doorAnim;
AEGfxVertexList* doorMesh;
bool doorIsOpen = false; // tracks fully-open state
AEGfxTexture* doorTex;

// Font resource (must be destroyed in Unload to avoid leak)
static s8 font = -1;

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
	soundEffects = AEAudioCreateGroup();
	soundEffects = AEAudioCreateGroup();   // short for 'sound effect'


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
	mushroomIdleTexture[7] = AEGfxTextureLoad("Assets/Enemy/MushroomIdle/MushroomIdle7.png");
	mushroomIdleTexture[8] = AEGfxTextureLoad("Assets/Enemy/MushroomIdle/MushroomIdle8.png");

	// Load platform assets
	render::drawPlatform();

	// Load textures - these are defined in draw.cpp
	characterPictest = AEGfxTextureLoad("Assets/astronautRight.png");
	base5test = AEGfxTextureLoad("Assets/Base5.png");
	plasma = AEGfxTextureLoad("Assets/plasma.png");
}

void Level1_Initialize()
{
	AEAudioPlay(L1, bgm, 0.5f, 1.f, -1);

	// Create font for gameover text (stored so we can destroy it in Unload)
	font = AEGfxCreateFont("Assets/Fonts/gameover.ttf", 72);



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

	if (!ImportMapDataFromFile("Assets/Map/Level1_Map.txt")) {
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

	objectinfo[player].xPos = 0.0f;
	objectinfo[player].yPos = 0.0f;
	objectinfo[player].xScale = 60.0f;
	objectinfo[player].yScale = 60.0f;

	// Initialize player health to 100 HP with no invincibility active
	InitPlayerHealth(objectinfo[player]);

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

	// DOOR
	animSystem::buildMesh(&doorMesh, DOOR_FRAME_COUNT);
	doorTex = AEGfxTextureLoad("Assets/DoorOpen.png");
	if (!doorTex)
		printf("DOOR TEXTURE NOT FOUND!\n");
	else
		printf("DOOR OK\n");

	animSystem::init(doorAnim, DOOR_FRAME_COUNT, DOOR_FRAME_DELAY, ANIM_IDLE, 0);
	doorIsOpen = false;
}

void Level1_Update()
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
	gamelogic::OBJ_to_map(map, x, s, &enemies[0].shape, 15);
	gamelogic::OBJ_to_map(map, x, s, &enemies[1].shape, 15);
	gamelogic::OBJ_to_map(map, x, s, &objectinfo[player], 15);

	// -----------------------------------------------------------------------
	// Door animation -- hardcoded proximity check
	// -----------------------------------------------------------------------
	f32 dx = objectinfo[player].xPos - doorX;
	f32 dy = objectinfo[player].yPos - doorY;
	f32 dist = sqrtf(dx * dx + dy * dy);
	bool playerNear = (dist <= DOOR_TRIGGER_RADIUS);

	// Trigger open: player just entered range and door is fully closed
	if (playerNear && !doorIsOpen && doorAnim.playMode != ANIM_PLAY_ONCE)
		animSystem::play(doorAnim, ANIM_PLAY_ONCE);

	// Trigger close: player just left range and door is fully open
	if (!playerNear && doorIsOpen && doorAnim.playMode != ANIM_PLAY_REVERSE)
		animSystem::play(doorAnim, ANIM_PLAY_REVERSE);

	animSystem::update(doorAnim, dt);

	// justFinished is true for one frame when a one-shot completes
	if (doorAnim.justFinished)
		doorIsOpen = (doorAnim.currentFrame == DOOR_FRAME_COUNT - 1);
	// -----------------------------------------------------------------------
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
	renderlogic::drawmap_Wall_floor(map, x, y, s);

	// ==== ENEMIES RENDER =======//
	enemySystem::renderEnemies(enemies, MAX_ENEMIES, pTestMesh,
		meleeEnemyTexture, rangedEnemyTexture);

	// Render enemy projectiles with plasma texture
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
	projectileSystem::renderProjectiles(enemyProjectiles, MAX_PROJECTILES, plasma, pTestMesh);

	//====== PLAYER RENDER =========//
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxTextureSet(characterPictest, 0, 0);
	renderlogic::Drawsquare(objectinfo[player].xPos, objectinfo[player].yPos,
		objectinfo[player].xScale, objectinfo[player].yScale);
	AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);

	// Render player projectiles with plasma texture
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
	projectileSystem::renderProjectiles(Projectiles, MAX_PROJECTILES, plasma, pTestMesh);

	// ====== HUD: Player Health Display ======
	// Drawn last so it appears on top of all world geometry.
	// AEGfxPrint uses normalized coords: (-1,-1) = bottom-left, (1,1) = top-right.
	if (font >= 0)
	{
		char healthText[32];
		snprintf(healthText, sizeof(healthText), "Health: %d", objectinfo[player].health);

		// Prepare render state for font (font uses a glyph texture atlas)
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
		AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);

		// Print at top-left corner of the screen (white text)
		AEGfxPrint(font, healthText, -0.95f, 0.85f, 0.8f, 1.0f, 1.0f, 1.0f, 1.0f);
	}
}

void Level1_Free()
{
	// FREE MESHES AND MAP
	if (pMesh) {
		AEGfxMeshFree(pMesh);
		pMesh = nullptr;
	}

	if (pTestMesh) {
		AEGfxMeshFree(pTestMesh);
		pTestMesh = nullptr;
	}

	if (platformMesh) {
		AEGfxMeshFree(platformMesh);
		pTestMesh = nullptr;
	}

	if (map) {
		delete[] map;
		map = nullptr;
	}

	if (doorMesh) {
		AEGfxMeshFree(doorMesh);
		doorMesh = nullptr;
	}

	FreeMapData();
}

void Level1_Unload()
{
	// Unload ALL textures that were loaded in Initialize
	if (characterPictest) { AEGfxTextureUnload(characterPictest); characterPictest = nullptr; }
	if (base5test) { AEGfxTextureUnload(base5test); base5test = nullptr; }
	if (plasma) { AEGfxTextureUnload(plasma); plasma = nullptr; }
	render::unloadPlatform();
	if (meleeEnemyTexture) { AEGfxTextureUnload(meleeEnemyTexture); meleeEnemyTexture = nullptr; }
	if (rangedEnemyTexture) { AEGfxTextureUnload(rangedEnemyTexture); rangedEnemyTexture = nullptr; }
	if (doorTex) { AEGfxTextureUnload(doorTex); doorTex = nullptr; }

	// Destroy the font created in Initialize
	if (font != -1) { AEGfxDestroyFont(font); font = -1; }

	// Unload ALL audio resources that were loaded in Load
	AEAudioUnloadAudio(L1);
	AEAudioUnloadAudio(LaserBlast);
	AEAudioUnloadAudio(Punch);
	AEAudioUnloadAudioGroup(bgm);
	AEAudioUnloadAudioGroup(soundEffects);
}
