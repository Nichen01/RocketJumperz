#pragma once
#include <vector>
#include "Animation.h"

struct DoorLink {
    int id;
    int level;
    int row, col;
    float worldX, worldY;
    bool isOpen = false;
    SpriteAnimation anim;  // each door has its own animation
};

extern std::vector<DoorLink> doors;

DoorLink* findLinkedDoor(int id, int currentLevel);