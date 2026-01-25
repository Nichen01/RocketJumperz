#pragma once
#include <crtdbg.h> // To check for memory leaks
#include "AEEngine.h"
struct objectsquares {
	f32 xPos;

	f32 yPos;

	f32 xScale;

	f32 yScale;

	f32 velocityX;
	
	f32 velocityY;
};

enum objecttype {
	player,
	obstacle,
	projectile
};

namespace gamelogic {
	s8 collision(objectsquares* player, objectsquares* obstacle);
}

extern objectsquares objectinfo[2];

extern AEGfxVertexList* pMesh;