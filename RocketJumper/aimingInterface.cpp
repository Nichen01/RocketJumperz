#include "aimingInterface.h"
namespace aiming {
	f32 angle;
	AEMtx33 aimTransform = { 0 };
	AEGfxTexture* aimingInterface = nullptr;
	void loadAiming() {
		aimingInterface = AEGfxTextureLoad("Assets/aim.png");
		if (!aimingInterface)  printf("\ntexture failed\n"); 
	}
	void updateAiming(objectsquares& player) {
		AEMtx33 scale = { 0 };
		AEMtx33Scale(&scale, player.xScale, player.yScale);

		AEMtx33 rotate = { 0 };
		//angle = movement::getMouse(player).y/ movement::getMouse(player).x;
		angle = static_cast<f32>(atan2(movement::getMouse(player).y , movement::getMouse(player).x));
		printf("\nangle:%f\n", angle);
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
		//AEGfxSetColorToAdd(0, 0, 0, 0);
		AEGfxSetColorToMultiply(1, 1, 1, 1);
		AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);
	}
	void unloadAiming() {
		if (aimingInterface) AEGfxTextureUnload(aimingInterface);
	}
}