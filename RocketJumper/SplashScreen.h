#pragma once
/* Start Header ************************************************************************/
/*!
\file         SplashScreen.h
\author       Nicholas Chen
\brief        DigiPen splash screen shown on game launch. Displays the DigiPen logo
              centered on screen with a fade-in/hold/fade-out animation.
              Press SPACE to skip directly to the main menu.

Copyright (C) 2026 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#include "AEEngine.h"

// Standard game state function declarations
void SplashScreen_Load();
void SplashScreen_Init();
void SplashScreen_Update();
void SplashScreen_Draw();
void SplashScreen_Free();
void SplashScreen_Unload();
