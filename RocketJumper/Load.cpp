#include "Load.h"
#include "Draw.h"
#include "AssetManager.h"

// define texture array
AEGfxTexture* mushroomDieTexture[9] = { nullptr };
AEGfxTexture* mushroomHitTexture[5] = { nullptr };
AEGfxTexture* mushroomIdleTexture[9] = { nullptr };
AEGfxTexture* cooldownFrame[6] = { nullptr };

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
	AEGfxTexture* menuTex = NULL, * buttonTex = NULL;
	AEGfxTexture* backgroundTex = NULL;
	AEGfxTexture* ammoDrop = NULL;

	int keyX{}, keyY{};

// define the mesh
AEGfxVertexList* platformMesh = nullptr;
AEGfxVertexList* enemyMesh = nullptr;
AEGfxVertexList* pMesh = nullptr;
AEGfxVertexList* projectileMesh = nullptr;
AEGfxVertexList* uiMesh = nullptr;
AEGfxVertexList* buttonMesh = nullptr;

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
		AssetManager::LoadTexture(TEX_DROP, "Assets/ammo.jpeg");

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
		ammoDrop= AssetManager::GetTexture(TEX_DROP);
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

	void cooldownBar() {
		AssetManager::LoadTexture(TEX_COOLDOWNBAR0, "Assets/Cooldown/cooldownBar0.png");
		AssetManager::LoadTexture(TEX_COOLDOWNBAR1, "Assets/Cooldown/cooldownBar1.png");
		AssetManager::LoadTexture(TEX_COOLDOWNBAR2, "Assets/Cooldown/cooldownBar2.png");
		AssetManager::LoadTexture(TEX_COOLDOWNBAR3, "Assets/Cooldown/cooldownBar3.png");
		AssetManager::LoadTexture(TEX_COOLDOWNBAR4, "Assets/Cooldown/cooldownBar4.png");
		AssetManager::LoadTexture(TEX_COOLDOWNBAR5, "Assets/Cooldown/cooldownBar5.png");
		cooldownFrame[0] = AssetManager::GetTexture(TEX_COOLDOWNBAR0);
		cooldownFrame[1] = AssetManager::GetTexture(TEX_COOLDOWNBAR1);
		cooldownFrame[2] = AssetManager::GetTexture(TEX_COOLDOWNBAR2);
		cooldownFrame[3] = AssetManager::GetTexture(TEX_COOLDOWNBAR3);
		cooldownFrame[4] = AssetManager::GetTexture(TEX_COOLDOWNBAR4);
		cooldownFrame[5] = AssetManager::GetTexture(TEX_COOLDOWNBAR5);
	}

	void pauseMenu() {
		AssetManager::LoadTexture(TEX_MENU, "Assets/UI/Menus/Menu.png");
		AssetManager::LoadTexture(TEX_BUTTON, "Assets/UI/Menus/button.png");
		menuTex = AssetManager::GetTexture(TEX_MENU);
		buttonTex = AssetManager::GetTexture(TEX_BUTTON);
	}
	
	void background() {
		AssetManager::LoadTexture(TEX_BACKGROUND, "Assets/Map/background.jpg");
		backgroundTex = AssetManager::GetTexture(TEX_BACKGROUND);
	}

	// Null every extern texture/mesh pointer so they don't dangle after
	// AssetManager::UnloadAllTextures() / FreeAllMeshes() have freed the
	// underlying GPU resources.  Call this in each level's Unload function.
	void NullExternPointers() {
		// --- Platform textures ---
		platform1 = nullptr;  platform2 = nullptr;  platform3 = nullptr;
		platform4 = nullptr;  platform5 = nullptr;  platform6 = nullptr;
		platform7 = nullptr;  platform8 = nullptr;  platform9 = nullptr;

		// --- Glass textures ---
		glass0 = nullptr;  glass1 = nullptr;  glass2 = nullptr;
		glass3 = nullptr;  glass4 = nullptr;

		// --- Trap textures (the original bug: these were never nulled) ---
		trap1 = nullptr;
		trap2 = nullptr;
		trapBeam = nullptr;

		// --- UI textures ---
		ctrl1 = nullptr;      ctrl2 = nullptr;
		leftArrow = nullptr;  rightArrow = nullptr;
		leftClick = nullptr;  rightClick = nullptr;
		eButton = nullptr;    gButton = nullptr;
		sButton = nullptr;    zButton = nullptr;
		button1 = nullptr;    button2 = nullptr;
		button3 = nullptr;    spacebar = nullptr;
		inventory = nullptr;  keycardInventory = nullptr;

		// --- Key ---
		keyTexture = nullptr;

		// --- Error prompt / buttons ---
		errorPromptMenu = nullptr;
		redButton = nullptr;
		greenButton = nullptr;

		// --- Prompt textures ---
		prompt1 = nullptr;  prompt2 = nullptr;  prompt3 = nullptr;
		prompt4 = nullptr;  prompt5 = nullptr;  prompt6 = nullptr;
		prompt7 = nullptr;  prompt8 = nullptr;  prompt9 = nullptr;

		// --- Broken door textures ---
		brokenDoor0 = nullptr;  brokenDoor1 = nullptr;  brokenDoor2 = nullptr;

		// --- Wire inventory textures ---
		wireInventory0 = nullptr;  wireInventory1 = nullptr;  wireInventory2 = nullptr;

		// --- Menu textures ---
		menuTex = nullptr;  buttonTex = nullptr;

		// --- Background / drops ---
		backgroundTex = nullptr;
		ammoDrop = nullptr;

		// --- Cooldown bar frames ---
		for (int i = 0; i < 6; ++i) { cooldownFrame[i] = nullptr; }

		// --- Meshes ---
		platformMesh = nullptr;
		enemyMesh = nullptr;
		pMesh = nullptr;
		projectileMesh = nullptr;
		uiMesh = nullptr;
		buttonMesh = nullptr;
	}
}
