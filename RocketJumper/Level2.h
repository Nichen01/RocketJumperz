#pragma once
/* Start Header ************************************************************************/
/*!
\file		  Level2.h
\date         April, 01, 2026
\brief        Header file for level 2

Copyright (C) 2026 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#include "pch.h"
#include "Animation.h"
#include "AssetManager.h"
#include "BinaryMap.h"
#include "Collision.h"
#include "Draw.h"
#include "Enemies.h"
#include "GameStateManager.h"
#include "GameStateList.h"
#include "Load.h"
#include "Movement.h"
#include "Player.h"
#include "Projectile.h"
#include "drops.h"

extern int screenWidth; extern int screenLength;
extern bool playerEnteredDoor2;

void Level2_Load();

void Level2_Initialize();

void Level2_Update();

void Level2_Draw();

void Level2_Free();

void Level2_Unload();
