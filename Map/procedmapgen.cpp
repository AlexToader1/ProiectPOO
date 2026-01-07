#include "procedmapgen.h"
#include <cstdlib>
#include <ctime>

ProceduralMapGenerator::ProceduralMapGenerator(float density) : wallDensity(density) {
    std::srand(std::time(nullptr)); 
}

std::vector<std::vector<char>> ProceduralMapGenerator::generate(int rows, int cols, int numStations, int numClients) {
    std::vector<std::vector<char>> map(rows, std::vector<char>(cols, CellType::EMPTY));

    map[0][0] = CellType::BASE;

    auto getFreeCoords = [&]() -> std::pair<int, int> {
        int r, c;
        do {
            r = std::rand() % rows;
            c = std::rand() % cols;
        } while (map[r][c] != CellType::EMPTY); 
        return {r, c};
    };

    for (int i = 0; i < numStations; ++i) {
        auto coords = getFreeCoords();
        map[coords.first][coords.second] = CellType::STATION;
    }

    for (int i = 0; i < numClients; ++i) {
        auto coords = getFreeCoords();
        map[coords.first][coords.second] = CellType::CLIENT;
    }

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (map[i][j] == CellType::EMPTY) {
                float chance = static_cast<float>(std::rand()) / RAND_MAX;
                if (chance < wallDensity) {
                    map[i][j] = CellType::WALL;
                }
            }
        }
    }

    return map;
}