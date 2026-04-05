/* Start Header ************************************************************************/
/*!
\file         traps.h
\author       Jeremiah He Shuo (jeremiahhe.shuo)
\brief        Code regarding traps: trap initiation, trap physics calculation, trap rendering and helper functions

Copyright (C) 2026 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#pragma once
#include "collision.h"
#include "enemies.h"
#include "animation.h"

namespace traps {
	constexpr s8 trapCooldown = 2;
	constexpr f32 trapSuction = 0.55f;

	// Saw animation state -- shared across all saw trap tiles (they all spin in sync)
	extern SpriteAnimation sawAnim;

	void initTraps();
	void updateTraps(Enemy enemies[], objectsquares objectinfo[], int* map, int x,int y, float s);
	void drawTraps();

	// Call once per frame to advance the saw spin animation
	void UpdateSawAnim(f32 deltaTime);

	// Returns the current U offset for the saw spritesheet (use in draw)
	f32  GetSawUOffset();
}
