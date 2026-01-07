#include "drone.h"

Drone::Drone(int id, std::pair<int, int> startPos) 
    : Agent(id, startPos, {"Drone", '^', 3, 100.0f, 10.0f, 15, 1}) {}
