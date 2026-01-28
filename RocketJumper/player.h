#pragma once
#include "AEEngine.h"

struct objectsquares {
	f32 xPos;
	f32 yPos;

	f32 xScale;
	f32 yScale;

	int leftX;
	int rightX;
	int topY;
	int bottomY;
	int TR;
	int TL;
	int BR;
	int BL;

	f32 speed;
	f32 velocityX;
	f32 velocityY;

	f32 dx;
	f32 dy;
};

enum objecttype {
	player,
	obstacle
};


void calcCorners(int map[], int mapX, objectsquares* player1);

extern objectsquares objectinfo[2];
