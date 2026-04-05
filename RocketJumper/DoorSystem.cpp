/* Start Header ************************************************************************/
/*!
\file		  LevelEditor.cpp
\author       Chan Joraye (c.joraye)
\date         April, 01, 2026
\brief        Implementation file for the Level Editor’s door linking system.
              Defines global state variables for the current game level and player’s door entry,
              maintains a collection of DoorLink objects, and provides utility functions
              to find and manage linked doors across levels.

              Copyright (C) 2026 DigiPen Institute of Technology.
              Reproduction or disclosure of this file or its contents
              without the prior written consent of DigiPen Institute of
              Technology is prohibited.
*/
/* End Header **************************************************************************/

#include "pch.h"
#include "DoorSystem.h"

int currentGameLevel = 1;
int playerEnteredDoorId = 0;

std::vector<DoorLink> doors;

DoorLink* findLinkedDoor(int id, int currentLevel) {
    for (auto& d : doors) {
        if (d.id == id) {
            // return this door if currentLevel is one of its connected levels
            if (d.entranceLevel == currentLevel || d.exitLevel == currentLevel)
                return &d;
        }
    }
    return nullptr;
}
