/* Start Header ************************************************************************/
/*!
\file		  ProjectileTest.cpp
\author       Ivan Chong, i.chong, 2503476
\par          i.chong@digipen.edu
\date         January, 19, 2026
\brief        Contain functions for Projectile Test Level - isolated projectile testing

Copyright (C) 2026 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#include <iostream>
#include "physicsEngine.h"
#include "ProjectileTest.h"
#include "collision.h"
#include "projectile.h"
#include "Movement.h"
#include "render.h"


// Local variables for projectile test level
static objectsquares testObjects[3] = { 0 };
static Projectile testProjectiles[MAX_PROJECTILES];
static AEGfxVertexList* pTestMesh = nullptr;
static AEGfxVertexList* hTestMesh = nullptr;

void ProjectileTest_Load()
{
	// Initialize player
	testObjects[player].xPos = 0.0f;
	testObjects[player].yPos = 0.0f;
	testObjects[player].xScale = 100.0f;
	testObjects[player].yScale = 100.0f;

	// Initialize player movement system
	movement::initPlayerMovement(testObjects[player]);

	// Initialize obstacle
	testObjects[obstacle].xPos = -400.0f;
	testObjects[obstacle].yPos = 0.0f;
	testObjects[obstacle].xScale = 100.0f;
	testObjects[obstacle].yScale = 400.0f;

	// Initialize projectile system
	projectileSystem::initProjectiles(testProjectiles, MAX_PROJECTILES);

	// Create square mesh
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

	// Create half-mesh (if needed)
	AEGfxMeshStart();

	AEGfxTriAdd(
		0.0f, -0.5f, 0xFF000000, 0.0f, 1.0f,
		1.0f, -0.5f, 0xFF000000, 1.0f, 1.0f,
		0.0f, 0.5f, 0xFF000000, 0.0f, 0.0f);

	AEGfxTriAdd(
		1.0f, -0.5f, 0xFF000000, 1.0f, 1.0f,
		1.0f, 0.5f, 0xFF000000, 1.0f, 0.0f,
		0.0f, 0.5f, 0xFF000000, 0.0f, 0.0f);

	hTestMesh = AEGfxMeshEnd();

	printf("ProjectileTest_Load: Projectile test level loaded!\n");
}

void ProjectileTest_Initialize()
{
	physics::initPhysics();
	printf("ProjectileTest_Initialize: Ready for testing!\n");
}

void ProjectileTest_Update()
{
	

	// ========== PLAYER MOVEMENT ==========
	/* OLD WASD MOVEMENT CODE
	 s8 speed = 10;
	 
	if (AEInputCheckCurr(AEVK_D)) {
		testObjects[player].xPos += static_cast<f32>(speed);
	}

	if (AEInputCheckCurr(AEVK_A)) {
		testObjects[player].xPos -= static_cast<f32>(speed);
	}

	if (AEInputCheckCurr(AEVK_W)) {
		testObjects[player].yPos += static_cast<f32>(speed);
	}

	if (AEInputCheckCurr(AEVK_S)) {
		testObjects[player].yPos -= static_cast<f32>(speed);
	}
	*/
	
	physics::physicsInput();
	// Get mouse inputs
	s32 mouseX, mouseY;
	AEInputGetCursorPosition(&mouseX, &mouseY);

	// Convert screen coordinates to world coordinates
	f32 worldMouseX = static_cast<f32>(mouseX) - static_cast<f32>(screenWidth / 2);
	f32 worldMouseY = static_cast<f32>(screenLength / 2) - static_cast<f32>(mouseY);

	physics::updatePhysics(testObjects[player]);
	// ========== JETPACK MOVEMENT SYSTEM ==========
	// Apply thrust when spacebar is pressed
	movement::applyThrustTowardMouse(
		testObjects[player],
		static_cast<s32>(worldMouseX),
		static_cast<s32>(worldMouseY)
	);

	// Update player physics (drag + position)
	movement::updatePlayerPhysics(testObjects[player]);

	// ========== PROJECTILE SYSTEM UPDATE ==========
	// Fire projectiles
	projectileSystem::fireProjectiles(
		static_cast<s32>(worldMouseX),
		static_cast<s32>(worldMouseY),
		testObjects[player],
		testProjectiles,
		MAX_PROJECTILES
	);

	// Update all active projectiles
	projectileSystem::UpdateProjectiles(testProjectiles, MAX_PROJECTILES);

	// Check projectile collisions with obstacle
	projectileSystem::checkProjectileCollisions(testProjectiles, MAX_PROJECTILES, testObjects[obstacle]);

	// Player and obstacle collision
	if (gamelogic::collision(&testObjects[player], &testObjects[obstacle])) {
		printf("PLAYER COLLISION WITH OBSTACLE\n");
	}
}

void ProjectileTest_Draw()
{
	AEGfxSetBackgroundColor(0.5f, 0.5f, 0.5f);

	AEGfxSetRenderMode(AE_GFX_RM_COLOR);

	AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);

	AEGfxSetBlendMode(AE_GFX_BM_BLEND);
	AEGfxSetTransparency(1.0f);

	// Render Obstacle (RED)
	AEGfxSetColorToAdd(1.0f, 0.0f, 0.0f, 0.0f);
	render::Drawsquare(testObjects[obstacle].xPos, testObjects[obstacle].yPos,
		testObjects[obstacle].xScale, testObjects[obstacle].yScale);
	AEGfxMeshDraw(pTestMesh, AE_GFX_MDM_TRIANGLES);

	// Render Player (WHITE)
	AEGfxSetColorToAdd(1.0f, 1.0f, 1.0f, 1.0f);
	render::Drawsquare(testObjects[player].xPos, testObjects[player].yPos,
		testObjects[player].xScale, testObjects[player].yScale);
	AEGfxMeshDraw(pTestMesh, AE_GFX_MDM_TRIANGLES);

	// Render all active projectiles (YELLOW)
	projectileSystem::renderProjectiles(testProjectiles, MAX_PROJECTILES, pTestMesh);
}

void ProjectileTest_Free()
{
	if (pTestMesh) {
		AEGfxMeshFree(pTestMesh);
		pTestMesh = nullptr;
	}

	if (hTestMesh) {
		AEGfxMeshFree(hTestMesh);
		hTestMesh = nullptr;
	}

	printf("ProjectileTest_Free: Meshes freed!\n");
}

void ProjectileTest_Unload()
{
	printf("ProjectileTest_Unload: Projectile test level unloaded!\n");
}