#pragma once
#include "agent.h"

class Robot : public Agent {
public:
    Robot(int id, std::pair<int, int> startPos);
    ~Robot() override = default;
    bool canPassWall() const override { return false; }
};