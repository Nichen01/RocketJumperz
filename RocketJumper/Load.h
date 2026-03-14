#pragma once
#include "AEEngine.h" 

extern AEGfxTexture* mushroomDieTexture[9];
extern AEGfxTexture* mushroomHitTexture[5];
extern AEGfxTexture* mushroomIdleTexture[9];

extern AEGfxVertexList* platformMesh;
extern AEGfxVertexList* enemyMesh;
extern AEGfxVertexList* pMesh;
extern AEGfxVertexList* projectileMesh;
extern AEGfxVertexList* uiMesh;

extern AEGfxTexture* ctrl1, *ctrl2, *leftArrow, *rightArrow, *leftClick, *rightClick, *eButton, *gButton, *sButton, *zButton, *button1, *button2, *button3, *spacebar;

extern AEGfxTexture* platform1, * platform2, *platform3, *platform4, *platform5, *platform6, *platform7, *platform8, *platform9;
extern AEGfxTexture* glass0, * glass1, * glass2, * glass3, * glass4;

namespace init {
	void enemy();
	void platform();
	void player();
	void projectile();
	void ui();
}

namespace load {
	void platform();
	void mushroom();
	void ui();
}