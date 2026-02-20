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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>




// ----------------------------------------------------------------------------
//
// externs
//
// ----------------------------------------------------------------------------
extern int BINARY_MAP_WIDTH;
extern int BINARY_MAP_HEIGHT;
extern int** MapData;
extern int** BinaryCollisionArray;


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


int		GetCellValue(int X, int Y);
int		CheckInstanceBinaryMapCollision(float PosX, float PosY, 
										float scaleX, float scaleY);
void	SnapToCell(float *Coordinate);
int		ImportMapDataFromFile(const char *FileName);
void	FreeMapData(void);
void	PrintRetrievedInformation(void);

#endif // BINARY_MAP_H_