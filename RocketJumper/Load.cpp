#include "Load.h"
#include "Draw.h"
#include "AssetManager.h"

// define texture array
AEGfxTexture* mushroomDieTexture[9] = { nullptr };
AEGfxTexture* mushroomHitTexture[5] = { nullptr };
AEGfxTexture* mushroomIdleTexture[9] = { nullptr };

// define textures

	// different types of platforms
	AEGfxTexture* platform1 = NULL, * platform2 = NULL, * platform3 = NULL, * platform4 = NULL, * platform5 = NULL, * platform6 = NULL, * platform7 = NULL, * platform8 = NULL, * platform9 = NULL;
	AEGfxTexture* glass0 = NULL, * glass1 = NULL, * glass2 = NULL, * glass3 = NULL, * glass4 = NULL;
	AEGfxTexture* ctrl1 = NULL, * ctrl2 = NULL, * leftArrow = NULL, * rightArrow = NULL, * leftClick = NULL, * rightClick = NULL,* eButton = NULL, * gButton = NULL, * sButton = NULL, * zButton = NULL, * button1 = NULL, * button2 = NULL, * button3 = NULL, * button4 = NULL, * spacebar = NULL;


// define the mesh
AEGfxVertexList* platformMesh = nullptr;
AEGfxVertexList* enemyMesh = nullptr;
AEGfxVertexList* pMesh = nullptr;
AEGfxVertexList* projectileMesh = nullptr;
AEGfxVertexList* uiMesh = nullptr;

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

	void ui() {
		AEGfxMeshStart();
		AEGfxTriAdd(
			-0.5f, -0.5f, 0xFFFFFFFF, 0.0f, 1.0f,
			0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f,
			-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f);

		AEGfxTriAdd(
			0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f,
			0.5f, 0.5f, 0xFFFFFFFF, 1.0f, 0.0f,
			-0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f);
		uiMesh = AEGfxMeshEnd();
	}

}

namespace load {

	void platform() {
		platform1 = AssetManager::LoadTexture("platform1", "Assets/Platform/platform1.png");
		platform2 = AssetManager::LoadTexture("platform2", "Assets/Platform/platform2.png");
		platform3 = AssetManager::LoadTexture("platform3", "Assets/Platform/platform3.png");
		platform4 = AssetManager::LoadTexture("platform4", "Assets/Platform/platform4.png");
		platform5 = AssetManager::LoadTexture("platform5", "Assets/Platform/platform5.png");
		platform6 = AssetManager::LoadTexture("platform6", "Assets/Platform/platform6.png");
		platform7 = AssetManager::LoadTexture("platform7", "Assets/Platform/platform7.png");
		platform8 = AssetManager::LoadTexture("platform8", "Assets/Platform/platform8.png");
		platform9 = AssetManager::LoadTexture("platform9", "Assets/Platform/platform0.png");
		glass0 = AssetManager::LoadTexture("glass0", "Assets/Platform/glass0.jpg");
		glass1 = AssetManager::LoadTexture("glass1", "Assets/Platform/glass1.jpg");
		glass2 = AssetManager::LoadTexture("glass2", "Assets/Platform/glass2.jpg");
		glass3 = AssetManager::LoadTexture("glass3", "Assets/Platform/glass3.jpg");
		glass4 = AssetManager::LoadTexture("glass4", "Assets/Platform/glass4.jpg");
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

	void ui() {
		ctrl1 = AssetManager::LoadTexture("ctrl1", "Assets/UI/ctrl1.png");
		ctrl2 = AssetManager::LoadTexture("ctrl2", "Assets/UI/ctrl2.png");
		leftArrow = AssetManager::LoadTexture("leftArrow", "Assets/UI/leftArrow.png");
		rightArrow = AssetManager::LoadTexture("rightArrow", "Assets/UI/rightArrow.png");
		leftClick = AssetManager::LoadTexture("leftClick", "Assets/UI/leftClick.png");
		rightClick = AssetManager::LoadTexture("rightClick", "Assets/UI/rightClick.png");
		eButton = AssetManager::LoadTexture("eButton", "Assets/UI/eButton.png");
		gButton = AssetManager::LoadTexture("gButton", "Assets/UI/gButton.png");
		sButton = AssetManager::LoadTexture("sButton", "Assets/UI/sButton.png");
		zButton = AssetManager::LoadTexture("zButton", "Assets/UI/zButton.png");
		button1 = AssetManager::LoadTexture("button1", "Assets/UI/1Button.png");
		button2 = AssetManager::LoadTexture("button2", "Assets/UI/2Button.png");
		button3 = AssetManager::LoadTexture("button3", "Assets/UI/3Button.png");
		button4 = AssetManager::LoadTexture("button4", "Assets/UI/4Button.png");
		spacebar = AssetManager::LoadTexture("spacebar", "Assets/UI/spacebar.png");
	}
} 
