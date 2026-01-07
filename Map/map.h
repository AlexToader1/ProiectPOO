#pragma once
#include <vector>
#include <memory>
#include "imapgen.h"

class Map {
private:
    std::vector<std::vector<char>> grid;
    int rows;
    int cols;
    std::pair<int, int> baseLocation;

    bool isValid();

public:
    Map(int r, int c);
    
    void loadMap(IMapGenerator* generator, int numStations, int numClients);
    
    const std::vector<std::vector<char>>& getGrid() const { return grid; }
    std::pair<int, int> getBaseLocation() const { return baseLocation; }
    
    void printMap() const;
};