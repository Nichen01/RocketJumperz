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

namespace {
	// Calculate corners of player object relative to the tilemap.
	// mapX = number of tile columns, mapS = tile size in pixels.
	void calcCorners(int map[], int mapX, int mapS, objectsquares* player1) {

		// Determines coordinates in 2D array
		player1->leftX = (int)((player1->xPos - (player1->xScale / 2.0)) + 800) / mapS;
		player1->rightX = (int)((player1->xPos + (player1->xScale / 2.0)) + 800) / mapS;
		player1->topY = -((int)((player1->yPos + (player1->yScale / 2.0)) - 450) / mapS);
		player1->bottomY = -((int)((player1->yPos - (player1->yScale / 2.0)) - 450) / mapS);

		// Determine data in corners: top-right, top-left, bottom-right, bottom-left
		player1->TR = map[player1->topY * mapX + player1->rightX];
		player1->TL = map[player1->topY * mapX + player1->leftX];
		player1->BR = map[player1->bottomY * mapX + player1->rightX];
		player1->BL = map[player1->bottomY * mapX + player1->leftX];
	}
}

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
	s8 dynamic_collision(objectsquares* A, objectsquares* B) {
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
	void CheckInstanceBinaryMapCollision(objectsquares* object, int map[], int mapX, int index)
	{
		float x1, y1, x2, y2;
		object->flag = 0;

		// right
		x1 = object->xPos + object->xScale / 2.f;
		y1 = object->yPos + object->yScale / 4.f;

		x2 = object->xPos + object->xScale / 2.f;
		y2 = object->yPos - object->yScale / 4.f;

		if (map[(int)(y1 * mapX + x1)] == index || map[(int)(y2 * mapX + x2)] == index) {
			object->flag = static_cast<u8>(object->flag | COLLISION_RIGHT);
		}

		// left
		x1 = object->xPos - object->xScale / 2.f;
		y1 = object->yPos - object->yScale / 4.f;

		x2 = object->xPos - object->xScale / 2.f;
		y2 = object->yPos + object->yScale / 4.f;

		if (map[(int)(y1 * mapX + x1)] == index || map[(int)(y2 * mapX + x2)] == index) {
			object->flag = static_cast<u8>(object->flag | COLLISION_LEFT);
		}

		// top
		x1 = object->xPos - object->xScale / 4.f;
		y1 = object->yPos + object->yScale / 2.f;

		x2 = object->xPos + object->xScale / 4.f;
		y2 = object->yPos + object->yScale / 2.f;

		if (map[(int)(y1 * mapX + x1)] == index || map[(int)(y2 * mapX + x2)] == index) {
			object->flag = static_cast<u8>(object->flag | COLLISION_TOP);
		}

		// bottom
		x1 = object->xPos - object->xScale / 4.f;
		y1 = object->yPos - object->yScale / 2.f;

		x2 = object->xPos + object->xScale / 4.f;
		y2 = object->yPos - object->yScale / 2.f;

		if (map[(int)(y1 * mapX + x1)] == index || map[(int)(y2 * mapX + x2)] == index) {
			object->flag = static_cast<u8>(object->flag | COLLISION_BOTTOM);
		}
	}

	// Resolve object-to-tilemap collision. 'index' specifies which tile value
	// counts as solid (e.g., pass 1 for standard walls).
	// Uses our parameterized mapS via calcCorners (not hardcoded tile size).
	void OBJ_to_map(int map[], int x, int s, objectsquares* object, int index) {
		object->xPos += object->velocityX;

		calcCorners(map, x, s, object);

		int boolTR = (object->TR == index);
		int boolTL = (object->TL == index);
		int boolBR = (object->BR == index);
		int boolBL = (object->BL == index);

		if (object->velocityX > 0 && (boolTR || boolBR)) {
			object->xPos = ((float)((object->rightX * s) - (object->xScale / 2.0) - 0.001f - 800.0f));
			object->velocityX = 0;
		}

		if (object->velocityX < 0 && (boolTL || boolBL)) {
			object->xPos = ((float)(((object->rightX) * s) + (object->xScale / 2.0) + 0.001f - 800.0f));
			object->velocityX = 0;
		}

		object->yPos += object->velocityY;

		calcCorners(map, x, s, object);

		boolTR = (object->TR == index);
		boolTL = (object->TL == index);
		boolBR = (object->BR == index);
		boolBL = (object->BL == index);

		if (object->velocityY < 0 && (boolBL || boolBR)) {
			object->yPos = 450.0f - ((float)((object->bottomY * s) - (object->yScale / 2.0) - 0.001f));
			object->velocityY = 0;
		}

		if (object->velocityY > 0 && (boolTL || boolTR)) {
			object->yPos = 450.0f - ((float)((object->bottomY * s) + (object->yScale / 2.0) + 0.001f));
			object->velocityY = 0;
		}
	}
}
