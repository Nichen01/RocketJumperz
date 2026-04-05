/* Start Header ************************************************************************/
/*!
\file		  Collision.h
\author 	  Ivan Chong (i.chong)
\date         April, 04, 2026
\brief        Header file for Collision.cpp

Copyright (C) 2026 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

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

	// Swept-AABB dynamic collision check between two moving objects.
	// Returns 1 if A and B will collide within the current frame, 0 otherwise.
	s8 dynamic_collision(objectsquares* A, objectsquares* B, float& firstTimeOfCollision);

	// Static AABB collision (original verbose version kept for compatibility)
	s8 collision(objectsquares* player, objectsquares* obstacle);

	// Static AABB collision (compact version from Ivan)
	s8 static_collision(objectsquares* player, objectsquares* obstacle);

	inline bool CollisionIntersection_RectRect(objectsquares* player,objectsquares* obstacle, float& firstTimeOfCollision)		//Output: return the calculated value of tFirst
	{
		//Step 1
		bool staticCollision = false;
		staticCollision = static_collision(player, obstacle);
		if (staticCollision)
		{
			return true;
		}

		//Step 2 until 5
		return dynamic_collision(player, obstacle,firstTimeOfCollision);
	}
	//change x world position to normalized coordinate
	float posX_to_index(float pos, float MS);
	//change y world position to normalized coordinate
	float posY_to_index(float pos, float MS);
	//change x normalized coordinate to world position
	float index_to_posX(float pos, float MS);
	//change y normalized coordinate to world position
	float index_to_posY(float pos, float MS);

	// Hotspot check and binary map collision
	int CheckInstanceBinaryMapCollision(float PosX, float PosY, float scaleX, float scaleY, int map[], int index, int mapX);
	// calculate collision and movement of object
	void Collision_movement(objectsquares* object, int map[], int mapX, int mapS, int index);

}
