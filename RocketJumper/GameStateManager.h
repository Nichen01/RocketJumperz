/* Start Header ************************************************************************/
/*!
\file	GameStateManager.h
\author Chan Joraye, c.joraye, 2503791
\par	c.joraye@digipen.edu
\date	18/1/26
\brief	This file declares the function in GameStateManager.c
Copyright (C) 20xx DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/\

#pragma once
#include "GameStateList.h"

typedef void(*FP)(void);

extern int current, previous, next;

extern FP fpLoad, fpInitialize, fpUpdate, fpDraw, fpFree, fpUnload;

void GSM_Initialize(int startingState);
void GSM_Update();