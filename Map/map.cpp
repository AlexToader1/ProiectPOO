#include "map.h"
#include <queue>
#include <iostream>

Map::Map(int r, int c) : rows(r), cols(c) {
}

// STRATEGY PATTERN: Primim un "generator" abstract si il folosim fara sa stim
// daca e procedural sau din fisier. Comportamentul se schimba la runtime.
void Map::loadMap(IMapGenerator* generator, int numStations, int numClients) {
    bool valid = false;
    int attempts = 0;

    while (!valid && attempts < 100) {
        grid = generator->generate(rows, cols, numStations, numClients);
        
        if (!grid.empty()) {
            rows = grid.size();
            cols = grid[0].size();
        }

        bool baseFound = false;
        for(int i = 0; i < rows; ++i) {
            for(int j = 0; j < cols; ++j) {
                if(grid[i][j] == CellType::BASE) {
                    baseLocation = {i, j};
                    baseFound = true;
                    break;
                }
            }
            if(baseFound) break;
        }

        if (!baseFound) {
            std::cerr << "[Eroare] Harta generata nu are Baza (B)!\n";
            attempts++;
            continue;
        }

        valid = isValid();
        
        if (!valid) {
            attempts++;
        }
    }
    
    if (!valid) {
        std::cerr << "CRITIC: Nu s-a putut genera o harta valida dupa 100 incercari.\n";
    } else {
        std::cout << "[Succes] Harta generata si validata corect!\n";
    }
}

bool Map::isValid() {
    int totalTargets = 0;
    for(const auto& row : grid) {
        for(char cell : row) {
            if(cell == CellType::CLIENT || cell == CellType::STATION) {
                totalTargets++;
            }
        }
    }

    // BFS: Algoritm de parcurgere. 
    // Pornim de la baza si "inundam" harta ca sa vedem daca putem ajunge la
    // toti clientii si toate statiile. Daca raman tinte neatinse, harta e invalida.
    std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));
    std::queue<std::pair<int, int>> q;
    
    q.push(baseLocation);
    visited[baseLocation.first][baseLocation.second] = true;
    
    int foundTargets = 0;
    
    int dx[] = {-1, 1, 0, 0};
    int dy[] = {0, 0, -1, 1};

    while(!q.empty()) {
        std::pair<int, int> current = q.front();
        q.pop();
        
        int x = current.first;
        int y = current.second;

        for(int i = 0; i < 4; ++i) {
            int nx = x + dx[i];
            int ny = y + dy[i];

            if(nx >= 0 && nx < rows && ny >= 0 && ny < cols) {
                if(!visited[nx][ny] && grid[nx][ny] != CellType::WALL) {
                    visited[nx][ny] = true;
                    q.push({nx, ny});
                    
                    char cell = grid[nx][ny];
                    if(cell == CellType::CLIENT || cell == CellType::STATION) {
                        foundTargets++;
                    }
                }
            }
        }
    }

    return foundTargets == totalTargets;
}

void Map::printMap() const {
    std::cout << "\n--- Harta Simulare ---\n";
    for (const auto& row : grid) {
        for (char cell : row) {
            std::cout << cell << " ";
        }
        std::cout << "\n";
    }
    std::cout << "----------------------\n";
}