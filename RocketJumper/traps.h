#pragma once
#include "collision.h"
#include "enemies.h"
namespace traps {
	constexpr s8 trapCooldown = 2;
	constexpr f32 trapSuction = 0.5;
	void initTraps();
	void updateTraps(Enemy enemies[], objectsquares objectinfo[], int* map, int x, float s);
}