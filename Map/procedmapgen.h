#pragma once
#include "imapgen.h"
#include <vector>

class ProceduralMapGenerator : public IMapGenerator {
private:
    float wallDensity;
public:
    ProceduralMapGenerator(float density = 0.2f);
    
    std::vector<std::vector<char>> generate(int rows, int cols, int numStations, int numClients) override;
};