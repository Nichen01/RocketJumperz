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