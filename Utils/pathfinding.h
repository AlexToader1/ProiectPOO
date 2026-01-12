#pragma once
#include <vector>
#include <utility>
#include "../Map/map.h"

class Pathfinding {
    public:
        static std::vector<std::pair<int, int>> getPath(
            const std::vector<std::vector<char>>& grid,
            std::pair<int, int> start,
            std::pair<int, int> end,
            bool canPassWalls
        );
};