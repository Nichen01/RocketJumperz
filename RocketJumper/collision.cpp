#include <crtdbg.h> // To check for memory leaks
#include "AEEngine.h"
#include "collision.h"


namespace gamelogic {
	s8 collision(objectsquares* player, objectsquares* obstacle) {
		s8 collisionX;
		s8 collisionY;

		f32 distanceX = (player->xscale / 2.0f) + (obstacle->xscale / 2.0f);
		f32 distanceY = (player->yscale / 2.0f) + (obstacle->yscale / 2.0f);

		if (player->xpos + distanceX >= obstacle->xpos &&
			obstacle->xpos + distanceX >= player->xpos)
		{
			collisionX = 1;
		}
		else {
			collisionX = 0;
		}
		// collision y-axis?
		if (player->ypos + distanceY >= obstacle->ypos &&
			obstacle->ypos + distanceY >= player->ypos)

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
}