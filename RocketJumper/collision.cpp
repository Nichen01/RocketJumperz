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

	void Xcheck(int map[],int x,int s) {
		objectinfo[player].xPos += objectinfo[player].velocityX;

		calcCorners(map, x, &objectinfo[player]);

		int boolTR = (objectinfo[player].TR == 1);
		int boolTL = (objectinfo[player].TL == 1);
		int boolBR = (objectinfo[player].BR == 1);
		int boolBL = (objectinfo[player].BL == 1);

		if (objectinfo[player].velocityX > 0 && (boolTR || boolBR)) {
			objectinfo[player].xPos = ((float)((objectinfo[player].rightX * s) - (objectinfo[player].xScale / 2.0) - 0.001f - 800.0f));
			objectinfo[player].velocityX = 0;
		}

		if (objectinfo[player].velocityX < 0 && (boolTL || boolBL)) {
			objectinfo[player].xPos = ((float)(((objectinfo[player].rightX) * s) + (objectinfo[player].xScale / 2.0) + 0.001f - 800.0f));
			objectinfo[player].velocityX = 0;
		}
	}
	void Ycheck(int map[], int x, int s) {
		objectinfo[player].yPos += objectinfo[player].velocityY;

		calcCorners(map, x, &objectinfo[player]);

		int boolTR = (objectinfo[player].TR == 1);
		int boolTL = (objectinfo[player].TL == 1);
		int boolBR = (objectinfo[player].BR == 1);
		int boolBL = (objectinfo[player].BL == 1);

		if (objectinfo[player].velocityY < 0 && (boolBL || boolBR)) {
			objectinfo[player].yPos = 450.0f - ((float)((objectinfo[player].bottomY * s) - (objectinfo[player].yScale / 2.0) - 0.001f));
			objectinfo[player].velocityY = 0;

		}

		if (objectinfo[player].velocityY > 0 && (boolTL || boolTR)) {
			objectinfo[player].yPos = 450.0f - ((float)((objectinfo[player].bottomY * s) + (objectinfo[player].yScale / 2.0) + 0.001f));
			objectinfo[player].velocityY = 0;

		}
	}
}