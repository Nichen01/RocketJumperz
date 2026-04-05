/* Start Header ************************************************************************/
/*!
\file		  Tutorial.h
\author		  Nicholas Chen (c.chen), Joraye Chan (c.joraye)	
\brief        Header file for tutorial

Copyright (C) 2026 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#pragma once

#include "Load.h"
#include "Draw.h"
#include "Collision.h"
#include "Player.h"
#include "GameStateManager.h"
#include "Projectile.h"
#include "Movement.h"
#include "Enemies.h"

extern bool playerEnteredDoor0;

void Tutorial_Load();

void Tutorial_Initialize();

void Tutorial_Update();

void Tutorial_Draw();

void Tutorial_Free();

void Tutorial_Unload();
