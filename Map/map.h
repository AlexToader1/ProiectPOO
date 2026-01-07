#pragma once
#include <vector>
#include <memory>
#include "imapgen.h"

class Map {
private:
    std::vector<std::vector<char>> grid;
    int rows;
    int cols;
    std::pair<int, int> baseLocation; // Coordonatele Bazei

    // Validare folosind BFS (Breadth-First Search)
    bool isValid();

public:
    Map(int r, int c);
    
    // Încarcă harta folosind o strategie specifică
    void loadMap(IMapGenerator* generator, int numStations, int numClients);
    
    // Getters
    const std::vector<std::vector<char>>& getGrid() const { return grid; }
    std::pair<int, int> getBaseLocation() const { return baseLocation; }
    
    // Metodă de afișare pentru debug
    void printMap() const;
};