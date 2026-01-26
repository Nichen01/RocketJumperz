#include <crtdbg.h> // To check for memory leaks
#include "AEEngine.h"
#include "player.h"

namespace gamelogic {
	s8 collision(objectsquares* player, objectsquares* obstacle);
	void Xcheck(int map[], int x, int s);
	void Ycheck(int map[], int x, int s);
}


