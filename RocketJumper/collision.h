#include <crtdbg.h> // To check for memory leaks
#include "AEEngine.h"
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

extern objectsquares objectinfo[2];

extern AEGfxVertexList* pMesh;
extern AEGfxVertexList* HMesh;