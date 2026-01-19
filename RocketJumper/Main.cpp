// ---------------------------------------------------------------------------
// includes

#include <crtdbg.h> // To check for memory leaks
#include "drawWallsLevel1.hpp" 

// ---------------------------------------------------------------------------
// main

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	int gGameRunning = 1;

	AESysInit(hInstance, nCmdShow, 1600, 900, 1, 60, false, NULL);
	AESysSetWindowTitle("TeamProjTest");
	AESysReset();
	printf("Team project test\n");

	initMesh(); // Initializing the meshes for the map
	loadTextures(); // Loading textures
	initTransforms(); // Initializing the transforms

	while (gGameRunning)
	{
		AESysFrameStart();

		if (AEInputCheckTriggered(AEVK_ESCAPE) || 0 == AESysDoesWindowExist())
			gGameRunning = 0;

		AEGfxSetBackgroundColor(0.1f, 0.1f, 0.1f);
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
		AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		AEGfxSetTransparency(1.0f);

		drawFloors(); // Draw the map floors within the game loop
		drawCharacter(); // Draw the character within the game loop

		AESysFrameEnd();
	}
	AEGfxMeshFree(textureMesh);
	unloadTextures();
	AESysExit();
}