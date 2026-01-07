#include <iostream>
#include <vector>
#include "map.h"
#include "procedmapgen.h"
#include "Agents/drone.h"
#include "Agents/robot.h"
#include "Agents/scooter.h"

using namespace std;

int main() {
    cout << "=== Testare Flota Agenti ===" << endl;

    Map cityMap(10, 10);
    ProceduralMapGenerator* pGen = new ProceduralMapGenerator(0.1f);
    cityMap.loadMap(pGen, 2, 2);

    pair<int, int> start = cityMap.getBaseLocation();

    vector<Agent*> fleet;
    
    fleet.push_back(new Drone(1, start));
    fleet.push_back(new Robot(2, start));
    fleet.push_back(new Scooter(3, start));

    for(auto* agent : fleet) {
        cout << "\n[Agent " << agent->getId() << "]: " << agent->getType() << endl;
        cout << "   - Simbol: " << agent->getSymbol() << endl;
        cout << "   - Pozitie: (" << agent->getPosition().first << ", " 
             << agent->getPosition().second << ")" << endl;
        cout << "   - Zboara peste ziduri? " << (agent->canPassWall() ? "DA" : "NU") << endl;
    }

    for(auto* agent : fleet) delete agent;
    delete pGen;

    cout << "\n=== Test Finalizat ===" << endl;
    char temp; cin >> temp;
    return 0;
}