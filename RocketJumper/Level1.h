#pragma once
/* Start Header ************************************************************************/
/*!
\file		  Level1.h
\date         April, 01, 2026
\brief        Header file for level 1

Copyright (C) 2026 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#include "pch.h"
#include "Draw.h"
#include "Collision.h"
#include "GameStateManager.h"
#include "Projectile.h"
#include "Movement.h"
#include "Load.h"
#include "Enemies.h"
#include "BinaryMap.h"
#include "GameStateList.h"
#include "Animation.h"
#include "Sound.h"
#include "AssetManager.h"
#include "Drops.h"
#include "Traps.h"
#include "WeaponSprite.h"

extern bool playerEnteredDoor1;

void Level1_Load();

void Level1_Initialize();

void Level1_Update();

void Level1_Draw();

void Level1_Free();

void Level1_Unload();