#include "pch.h"
#include "DoorSystem.h"

int currentGameLevel = 0;
int playerEnteredDoorId = 0;

std::vector<DoorLink> doors;

DoorLink* findLinkedDoor(int id, int currentLevel) {
    for (auto& d : doors) {
        if (d.id == id) {
            // return this door if currentLevel is one of its connected levels
            if (d.firstLevel == currentLevel || d.secondLevel == currentLevel)
                return &d;
        }
    }
    return nullptr;
}
