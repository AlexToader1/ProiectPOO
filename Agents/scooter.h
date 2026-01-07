#pragma once
#include "agent.h"

class Scooter : public Agent {
    public:
        Scooter(int id, std::pair<int, int> startPos);
        ~Scooter() override = default;
        virtual bool canPassWall() const override { return false; }
};