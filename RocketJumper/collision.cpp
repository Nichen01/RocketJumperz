#include "main.h"
#include "collision.h"
#include "binaryMap.h"

f64					g_fixedDT = 0.01667;

// stores the game loop time that you must use in all your physics calculations
f64					g_dt = 0.0;

// stores to total application time until the current loop
f64					g_appTime = 0.0;

f32         BOUNDING_RECT_SIZE = 1.0f;

int COLLISION_LEFT = 0x00000001;	//0001
int COLLISION_RIGHT = 0x00000002;	//0010
int COLLISION_TOP = 0x00000004;	//0100
int COLLISION_BOTTOM = 0x00000008;	//1000


namespace gamelogic {

	// Create bounding box using AEVec2-based BB struct (Ivan's approach)
	void Obj_boundingbox(objectsquares* object) {
		AEVec2Scale(&object->boundingBox.min, &object->scale, -(BOUNDING_RECT_SIZE / 2.0f));
		AEVec2Add(&object->boundingBox.min, &object->boundingBox.min, &object->posPrev);

		AEVec2Scale(&object->boundingBox.max, &object->scale, (BOUNDING_RECT_SIZE / 2.0f));
		AEVec2Add(&object->boundingBox.max, &object->boundingBox.max, &object->posPrev);
	}

	// Namespace-level variables used by dynamic_collision for swept-AABB timing
	float tFirst = 0.0f;
	float tLast = 0.0f;

	// Swept-AABB dynamic collision between two moving objects.
	// Checks if objects A and B will overlap at any point during the current
	// frame, accounting for both objects' velocities.
	// Returns 1 (true) if collision occurs, 0 (false) otherwise.
	s8 dynamic_collision(objectsquares* A, objectsquares* B, float& firstTimeOfCollision) {
		tFirst = 0.0f;
		tLast = (float)g_dt;

		// --- X-axis interval check ---
		float Vb = (B->velocityX) - (A->velocityX);
		if (Vb < 0) {
			if (A->boundingBox.min.x > B->boundingBox.max.x) {
				return false;
			}
			if (A->boundingBox.max.x < B->boundingBox.min.x) {
				tFirst = tFirst > ((A->boundingBox.max.x - B->boundingBox.min.x) / Vb) ? tFirst : (A->boundingBox.max.x - B->boundingBox.min.x) / Vb;
			}
			if (A->boundingBox.min.x < B->boundingBox.max.x) {
				tLast = tLast < ((A->boundingBox.min.x - B->boundingBox.max.x) / Vb) ? tLast : (A->boundingBox.min.x - B->boundingBox.max.x) / Vb;
			}
		}
		else if (Vb > 0) {
			if (A->boundingBox.min.x > B->boundingBox.max.x) {
				tFirst = tFirst > ((A->boundingBox.min.x - B->boundingBox.max.x) / Vb) ? tFirst : (A->boundingBox.min.x - B->boundingBox.max.x) / Vb;
			}
			if (A->boundingBox.max.x > B->boundingBox.min.x) {
				tLast = tLast < ((A->boundingBox.max.x - B->boundingBox.min.x) / Vb) ? tLast : (A->boundingBox.max.x - B->boundingBox.min.x) / Vb;
			}
			if (A->boundingBox.max.x < B->boundingBox.min.x) {
				return false;
			}
		}
		else {
			// No relative X velocity -- objects must already overlap on X
			if (A->boundingBox.max.x < B->boundingBox.min.x) {
				return false;
			}
			else if (A->boundingBox.min.x > B->boundingBox.max.x) {
				return false;
			}
		}

		if (tFirst > tLast) {
			return false;
		}

		// --- Y-axis interval check ---
		Vb = (B->velocityY) - (A->velocityY);

		if (Vb < 0) {
			if (A->boundingBox.min.y > B->boundingBox.max.y) {
				return false;
			}
			if (A->boundingBox.max.y < B->boundingBox.min.y) {
				tFirst = tFirst > ((A->boundingBox.max.y - B->boundingBox.min.y) / Vb) ? tFirst : (A->boundingBox.max.y - B->boundingBox.min.y) / Vb;
			}
			if (A->boundingBox.min.y < B->boundingBox.max.y) {
				tLast = tLast < ((A->boundingBox.min.y - B->boundingBox.max.y) / Vb) ? tLast : (A->boundingBox.min.y - B->boundingBox.max.y) / Vb;
			}
		}
		else if (Vb > 0) {
			if (A->boundingBox.min.y > B->boundingBox.max.y) {
				tFirst = tFirst > ((A->boundingBox.min.y - B->boundingBox.max.y) / Vb) ? tFirst : (A->boundingBox.min.y - B->boundingBox.max.y) / Vb;
			}
			if (A->boundingBox.max.y > B->boundingBox.min.y) {
				tLast = tLast < ((A->boundingBox.max.y - B->boundingBox.min.y) / Vb) ? tLast : (A->boundingBox.max.y - B->boundingBox.min.y) / Vb;
			}
			if (A->boundingBox.max.y < B->boundingBox.min.y) {
				return false;
			}
		}
		else {
			// No relative Y velocity -- objects must already overlap on Y
			if (A->boundingBox.max.y < B->boundingBox.min.y) {
				return false;
			}
			else if (A->boundingBox.min.y > B->boundingBox.max.y) {
				return false;
			}
		}

		if (tFirst > tLast) {
			return false;
		}

		firstTimeOfCollision = tFirst;
		return true;
	}

