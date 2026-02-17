#pragma once
#include "AEEngine.h"

struct objectsquares {
	f32 xPos;
	f32 yPos;

	f32 xScale;
	f32 yScale;

	f32 BBminx;
	f32 BBminy;

	f32 BBmaxx;
	f32 BBmaxy;

	u8 flag;

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
	obstacle,
	enemy
};




extern objectsquares objectinfo[2];
