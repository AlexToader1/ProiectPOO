#include "agent.h"
#include <iostream>

Agent::Agent(int id, std::pair<int, int> startPos, AgentStats stats)
    : id(id), position(startPos), stats(stats) {
    this->currentBattery = stats.maxBattery;
    this->state = IDLE;
}

void Agent::update() {
    if (state == DEAD) return;

    if (state == MOVING) {
        move();
        currentBattery -= stats.batteryDrain;
        if (currentBattery <= 0) {
            currentBattery = 0;
            state = DEAD;
            std::cout << "!!! Agent " << id << " a murit (Baterie 0) !!!\n";
        }
    }
    else if (state == CHARGING) {
        charge();
    }
}

void Agent::move() {
    if (currentPath.empty()) {
        state = IDLE;
        return;
    }
    for (int i = 0; i < stats.speed && !currentPath.empty(); ++i) {
        position = currentPath.front();
        currentPath.erase(currentPath.begin());
    }
    if (currentPath.empty()) state = IDLE;
}

void Agent::charge() {
    float chargeAmount = stats.maxBattery * 0.25f; // 25% pe tick
    currentBattery += chargeAmount;
    
    if (currentBattery >= stats.maxBattery) {
        currentBattery = stats.maxBattery;
        state = IDLE;
        std::cout << "Agent " << id << " incarcat 100%.\n";
    }
}

void Agent::setPath(const std::vector<std::pair<int, int>>& path) {
    currentPath = path;
    if (!currentPath.empty()) state = MOVING;
}