	// Static AABB collision (compact version)
	s8 static_collision(objectsquares* player, objectsquares* obstacle) {
		return (player->xPos - (player->xScale / 2.0f) < obstacle->xPos + (obstacle->xScale / 2.0f) &&
			player->xPos + (player->xScale / 2.0f) > obstacle->xPos - (obstacle->xScale / 2.0f) &&
			(player->yPos - (player->yScale / 2.0f) < obstacle->yPos + (obstacle->yScale / 2.0f) &&
				player->yPos + (player->yScale / 2.0f) > obstacle->yPos - (obstacle->yScale / 2.0f)));
	}

	// Static AABB collision (original version kept for compatibility)
	s8 collision(objectsquares* player, objectsquares* obstacle) {
		return (player->xPos - (player->xScale / 2.0f) < obstacle->xPos + (obstacle->xScale / 2.0f) &&
			player->xPos + (player->xScale / 2.0f) > obstacle->xPos - (obstacle->xScale / 2.0f) &&
			(player->yPos - (player->yScale / 2.0f) < obstacle->yPos + (obstacle->yScale / 2.0f) &&
				player->yPos + (player->yScale / 2.0f) > obstacle->yPos - (obstacle->yScale / 2.0f)));
	}

	// Hotspot check and binary map collision.
	// mapX = number of tile columns. index = tile value to treat as solid.
	// Collision flags (COLLISION_LEFT, etc.) are defined in binaryMap.h.

	float posX_to_index(float pos, float MS) {
		return (pos + 800) / (float)MS;
	}

	float posY_to_index(float pos, float MS) {
		return -((pos - 450) / (float)MS);
	}


	int CheckInstanceBinaryMapCollision(float PosX, float PosY, float scaleX, float scaleY, int map[], int index, int mapX)
	{
		float x1, y1, x2, y2;
		int flag = 0;
		// right

		x1 = PosX + scaleX / 2.0f;
		y1 = PosY + scaleY / 4.0f;

		x2 = PosX + scaleX / 2.0f;
		y2 = PosY - scaleY / 4.0f;
	
		int index1 = (int)y1 * mapX + (int)x1;
		int index2 = (int)y2 * mapX + (int)x2;

		if (map[index1] == index || map[index2] == index) {
			flag = flag | COLLISION_RIGHT;
		}

		// left
		x1 = PosX - scaleX / 2.0f;
		y1 = PosY - scaleY / 4.0f;

		x2 = PosX - scaleX / 2.0f;
		y2 = PosY + scaleY / 4.0f;

		index1 = (int)y1 * mapX + (int)x1;
		index2 = (int)y2 * mapX + (int)x2;

		if (map[index1] == index || map[index2] == index) {
			flag = flag | COLLISION_LEFT;
		}

		// top
		x1 = PosX - scaleX / 4.0f;
		y1 = PosY + scaleY / 2.0f;

		x2 = PosX + scaleX / 4.0f;
		y2 = PosY + scaleY / 2.0f;


		index1 = (int)y1 * mapX + (int)x1;
		index2 = (int)y2 * mapX + (int)x2;

		if (map[index1] == index || map[index2] == index) {
			flag = flag | COLLISION_TOP;
		}

		// bottom
		x1 = PosX - scaleX / 4.0f;
		y1 = PosY - scaleY / 2.0f;

		x2 = PosX + scaleX / 4.0f;
		y2 = PosY - scaleY / 2.0f;

		index1 = (int)y1 * mapX + (int)x1;
		index2 = (int)y2 * mapX + (int)x2;

		if (map[index1] == index || map[index2] == index) {
			flag = flag | COLLISION_BOTTOM;
		}
		
		return flag;
	}

	void Collision_movement(objectsquares* object, int map[], int mapX, int mapS, int index) {
		object->xPos += object->velocityX;
		object->yPos += object->velocityY;

		float NposX = posX_to_index(object->xPos, (float)mapS);
		float NposY = posY_to_index(object->yPos, (float)mapS);
		float NScaleX = object->xScale/ mapS;
		float NScaleY = object->xScale / mapS;
		
		object->gridCollisionFlag = CheckInstanceBinaryMapCollision(NposX, NposY, NScaleX, NScaleY, map, index, mapX);


		if ((object->gridCollisionFlag & COLLISION_BOTTOM) == COLLISION_BOTTOM) {
			SnapToCell(&NposY);
			object->yPos= (float)(screenLength / 2) -(NposY * mapS);
			object->velocityY = 0;
		}
		if ((object->gridCollisionFlag & COLLISION_TOP) == COLLISION_TOP) {
			SnapToCell(&NposY);
			object->yPos = (float)(screenLength / 2) -(NposY * mapS);
			object->velocityY = 0;
		}
		if ((object->gridCollisionFlag & COLLISION_LEFT) == COLLISION_LEFT) {
			SnapToCell(&NposX);
			object->xPos = mapS * NposX - (float)(screenWidth/2);
			object->velocityX = 0;
		}
		if ((object->gridCollisionFlag & COLLISION_RIGHT) == COLLISION_RIGHT) {
			SnapToCell(&NposX);
			object->xPos = mapS * NposX - (float)(screenWidth / 2);
			object->velocityX = 0;
		}
	}

	
}
