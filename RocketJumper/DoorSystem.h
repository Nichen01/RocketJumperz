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
};

extern std::vector<DoorLink> doors;

DoorLink* findLinkedDoor(int id, int currentLevel);