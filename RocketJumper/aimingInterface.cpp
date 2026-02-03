#include "aimingInterface.h"
namespace aiming {
	f32 angle;
	void loadAiming() {
		aimingInterface = AEGfxTextureLoad("Assets/aim.png");
	}
	void updateAiming(objectsquares& player) {
		AEMtx33 scale = { 0 };
		AEMtx33Scale(&scale, player.xScale, player.yScale);

		AEMtx33 rotate = { 0 };
		angle = static_cast<f32>(acos(movement::getMouse(player).x));
		AEMtx33Rot(&rotate, angle);

		AEMtx33 translate = { 0 };
		AEMtx33Trans(&translate, player.xPos, player.yPos);

		AEMtx33 transform = { 0 };
		AEMtx33Concat(&transform, &rotate, &scale);
		AEMtx33Concat(&transform, &translate, &transform);

		AEGfxSetTransform(transform.m);
	}
	void drawAiming() {
		AEGfxTextureSet(aimingInterface, 0, 0);
		AEGfxMeshDraw(textureMesh, AE_GFX_MDM_TRIANGLES);
	}
	void unloadAiming() {
		if (aimingInterface) AEGfxTextureUnload(aimingInterface);
	}
}