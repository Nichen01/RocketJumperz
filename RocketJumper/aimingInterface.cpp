#include "aimingInterface.h"
namespace aiming {
	f32 angle;
	AEMtx33 aimTransform = { 0 };
	AEGfxTexture* aimingInterface = nullptr;
	s8 pFont = 0;
	void loadAiming() {
		aimingInterface = AEGfxTextureLoad("Assets/aim.png");
		if (!aimingInterface)  printf("\ntexture failed\n"); 
		pFont = AEGfxCreateFont("Assets/Fonts/gameover.ttf", 48);
		if (!pFont) printf("\nfont failed\n");
	}
	void updateAiming(objectsquares& player) {
		AEMtx33 scale = { 0 };
		AEMtx33Scale(&scale, player.xScale*3 , player.yScale*3);

		AEMtx33 rotate = { 0 };
		//angle = movement::getMouse(player).y/ movement::getMouse(player).x;
		angle = static_cast<f32>(atan2(movement::getMouse(player).y , movement::getMouse(player).x));
		angle += 5*PI / 4;
		AEMtx33Rot(&rotate, angle);

		AEMtx33 translate = { 0 };
		AEMtx33Trans(&translate, player.xPos, player.yPos);

		aimTransform = { 0 };
		AEMtx33Concat(&aimTransform, &rotate, &scale);
		AEMtx33Concat(&aimTransform, &translate, &aimTransform);

	}
	void drawAiming() {
		AEGfxTextureSet(aimingInterface, 0, 0);
		AEGfxSetTransform(aimTransform.m);
		AEGfxSetColorToMultiply(1, 1, 1, 1);
		AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
		char pText[4];
		_itoa_s(movement::bulletCount, pText, 10);
		f32 width, height;
		AEGfxGetPrintSize(pFont, pText, 1.f, &width, &height);
		AEGfxPrint(pFont, pText, 0.8, 0.8, 1,
					1, 1, 1, 1);
	}
	void unloadAiming() {
		if (aimingInterface) AEGfxTextureUnload(aimingInterface);
		if (pFont) AEGfxDestroyFont(pFont);
	}
}