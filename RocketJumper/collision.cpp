#include <crtdbg.h> // To check for memory leaks
#include "AEEngine.h"
#include "player.h"
#include "collision.h"
#include "binaryMap.h"
f64					g_fixedDT = 0.01667;

// stores the game loop time that you must use in all your physics calculations
f64					g_dt = 0.0;

// stores to total application time until the current loop
f64					g_appTime = 0.0;

namespace {
	

	void calcCorners(int map[], int mapX, int mapS, objectsquares* player1) {

		//determines coordinates in 2D array

		player1->leftX = (int)((player1->xPos - (player1->xScale / 2.0)) + 800) / mapS;
		player1->rightX = (int)((player1->xPos + (player1->xScale / 2.0)) + 800) / mapS;
		player1->topY = -((int)((player1->yPos + (player1->yScale / 2.0)) - 450) / mapS);
		player1->bottomY = -((int)((player1->yPos - (player1->yScale / 2.0)) - 450) / mapS);


		//determine data in corners, top bottom left right
		player1->TR = map[player1->topY * mapX + player1->rightX];
		player1->TL = map[player1->topY * mapX + player1->leftX];
		player1->BR = map[player1->bottomY * mapX + player1->rightX];
		player1->BL = map[player1->bottomY * mapX + player1->leftX];
	}

	void boundingbox(objectsquares* object) {
		object->BBminx = (object->xScale * -(1.0f / 2.0f)) + object->xPos;
		object->BBminy = (object->yScale * -(1.0f / 2.0f)) + object->yPos;

		object->BBmaxx = (object->xScale * (1.0f / 2.0f)) + object->xPos;
		object->BBmaxy = (object->yScale * (1.0f / 2.0f)) + object->yPos;
	}
}


