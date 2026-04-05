/* Start Header ************************************************************************/
/*!
\file		BinaryMap.h
\author 	Chan Joraye (c.joraye)
\date   	April, 01, 2026
\brief		File contains binary map collision function declaration and various variables

Copyright (C) 2026 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#ifndef BINARY_MAP_H_
#define BINARY_MAP_H_

#include "pch.h"
#include "Draw.h"
#include "DoorSystem.h"

// ----------------------------------------------------------------------------
//
// externs
//
// ----------------------------------------------------------------------------
extern int BINARY_MAP_WIDTH;
extern int BINARY_MAP_HEIGHT;
extern int** MapData;
extern int** BinaryCollisionArray;
extern int** glassMap;

// ----------------------------------------------------------------------------
//
// different types of objects
//
// ----------------------------------------------------------------------------
enum TYPE_OBJECT
{
	TYPE_OBJECT_EMPTY,			//0
	TYPE_OBJECT_COLLISION,		//1
	TYPE_OBJECT_HERO,			//2
	TYPE_OBJECT_ENEMY1,			//3
	TYPE_OBJECT_COIN			//4
};

struct Key {
	int row;	// grid row
	int col;	// grid column
	f32 worldX;
	f32 worldY;
	f32 size{ 80.f };
	bool active{ true };
};

struct HealthPack {
	f32 worldX;
	f32 worldY;
	f32 size{ 60.f };
	bool active{ true };
	bool collected = false;
};

extern f32 tileSize; // Global tile size
extern brokenDoor finalDoor;
extern HealthPack hp;
extern Key key;
extern int keyCountLevel0;
extern int keyCountLevel1; // one level can only have one key
extern int keyCountLevel2;

extern int tutDoorCount; // Tutorial Door
extern int door1Count; // Level 1 Door
extern int door2Count; // Level 2 Door
extern int door3Count; // Level 3 Door

extern int healthCountLevel1;
extern int healthCountLevel2;
extern int healthCountLevel3;

extern int rEnemyLevel1;
extern int mEnemyLevel1;
extern int rEnemyLevel2;
extern int mEnemyLevel2;
extern int rEnemyLevel3;
extern int mEnemyLevel3;

extern int finalDoorCount; // Final Door Count

extern f32 enemy1X, enemy1Y, enemy2X, enemy2Y;

int		GetCellValue(int X, int Y);
void	SnapToCell(float *Coordinate);
int		ImportMapDataFromFile(const char *FileName);
void	FreeMapData(void);
void	PrintRetrievedInformation(void);
int		ExportMapDataToFile(const char* FileName);
void	ResetMapData();

#endif // BINARY_MAP_H_