#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <iomanip> 
#include <cstdlib> 
#include "Config/ConfigLoader.h"
#include "Map/map.h"
#include "Map/procedmapgen.h"
#include "Core/HiveMind.h"
#include "Agents/factory.h" 

using namespace std;

string getStatusString(AgentState state) {
    switch(state) {
        case IDLE: return "IDLE";
        case MOVING: return "MOVING";
        case CHARGING: return "CHARGING";
        case DEAD: return "DEAD";
        default: return "UNKNOWN";
    }
}

int main() {
    cout << "=== PORNIRE SIMULARE HIVEMIND (FINAL) ===" << endl;

    SimSettings cfg = ConfigLoader::load("simulation_setup.txt");
    cout << "[SETUP] Harta: " << cfg.rows << "x" << cfg.cols 
         << " | Max Ticks: " << cfg.maxTicks << endl;

    Map cityMap(cfg.rows, cfg.cols);
    ProceduralMapGenerator* pGen = new ProceduralMapGenerator(0.2f);

    cout << "[HARTA] Se genereaza si valideaza harta..." << endl;
    cityMap.loadMap(pGen, cfg.maxStations, cfg.clientsCount);
    cityMap.printMap();
    
    pair<int, int> basePos = cityMap.getBaseLocation();

    vector<Agent*> fleet;
    int idCounter = 1;

    cout << "[FLOTA] Se construiesc agentii..." << endl;
    for(int i=0; i<cfg.drones; ++i) 
        fleet.push_back(AgentFactory::createAgent("Drona", idCounter++, basePos));
    
    for(int i=0; i<cfg.robots; ++i) 
        fleet.push_back(AgentFactory::createAgent("Robot", idCounter++, basePos));
    
    for(int i=0; i<cfg.scooters; ++i) 
        fleet.push_back(AgentFactory::createAgent("Scuter", idCounter++, basePos));

    HiveMind* brain = HiveMind::getInstance();
    brain->initialize(&cityMap, fleet);
    brain->setSimulationParams(cfg.spawnFrequency, cfg.totalPackages);

    cout << "\n=== START SIMULARE (Apasa ENTER) ===\n";
    cin.get();

    for (int tick = 0; tick <= cfg.maxTicks; ++tick) {

        cout << "\n--- TICK " << tick << " / " << cfg.maxTicks << " | PROFIT: " << brain->getProfit() << " $ ---" << endl;

        brain->update();

        cout << left << setw(5) << "ID" 
             << setw(10) << "TIP" 
             << setw(10) << "POZITIE" 
             << setw(10) << "BATERIE" 
             << setw(15) << "STATUS" << endl;
        cout << "--------------------------------------------------" << endl;

        bool allDead = true;

        for(auto* a : fleet) {
            if (!a->isDead()) allDead = false;

            string pos = "(" + to_string(a->getPosition().first) + "," + to_string(a->getPosition().second) + ")";
            string bat = to_string((int)(a->getBatteryPercentage() * 100)) + "%";
            
            cout << left << setw(5) << a->getId() 
                 << setw(10) << a->getSymbol() // Sau a->getType()
                 << setw(10) << pos 
                 << setw(10) << bat 
                 << setw(15) << getStatusString(a->getState()); 
            cout << endl;
        }

        if (allDead) {
            cout << "\n[CRITIC] Toti agentii au murit! Simularea se opreste.\n";
            break;
        }

        this_thread::sleep_for(chrono::milliseconds(200));
    }

    cout << "\n=== GENERARE RAPORT ===" << endl;
    brain->generateReport();

    for(auto* a : fleet) delete a;
    delete pGen;
    
    cout << "Simulare completa. Verifica fisierul 'simulation.txt'." << endl;

    system("pause");

    return 0;
}