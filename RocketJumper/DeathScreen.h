/* Start Header ************************************************************************/
/*!
\file		  DeathScreen.h
\author 	  Ivan Chong (i.chong)
\date         April, 04, 2026
\brief        Header file for DeathScreen.cpp

Copyright (C) 2026 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#pragma once
#include "AEEngine.h"
#include "MainMenu.h"

void DeathScreen_Load();
void DeathScreen_Init();
void DeathScreen_Update();
void DeathScreen_Draw();
void DeathScreen_Free();
void DeathScreen_Unload();

void DDrawBackground();