#include "ConfigLoader.h"
#include <fstream>
#include <iostream>
#include <sstream>

SimSettings ConfigLoader::load(const std::string& filename) {
    SimSettings settings;
    std::ifstream fin(filename);
    std::string line, key;
    
    if (!fin.is_open()) {
        std::cerr << "CRITIC: Nu s-a gasit " << filename << "! Se folosesc valori default.\n";
        return {15, 20, 100, 2, 5, 1, 1, 1, 10, 10}; 
    }

    while (std::getline(fin, line)) {
        if (line.empty() || line[0] == '/') continue;
        std::stringstream ss(line);
        ss >> key;

        if (key == "MAP_SIZE:") ss >> settings.rows >> settings.cols;
        else if (key == "MAX_TICKS:") ss >> settings.maxTicks;
        else if (key == "MAX_STATIONS:") ss >> settings.maxStations;
        else if (key == "CLIENTS_COUNT:") ss >> settings.clientsCount;
        else if (key == "DRONES:") ss >> settings.drones;
        else if (key == "ROBOTS:") ss >> settings.robots;
        else if (key == "SCOOTERS:") ss >> settings.scooters;
        else if (key == "TOTAL_PACKAGES:") ss >> settings.totalPackages;
        else if (key == "SPAWN_FREQUENCY:") ss >> settings.spawnFrequency;
    }
    return settings;
}