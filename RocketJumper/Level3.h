#pragma once
/* Start Header ************************************************************************/
/*!
\file		  Level2.h
\date         April, 01, 2026
\brief        Header file for level 3

Copyright (C) 2026 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#include "pch.h"
#include "Draw.h"
#include "Collision.h"
#include "Player.h"
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
#include "WeaponSprite.h"

void Level3_Load();

void Level3_Initialize();

void Level3_Update();

void Level3_Draw();

void Level3_Free();

void Level3_Unload();