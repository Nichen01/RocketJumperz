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

extern AEGfxTexture* ctrl1, *ctrl2, *leftArrow, *rightArrow, *leftClick, *rightClick, *eButton, *gButton, *sButton, *qButton, *zButton, *button1, *button2, *button3, *spacebar;
extern AEGfxTexture* platform1, * platform2, *platform3, *platform4, *platform5, *platform6, *platform7, *platform8, *platform9;
extern AEGfxTexture* glass0, * glass1, * glass2, * glass3, * glass4;
extern AEGfxTexture* eyeTrap, * sawTrap, * trapBeam;
extern AEGfxTexture* keyTexture;
extern AEGfxTexture* errorPromptMenu, * redButton, * greenButton;
extern AEGfxTexture* inventory, * keycardInventory;
extern AEGfxTexture* brokenDoor0, * brokenDoor1, * brokenDoor2;
extern AEGfxTexture* wireInventory0, * wireInventory1, * wireInventory2, * wireInventory3;
extern AEGfxTexture* menuTex, *buttonTex;
extern AEGfxTexture* backgroundTex, * ammoDrop, * healthDrop;
extern AEGfxTexture* wallBg1;
extern AEGfxTexture* borderTL, * borderT, * borderTR;
extern AEGfxTexture* borderCL, * borderCR;
extern AEGfxTexture* borderBL, * borderB, * borderBR;
extern AEGfxTexture* staticDoor;

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
	void background2();

}