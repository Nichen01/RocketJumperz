#include <crtdbg.h> // To check for memory leaks
#include "AEEngine.h"
#include "player.h"
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

		calcCorners(map, x, &objectinfo[player]);

		int boolTR = (object->TR == 1);
		int boolTL = (object->TL == 1);
		int boolBR = (object->BR == 1);
		int boolBL = (object->BL == 1);

		if (object->velocityX > 0 && (boolTR || boolBR)) {
			object->xPos = ((float)((object->rightX * s) - (object->xScale / 2.0) - 0.001f - 800.0f));
			object->velocityX = 0;
		}

		if (object->velocityX < 0 && (boolTL || boolBL)) {
			object->xPos = ((float)(((object->rightX) * s) + (object->xScale / 2.0) + 0.001f - 800.0f));
			object->velocityX = 0;
		}

		object->yPos += object->velocityY;

		calcCorners(map, x, &objectinfo[player]);

		boolTR = (object->TR == 1);
		boolTL = (object->TL == 1);
		boolBR = (object->BR == 1);
		boolBL = (object->BL == 1);

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