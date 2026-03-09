#include "Load.h"
#include "Draw.h"

AEGfxTexture* mushroomDieTexture[9] = { nullptr };
AEGfxTexture* mushroomHitTexture[5] = { nullptr };
AEGfxTexture* mushroomIdleTexture[9] = { nullptr };

// define the mesh
AEGfxVertexList* platformMesh = nullptr;
AEGfxVertexList* enemyMesh = nullptr;
AEGfxVertexList* pMesh = nullptr;
AEGfxVertexList* projectileMesh = nullptr;

namespace init {

	void enemy() {
		AEGfxMeshStart();
		AEGfxTriAdd(
			-0.5f, -0.5f, 0xFFFFFFFF, 0.0f, 1.0f,
			0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f,
			-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f);
		AEGfxTriAdd(
			0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f,
			0.5f, 0.5f, 0xFFFFFFFF, 1.0f, 0.0f,
			-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f);
		enemyMesh = AEGfxMeshEnd();
	}

	void platform() {
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
	}

	void player() {
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
	}

	void projectile() {
		AEGfxMeshStart();
		AEGfxTriAdd(
			-0.5f, -0.5f, 0xFFFFFFFF, 0.0f, 1.0f,
			0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f,
			-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f);
		AEGfxTriAdd(
			0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f,
			0.5f, 0.5f, 0xFFFFFFFF, 1.0f, 0.0f,
			-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f);
		projectileMesh = AEGfxMeshEnd();
	}
}

namespace load {

	void platform() {
		platform1 = AEGfxTextureLoad("Assets/Platform/platform1.png");
		platform2 = AEGfxTextureLoad("Assets/Platform/platform2.png");
		platform3 = AEGfxTextureLoad("Assets/Platform/platform3.png");
		platform4 = AEGfxTextureLoad("Assets/Platform/platform4.png");
		platform5 = AEGfxTextureLoad("Assets/Platform/platform5.png");
		platform6 = AEGfxTextureLoad("Assets/Platform/platform6.png");
		platform7 = AEGfxTextureLoad("Assets/Platform/platform7.png");
		platform8 = AEGfxTextureLoad("Assets/Platform/platform8.png");
		platform9 = AEGfxTextureLoad("Assets/Platform/platform0.png");
		glass0 = AEGfxTextureLoad("Assets/Platform/glass0.jpg");
		glass1 = AEGfxTextureLoad("Assets/Platform/glass1.jpg");
		glass2 = AEGfxTextureLoad("Assets/Platform/glass2.jpg");
		glass3 = AEGfxTextureLoad("Assets/Platform/glass3.jpg");
		glass4 = AEGfxTextureLoad("Assets/Platform/glass4.jpg");
	}

	void mushroom() {
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
		mushroomHitTexture[0] = AEGfxTextureLoad( "Assets/Enemy/MushroomHit/MushroomHit0.png");
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
	}

} 

namespace freeAsset {
	void platform() {
		if (platformMesh) {
			AEGfxMeshFree(platformMesh);
			platformMesh = nullptr;
		}
	}

	void door() {
		if (doorMesh) {
			AEGfxMeshFree(doorMesh);
			doorMesh = nullptr;
		}
	}

	void enemy() {
		if (enemyMesh) {
			AEGfxMeshFree(enemyMesh);
			enemyMesh = nullptr;
		}
	}

	void player() {
		if (pMesh) {
			AEGfxMeshFree(pMesh);
			pMesh = nullptr;
		}
	}

	void projectile() {
		if (projectileMesh) {
			AEGfxMeshFree(projectileMesh);
			projectileMesh = nullptr;
		}
	}
}

namespace unload {

	void platform() {
		if (platform1) { AEGfxTextureUnload(platform1); platform1 = nullptr; }
		if (platform2) { AEGfxTextureUnload(platform2); platform2 = nullptr; }
		if (platform3) { AEGfxTextureUnload(platform3); platform3 = nullptr; }
		if (platform4) { AEGfxTextureUnload(platform4); platform4 = nullptr; }
		if (platform5) { AEGfxTextureUnload(platform5); platform5 = nullptr; }
		if (platform6) { AEGfxTextureUnload(platform6); platform6 = nullptr; }
		if (platform7) { AEGfxTextureUnload(platform7); platform7 = nullptr; }
		if (platform8) { AEGfxTextureUnload(platform8); platform8 = nullptr; }
		if (platform9) { AEGfxTextureUnload(platform9); platform9 = nullptr; }
	}

	void glass() {
		if (glass0) { AEGfxTextureUnload(glass0); glass0 = nullptr; }
		if (glass1) { AEGfxTextureUnload(glass1); glass1 = nullptr; }
		if (glass2) { AEGfxTextureUnload(glass2); glass2 = nullptr; }
		if (glass3) { AEGfxTextureUnload(glass3); glass3 = nullptr; }
		if (glass4) { AEGfxTextureUnload(glass4); glass4 = nullptr; }
	}
}