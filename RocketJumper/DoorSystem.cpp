#include "pch.h"
#include "DoorSystem.h"

std::vector<DoorLink> doors;

DoorLink* findLinkedDoor(int id, int currentLevel) {
    for (auto& d : doors) {
        if (d.id == id && d.level != currentLevel) {
            return &d;
        }
    }
    return nullptr;
}
