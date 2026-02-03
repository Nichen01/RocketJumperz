#include <crtdbg.h> // To check for memory leaks
#include "AEEngine.h"
#include "collision.h"

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

	void OBJ_to_map(int map[],int x,int s, objectsquares* object) {
		object->xPos += object->velocityX;

		calcCorners(map, x, object);

		/*int boolTR = (object->TR == 1);
		int boolTL = (object->TL == 1);
		int boolBR = (object->BR == 1);
		int boolBL = (object->BL == 1);*/
		int boolTR = object->TR;
		int boolTL = object->TL;
		int boolBR = object->BR;
		int boolBL = object->BL;

		if (object->velocityX > 0 && (boolTR==1 || boolBR==1)) {
			object->xPos = ((float)((object->rightX * s) - (object->xScale / 2.0) - 0.001f - 800.0f));
			object->velocityX = 0;
		}

		if (object->velocityX < 0 && (boolTL==1 || boolBL==1)) {
			object->xPos = ((float)(((object->rightX) * s) + (object->xScale / 2.0) + 0.001f - 800.0f));
			object->velocityX = 0;
		}

		if ((object->velocityX < 0 && (boolTL == 10 || boolBL == 10))||
			(object->velocityX > 0 && (boolTR == 10 || boolBR == 10))){
			next = (next == GS_LEVEL1) ? GS_LEVEL2 : GS_LEVEL1;
		}

		object->yPos += object->velocityY;

		calcCorners(map, x, object);

		boolTR = object->TR;
		boolTL = object->TL;
		boolBR = object->BR;
		boolBL = object->BL;

		if (object->velocityY < 0 && (boolBL==1 || boolBR==1)) {
			object->yPos = 450.0f - ((float)((object->bottomY * s) - (object->yScale / 2.0) - 0.001f));
			object->velocityY = 0;

		}

		if (object->velocityY > 0 && (boolTL==1 || boolTR==1)) {
			object->yPos = 450.0f - ((float)((object->bottomY * s) + (object->yScale / 2.0) + 0.001f));
			object->velocityY = 0;

		}
		if ((object->velocityY < 0 && (boolBL == 10 || boolBR == 10)) ||
			(object->velocityY > 0 && (boolTL == 10 || boolTR == 10))) {
			next = (next == GS_LEVEL1) ? GS_LEVEL2 : GS_LEVEL1;
		}
	}
	
}