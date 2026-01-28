/* Start Header ************************************************************************/
/*!
\file		  Input.cpp
\author       Ivan Chong, i.chong, 2503476
\par          i.chong@digipen.edu
\date         January, 16, 2026
\brief        Contain functions to take in player Input

Copyright (C) 2026 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#include "Input.h"
#include "AEEngine.h"
#include "GameStateList.h"
#include "GameStateManager.h"
#include "collision.h"

f32 speed = 10;

void Input_Handle() {
	if (AEInputCheckTriggered(AEVK_ESCAPE))
		next = GS_QUIT;

	if (AEInputCheckCurr(AEVK_D)) {
		objectinfo[player].xPos += speed;
	}

	if (AEInputCheckCurr(AEVK_A)) {
		objectinfo[player].xPos -= speed;
	}

	if (AEInputCheckCurr(AEVK_W)) {
		objectinfo[player].yPos += speed;
	}

	if (AEInputCheckCurr(AEVK_S)) {
		objectinfo[player].yPos -= speed;
	}
}
