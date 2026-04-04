/* Start Header ************************************************************************/
/*!
\file		  Collision.h
\date         April, 04, 2026
\brief        Header file for Collision.cpp

Copyright (C) 2026 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#pragma once
#include "AEEngine.h"
#include "MainMenu.h"
#include "load.h"

void Confirmation_Init(MenuButton& yesButton, MenuButton& noButton);
void Confirmation_Update(MenuButton& yesButton, MenuButton& noButton, s8& leave);
void Confirmation_Draw(s8 font, MenuButton& yesButton, MenuButton& noButton);


