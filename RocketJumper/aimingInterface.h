#pragma once
#include "AEEngine.h"
#include "drawWallsLevel1.h"
#include "movement.h"
#include "collision.h"

namespace aiming {
	extern AEGfxTexture *aimingInterface = nullptr;
	extern AEVec2 movement::getMouse(objectsquares&);
	void loadAiming();
	void updateAiming(objectsquares&);
	void drawAiming();
	void unloadAiming();
}