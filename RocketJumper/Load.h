#pragma once
#include "AEEngine.h" 

extern AEGfxTexture* mushroomDieTexture[9];
extern AEGfxTexture* mushroomHitTexture[5];
extern AEGfxTexture* mushroomIdleTexture[9];

extern AEGfxVertexList* platformMesh;
extern AEGfxVertexList* enemyMesh;
extern AEGfxVertexList* pMesh;
extern AEGfxVertexList* projectileMesh;

namespace init {
	void enemy();
	void platform();
	void player();
	void projectile();
}

namespace load {
	void platform();
	void mushroom();
}

namespace freeAsset {
	void platform();
	void door();
	void enemy();
	void player();
	void projectile();
}

namespace unload {
	void platform();
	void glass();
}