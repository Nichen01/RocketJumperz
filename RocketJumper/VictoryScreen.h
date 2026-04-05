/* Start Header ************************************************************************/
/*!
\file		  VictoryScreen.h
\date         April, 04, 2026
\brief        Header file for VictoryScreen.cpp

Copyright (C) 2026 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#pragma once
#include "AEEngine.h"
#include "MainMenu.h"

void VictoryScreen_Load();
void VictoryScreen_Init();
void VictoryScreen_Update();
void VictoryScreen_Draw();
void VictoryScreen_Free();
void VictoryScreen_Unload();

void VDrawBackground();