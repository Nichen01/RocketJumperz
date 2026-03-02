#pragma once
/* Start Header ************************************************************************/
/*!
\file		  Level1.h
\author       Ivan Chong, i.chong, 2503476
\par          i.chong@digipen.edu
\date         January, 16, 2026
\brief        Contain declarations for Variables and functions in GameStateManager.cpp

Copyright (C) 2026 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#include "pch.h"
#include "draw.h"
#include "collision.h"
#include "player.h"
#include "GameStateManager.h"
#include "projectile.h"
#include "Movement.h"
#include "render.h"
#include "enemies.h"
#include "binaryMap.h"

void Level1_Load();

void Level1_Initialize();

void Level1_Update();

void Level1_Draw();

void Level1_Free();

void Level1_Unload();