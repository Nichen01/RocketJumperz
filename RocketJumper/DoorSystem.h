/* Start Header ************************************************************************/
/*!
\file		DoorSystem.h
\author 	Chan Joraye (c.joraye)
\date   	April, 01, 2026
\brief		Header file for the doors in the game & the transition between doors

Copyright (C) 2026 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/

#pragma once
#include <vector>
#include "Animation.h"

extern int currentGameLevel;
extern int playerEnteredDoorId;

struct DoorLink {
    int id;
    int entranceLevel;
    int exitLevel;
    int row, col;
    float worldX, worldY;
    bool isOpen = false;
    bool isLocked;
    SpriteAnimation anim;
    float sparkTimer = 0.0f;  // Counts up to 4 seconds between spark bursts
};

struct brokenDoor {
    f32 worldX;
    f32 worldY;
    int state;
};

extern std::vector<DoorLink> doors;

DoorLink* findLinkedDoor(int id, int currentLevel);