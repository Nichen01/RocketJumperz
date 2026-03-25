#pragma once
#include "AEEngine.h" 

extern AEGfxTexture* mushroomDieTexture[9];
extern AEGfxTexture* mushroomHitTexture[5];
extern AEGfxTexture* mushroomIdleTexture[9];
extern AEGfxTexture* cooldownFrame[6];

extern AEGfxVertexList* platformMesh;
extern AEGfxVertexList* enemyMesh;
extern AEGfxVertexList* pMesh;
extern AEGfxVertexList* projectileMesh;
extern AEGfxVertexList* uiMesh;
extern AEGfxVertexList* buttonMesh;

extern AEGfxTexture* ctrl1, *ctrl2, *leftArrow, *rightArrow, *leftClick, *rightClick, *eButton, *gButton, *sButton, *zButton, *button1, *button2, *button3, *spacebar;
extern AEGfxTexture* platform1, * platform2, *platform3, *platform4, *platform5, *platform6, *platform7, *platform8, *platform9;
extern AEGfxTexture* glass0, * glass1, * glass2, * glass3, * glass4;
extern AEGfxTexture* trap1, * trap2, * trapBeam;
extern AEGfxTexture* keyTexture;
extern AEGfxTexture* errorPromptMenu, * redButton, * greenButton;
extern AEGfxTexture* prompt1, * prompt2, * prompt3, * prompt4, * prompt5, * prompt6, * prompt7, * prompt8, * prompt9;
extern AEGfxTexture* inventory, * keycardInventory;
extern AEGfxTexture* brokenDoor0, * brokenDoor1, * brokenDoor2;
extern AEGfxTexture* wireInventory0, * wireInventory1, * wireInventory2;
extern AEGfxTexture* menuTex, *buttonTex;
extern AEGfxTexture* backgroundTex, * ammoDrop;

extern int keyX, keyY;
extern int brokenDoorX, brokenDoorY;

namespace load {
	void platform();
	void ui();
	void key();
	void errorPrompt();
	void redButtonOption();
	void greenButtonOption();
	void brokenDoor();
	void wireInventory();
	void cooldownBar();
	void pauseMenu();
	void background();

	// Null all extern texture and mesh pointers after AssetManager has
	// freed the underlying GPU resources.  Prevents dangling-pointer
	// reads (which cause textures to render as black).
	void NullExternPointers();
}