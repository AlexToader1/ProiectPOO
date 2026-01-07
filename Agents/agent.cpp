#include "agent.h"
#include <iostream>

Agent::Agent(int id, std::pair<int, int> startPos, const Stats& stats)
    : id(id), position(startPos), currentBattery(stats.maxBattery), state(IDLE), stats(stats) {}

void Agent::updateState() {
    if (state == DEAD) return;

    switch (state)
    {
    case MOVING:
        move();
        break;
    case CHARGING:
        charge();
        break;
    case IDLE:
        break;
    default:
        break;
    }

    if (currentBattery <= 0) {
        state = DEAD;
        std::cout << "Agent " << stats.type << " has died due to battery depletion.\n";
    }
}

void Agent::move() {
    if (currPath.empty()) {
        state = IDLE;
        return;
    }

    for(int step = 0; step < stats.speed && !currPath.empty(); ++step) {
        position = currPath.front();
        currPath.erase(currPath.begin());
    }

    currentBattery -= stats.batteryConsumption;
}

void Agent::charge() {
    currentBattery += stats.capacity;
    if (currentBattery >= stats.maxBattery) {
        currentBattery = stats.maxBattery;
        state = IDLE;
    }
}

void Agent::setPath(const std::vector<std::pair<int, int>>& path) {
    currPath = path;
    if (!currPath.empty()) {
        state = MOVING;
    }
}

