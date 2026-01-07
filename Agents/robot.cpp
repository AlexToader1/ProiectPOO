#include "robot.h"

Robot::Robot(int id, std::pair<int, int> startPos)
    : Agent(id, startPos, {"Robot", 'R', 1, 300.0f, 2.0f, 1, 4}) {}    