namespace gamelogic {
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
			if (A->BBminx > B->BBmaxx) {
				return false;
			}
			if (A->BBmaxx < B->BBminx) {
				tFirst = tFirst > ((A->BBmaxx - B->BBminx) / Vb) ? tFirst : (A->BBmaxx - B->BBminx) / Vb;
			}
			if (A->BBminx < B->BBmaxx) {
				tLast = tLast < ((A->BBminx - B->BBmaxx) / Vb) ? tLast : (A->BBminx - B->BBmaxx) / Vb;
			}
		}
		else if (Vb > 0) {
			if (A->BBminx > B->BBmaxx) {
				tFirst = tFirst > ((A->BBminx - B->BBmaxx) / Vb) ? tFirst : (A->BBminx - B->BBmaxx) / Vb;
			}
			if (A->BBmaxx > B->BBminx) {
				tLast = tLast < ((A->BBmaxx - B->BBminx) / Vb) ? tLast : (A->BBmaxx - B->BBminx) / Vb;
			}
			if (A->BBmaxx < B->BBminx) {
				return false;
			}
		}
		else {
			// No relative X velocity -- objects must already overlap on X
			if (A->BBmaxx < B->BBminx) {
				return false;
			}
			else if (A->BBminx > B->BBmaxx) {
				return false;
			}
		}

		if (tFirst > tLast) {
			return false;
		}

		// --- Y-axis interval check ---
		Vb = (B->velocityY) - (A->velocityY);

		if (Vb < 0) {
			if (A->BBminy > B->BBmaxy) {
				return false;
			}
			if (A->BBmaxy < B->BBminy) {
				tFirst = tFirst > ((A->BBmaxy - B->BBminy) / Vb) ? tFirst : (A->BBmaxy - B->BBminy) / Vb;
			}
			if (A->BBminy < B->BBmaxy) {
				tLast = tLast < ((A->BBminy - B->BBmaxy) / Vb) ? tLast : (A->BBminy - B->BBmaxy) / Vb;
			}
		}
		else if (Vb > 0) {
			if (A->BBminy > B->BBmaxy) {
				tFirst = tFirst > ((A->BBminy - B->BBmaxy) / Vb) ? tFirst : (A->BBminy - B->BBmaxy) / Vb;
			}
			if (A->BBmaxy > B->BBminy) {
				tLast = tLast < ((A->BBmaxy - B->BBminy) / Vb) ? tLast : (A->BBmaxy - B->BBminy) / Vb;
			}
			if (A->BBmaxy < B->BBminy) {
				return false;
			}
		}
		else {
			// No relative Y velocity -- objects must already overlap on Y
			if (A->BBmaxy < B->BBminy) {
				return false;
			}
			else if (A->BBminy > B->BBmaxy) {
				return false;
			}
		}

		if (tFirst > tLast) {
			return false;
		}

		return true;
	}

	// Static AABB collision check (unchanged from original)
	s8 collision(objectsquares* player, objectsquares* obstacle) {
		s8 collisionX;
		s8 collisionY;

		if (player->xPos- (player->xScale / 2.0f) < obstacle->xPos + (obstacle->xScale / 2.0f) &&
			player->xPos + (player->xScale / 2.0f) > obstacle->xPos - (obstacle->xScale / 2.0f))
		{
			collisionX = 1;
		}
		else {
			collisionX = 0;
		}
		if (player->yPos - (player->yScale / 2.0f) < obstacle->yPos + (obstacle->yScale / 2.0f) &&
			player->yPos + (player->yScale / 2.0f) > obstacle->yPos - (obstacle->yScale / 2.0f))

		{
			collisionY = 1;
		}
		else {
			collisionY = 0;
		}
		// collision only if on both axes
		if (collisionX && collisionY) {
			return 1;
		}
		else {
			return 0;
		}
	}

	// Resolve object-to-tilemap collision. 'index' specifies which tile value
	// counts as solid (e.g., pass 1 for standard walls).
	void OBJ_to_map(int map[],int x,int s, objectsquares* object, int index) {
		object->xPos += object->velocityX;

		calcCorners(map, x, s ,object);

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
	

	void CheckInstanceBinaryMapCollision(objectsquares* object, int map[])
	{
		// Collision flags (COLLISION_LEFT, etc.) are defined in binaryMap.h

		float x1, y1, x2, y2;
		object->flag = 0;

		//right
		x1 = object->xPos + object->xScale / 2;
		y1 = object->yPos + object->yScale / 4;

		x2 = object->xPos + object->xScale / 2;
		y2 = object->yPos - object->yScale / 4;

		if (map[(int)(y1 * 32 + x1)] == 1 || map[(int)(y2 * 32 + x2)] == 1) {
			object->flag = object->flag | COLLISION_RIGHT;
		}

		//left
		x1 = object->xPos - object->xScale / 2;
		y1 = object->yPos - object->yScale / 4;

		x2 = object->xPos - object->xScale / 2;
		y2 = object->yPos + object->yScale / 4;

		if (map[(int)(y1 * 32 + x1)] == 1 || map[(int)(y2 * 32 + x2)] == 1) {
			object->flag = object->flag | COLLISION_LEFT;
		}
		//top
		x1 = object->xPos - object->xScale / 4;
		y1 = object->yPos + object->yScale / 2;

		x2 = object->xPos + object->xScale / 4;
		y2 = object->yPos + object->yScale / 2;

		if (map[(int)(y1 * 32 + x1)] == 1 || map[(int)(y2 * 32 + x2)] == 1) {
			object->flag = object->flag | COLLISION_TOP;
		}
		//bottom
		x1 = object->xPos - object->xScale / 4;
		y1 = object->yPos - object->yScale / 2;

		x2 = object->xPos + object->xScale / 4;
		y2 = object->yPos - object->yScale / 2;

		if (map[(int)(y1 * 32 + x1)] == 1 || map[(int)(y2 * 32 + x2)] == 1) {
			object->flag = object->flag | COLLISION_BOTTOM;
		}
	}
}