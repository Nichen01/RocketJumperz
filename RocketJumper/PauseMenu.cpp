#include "PauseMenu.h"
#include "Draw.h"
#include "Main.h"
#include "Confirmation.h"



static s8 pausefont = -1;
static f32 width, height;

static MenuButton resumeButton;
static MenuButton exitButton;
static MenuButton tomenuButton;
static MenuButton yesButton;
static MenuButton noButton;
static s8 sCount;

static bool destructive = false;
static s8 leave = 0;
extern bool prevCleared1, prevCleared2, prevCleared3;

void Pause_Load() {

	load::pauseMenu();
	
	pausefont = AEGfxCreateFont("Assets/Fonts/gameover.ttf", 72);
}

void Pause_Initialize() {

	AssetManager::BuildSqrMesh(MESH_BUTTON);
	buttonMesh = AssetManager::GetMesh(MESH_BUTTON);

	sCount = 10;

	Confirmation_Init(yesButton,noButton);
	float buttonwidth = 390.0f;
	float buttonlength = 80.0f;
	resumeButton = { 0.0f, 0.0f, buttonwidth, buttonlength, 1.0f, 1.0f, "RESUME", false };
	tomenuButton = { 0.0f, -120.0f, buttonwidth, buttonlength, 1.0f, 1.0f, "MAIN MENU", false };
	exitButton = { 0.0f, -240.0f, buttonwidth, buttonlength, 1.0f, 1.0f, "EXIT", false };
}
void Pause_Update() {
	if (!destructive) {
		MenuHelpers::updateButtonHover(resumeButton);
		MenuHelpers::updateButtonHover(tomenuButton);
		MenuHelpers::updateButtonHover(exitButton);
	}
	else {
		Confirmation_Update(yesButton, noButton,leave);
	}

	//====== AUDIO CONTROLS ======//
	if (AEInputCheckTriggered(AEVK_1)) {
		MainVolume -= 0.1f;
		MainVolume = MainVolume <= 0.f ? 0.0f : MainVolume;
		sCount = static_cast<int>(MainVolume * 10.0f);
		AEAudioSetGroupVolume(bgm, MainVolume);
	}
	if (AEInputCheckTriggered(AEVK_2)) {
		MainVolume += 0.1f;
		MainVolume = MainVolume >= 1.f ? 1.0f : MainVolume;
		sCount = static_cast<int>(MainVolume * 10.0f);
		AEAudioSetGroupVolume(bgm, MainVolume);
	}

	if (AEInputCheckTriggered(AEVK_LBUTTON)) {
		if (resumeButton.isHovered) {
			pause = false;  // Change to test file if needed
			printf("Play button clicked - Starting game!\n");
		}
		if (tomenuButton.isHovered) {
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
			printf("Play button clicked - Starting game!\n");
		}
		else if (exitButton.isHovered) {
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

	renderlogic::drawTexture(0.f, 0.f, menuTex, buttonMesh, 500.f, 640.f);

	AEGfxGetPrintSize(pausefont, "PAUSE", 1.f, &width, &height);
	AEGfxPrint(pausefont, "PAUSE", -0.06f-width / 2, 0.55f - height / 2, 1, 1, 1, 1, 1);

	MenuHelpers::TexdrawButton(resumeButton, buttonMesh, pausefont, buttonTex);
	MenuHelpers::TexdrawButton(tomenuButton, buttonMesh, pausefont, buttonTex);
	MenuHelpers::TexdrawButton(exitButton, buttonMesh, pausefont, buttonTex);

	for (s8 i{}; i < sCount; ++i) {
		renderlogic::drawTexture(-400.f, -500.f+(static_cast<f32>(i)*100), menuTex, buttonMesh, 100.f, 100.f);
	}

	if (destructive) {
		Confirmation_Draw(pausefont, yesButton,noButton);
	}
}

void Pause_Free() {

	AssetManager::FreeAllMeshes();
}
void Pause_Unload() {
	AssetManager::UnloadAllTextures();
}