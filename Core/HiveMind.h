#pragma once
#include <vector>
#include <fstream>
#include "../Map/map.h"
#include "../Agents/agent.h"
#include "../Business/Package.h"

class HiveMind {
private:
    static HiveMind* instance;
    
    Map* map = nullptr;
    std::vector<Agent*> fleet;
    std::vector<Package> activePackages;
    
    int currentTick = 0;

    double profit = 0.0;
    int deadAgents = 0;
    int latePackages = 0;
    int onTimePackages = 0;
    
    int spawnFrequency = 10;
    int totalPackagesToSpawn = 0;

    HiveMind(); 

public:
    static HiveMind* getInstance();
    
    void initialize(Map* m, const std::vector<Agent*>& agents);
    void setSimulationParams(int freq, int maxPkgs);
    
    void update(); 
    void spawnPackage();
    void assignTasks(); 
    
    void generateReport();
    double getProfit() const { return profit; }
};