/* Start Header ************************************************************************/
/*!
\file		  Load.cpp
\date         April, 01, 2026
\brief        File with helper functions to load the assets in that file

Copyright (C) 2026 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

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
	AEGfxTexture* ctrl1 = NULL, * ctrl2 = NULL, * leftArrow = NULL, * rightArrow = NULL, * leftClick = NULL, * rightClick = NULL,* eButton = NULL, * shiftButton = NULL, *lButton = NULL, * sButton = NULL, *qButton = NULL, * zButton = NULL, * button1 = NULL, * button2 = NULL, * button3 = NULL, * spacebar = NULL;
	AEGfxTexture* eyeTrap = NULL, * sawTrap = NULL, * trapBeam = NULL;
	AEGfxTexture* keyTexture = NULL;
	AEGfxTexture* redButton = NULL, * greenButton = NULL;
	AEGfxTexture* inventory = NULL, * keycardInventory = NULL;
	AEGfxTexture* brokenDoor0 = NULL, * brokenDoor1 = NULL, * brokenDoor2 = NULL;
	AEGfxTexture* wireInventory0 = NULL, * wireInventory1 = NULL, * wireInventory2 = NULL, * wireInventory3 = NULL;
	AEGfxTexture* menuTex = NULL, * buttonTex = NULL;
	AEGfxTexture* backgroundTex = NULL;
	AEGfxTexture* ammoDrop = NULL;
	AEGfxTexture* healthDrop = NULL;
	AEGfxTexture* wallBg1 = NULL;
	AEGfxTexture* borderTL = NULL, * borderT = NULL, * borderTR = NULL;
	AEGfxTexture* borderCL = NULL, * borderCR = NULL;
	AEGfxTexture* borderBL = NULL, * borderB = NULL, * borderBR = NULL;
	AEGfxTexture* staticDoor = NULL;
	AEGfxTexture* padlock = NULL;
	AEGfxTexture* setting = NULL;

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
		AssetManager::LoadTexture(TEX_SAW, "Assets/Items/saw.png");
		AssetManager::LoadTexture(TEX_EYETRAP, "Assets/Items/eye.png");
		AssetManager::LoadTexture(TEX_TRAPBEAM, "Assets/Enemy/tentacle.png");
		AssetManager::LoadTexture(TEX_DROP, "Assets/Items/bullets.png");
		AssetManager::LoadTexture(TEX_HEALTH, "Assets/Items/health.png");

		// Saw trap spritesheet (1 row, 7 cols spinning saw blade)
		AssetManager::LoadTexture(TEX_SAW, "Assets/Traps/saw.png");

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
		eyeTrap = AssetManager::GetTexture(TEX_EYETRAP);
		sawTrap = AssetManager::GetTexture(TEX_SAW);
		trapBeam = AssetManager::GetTexture(TEX_TRAPBEAM);
		ammoDrop   = AssetManager::GetTexture(TEX_DROP);
		healthDrop = AssetManager::GetTexture(TEX_HEALTH);

		// Background wall tile (decorative, non-collidable)
		AssetManager::LoadTexture(TEX_WALL_BG_1, "Assets/WallBackground.png");
		wallBg1 = AssetManager::GetTexture(TEX_WALL_BG_1);

		// Border tile textures (8 directional variants for wall borders)
		AssetManager::LoadTexture(TEX_BORDER_TL, "Assets/Platform/borderTL.png");
		AssetManager::LoadTexture(TEX_BORDER_T,  "Assets/Platform/borderT.png");
		AssetManager::LoadTexture(TEX_BORDER_TR, "Assets/Platform/borderTR.png");
		AssetManager::LoadTexture(TEX_BORDER_CL, "Assets/Platform/borderCL.png");
		AssetManager::LoadTexture(TEX_BORDER_CR, "Assets/Platform/borderCR.png");
		AssetManager::LoadTexture(TEX_BORDER_BL, "Assets/Platform/borderBL.png");
		AssetManager::LoadTexture(TEX_BORDER_B,  "Assets/Platform/borderB.png");
		AssetManager::LoadTexture(TEX_BORDER_BR, "Assets/Platform/borderBR.png");

		// Sync extern pointers for border textures
		borderTL = AssetManager::GetTexture(TEX_BORDER_TL);
		borderT  = AssetManager::GetTexture(TEX_BORDER_T);
		borderTR = AssetManager::GetTexture(TEX_BORDER_TR);
		borderCL = AssetManager::GetTexture(TEX_BORDER_CL);
		borderCR = AssetManager::GetTexture(TEX_BORDER_CR);
		borderBL = AssetManager::GetTexture(TEX_BORDER_BL);
		borderB  = AssetManager::GetTexture(TEX_BORDER_B);
		borderBR = AssetManager::GetTexture(TEX_BORDER_BR);
	}

	void ui() {
		AssetManager::LoadTexture(TEX_CTRL1, "Assets/UI/ctrl1.png");
		AssetManager::LoadTexture(TEX_CTRL2, "Assets/UI/ctrl2.png");
		AssetManager::LoadTexture(TEX_LEFT_ARROW, "Assets/UI/leftArrow.png");
		AssetManager::LoadTexture(TEX_RIGHT_ARROW, "Assets/UI/rightArrow.png");
		AssetManager::LoadTexture(TEX_LEFT_CLICK, "Assets/UI/leftClick.png");
		AssetManager::LoadTexture(TEX_RIGHT_CLICK, "Assets/UI/rightClick.png");
		AssetManager::LoadTexture(TEX_E_BUTTON, "Assets/UI/eButton.png");
		AssetManager::LoadTexture(TEX_G_BUTTON, "Assets/UI/shiftButton.png");
		AssetManager::LoadTexture(TEX_L_BUTTON, "Assets/UI/lButton.png");
		AssetManager::LoadTexture(TEX_S_BUTTON, "Assets/UI/sButton.png");
		AssetManager::LoadTexture(TEX_Q_BUTTON, "Assets/UI/qButton.png");
		AssetManager::LoadTexture(TEX_Z_BUTTON, "Assets/UI/zButton.png");
		AssetManager::LoadTexture(TEX_BUTTON1, "Assets/UI/1Button.png");
		AssetManager::LoadTexture(TEX_BUTTON2, "Assets/UI/2Button.png");
		AssetManager::LoadTexture(TEX_BUTTON3, "Assets/UI/3Button.png");
		AssetManager::LoadTexture(TEX_SPACEBAR, "Assets/UI/spacebar.png");
		AssetManager::LoadTexture(TEX_INVENTORY, "Assets/UI/inventory.png");
		AssetManager::LoadTexture(TEX_KEYCARDINVENTORY, "Assets/UI/keycardInventory.png");
		AssetManager::LoadTexture(TEX_STATIC_DOOR, "Assets/Platform/staticDoor.jpg");
		AssetManager::LoadTexture(TEX_PADLOCK, "Assets/Items/padlock.png");
		AssetManager::LoadTexture(TEX_SETTINGS, "Assets/UI/Menus/settings.png");

		// Sync the extern pointers so other files can use them directly
		ctrl1 = AssetManager::GetTexture(TEX_CTRL1);
		ctrl2 = AssetManager::GetTexture(TEX_CTRL2);
		leftArrow = AssetManager::GetTexture(TEX_LEFT_ARROW);
		rightArrow = AssetManager::GetTexture(TEX_RIGHT_ARROW);
		leftClick = AssetManager::GetTexture(TEX_LEFT_CLICK);
		rightClick = AssetManager::GetTexture(TEX_RIGHT_CLICK);
		eButton = AssetManager::GetTexture(TEX_E_BUTTON);
		shiftButton = AssetManager::GetTexture(TEX_G_BUTTON);
		lButton = AssetManager::GetTexture(TEX_L_BUTTON);
		sButton = AssetManager::GetTexture(TEX_S_BUTTON);
		qButton = AssetManager::GetTexture(TEX_Q_BUTTON);
		zButton = AssetManager::GetTexture(TEX_Z_BUTTON);
		button1 = AssetManager::GetTexture(TEX_BUTTON1);
		button2 = AssetManager::GetTexture(TEX_BUTTON2);
		button3 = AssetManager::GetTexture(TEX_BUTTON3);
		spacebar = AssetManager::GetTexture(TEX_SPACEBAR);
		inventory = AssetManager::GetTexture(TEX_INVENTORY);
		keycardInventory = AssetManager::GetTexture(TEX_KEYCARDINVENTORY);
		staticDoor = AssetManager::GetTexture(TEX_STATIC_DOOR);
		padlock = AssetManager::GetTexture(TEX_PADLOCK);
		setting = AssetManager::GetTexture(TEX_SETTINGS);
	}

	void key() {
		AssetManager::LoadTexture(TEX_KEYCARD, "Assets/Items/keycard.png");
		keyTexture = AssetManager::GetTexture(TEX_KEYCARD);
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
		AssetManager::LoadTexture(TEX_WIREINVENTORY3, "Assets/UI/wireInventory3.png");
		wireInventory0 = AssetManager::GetTexture(TEX_WIREINVENTORY0);
		wireInventory1 = AssetManager::GetTexture(TEX_WIREINVENTORY1);
		wireInventory2 = AssetManager::GetTexture(TEX_WIREINVENTORY2);
		wireInventory3 = AssetManager::GetTexture(TEX_WIREINVENTORY3);
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

	void background2() {
		AssetManager::LoadTexture(TEX_BACKGROUND, "Assets/Map/background2.png");
		backgroundTex = AssetManager::GetTexture(TEX_BACKGROUND);
	}

}
