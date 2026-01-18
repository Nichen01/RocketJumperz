// ---------------------------------------------------------------------------
// includes

#include <crtdbg.h> // To check for memory leaks
#include "AEEngine.h"
#include "collision.h"   // Provides objectsquares struct and objecttype enum
#include "projectile.h"  // Provides Projectile struct and projectileSystem namespace



// ---------------------------------------------------------------------------
// main

namespace renderlogic {
	void Drawsquare(f32 xpos, f32 ypos, f32 xsize, f32 ysize) {
		AEMtx33 scale = { 0 };
		AEMtx33Scale(&scale, xsize, ysize);

		AEMtx33 rotate = { 0 };
		AEMtx33Rot(&rotate, 0.0f);

		AEMtx33 translate = { 0 };
		AEMtx33Trans(&translate, xpos, ypos);

		AEMtx33 transform = { 0 };
		AEMtx33Concat(&transform, &rotate, &scale);
		AEMtx33Concat(&transform, &translate, &transform);

		AEGfxSetTransform(transform.m);
	}
}


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	int gGameRunning = 1;

	// Initialization of your own variables go here
	s8 speed = 10;

	// Use the objecttype enum from collision.h for array indexing
	objectsquares objectinfo[3] = { 0 };

	// ========== PROJECTILE SYSTEM ==========
	Projectile projectiles[MAX_PROJECTILES];
	projectileSystem::initProjectiles(projectiles, MAX_PROJECTILES);

	// Initialize player using enum from collision.h
	objectinfo[player].xpos = 0.0f;
	objectinfo[player].ypos = 0.0f;
	objectinfo[player].xscale = 100.0f;
	objectinfo[player].yscale = 100.0f;

	// Initialize obstacle using enum from collision.h
	objectinfo[obstacle].xpos = -400.0f;
	objectinfo[obstacle].ypos = 0.0f;
	objectinfo[obstacle].xscale = 100.0f;
	objectinfo[obstacle].yscale = 400.0f;

	// Using custom window procedure
	AESysInit(hInstance, nCmdShow, 1600, 900, 1, 60, false, NULL);

	// Changing the window title
	AESysSetWindowTitle("Rocket Jumperz");

	// reset the system modules
	AESysReset();
	AEGfxMeshStart();

	AEGfxTriAdd(
		-0.5f, -0.5f, 0xFF000000, 0.0f, 1.0f,
		0.5f, -0.5f, 0xFF000000, 1.0f, 1.0f,
		-0.5f, 0.5f, 0xFF000000, 0.0f, 0.0f);

	AEGfxTriAdd(
		0.5f, -0.5f, 0xFF000000, 1.0f, 1.0f,
		0.5f, 0.5f, 0xFF000000, 1.0f, 0.0f,
		-0.5f, 0.5f, 0xFF000000, 0.0f, 0.0f);

	AEGfxVertexList* pMesh = AEGfxMeshEnd();

	AEGfxTriAdd(
		0.0f, -0.5f, 0xFF000000, 0.0f, 1.0f,
		1.0f, -0.5f, 0xFF000000, 1.0f, 1.0f,
		0.0f, 0.5f, 0xFF000000, 0.0f, 0.0f);

	AEGfxTriAdd(
		1.0f, -0.5f, 0xFF000000, 1.0f, 1.0f,
		1.0f, 0.5f, 0xFF000000, 1.0f, 0.0f,
		0.0f, 0.5f, 0xFF000000, 0.0f, 0.0f);

	AEGfxVertexList* HMesh = AEGfxMeshEnd();

	// Game Loop
	while (gGameRunning)
	{
		// Informing the system about the loop's start
		AESysFrameStart();

		// Basic way to trigger exiting the application
		// when ESCAPE is hit or when the window is closed
		if (AEInputCheckTriggered(AEVK_ESCAPE) || 0 == AESysDoesWindowExist())
			gGameRunning = 0;

		// ========== PLAYER MOVEMENT ==========
		if (AEInputCheckCurr(AEVK_D)) {
			objectinfo[player].xpos += static_cast<f32>(speed);
		}

		if (AEInputCheckCurr(AEVK_A)) {
			objectinfo[player].xpos -= static_cast<f32>(speed);
		}

		if (AEInputCheckCurr(AEVK_W)) {
			objectinfo[player].ypos += static_cast<f32>(speed);
		}

		if (AEInputCheckCurr(AEVK_S)) {
			objectinfo[player].ypos -= static_cast<f32>(speed);
		}

		// Get mouse inputs
		s32 mouseX, mouseY;
		AEInputGetCursorPosition(&mouseX, &mouseY);

		// Convert screen coordinates to world coordinates
		f32 worldMouseX = static_cast<f32>(mouseX) - 800.0f;
		f32 worldMouseY = 450.0f - static_cast<f32>(mouseY);

		// ========== PROJECTILE SYSTEM UPDATE ==========
		// Fire projectiles
		projectileSystem::fireProjectiles(
			static_cast<s32>(worldMouseX),
			static_cast<s32>(worldMouseY),
			objectinfo[player],
			projectiles,
			MAX_PROJECTILES
		);

		// Update all active projectiles
		projectileSystem::UpdateProjectiles(projectiles, MAX_PROJECTILES);

		// Check projectile collisions with obstacle
		projectileSystem::checkProjectileCollisions(projectiles, MAX_PROJECTILES, objectinfo[obstacle]);

		// Player and obstacle collision
		if (gamelogic::collision(&objectinfo[player], &objectinfo[obstacle])) {
			printf("PLAYER COLLISION\n");
		}

		// ========== RENDERING ==========
		AEGfxSetBackgroundColor(0.5f, 0.5f, 0.5f);

		AEGfxSetRenderMode(AE_GFX_RM_COLOR);

		AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);

		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		AEGfxSetTransparency(1.0f);

		// Render Obstacle (RED)
		AEGfxSetColorToAdd(1.0f, 0.0f, 0.0f, 0.0f);
		renderlogic::Drawsquare(objectinfo[obstacle].xpos, objectinfo[obstacle].ypos, objectinfo[obstacle].xscale, objectinfo[obstacle].yscale);
		AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);

		// Render Player (WHITE)
		AEGfxSetColorToAdd(1.0f, 1.0f, 1.0f, 1.0f);
		renderlogic::Drawsquare(objectinfo[player].xpos, objectinfo[player].ypos, objectinfo[player].xscale, objectinfo[player].yscale);
		AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);

		// Render all active projectiles (YELLOW)
		projectileSystem::renderProjectiles(projectiles, MAX_PROJECTILES, pMesh);

		// Informing the system about the loop's end
		AESysFrameEnd();
	}

	AEGfxMeshFree(HMesh);
	AEGfxMeshFree(pMesh);
	// free the system
	AESysExit();
}