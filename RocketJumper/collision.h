#include <crtdbg.h> // To check for memory leaks
#include "AEEngine.h"
#include "player.h"

// Time variables used by physics calculations (defined in collision.cpp)
extern f64 g_fixedDT;
extern f64 g_dt;
extern f64 g_appTime;

namespace gamelogic {
	s8 collision(objectsquares* player, objectsquares* obstacle);

	// Swept-AABB dynamic collision check between two moving objects.
	// Returns 1 if A and B will collide within the current frame, 0 otherwise.
	s8 dynamic_collision(objectsquares* A, objectsquares* B);

	// Resolve object-to-tilemap collision. The 'index' param specifies which
	// tile value counts as solid (pass 1 for standard walls).
	void OBJ_to_map(int map[], int x, int s, objectsquares* object, int index);
}


