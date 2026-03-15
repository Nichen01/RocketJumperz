#pragma once
#include "player.h"
struct drop {
	objectsquares info;
};
namespace pickup {
	void initDrops(drop[],int,f32);
	void updateDrops(drop[],int,objectsquares&);
	void drawDrops(drop[],int);
}