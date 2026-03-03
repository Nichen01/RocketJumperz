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
		platform1 = AEGfxTextureLoad("Assets/Platform/platform1.png");
		platform2 = AEGfxTextureLoad("Assets/Platform/platform2.png");
		platform3 = AEGfxTextureLoad("Assets/Platform/platform3.png");
		platform4 = AEGfxTextureLoad("Assets/Platform/platform4.png");
		platform5 = AEGfxTextureLoad("Assets/Platform/platform5.png");
		platform6 = AEGfxTextureLoad("Assets/Platform/platform6.png");
		platform7 = AEGfxTextureLoad("Assets/Platform/platform7.png");
		platform8 = AEGfxTextureLoad("Assets/Platform/platform8.png");
		platform9 = AEGfxTextureLoad("Assets/Platform/platform0.png");
		glass = AEGfxTextureLoad("Assets/Platform/glass.png");
	}

	void unloadPlatform() {
		if (platform1) { AEGfxTextureUnload(platform1); platform1 = nullptr; }
		if (platform2) { AEGfxTextureUnload(platform2); platform2 = nullptr; }
		if (platform3) { AEGfxTextureUnload(platform3); platform3 = nullptr; }
		if (platform4) { AEGfxTextureUnload(platform4); platform4 = nullptr; }
		if (platform5) { AEGfxTextureUnload(platform5); platform5 = nullptr; }
		if (platform6) { AEGfxTextureUnload(platform6); platform6 = nullptr; }
		if (platform7) { AEGfxTextureUnload(platform7); platform7 = nullptr; }
		if (platform8) { AEGfxTextureUnload(platform8); platform8 = nullptr; }
		if (platform9) { AEGfxTextureUnload(platform9); platform9 = nullptr; }
		if (glass) { AEGfxTextureUnload(glass); glass = nullptr; }
	}
}