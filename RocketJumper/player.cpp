#include "player.h"
#include "collision.h"


	void calcCorners(int map[], int mapX, objectsquares* player1) {

		//determines coordinates in 2D array
		
		player1->leftX = (int)((player1->xPos - (player1->xScale / 2.0))+800)/ 100;
		player1->rightX = (int)((player1->xPos + (player1->xScale / 2.0)) + 800) / 100;
		player1->topY = -((int)((player1->yPos + (player1->yScale / 2.0)) - 450) / 100);
		player1->bottomY = -((int)((player1->yPos - (player1->yScale / 2.0)) - 450) / 100);
		

		//determine data in corners, top bottom left right
		player1->TR = map[player1->topY * mapX + player1->rightX];
		player1->TL = map[player1->topY * mapX + player1->leftX];
		player1->BR = map[player1->bottomY * mapX + player1->rightX];
		player1->BL = map[player1->bottomY * mapX + player1->leftX];
	}
