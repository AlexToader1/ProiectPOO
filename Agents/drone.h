#pragma once
#include "agent.h"

class Drone : public Agent {
    public:
        Drone(int id, std::pair<int, int> startPos);
        ~Drone() override = default;
        virtual bool canPassWall() const override { return true; }
};