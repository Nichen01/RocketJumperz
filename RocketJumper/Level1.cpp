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


s32* map = new s32[144]{ 0 };
int x = 16;
int y = 9;
int s = 100;

objectsquares objectinfo[2] = { 0 };

// Local variables for projectile test level
static Projectile Projectiles[MAX_PROJECTILES];
static AEGfxVertexList* pTestMesh = nullptr;

void Level1_Load()
{
	
}
void Level1_Initialize()
{	
	characterPictest = AEGfxTextureLoad("Assets/CharacterRight.png");
	base5test = AEGfxTextureLoad("Assets/Base5.png");

	// Initialize player movement system
	movement::initPlayerMovement(objectinfo[player]);

	// Added after obstacle initialization:
	projectileSystem::initProjectiles(Projectiles, MAX_PROJECTILES);

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

	//=============CREATE SQUARE MESH==================//
	AEGfxMeshStart();
	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFF000000, 0.0f, 1.0f,
		0.5f, -0.5f, 0xFF000000, 1.0f, 1.0f,
		-0.5f, 0.5f, 0xFF000000, 0.0f, 0.0f);
	AEGfxTriAdd(
		0.5f, -0.5f, 0xFF000000, 1.0f, 1.0f,
		0.5f, 0.5f, 0xFF000000, 1.0f, 0.0f,
		-0.5f, 0.5f, 0xFF000000, 0.0f, 0.0f);
	pTestMesh = AEGfxMeshEnd();
	//==================================================//

	int x = 0, y = 0;
	for (y = 0; y < 9; y++) {
		if (y == 0 || y == 9 - 1) {
			for (x = 0; x < 16; x++) {
				map[(y * 16 + x)] = 1;
			}
		}
		else {
			for (x = 0; x < 16; x++) {
				if (x == 0 || x == 16 - 1) {
					map[(y * 16 + x)] = 1;
				}
				else {
					map[(y * 16 + x)] = 0;
				}
			}
		}
	}
	map[(4 * 16 + 6)] = 1;

	objectinfo[player].xPos = 0.0f;
	objectinfo[player].yPos = 0.0f;
	objectinfo[player].xScale = 70.0f;
	objectinfo[player].yScale = 70.0f;

	objectinfo[obstacle].xPos = -400.0f;
	objectinfo[obstacle].yPos = 0.0f;
	objectinfo[obstacle].xScale = 100.0f;
	objectinfo[obstacle].yScale = 400.0f;

	
}

void Level1_Update()
{

	//=============== GET MOUSE INPUTS (To be made into helper function) =====================//
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
	//==================================================//

	// Update all active projectiles
	projectileSystem::UpdateProjectiles(Projectiles, MAX_PROJECTILES);

	
	gamelogic::OBJ_to_map(map, x, s, &objectinfo[player]);
	//gamelogic::OBJ_to_map(map, x, s, &(enemies[0].shape));


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


}

void Level1_Draw()
{
	AEGfxSetBackgroundColor(0.5f, 0.5f, 0.5f);
	AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
	AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(1.0f);

	renderlogic::drawmap_Wall_floor(map, x, y, s);

	AEGfxTextureSet(characterPictest, 0, 0);
	renderlogic::Drawsquare(objectinfo[player].xPos, objectinfo[player].yPos, objectinfo[player].xScale, objectinfo[player].yScale);
	AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);

	// Render all active projectiles (YELLOW)
	projectileSystem::renderProjectiles(Projectiles, MAX_PROJECTILES, pTestMesh);
}

void Level1_Free()
{
	AEGfxMeshFree(pMesh);
	delete[] map;

	if (pTestMesh) {
		AEGfxMeshFree(pTestMesh);
		pTestMesh = nullptr;
	}
}

void Level1_Unload()
{
	AEGfxTextureUnload(characterPic);
	AEGfxTextureUnload(base5);
}