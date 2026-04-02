/* Start Header ************************************************************************/
/*!
\file		  GameStateList.h
\date         April, 01, 2026
\brief        File that manages the different game states

Copyright (C) 2026 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#include "pch.h"
#include "MainMenu.h"
#include "GameStateList.h"
#include "Level1.h"
#include "Level2.h"
#include "Level3.h"
#include "LevelEditor.h"
#include "Tutorial.h"
#include "VictoryScreen.h"
#include "DeathScreen.h"
#include "SplashScreen.h"

#pragma once
extern bool firstTimeLevel1;
extern bool firstTimeLevel2;

typedef void(*FP)(void);

extern int current, previous, next;

extern FP fpLoad, fpInitialize, fpUpdate, fpDraw, fpFree, fpUnload;

void GSM_Initialize(int startingState);
void GSM_Update();