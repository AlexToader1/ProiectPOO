#pragma once
#include <vector>

enum CellType {
    EMPTY = '.',
    WALL = '#',
    BASE = 'B',
    STATION = 'S',
    CLIENT = 'D'
};

class IMapGenerator {
public:
    virtual ~IMapGenerator() = default;

    virtual std::vector<std::vector<char>> generate(int rows, int cols, int numStations, int numClients) = 0;
};