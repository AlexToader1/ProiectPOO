#pragma once
#include "imapgen.h" // Legătura cu părintele
#include <vector>

class ProceduralMapGenerator : public IMapGenerator {
private:
    float wallDensity;
public:
    // Constructor cu valoare default
    ProceduralMapGenerator(float density = 0.2f);
    
    // Metoda de generare
    std::vector<std::vector<char>> generate(int rows, int cols, int numStations, int numClients) override;
};