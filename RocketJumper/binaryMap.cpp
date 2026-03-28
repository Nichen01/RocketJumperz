/* Start Header ************************************************************************/
/*!
\file		BinaryMap.cpp
\author 	Chan Joraye (c.joraye)
\par    	c.joraye@digipen.edu
\date   	16/02/26
\brief		

Copyright (C) 2026 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#include "BinaryMap.h"
#include <iostream>
#include <fstream>
#include <AEEngine.h>

//The number of horizontal elements
int BINARY_MAP_WIDTH;

//The number of vertical elements
int BINARY_MAP_HEIGHT;

//This will contain all the data of the map, which will be retreived from a file
//when the "ImportMapDataFromFile" function is called
int** MapData;
static int** OriginalMapData;

int** BinaryCollisionArray;
int** glassMap;

// extern key obj
Key key{};
HealthPack hp;

int keyCountLevel0 = 0;
int keyCountLevel1 = 0;
int keyCountLevel2 = 0;
int finalDoorCount = 0;

int healthCountLevel1 = 0;
int healthCountLevel2 = 0;
int healthCountLevel3 = 0;

int rEnemyLevel1;
int mEnemyLevel1;
int rEnemyLevel2;
int mEnemyLevel2;
int rEnemyLevel3;
int mEnemyLevel3;

int tutDoorCount = 0, door1Count = 0, door2Count = 0, door3Count = 0;

brokenDoor finalDoor{};

f32 enemy1X, enemy1Y, enemy2X, enemy2Y;

f32 tileSize = 80.f;

int ImportMapDataFromFile(const char* FileName)
{
	doors.clear();

	keyCountLevel1 = 0, keyCountLevel2 = 0;
	tutDoorCount = 0, door1Count = 0, door2Count = 0, door3Count = 0;

	std::ifstream ifs(FileName, std::ios::in);
	if (!ifs) return 0;

	ifs >> BINARY_MAP_WIDTH;
	ifs >> BINARY_MAP_HEIGHT;

	OriginalMapData = new int* [BINARY_MAP_HEIGHT];
	MapData = new int* [BINARY_MAP_HEIGHT];
	BinaryCollisionArray = new int* [BINARY_MAP_HEIGHT];
	glassMap = new int* [BINARY_MAP_HEIGHT];   // allocate glassMap rows

	for (int row = 0; row < BINARY_MAP_HEIGHT; row++) {
		MapData[row] = new int[BINARY_MAP_WIDTH];
		OriginalMapData[row] = new int[BINARY_MAP_WIDTH];
		BinaryCollisionArray[row] = new int[BINARY_MAP_WIDTH];
		glassMap[row] = new int[BINARY_MAP_WIDTH]; // allocate glassMap cols
	}

	for (int row = 0; row < BINARY_MAP_HEIGHT; row++) {
		for (int col = 0; col < BINARY_MAP_WIDTH; col++) {
			int value;
			ifs >> value;
			// assign random glass type if tile is "air"
			OriginalMapData[row][col] = value;
			MapData[row][col] = value;
			// FOR BINARY COLLIISION
			if (value / 10 == 1) {
				BinaryCollisionArray[row][col] = 1;
			}
			else if (value / 10 == 5) {
				BinaryCollisionArray[row][col] = 2;
			}
			else {
				BinaryCollisionArray[row][col] = 0;
			}

			// SAVING COORDINATES OF HEALTH PACK
			if (value == 60) {
				hp.worldX = ((float)col * tileSize + tileSize / 2) - 800.f;
				hp.worldY = 450.f - ((float)row * tileSize + tileSize / 2);
				hp.size = (float)tileSize;
				hp.active = true;
				hp.collected = false;
				if (currentGameLevel == 1) healthCountLevel1 = 1;
				else if (currentGameLevel == 2) healthCountLevel2 = 1;
				else healthCountLevel3 = 1;
			}

			// SAVING COORDINATES OF KEY
			else if (value == 67) {
				if ((currentGameLevel == 0 && keycardCollected0) ||
					(currentGameLevel == 1 && keycardCollected1) ||
					(currentGameLevel == 2 && keycardCollected2)) {

					MapData[row][col] = 0;
					BinaryCollisionArray[row][col] = 0;
					key.active = false;
				}
				else {
					// spawn key normally
					key.row = row;
					key.col = col;
					key.worldX = (col * key.size + key.size / 2.f) - static_cast<f32>(AEGfxGetWindowWidth() / 2);
					key.worldY = static_cast<f32>(AEGfxGetWindowHeight() / 2) - (row * key.size + key.size / 2.0f);
					key.active = true;

					if (currentGameLevel == 0) keyCountLevel0 = 1;
					else if (currentGameLevel == 1) keyCountLevel1 = 1;
					else if (currentGameLevel == 2) keyCountLevel2 = 1;
				}
			}

			// SAVING COORDINATES OF BROKEN DOOR
			if (value == 69) {
				finalDoor.worldX = (col * tileSize + tileSize / 2.f) - static_cast<f32>(AEGfxGetWindowWidth() / 2);
				finalDoor.worldY = static_cast<f32>(AEGfxGetWindowHeight() / 2) - (row * tileSize + tileSize / 2.f);
				finalDoor.state = 0;
				finalDoorCount = 1;
			}

			// check the counts of door in one level
			if (value == 21) {
				tutDoorCount = 1;
			}
			else if (value == 22) {
				door1Count = 1;
			}
			else if (value == 23) {
				door2Count = 1;
			}
			else if (value == 24) {
				door3Count = 1;
			}
			
			// SAVING THE COORDINATES OF THE ENEMIES
			else if (value == 81) {
				enemy1X = (col * tileSize + tileSize / 2.0f) - static_cast<f32>(AEGfxGetWindowWidth() / 2);
				enemy1Y = static_cast<f32>(AEGfxGetWindowHeight() / 2) - (row * tileSize + tileSize / 2.0f);
				if (currentGameLevel == 1) rEnemyLevel1 = 1;
				else if (currentGameLevel == 2) rEnemyLevel2 = 1;
				else if (currentGameLevel == 3) rEnemyLevel3 = 1;
			}
			else if (value == 82) {
				enemy2X = (col * tileSize + tileSize / 2.0f) - static_cast<f32>(AEGfxGetWindowWidth() / 2);
				enemy2Y = static_cast<f32>(AEGfxGetWindowHeight() / 2) - (row * tileSize + tileSize / 2.0f);
				if (currentGameLevel == 1) mEnemyLevel1 = 1;
				else if (currentGameLevel == 2) mEnemyLevel2 = 1;
				else if (currentGameLevel == 3) mEnemyLevel3 = 1;
			}


			// assign random glass type if tile is "air", decoration tiles, enemy, traps/items
			if (value == 0 || (value >= 70 && value <= 78) || (value >= 80 && value <= 89) || (value >= 50 && value <= 69)) {
				glassMap[row][col] = rand() % 5;
			}
			else {
				glassMap[row][col] = -1;
			}
		}
	}

	for (int row = 0; row < BINARY_MAP_HEIGHT; ++row) {
		for (int col = 0; col < BINARY_MAP_WIDTH; ++col) {
			int tile = MapData[row][col];
			if (tile >= 21 && tile <= 29) {
				DoorLink door;
				door.id = tile; // door ID
				door.row = row;
				door.col = col;
				door.worldX = (col * 80) + 40 - 800.0f;
				door.worldY = 450.0f - (row * 80 + 40);
				door.isLocked = (tile == 21) ? false : true;

				switch (tile) {
				case 21: door.entranceLevel = 0; door.exitLevel = 1; break;
				case 22: door.entranceLevel = 1; door.exitLevel = 2; break;
				case 23: door.entranceLevel = 2; door.exitLevel = 3; break;
				default: door.entranceLevel = 0; door.exitLevel = 0; break;
				}

			animSystem::init(door.anim, 7, 1, 7, 0.08f, ANIM_IDLE);

				doors.push_back(door);
			}
		}
	}
	ifs.close();
	return 1;
}

void FreeMapData(void){
	for (int i = 0; i < BINARY_MAP_HEIGHT; i++) {
		delete[] MapData[i];
		delete[] BinaryCollisionArray[i];
		delete[] glassMap[i];
		delete[] OriginalMapData[i];
	}
	delete[] MapData;
	delete[] BinaryCollisionArray;
	delete[] glassMap;
	delete[] OriginalMapData;
	OriginalMapData = nullptr;
	MapData = nullptr;
	BinaryCollisionArray = nullptr;
	glassMap = nullptr;

}

void PrintRetrievedInformation()
{
	std::cout << "Width " << BINARY_MAP_WIDTH << std::endl;
	std::cout << "Height " << BINARY_MAP_HEIGHT << std::endl;

	for (int i{}; i < BINARY_MAP_HEIGHT; i++) {
		for (int j{}; j < BINARY_MAP_WIDTH; j++) {
			std::cout << BinaryCollisionArray[i][j] << " ";
		}
		std::cout << std::endl;
	}
}

int GetCellValue(int X, int Y)
{
	if (X < 0 || X >= BINARY_MAP_WIDTH || Y < 0 || Y >= BINARY_MAP_HEIGHT) return 0;
	else return BinaryCollisionArray[Y][X];
}

void SnapToCell(float* Coordinate)
{
	int index = static_cast<int>(*Coordinate);
	*Coordinate = index + 0.5f;
}

int ExportMapDataToFile(const char* FileName)
{
	std::ofstream ofs(FileName, std::ios::out);
	if (!ofs) return 0;

	ofs << BINARY_MAP_WIDTH << std::endl;
	ofs << BINARY_MAP_HEIGHT << std::endl;

	for (s32 i{}; i < BINARY_MAP_HEIGHT; i++) {
		for (s32 j{}; j < BINARY_MAP_WIDTH; j++) {
			ofs << MapData[i][j];
			if (j < BINARY_MAP_WIDTH - 1) ofs << " ";
		}
		ofs << std::endl;
	}
	ofs.close();
	return 1;

}

void ResetMapData() {
	for (int row = 0; row < BINARY_MAP_HEIGHT; ++row) {
		for (int col = 0; col < BINARY_MAP_WIDTH; ++col) {
			MapData[row][col] = OriginalMapData[row][col];
		}
	}
}

