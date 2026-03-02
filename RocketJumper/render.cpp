#include "render.h"
#include "draw.h"

namespace render {
	void Drawsquare(f32 xPos, f32 yPos, f32 xsize, f32 ysize) {
		AEMtx33 scale = { 0 };
		AEMtx33Scale(&scale, xsize, ysize);

		AEMtx33 rotate = { 0 };
		AEMtx33Rot(&rotate, 0.0f);

		AEMtx33 translate = { 0 };
		AEMtx33Trans(&translate, xPos, yPos);

		AEMtx33 transform = { 0 };
		AEMtx33Concat(&transform, &rotate, &scale);
		AEMtx33Concat(&transform, &translate, &transform);

		AEGfxSetTransform(transform.m);
	}

	void drawPlatform() {
		platform0 = AEGfxTextureLoad("Assets/Platform/platform0.png");
		platform1 = AEGfxTextureLoad("Assets/Platform/platform1.png");
		platform2 = AEGfxTextureLoad("Assets/Platform/platform2.png");
		platform3 = AEGfxTextureLoad("Assets/Platform/platform3.png");
		platform4 = AEGfxTextureLoad("Assets/Platform/platform4.png");
		platform5 = AEGfxTextureLoad("Assets/Platform/platform5.png");
	}
}