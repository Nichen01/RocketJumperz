#pragma once
#include <vector>
#include "Animation.h"

struct DoorLink {
    int id;
    int firstLevel;
    int secondLevel;
    int row, col;
    float worldX, worldY;
    bool isOpen = false;
    SpriteAnimation anim;
};

extern std::vector<DoorLink> doors;

DoorLink* findLinkedDoor(int id, int currentLevel);