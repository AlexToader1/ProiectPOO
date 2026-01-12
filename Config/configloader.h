#pragma once
#include <string>
#include <map>

struct SimSettings {
    int rows, cols;
    int maxTicks;
    int maxStations;
    int clientsCount;
    int drones, robots, scooters;
    int totalPackages;
    int spawnFrequency;
};

class ConfigLoader {
public:
    static SimSettings load(const std::string& filename);
};