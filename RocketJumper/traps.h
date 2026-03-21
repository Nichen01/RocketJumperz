#pragma once
#include "collision.h"
#include "enemies.h"
namespace traps {
	constexpr s8 trapCooldown = 2;
	void updateTraps(Enemy enemies[], objectsquares objectinfo[], int* map, int x, int s);
}