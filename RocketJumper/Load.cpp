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
	AEGfxTexture* ctrl1 = NULL, * ctrl2 = NULL, * leftArrow = NULL, * rightArrow = NULL, * leftClick = NULL, * rightClick = NULL,* eButton = NULL, * gButton = NULL, * sButton = NULL, * zButton = NULL, * button1 = NULL, * button2 = NULL, * button3 = NULL, * spacebar = NULL;
	AEGfxTexture* trap1 = NULL, * trap2 = NULL, * trapBeam = NULL;
	AEGfxTexture* keyTexture = NULL;
	AEGfxTexture* errorPromptMenu = NULL, * redButton = NULL, * greenButton = NULL;
	AEGfxTexture* prompt1 = NULL, * prompt2 = NULL, * prompt3 = NULL, * prompt4 = NULL, * prompt5 = NULL, * prompt6 = NULL, * prompt7 = NULL, * prompt8 = NULL, * prompt9 = NULL;
	AEGfxTexture* inventory = NULL, * keycardInventory = NULL;
	AEGfxTexture* brokenDoor0 = NULL, * brokenDoor1 = NULL, * brokenDoor2 = NULL;
	AEGfxTexture* wireInventory0 = NULL, * wireInventory1 = NULL, * wireInventory2 = NULL;

	int keyX{}, keyY{};

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
		AssetManager::LoadTexture(TEX_PLATFORM1, "Assets/Platform/platform1.png");
		AssetManager::LoadTexture(TEX_PLATFORM2, "Assets/Platform/platform2.png");
		AssetManager::LoadTexture(TEX_PLATFORM3, "Assets/Platform/platform3.png");
		AssetManager::LoadTexture(TEX_PLATFORM4, "Assets/Platform/platform4.png");
		AssetManager::LoadTexture(TEX_PLATFORM5, "Assets/Platform/platform5.png");
		AssetManager::LoadTexture(TEX_PLATFORM6, "Assets/Platform/platform6.png");
		AssetManager::LoadTexture(TEX_PLATFORM7, "Assets/Platform/platform7.png");
		AssetManager::LoadTexture(TEX_PLATFORM8, "Assets/Platform/platform8.png");
		AssetManager::LoadTexture(TEX_PLATFORM9, "Assets/Platform/platform9.png");
		AssetManager::LoadTexture(TEX_GLASS0, "Assets/Platform/glass0.jpg");
		AssetManager::LoadTexture(TEX_GLASS1, "Assets/Platform/glass1.jpg");
		AssetManager::LoadTexture(TEX_GLASS2, "Assets/Platform/glass2.jpg");
		AssetManager::LoadTexture(TEX_GLASS3, "Assets/Platform/glass3.jpg");
		AssetManager::LoadTexture(TEX_GLASS4, "Assets/Platform/glass4.jpg");
		AssetManager::LoadTexture(TEX_TRAP1, "Assets/spike.png");
		AssetManager::LoadTexture(TEX_TRAP2, "Assets/eye.png");
		AssetManager::LoadTexture(TEX_TRAPBEAM, "Assets/tentacle.png");

		// Sync the extern pointers so draw.cpp can use them directly
		platform1 = AssetManager::GetTexture(TEX_PLATFORM1);
		platform2 = AssetManager::GetTexture(TEX_PLATFORM2);
		platform3 = AssetManager::GetTexture(TEX_PLATFORM3);
		platform4 = AssetManager::GetTexture(TEX_PLATFORM4);
		platform5 = AssetManager::GetTexture(TEX_PLATFORM5);
		platform6 = AssetManager::GetTexture(TEX_PLATFORM6);
		platform7 = AssetManager::GetTexture(TEX_PLATFORM7);
		platform8 = AssetManager::GetTexture(TEX_PLATFORM8);
		platform9 = AssetManager::GetTexture(TEX_PLATFORM9);
		glass0 = AssetManager::GetTexture(TEX_GLASS0);
		glass1 = AssetManager::GetTexture(TEX_GLASS1);
		glass2 = AssetManager::GetTexture(TEX_GLASS2);
		glass3 = AssetManager::GetTexture(TEX_GLASS3);
		glass4 = AssetManager::GetTexture(TEX_GLASS4);
		trap1 = AssetManager::GetTexture(TEX_TRAP1);
		trap2 = AssetManager::GetTexture(TEX_TRAP2);
		trapBeam = AssetManager::GetTexture(TEX_TRAPBEAM);
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
		AssetManager::LoadTexture(TEX_CTRL1, "Assets/UI/ctrl1.png");
		AssetManager::LoadTexture(TEX_CTRL2, "Assets/UI/ctrl2.png");
		AssetManager::LoadTexture(TEX_LEFT_ARROW, "Assets/UI/leftArrow.png");
		AssetManager::LoadTexture(TEX_RIGHT_ARROW, "Assets/UI/rightArrow.png");
		AssetManager::LoadTexture(TEX_LEFT_CLICK, "Assets/UI/leftClick.png");
		AssetManager::LoadTexture(TEX_RIGHT_CLICK, "Assets/UI/rightClick.png");
		AssetManager::LoadTexture(TEX_E_BUTTON, "Assets/UI/eButton.png");
		AssetManager::LoadTexture(TEX_G_BUTTON, "Assets/UI/gButton.png");
		AssetManager::LoadTexture(TEX_S_BUTTON, "Assets/UI/sButton.png");
		AssetManager::LoadTexture(TEX_Z_BUTTON, "Assets/UI/zButton.png");
		AssetManager::LoadTexture(TEX_BUTTON1, "Assets/UI/1Button.png");
		AssetManager::LoadTexture(TEX_BUTTON2, "Assets/UI/2Button.png");
		AssetManager::LoadTexture(TEX_BUTTON3, "Assets/UI/3Button.png");
		AssetManager::LoadTexture(TEX_SPACEBAR, "Assets/UI/spacebar.png");
		AssetManager::LoadTexture(TEX_INVENTORY, "Assets/UI/inventory.png");
		AssetManager::LoadTexture(TEX_KEYCARDINVENTORY, "Assets/UI/keycardInventory.png");

		// Sync the extern pointers so other files can use them directly
		ctrl1 = AssetManager::GetTexture(TEX_CTRL1);
		ctrl2 = AssetManager::GetTexture(TEX_CTRL2);
		leftArrow = AssetManager::GetTexture(TEX_LEFT_ARROW);
		rightArrow = AssetManager::GetTexture(TEX_RIGHT_ARROW);
		leftClick = AssetManager::GetTexture(TEX_LEFT_CLICK);
		rightClick = AssetManager::GetTexture(TEX_RIGHT_CLICK);
		eButton = AssetManager::GetTexture(TEX_E_BUTTON);
		gButton = AssetManager::GetTexture(TEX_G_BUTTON);
		sButton = AssetManager::GetTexture(TEX_S_BUTTON);
		zButton = AssetManager::GetTexture(TEX_Z_BUTTON);
		button1 = AssetManager::GetTexture(TEX_BUTTON1);
		button2 = AssetManager::GetTexture(TEX_BUTTON2);
		button3 = AssetManager::GetTexture(TEX_BUTTON3);
		spacebar = AssetManager::GetTexture(TEX_SPACEBAR);
		inventory = AssetManager::GetTexture(TEX_INVENTORY);
		keycardInventory = AssetManager::GetTexture(TEX_KEYCARDINVENTORY);
	}

	void key() {
		AssetManager::LoadTexture(TEX_KEYCARD, "Assets/Items/keycard.png");
		keyTexture = AssetManager::GetTexture(TEX_KEYCARD);
	}

	void errorPrompt() {
		AssetManager::LoadTexture(TEX_ERRORPROMPT, "Assets/UI/menu.png");
		AssetManager::LoadTexture(TEX_PROMPT1, "Assets/UI/prompt1.png");
		AssetManager::LoadTexture(TEX_PROMPT2, "Assets/UI/prompt2.png");
		AssetManager::LoadTexture(TEX_PROMPT3, "Assets/UI/prompt3.png");
		AssetManager::LoadTexture(TEX_PROMPT4, "Assets/UI/prompt4.png");
		AssetManager::LoadTexture(TEX_PROMPT5, "Assets/UI/prompt5.png");
		AssetManager::LoadTexture(TEX_PROMPT6, "Assets/UI/prompt6.png");
		AssetManager::LoadTexture(TEX_PROMPT7, "Assets/UI/prompt7.png");
		AssetManager::LoadTexture(TEX_PROMPT8, "Assets/UI/prompt8.png");
		AssetManager::LoadTexture(TEX_PROMPT9, "Assets/UI/prompt9.png");
		errorPromptMenu = AssetManager::GetTexture(TEX_ERRORPROMPT);
		prompt1 = AssetManager::GetTexture(TEX_PROMPT1);
		prompt2 = AssetManager::GetTexture(TEX_PROMPT2);
		prompt3 = AssetManager::GetTexture(TEX_PROMPT3);
		prompt4 = AssetManager::GetTexture(TEX_PROMPT4);
		prompt5 = AssetManager::GetTexture(TEX_PROMPT5);
		prompt6 = AssetManager::GetTexture(TEX_PROMPT6);
		prompt7 = AssetManager::GetTexture(TEX_PROMPT7);
		prompt8 = AssetManager::GetTexture(TEX_PROMPT8);
		prompt9 = AssetManager::GetTexture(TEX_PROMPT9);
	}

	void redButtonOption() {
		AssetManager::LoadTexture(TEX_REDBUTTON, "Assets/UI/redButton.png");
		redButton = AssetManager::GetTexture(TEX_REDBUTTON);
	}

	void greenButtonOption() {
		AssetManager::LoadTexture(TEX_GREENBUTTON, "Assets/UI/greenButton.png");
		greenButton = AssetManager::GetTexture(TEX_GREENBUTTON);
	}

	void brokenDoor() {
		AssetManager::LoadTexture(TEX_BROKENDOOR0, "Assets/Platform/brokenDoor0.png");
		AssetManager::LoadTexture(TEX_BROKENDOOR1, "Assets/Platform/brokenDoor1.png");
		AssetManager::LoadTexture(TEX_BROKENDOOR2, "Assets/Platform/brokenDoor2.png");
		brokenDoor0 = AssetManager::GetTexture(TEX_BROKENDOOR0);
		brokenDoor1 = AssetManager::GetTexture(TEX_BROKENDOOR1);
		brokenDoor2 = AssetManager::GetTexture(TEX_BROKENDOOR2);
	}

	void wireInventory() {
		AssetManager::LoadTexture(TEX_WIREINVENTORY0, "Assets/UI/wireInventory0.png");
		AssetManager::LoadTexture(TEX_WIREINVENTORY1, "Assets/UI/wireInventory1.png");
		AssetManager::LoadTexture(TEX_WIREINVENTORY2, "Assets/UI/wireInventory2.png");
		wireInventory0 = AssetManager::GetTexture(TEX_WIREINVENTORY0);
		wireInventory1 = AssetManager::GetTexture(TEX_WIREINVENTORY1);
		wireInventory2 = AssetManager::GetTexture(TEX_WIREINVENTORY2);
	}
} 
