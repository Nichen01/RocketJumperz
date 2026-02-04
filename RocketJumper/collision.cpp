#include <crtdbg.h> // To check for memory leaks
#include "AEEngine.h"
#include "player.h"
#include "collision.h"
namespace {
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
}

namespace gamelogic {
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