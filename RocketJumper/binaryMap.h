/* Start Header ************************************************************************/
/*!
\file		BinaryMap.h
\author 	DigiPen, DigiPen, DigiPen
\par    	DigiPen@digipen.edu
\date   	February, 10, 2026
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
	f32 size{ 80.f };
	bool active{ true };
	bool collected = false;
};

extern std::vector<HealthPack> healthPacks;

extern f32 tileSize; // Global tile size
extern brokenDoor finalDoor;
extern Key key;
extern int keyCountLevel1; // one level can only have one key
extern int keyCountLevel2;

extern int tutDoorCount; // Tutorial Door
extern int door1Count; // Level 1 Door
extern int door2Count; // Level 2 Door
extern int door3Count; // Level 3 Door

extern int finalDoorCount; // Final Door Count

extern f32 enemy1X, enemy1Y, enemy2X, enemy2Y;

int		GetCellValue(int X, int Y);
void	SnapToCell(float *Coordinate);
int		ImportMapDataFromFile(const char *FileName);
void	FreeMapData(void);
void	PrintRetrievedInformation(void);
int		ExportMapDataToFile(const char* FileName);

#endif // BINARY_MAP_H_