#pragma once
/* Start Header ************************************************************************/
/*!
\file		  GameStateManager.h
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

typedef void(*FP)(void);

extern int current, previous, next;

extern FP fpLoad, fpInitialize, fpUpdate, fpDraw, fpFree, fpUnload;

void GSM_Initialize(int startingState);
void GSM_Update();