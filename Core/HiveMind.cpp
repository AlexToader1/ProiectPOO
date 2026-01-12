#include "hivemind.h"
#include "../Utils/pathfinding.h"
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <fstream>
#include <climits> 

HiveMind* HiveMind::instance = nullptr;

HiveMind::HiveMind() : map(nullptr), currentTick(0) {
    profit = 0.0;
    deadAgents = 0;
    latePackages = 0;
    onTimePackages = 0;
    spawnFrequency = 10;
    totalPackagesToSpawn = 0;
}

// SINGLETON PATTERN: ne asiguram ca exista doar un singur HiveMind 
HiveMind* HiveMind::getInstance() {
    if (instance == nullptr) instance = new HiveMind();
    return instance;
}

void HiveMind::initialize(Map* m, const std::vector<Agent*>& agents) {
    this->map = m;
    this->fleet = agents;
}

void HiveMind::setSimulationParams(int freq, int maxPkgs) {
    spawnFrequency = freq;
    totalPackagesToSpawn = maxPkgs;

    profit = 0.0;
    deadAgents = 0;
    latePackages = 0;
    onTimePackages = 0;
}

int getDist(std::pair<int, int> p1, std::pair<int, int> p2) {
    return abs(p1.first - p2.first) + abs(p1.second - p2.second);
}

std::pair<int, int> getNearestStation(const std::vector<std::vector<char>>& grid, std::pair<int, int> pos) {
    std::pair<int, int> nearest = {0, 0};
    double minDist = 99999.0;

    for(int i=0; i<grid.size(); ++i) {
        for(int j=0; j<grid[0].size(); ++j) {
            if (grid[i][j] == 'B' || grid[i][j] == 'S') {
                double dist = abs(pos.first - i) + abs(pos.second - j);
                if (dist < minDist) {
                    minDist = dist;
                    nearest = {i, j};
                }
            }
        }
    }
    return nearest;
}

void HiveMind::update() {
    currentTick++;

    if (totalPackagesToSpawn > 0 && currentTick % spawnFrequency == 0) {
        spawnPackage();
        totalPackagesToSpawn--;
    }

    assignTasks();

    for (auto* agent : fleet) {
        if (agent->isDead()) continue;
        bool wasAlive = !agent->isDead();
        
        agent->update();
        
        profit -= agent->getCostPerTick();

        if (wasAlive && agent->isDead()) {
            deadAgents++;
            profit -= 500;
            std::cout << "[DECES] -500 CR (Agent " << agent->getId() << " a murit)\n";
        }
    }

    for (auto& pkg : activePackages) {
        if (pkg.assigned && !pkg.delivered) {
            
            bool packageJustDelivered = false;

            for(auto* agent : fleet) {
                if(agent->getPosition() == pkg.destination && agent->isAvailable()) {
                    packageJustDelivered = true;
                    break; 
                }
            }

            if (packageJustDelivered) {
                pkg.delivered = true;
                profit += pkg.reward;
                
                int arrivalTime = currentTick;
                if (arrivalTime <= pkg.creationTick + pkg.deadlineTicks) {
                    onTimePackages++;
                    std::cout << "[LIVRARE] Pachet " << pkg.id << " OK (+" << pkg.reward << ")\n";
                } else {
                    latePackages++;
                    profit -= 50; 
                    std::cout << "[LIVRARE] Pachet " << pkg.id << " TARZIU (-50)\n";
                }
            }
        }
    }
}

void HiveMind::spawnPackage() {
    auto grid = map->getGrid();
    std::vector<std::pair<int, int>> clients;
    
    for(int i=0; i<grid.size(); ++i)
        for(int j=0; j<grid[0].size(); ++j)
            if(grid[i][j] == 'D') clients.push_back({i, j});

    if (clients.empty()) return;

    int idx = rand() % clients.size();
    Package pkg;
    pkg.id = currentTick;
    pkg.destination = clients[idx];
    pkg.reward = 200 + (rand() % 601);
    pkg.creationTick = currentTick;
    pkg.deadlineTicks = 30 + (rand() % 31);
    
    activePackages.push_back(pkg);
    std::cout << "[NEW] Pachet nou pt (" << pkg.destination.first << "," << pkg.destination.second << ")\n";
}

