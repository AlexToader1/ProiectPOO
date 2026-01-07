#pragma once
#include <vector>

// Definim tipurile de celule aici ca să le vadă toată lumea
enum CellType {
    EMPTY = '.',
    WALL = '#',
    BASE = 'B',
    STATION = 'S',
    CLIENT = 'D'
};

// Asta e clasa abstractă (Interfața)
class IMapGenerator {
public:
    virtual ~IMapGenerator() = default;

    // Metoda pur virtuală (= 0)
    virtual std::vector<std::vector<char>> generate(int rows, int cols, int numStations, int numClients) = 0;
};