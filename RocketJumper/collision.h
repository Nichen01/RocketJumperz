#include <crtdbg.h> // To check for memory leaks
#include "AEEngine.h"
#include "player.h"

namespace gamelogic {
	s8 collision(objectsquares* player, objectsquares* obstacle);
	void  OBJ_to_map(int map[], int x, int s, objectsquares* object,int index);
}


