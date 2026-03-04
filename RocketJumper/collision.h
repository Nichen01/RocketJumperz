#pragma once
#include <crtdbg.h> // To check for memory leaks
#include "AEEngine.h"
#include "player.h"
extern f64					g_fixedDT;
extern f64					g_dt;
extern f64					g_appTime;



// Time variables used by physics calculations (defined in collision.cpp)
extern f64 g_fixedDT;
extern f64 g_dt;
extern f64 g_appTime;


extern int	COLLISION_LEFT;	//0001
extern int	COLLISION_RIGHT;	//0010
extern int	COLLISION_TOP;	//0100
extern int	COLLISION_BOTTOM;	//1000

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
	int CheckInstanceBinaryMapCollision(objectsquares* object, int map[], int mapX, int index);

	void Collision_movement(objectsquares* object, int map[], int mapX, int index);

	// Resolve object-to-tilemap collision. 'index' specifies which tile value
	// counts as solid (pass 1 for standard walls).
	void OBJ_to_map(int map[], int x, int s, objectsquares* object, int index);
}
