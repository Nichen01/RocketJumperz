#include <crtdbg.h> // To check for memory leaks
#include "AEEngine.h"
#include "collision.h"



namespace gamelogic {
	s8 collision(objectsquares* player, objectsquares* obstacle) {
		s8 collisionX;
		s8 collisionY;

		f32 distanceX = (player->xScale / 2.0f) + (obstacle->xScale / 2.0f);
		f32 distanceY = (player->yScale / 2.0f) + (obstacle->yScale / 2.0f);

		if (player->xPos + distanceX >= obstacle->xPos &&
			obstacle->xPos + distanceX >= player->xPos)
		{
			collisionX = 1;
		}
		else {
			collisionX = 0;
		}
		if (player->yPos + distanceY >= obstacle->yPos &&
			obstacle->yPos + distanceY >= player->yPos)

		{
			collisionY = 1;
		}
		else {
			collisionY = 0;
		}
		if (collisionX && collisionY) {
			return 1;
		}
		else {
			return 0;
		}
	}
}