/* Start Header ************************************************************************/
/*!
\file		  PauseMenu.cpp
\author 	  Ivan Chong (i.chong)
\date         April, 04, 2026
\brief        functions used to create Pause Menu

Copyright (C) 2026 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#include "PauseMenu.h"
#include "Draw.h"
#include "Sound.h"
#include "Main.h"
#include "Confirmation.h"
#include "AssetManager.h"

static s8 pausefont = -1;
static f32 width, height;

static MenuButton resumeButton;
static MenuButton exitButton;
static MenuButton tomenuButton;
static MenuButton yesButton;
static MenuButton noButton;

// Volume control buttons (positioned at top/bottom of the sound bar)
static MenuButton volumeUpButton;
static MenuButton volumeDownButton;
static AEGfxTexture* volumeUpTex   = nullptr;
static AEGfxTexture* volumeDownTex = nullptr;

AEGfxTexture* sCounttex;

static bool destructive = false;
static s8 leave = 0;
extern bool prevCleared1, prevCleared2, prevCleared3;

void Pause_Load() {
	// load textures
	load::pauseMenu();
	sCounttex = AEGfxTextureLoad("Assets/UI/Menus/greenButton.png");
	// load font
	pausefont = AEGfxCreateFont("Assets/Fonts/gameover.ttf", static_cast<int>(48 * screenscale));

	// Load volume button textures
	AssetManager::LoadTexture(TEX_VOLUME_UP,   "Assets/UI/volumeUp.png");
	AssetManager::LoadTexture(TEX_VOLUME_DOWN,  "Assets/UI/volumeDown.png");
	volumeUpTex   = AssetManager::GetTexture(TEX_VOLUME_UP);
	volumeDownTex = AssetManager::GetTexture(TEX_VOLUME_DOWN);
}

void Pause_Initialize() {
	//initilise meshes
	AssetManager::BuildSqrMesh(MESH_BUTTON);
	buttonMesh = AssetManager::GetMesh(MESH_BUTTON);

	// initialize data
	Confirmation_Init(yesButton,noButton);
	float buttonwidth = 390.0f* screenscale;
	float buttonlength = 80.0f*  screenscale;
	resumeButton = { 0.0f, 0.0f, buttonwidth, buttonlength, 1.0f, 1.0f, "RESUME", false };
	tomenuButton = { 0.0f, -120.0f *  screenscale, buttonwidth, buttonlength, 1.0f, 1.0f, "MAIN MENU", false };
	exitButton = { 0.0f, -240.0f *  screenscale, buttonwidth, buttonlength, 1.0f, 1.0f, "EXIT", false };

	// Volume up/down buttons, aligned with the sound bar at x=-400.
	// The sound bar squares start at y=-250 and go up by 56px each (10 max).
	// Volume Up sits above the top of the bar, Volume Down below the bottom.
	const f32 volBtnSize = 50.0f *  screenscale;
	const f32 soundBarX  = -400.0f *  screenscale;
	volumeUpButton   = { soundBarX , 310.0f *  screenscale,  volBtnSize , volBtnSize , 1.0f, 1.0f, "", false };
	volumeDownButton = { soundBarX , -310.0f *  screenscale, volBtnSize , volBtnSize , 1.0f, 1.0f, "", false };
}
void Pause_Update() {
	//checks if button are hovered over
	if (!destructive) {
		MenuHelpers::updateButtonHover(resumeButton);
		MenuHelpers::updateButtonHover(tomenuButton);
		MenuHelpers::updateButtonHover(exitButton);
		MenuHelpers::updateButtonHover(volumeUpButton);
		MenuHelpers::updateButtonHover(volumeDownButton);
	}
	else {
		Confirmation_Update(yesButton, noButton,leave);
	}

	//====== AUDIO CONTROLS ======//
	if (AEInputCheckTriggered(AEVK_MINUS)) {
		MainVolume -= 0.1f;
		--sCount;
		MainVolume = MainVolume <= 0.f ? 0.0f : MainVolume;
		sCount = sCount <= 0? 0:sCount ;
		AEAudioSetGroupVolume(bgm, MainVolume);
	}
	if (AEInputCheckTriggered(AEVK_EQUAL)) {
		MainVolume += 0.1f;
		++sCount;
		MainVolume = MainVolume >= 1.f ? 1.0f : MainVolume;
		sCount = sCount >= 10 ? 10 : sCount;
		AEAudioSetGroupVolume(bgm, MainVolume);
	}

	//====== AUDIO CONTROLS (clickable volume buttons) ======//
	if (AEInputCheckTriggered(AEVK_LBUTTON)) {
		if (volumeUpButton.isHovered) {
			MainVolume += 0.1f;
			++sCount;
			MainVolume = MainVolume >= 1.f ? 1.0f : MainVolume;
			sCount = sCount >= 10 ? 10 : sCount;
			AEAudioSetGroupVolume(bgm, MainVolume);
		}
		if (volumeDownButton.isHovered) {
			MainVolume -= 0.1f;
			--sCount;
			MainVolume = MainVolume <= 0.f ? 0.0f : MainVolume;
			sCount = sCount <= 0 ? 0 : sCount;
			AEAudioSetGroupVolume(bgm, MainVolume);
		}
	}

	if (AEInputCheckTriggered(AEVK_LBUTTON)) {
		if (resumeButton.isHovered) {
			pause = false;  // Change to test file if needed
		}
		if (tomenuButton.isHovered) {
			//confirmation check
			destructive = true;
			if (leave == 1) {
				destructive = false;
				leave = 0;
				next = GS_MAINMENU;  // Change to test file if needed
			}
			else if (leave == 2) {
				destructive = false;
				leave = 0;
			}
			prevCleared1 = 0;
			prevCleared2 = 0;
			prevCleared3 = 0;
		}
		else if (exitButton.isHovered) {
			//confirmation check
			destructive = true;
			if (leave==1) {
				destructive = false;
				leave = 0;
				next = GS_QUIT;
			}
			else if (leave == 2) {
				destructive = false;
				leave = 0;
			}
		}
	}
}

void Pause_Draw() {
	// render pause menu
	renderlogic::drawTexture(0.f, 0.f, menuTex, buttonMesh, 500.f *  screenscale, 640.f *  screenscale);

	AEGfxGetPrintSize(pausefont, "PAUSE", 1.f, &width, &height);
	AEGfxPrint(pausefont, "PAUSE", -0.06f-width / 2, 0.55f - height / 2, 1, 1, 1, 1, 1);

	MenuHelpers::TexdrawButton(resumeButton, buttonMesh, pausefont, buttonTex);
	MenuHelpers::TexdrawButton(tomenuButton, buttonMesh, pausefont, buttonTex);
	MenuHelpers::TexdrawButton(exitButton, buttonMesh, pausefont, buttonTex);

	// render sound
	renderlogic::drawTexture(-400.f *  screenscale, 0.0f, buttonTex, buttonMesh, 640.0f *  screenscale, 150.0f *  screenscale,PI/2.0f);
	for (s8 i{}; i < sCount; ++i) {
		renderlogic::drawTexture(-400.f *  screenscale, (- 250.f + (static_cast<f32>(i) * 56)) *  screenscale, sCounttex, buttonMesh, 45.f *  screenscale, 45.f *  screenscale);
	}

	// Draw volume up button (above the sound bar) and volume down button (below)
	if (volumeUpTex) {
		renderlogic::drawTexture(volumeUpButton.x, volumeUpButton.y,
			volumeUpTex, buttonMesh,
			volumeUpButton.width * volumeUpButton. scale,
			volumeUpButton.height * volumeUpButton.scale);
	}
	if (volumeDownTex) {
		renderlogic::drawTexture(volumeDownButton.x, volumeDownButton.y,
			volumeDownTex, buttonMesh,
			volumeDownButton.width * volumeDownButton. scale,
			volumeDownButton.height * volumeDownButton. scale);
	}

	if (destructive) {
		Confirmation_Draw(pausefont, yesButton,noButton);
	}
}

void Pause_Free() {
	// Free meshes
	AssetManager::FreeAllMeshes();
	if (sCounttex) {
		AEGfxTextureUnload(sCounttex);
		sCounttex = nullptr;
	}
}
void Pause_Unload() {
	// unload textures and sound
	AssetManager::UnloadAllTextures();
}