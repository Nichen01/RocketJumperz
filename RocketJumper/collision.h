#include <crtdbg.h> // To check for memory leaks
#include "AEEngine.h"#pragma once
struct objectsquares {
	f32 xpos;

	f32 ypos;

	f32 xscale;

	f32 yscale;
};

enum objecttype {
	player,
	obstacle
};

namespace gamelogic {
	s8 collision(objectsquares* player, objectsquares* obstacle);
}