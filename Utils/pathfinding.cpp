#include "pathfinding.h"
#include <queue>
#include <map>
#include <algorithm>

std::vector<std::pair<int, int>> Pathfinding::getPath(
    const std::vector<std::vector<char>>& grid,
    std::pair<int, int> start,
    std::pair<int, int> end,
    bool canPassWalls) 
    {
    int rows = grid.size();
    int cols = grid[0].size();
    std::vector<std::pair<int, int>> path;

    std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));
    std::map<std::pair<int, int>, std::pair<int, int>> parentMap;
    std::queue<std::pair<int, int>> q;

    q.push(start);
    visited[start.first][start.second] = true;

    std::vector<std::pair<int, int>> directions = {
        {1, 0}, {-1, 0}, {0, 1}, {0, -1}
    };

    bool found = false;

    while (!q.empty()) {
        auto current = q.front();
        q.pop();

        if (current == end) {
            found = true;
            break;
        }

        for (const auto& dir : directions) {
            int newRow = current.first + dir.first;
            int newCol = current.second + dir.second;

            if (newRow >= 0 && newRow < rows && newCol >= 0 && newCol < cols) {
                char cellType = grid[newRow][newCol];
                bool isWall = (cellType == CellType::WALL);

                if (!visited[newRow][newCol] && (canPassWalls || !isWall)) {
                    visited[newRow][newCol] = true;
                    parentMap[{newRow, newCol}] = current;
                    q.push({newRow, newCol});
                }
            }
        }
    }

    if (found) {
        for (auto at = end; at != start; at = parentMap[at]) {
            path.push_back(at);
        }
        path.push_back(start);
        std::reverse(path.begin(), path.end());
    }

    return path;
}