void HiveMind::assignTasks() {
    auto grid = map->getGrid();
    //BATERIE
    for (auto* agent : fleet) {
        if (agent->isDead()) continue;

        if (agent->isAvailable()) {
            char cell = grid[agent->getPosition().first][agent->getPosition().second];
            if (cell != 'B' && cell != 'S') {
                auto station = getNearestStation(grid, agent->getPosition());
                bool canFly = (agent->getType() == "Drona");
                auto path = Pathfinding::getPath(grid, agent->getPosition(), station, canFly);

                 if (!path.empty() && path[0] == agent->getPosition()) {
                    path.erase(path.begin());
                }

                if (!path.empty()) {
                    agent->setPath(path);
                }
            } else {
                 if (agent->getBatteryPercentage() < 0.99f) 
                    agent->startCharging();
            }
        }

        if (agent->getBatteryPercentage() < 0.3f) {
             if (agent->isAvailable()) { 
                 char cell = grid[agent->getPosition().first][agent->getPosition().second];
                 if (cell == 'B' || cell == 'S') {
                     agent->startCharging();
                 }
             }
             continue; 
        }
    }
     //ALOCARE PACHETE AICI
    for (auto& pkg : activePackages) {
        if (pkg.assigned || pkg.delivered) continue;

        Agent* bestAgent = nullptr;
        double bestScore = -99999.0;
        
        std::pair<int, int> baseLoc = map->getBaseLocation();

        int ticksLeft = (pkg.creationTick + pkg.deadlineTicks) - currentTick;
        int distStraight = getDist(baseLoc, pkg.destination);
 
        bool robotMightBeLate = (distStraight * 1.2f > ticksLeft);

        bool useDrone = robotMightBeLate;

        for (auto* agent : fleet) {
            bool isDrone = (agent->getType() == "Drona");

            if (useDrone) {
                if (!isDrone && agent->getType() != "Scuter") continue;
            } else {
                if (isDrone) continue;
            }

            if (!agent->isAvailable() || agent->isDead() || agent->getBatteryPercentage() < 0.3f) continue;

            int distToPickup = getDist(agent->getPosition(), baseLoc);
            int distToDeliver = getDist(baseLoc, pkg.destination);

            auto safeStation = getNearestStation(grid, pkg.destination);
            int distSafety = getDist(pkg.destination, safeStation);

            int totalDist = distToPickup + distToDeliver + distSafety; 

            if (isDrone && (totalDist * 3.0 / agent->getSpeed()) > 30) {
                 if ((float)totalDist / 3.0f > 10.0f) continue;
            }

            int speed = agent->getSpeed(); 
            if (speed == 0) speed = 1; 

            float ticksNeeded = (float)totalDist / speed;
            float batteryNeeded = ticksNeeded * agent->getBatteryDrain();
            float costOfOperation = ticksNeeded * agent->getCostPerTick();

            if (costOfOperation > pkg.reward + 50) { 
                continue;
            }

            if (agent->getBatteryPercentage() * agent->getMaxBattery() < batteryNeeded * 1.1f) {
                 continue; 
            }

            double score = 100.0;
            score -= (distToPickup + distToDeliver);

            if (score > bestScore) {
                bestScore = score;
                bestAgent = agent;
            }
        }

        if (bestAgent != nullptr) {
            bool canFly = (bestAgent->getType() == "Drona");

            std::vector<std::pair<int, int>> fullPath;

            if (bestAgent->getPosition() != baseLoc) {
                auto pathToBase = Pathfinding::getPath(grid, bestAgent->getPosition(), baseLoc, canFly);

                if (!pathToBase.empty()) {
                     if (pathToBase[0] == bestAgent->getPosition()) pathToBase.erase(pathToBase.begin());
                     fullPath.insert(fullPath.end(), pathToBase.begin(), pathToBase.end());
                }
            }

            auto pathToClient = Pathfinding::getPath(grid, baseLoc, pkg.destination, canFly);
            if (!pathToClient.empty()) {

                 if (!pathToClient.empty() && fullPath.empty() && pathToClient[0] == bestAgent->getPosition()) {
                     pathToClient.erase(pathToClient.begin());
                 }
                 fullPath.insert(fullPath.end(), pathToClient.begin(), pathToClient.end());
            }
            
            if (!fullPath.empty()) {
                bestAgent->setPath(fullPath);
                pkg.assigned = true;
                std::cout << "[DISPATCH] Pachet " << pkg.id << " -> " << bestAgent->getType() << " " << bestAgent->getId() << "\n";
            }
        }
    }
}

void HiveMind::generateReport() {
    int undeliveredCount = 0;
    for(const auto& pkg : activePackages) {
        if(!pkg.delivered) {
            undeliveredCount++;
            profit -= 200; 
        }
    }

    std::ofstream fout("simulation.txt");
    fout << "=== RAPORT SIMULARE HIVEMIND ===\n";
    fout << "Pachete Livrate la Timp: " << onTimePackages << "\n";
    fout << "Pachete Intarziate: " << latePackages << "\n";
    fout << "Pachete Nelivrate: " << undeliveredCount << "\n";
    fout << "Agenti Pierduti: " << deadAgents << "\n";
    fout << "-----------------------------\n";
    fout << "PROFIT FINAL: " << profit << " credite\n";
    fout << "=============================\n";
    fout.close();
    std::cout << "[INFO] Raport generat in simulation.txt\n";
}