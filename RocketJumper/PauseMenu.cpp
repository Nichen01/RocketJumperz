#include "PauseMenu.h"
#include "Draw.h"
#include "Main.h"


static s8 pausefont = -1;
static f32 width, height;

static MenuButton resumeButton;
static MenuButton exitButton;
static MenuButton tomenuButton;
extern bool prevCleared1, prevCleared2, prevCleared3;

void Pause_Load() {

	load::pauseMenu();
	
	pausefont = AEGfxCreateFont("Assets/Fonts/gameover.ttf", 72);
}

void Pause_Initialize() {

	AssetManager::BuildSqrMesh(MESH_BUTTON);
	buttonMesh = AssetManager::GetMesh(MESH_BUTTON);

	float buttonwidth = 390.0f;
	float buttonlength = 80.0f;
	resumeButton = { 0.0f, 0.0f, buttonwidth, buttonlength, 1.0f, 1.0f, "RESUME", false };
	tomenuButton = { 0.0f, -120.0f, buttonwidth, buttonlength, 1.0f, 1.0f, "MAIN MENU", false };
	exitButton = { 0.0f, -240.0f, buttonwidth, buttonlength, 1.0f, 1.0f, "EXIT", false };
}
void Pause_Update() {
	MenuHelpers::updateButtonHover(resumeButton);
	MenuHelpers::updateButtonHover(tomenuButton);
	MenuHelpers::updateButtonHover(exitButton);

	if (AEInputCheckTriggered(AEVK_LBUTTON)) {
		if (resumeButton.isHovered) {
			pause = false;  // Change to test file if needed
			printf("Play button clicked - Starting game!\n");
		}
		if (tomenuButton.isHovered) {
			next = GS_MAINMENU;  // Change to test file if needed
			prevCleared1 = 0;
			prevCleared2 = 0;
			prevCleared3 = 0;
			printf("Play button clicked - Starting game!\n");
		}
		else if (exitButton.isHovered) {
			next = GS_QUIT;
			printf("Exiting game!\n");
		}
	}
}

void Pause_Draw() {

	//AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
	//AEGfxTextureSet(menutex, 0, 0);
	//AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f); 
	//renderlogic::drawSquare(0.0f, 0.0f, 500.0f, 640.0f);
	//AEGfxMeshDraw(buttonMesh, AE_GFX_MDM_TRIANGLES);

	renderlogic::drawTexture(0.f, 0.f, menuTex, buttonMesh, 500.f, 640.f);

	AEGfxGetPrintSize(pausefont, "PAUSE", 1.f, &width, &height);
	AEGfxPrint(pausefont, "PAUSE", -0.06f-width / 2, 0.55f - height / 2, 1, 1, 1, 1, 1);

	MenuHelpers::TexdrawButton(resumeButton, buttonMesh, pausefont, buttonTex);
	MenuHelpers::TexdrawButton(tomenuButton, buttonMesh, pausefont, buttonTex);
	MenuHelpers::TexdrawButton(exitButton, buttonMesh, pausefont, buttonTex);
}

void Pause_Free() {

	AssetManager::FreeAllMeshes();
}
void Pause_Unload() {
	AssetManager::UnloadAllTextures();
}