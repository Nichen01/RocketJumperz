#include <crtdbg.h> // To check for memory leaks
#include "AEEngine.h"
#include "player.h"
extern f64					g_fixedDT;
extern f64					g_dt;
extern f64					g_appTime;

struct BB
{
	AEVec2	min;
	AEVec2	max;
};

extern int	COLLISION_LEFT;	//0001
extern int	COLLISION_RIGHT;	//0010
extern int	COLLISION_TOP;	//0100
extern int	COLLISION_BOTTOM;	//1000

namespace gamelogic {
	
	s8 dynamic_collision(objectsquares* player, objectsquares* obstacle);
	s8 collision(objectsquares* player, objectsquares* obstacle);
	void  OBJ_to_map(int map[], int x, int s, objectsquares* object,int index);
}


