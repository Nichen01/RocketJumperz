#include <crtdbg.h> // To check for memory leaks
#include "AEEngine.h"
#include "player.h"
#include "collision.h"
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
	
	//Calculate corners of player object(will be removed)
	void calcCorners(int map[], int mapX, objectsquares* player1) {

		//determines coordinates in 2D array

		player1->leftX = (int)((player1->xPos - (player1->xScale / 2.0)) + 800) / 100;
		player1->rightX = (int)((player1->xPos + (player1->xScale / 2.0)) + 800) / 100;
		player1->topY = -((int)((player1->yPos + (player1->yScale / 2.0)) - 450) / 100);
		player1->bottomY = -((int)((player1->yPos - (player1->yScale / 2.0)) - 450) / 100);


		//determine data in corners, top bottom left right
		player1->TR = map[player1->topY * mapX + player1->rightX];
		player1->TL = map[player1->topY * mapX + player1->leftX];
		player1->BR = map[player1->bottomY * mapX + player1->rightX];
		player1->BL = map[player1->bottomY * mapX + player1->leftX];
	}
	//create bounding box of object
	void boundingbox(objectsquares* object) {

		AEVec2Scale(&object->boundingBox.min, &object->scale, -(BOUNDING_RECT_SIZE / 2.0f)); //multiply scale by -(BOUNDING_RECT_SIZE / 2.0f) and set into pInst->boundingBox.min
		AEVec2Add(&object->boundingBox.min, &object->boundingBox.min, &object->posPrev);      //add pInst->posPrev to pInst->boundingBox.min

		AEVec2Scale(&object->boundingBox.max, &object->scale, (BOUNDING_RECT_SIZE / 2.0f));  //multiply scale by (BOUNDING_RECT_SIZE / 2.0f) and put into pInst->boundingBox.max
		AEVec2Add(&object->boundingBox.max, &object->boundingBox.max, &object->posPrev);
	}
}

namespace gamelogic {
	float tFirst = 0.0f;
	float tLast = 0.0f;
	//dynamic collision
	s8 dynamic_collision(objectsquares* A, objectsquares* B) {
		tFirst = 0.0f;
		tLast = (float)g_dt;
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
	//static collision
	s8 static_collision(objectsquares* player, objectsquares* obstacle) {

		return (player->xPos - (player->xScale / 2.0f) < obstacle->xPos + (obstacle->xScale / 2.0f) &&
			player->xPos + (player->xScale / 2.0f) > obstacle->xPos - (obstacle->xScale / 2.0f) &&
			(player->yPos - (player->yScale / 2.0f) < obstacle->yPos + (obstacle->yScale / 2.0f) &&
				player->yPos + (player->yScale / 2.0f) > obstacle->yPos - (obstacle->yScale / 2.0f)));

	}

	s8 collision(objectsquares* player, objectsquares* obstacle) {

		return (player->xPos - (player->xScale / 2.0f) < obstacle->xPos + (obstacle->xScale / 2.0f) &&
			player->xPos + (player->xScale / 2.0f) > obstacle->xPos - (obstacle->xScale / 2.0f) &&
			(player->yPos - (player->yScale / 2.0f) < obstacle->yPos + (obstacle->yScale / 2.0f) &&
				player->yPos + (player->yScale / 2.0f) > obstacle->yPos - (obstacle->yScale / 2.0f)));
		
	}
	//hotspot check and binary map collision
	void CheckInstanceBinaryMapCollision(objectsquares* object, int map[])
	{

		float x1, y1, x2, y2;
		object->flag = 0;

		//right
		x1 = object->xPos + object->xScale / 2;
		y1 = object->yPos + object->yScale / 4;

		x2 = object->xPos + object->xScale / 2;
		y2 = object->yPos - object->yScale / 4;

		if (map[(int)(y1 * 16 + x1)] == 1 || map[(int)(y2 * 16 + x2)] == 1) {
			object->flag = object->flag | COLLISION_RIGHT;
		}

		//left
		x1 = object->xPos - object->xScale / 2;
		y1 = object->yPos - object->yScale / 4;

		x2 = object->xPos - object->xScale / 2;
		y2 = object->yPos + object->yScale / 4;

		if (map[(int)(y1 * 16 + x1)] == 1 || map[(int)(y2 * 16 + x2)] == 1) {
			object->flag = object->flag | COLLISION_LEFT;
		}
		//top
		x1 = object->xPos - object->xScale / 4;
		y1 = object->yPos + object->yScale / 2;

		x2 = object->xPos + object->xScale / 4;
		y2 = object->yPos + object->yScale / 2;

		if (map[(int)(y1 * 16 + x1)] == 1 || map[(int)(y2 * 16 + x2)] == 1) {
			object->flag = object->flag | COLLISION_TOP;
		}
		//bottom
		x1 = object->xPos - object->xScale / 4;
		y1 = object->yPos - object->yScale / 2;

		x2 = object->xPos + object->xScale / 4;
		y2 = object->yPos - object->yScale / 2;

		if (map[(int)(y1 * 16 + x1)] == 1 || map[(int)(y2 * 16 + x2)] == 1) {
			object->flag = object->flag | COLLISION_BOTTOM;
		}
	}

	void OBJ_to_map(int map[],int x,int s, objectsquares* object,int index) {
		object->xPos += object->velocityX;

		calcCorners(map, x, object);

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

		calcCorners(map, x, object);

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