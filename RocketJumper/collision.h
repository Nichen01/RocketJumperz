#pragma once
#include <crtdbg.h> // To check for memory leaks
#include "AEEngine.h"
#include "player.h"

// Time variables used by physics calculations (defined in collision.cpp)
extern f64 g_fixedDT;
extern f64 g_dt;
extern f64 g_appTime;

// Note: COLLISION_LEFT/RIGHT/TOP/BOTTOM flags are defined in binaryMap.h
// Do NOT redeclare them here to avoid duplicate symbol errors.

namespace gamelogic {
	// Update bounding box for an object using AEVec2-based BB struct
	void Obj_boundingbox(objectsquares* object);

	// Swept-AABB dynamic collision check between two moving objects.
	// Returns 1 if A and B will collide within the current frame, 0 otherwise.
	s8 dynamic_collision(objectsquares* A, objectsquares* B);

	// Static AABB collision (original verbose version kept for compatibility)
	s8 collision(objectsquares* player, objectsquares* obstacle);

	// Static AABB collision (compact version from Ivan)
	s8 static_collision(objectsquares* player, objectsquares* obstacle);

	// Hotspot check and binary map collision
	void CheckInstanceBinaryMapCollision(objectsquares* object, int map[], int mapX, int index);

	// Resolve object-to-tilemap collision. 'index' specifies which tile value
	// counts as solid (pass 1 for standard walls).
	void OBJ_to_map(int map[], int x, int s, objectsquares* object, int index);
}
