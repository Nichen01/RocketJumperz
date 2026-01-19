// ---------------------------------------------------------------------------
// includes

#include <crtdbg.h> // To check for memory leaks
#include "AEEngine.h"
#include "collision.h"
#include "Input.h"
#include "GameStateManager.h"
#include "GameStateList.h"



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

namespace mechanics {
	void fireProjectile(s32 mouseX, s32 mouseY, objectsquares& player, objectsquares& projectile, f32 velX, s32 velY, s8 active)
	{
		if (AEInputCheckCurr(AEVK_LBUTTON) && active)
		{

		}
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
	objectsquares objectinfo[3] = { 0 };

	// PROJECTILE INFO
	objectsquares objectinfoprojectile;
	f32 projectileVelX = 0.0f;
	f32 projectileVelY = 0.0f; 
	s8  projectileActive = 0;


	objectinfo[player].xpos = 0.0f;
	objectinfo[player].ypos = 0.0f;
	objectinfo[player].xscale = 100.0f;
	objectinfo[player].yscale = 100.0f;

	objectinfo[obstacle].xpos = -400.0f;
	objectinfo[obstacle].ypos = 0.0f;
	objectinfo[obstacle].xscale = 100.0f;
	objectinfo[obstacle].yscale = 400.0f;

	// Projectile info
	objectinfo[projectile].xpos = 50.0f;
	objectinfo[projectile].ypos = 0.0f;
	objectinfo[projectile].xscale = 10.0f;
	objectinfo[projectile].yscale = 10.0f;

	// Using custom window procedure
	AESysInit(hInstance, nCmdShow, 1600, 900, 1, 60, false, NULL);

	// Changing the window title
	AESysSetWindowTitle("My New Demo!");

	// reset the system modules
	AESysReset();

	GSM_Initialize(GS_TEST);
	while (current != GS_QUIT)
{
        if (current != GS_RESTART) {
            GSM_Update();
            fpLoad();
        }
        else {
            
            current = previous;
            next = previous;
        }
        fpInitialize();
        while(next==current)
        {
			AESysFrameStart();
            Input_Handle();
            fpUpdate();
            fpDraw();
			AESysFrameEnd();
        }

        fpFree();

        if (next != GS_RESTART) {
            fpUnload();
        }
        
        previous = current;
        current = next;

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

		// GET MOUSE POSITION
		s32 mouseX, mouseY;

		AEInputGetCursorPosition(&mouseX, &mouseY);
		printf("X: %.2f, Y: %.2f", (float)mouseX, (float)mouseY);

		// Your own update logic goes here

		if (gamelogic::collision(&objectinfo[player], &objectinfo[obstacle])) {
			printf("collision");
		}

		

		// Your own rendering logic goes here
		AEGfxSetBackgroundColor(0.5f, 0.5f, 0.5f);

		AEGfxSetRenderMode(AE_GFX_RM_COLOR);

		AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);

		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		AEGfxSetTransparency(1.0f);

		// Obstacle
		AEGfxSetColorToAdd(1.0f, 0.0f, 0.0f, 0.0f);
		renderlogic::Drawsquare(objectinfo[obstacle].xpos, objectinfo[obstacle].ypos, objectinfo[obstacle].xscale, objectinfo[obstacle].yscale);
		AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);

		// White square
		AEGfxSetColorToAdd(1.0f, 1.0f, 1.0f, 1.0f);
		renderlogic::Drawsquare(objectinfo[player].xpos, objectinfo[player].ypos, objectinfo[player].xscale, objectinfo[player].yscale);
		AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);

		// Projectile
		AEGfxSetColorToAdd(1.0f, 1.0f, 1.0f, 1.0f);
		renderlogic::Drawsquare(objectinfo[projectile].xpos, objectinfo[projectile].ypos, objectinfo[projectile].xscale, objectinfo[projectile].yscale);
		AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);


		// Informing the system about the loop's end
		AESysFrameEnd();

	}

	AEGfxMeshFree(HMesh);
	AEGfxMeshFree(pMesh);
	// free the system
    }
	AESysExit();
}