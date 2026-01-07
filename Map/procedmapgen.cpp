#include "procedmapgen.h"
#include <cstdlib>
#include <ctime>

ProceduralMapGenerator::ProceduralMapGenerator(float density) : wallDensity(density) {
    std::srand(std::time(nullptr)); // Seed pentru random
}

std::vector<std::vector<char>> ProceduralMapGenerator::generate(int rows, int cols, int numStations, int numClients) {
    // 1. Inițializare hartă goală cu drumuri [cite: 14]
    std::vector<std::vector<char>> map(rows, std::vector<char>(cols, CellType::EMPTY));

    // 2. Plasare Bază (B) - Fix la (0,0) sau centru. Hai să o punem la (0,0) pentru simplitate [cite: 16]
    map[0][0] = CellType::BASE;

    // Helper lambda pentru a găsi coordonate libere
    auto getFreeCoords = [&]() -> std::pair<int, int> {
        int r, c;
        do {
            r = std::rand() % rows;
            c = std::rand() % cols;
        } while (map[r][c] != CellType::EMPTY); // Caută până găsește loc gol
        return {r, c};
    };

    // 3. Plasare Stații (S) [cite: 20]
    for (int i = 0; i < numStations; ++i) {
        auto coords = getFreeCoords();
        map[coords.first][coords.second] = CellType::STATION;
    }

    // 4. Plasare Clienți (D) [cite: 21]
    for (int i = 0; i < numClients; ++i) {
        auto coords = getFreeCoords();
        map[coords.first][coords.second] = CellType::CLIENT;
    }

    // 5. Plasare Ziduri (#) [cite: 15]
    // Iterăm prin celule libere și decidem random dacă punem zid
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (map[i][j] == CellType::EMPTY) {
                // Generăm un număr între 0 și 1
                float chance = static_cast<float>(std::rand()) / RAND_MAX;
                if (chance < wallDensity) {
                    map[i][j] = CellType::WALL;
                }
            }
        }
    }

    return map;
}