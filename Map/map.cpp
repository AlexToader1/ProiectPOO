#include "map.h"
#include <queue>
#include <iostream>

// Constructorul: Doar inițializează dimensiunile
Map::Map(int r, int c) : rows(r), cols(c) {
}

// Funcția care încarcă și validează harta
void Map::loadMap(IMapGenerator* generator, int numStations, int numClients) {
    bool valid = false;
    int attempts = 0;

    // Repetăm generarea până primim o hartă validă sau atingem limita de încercări
    while (!valid && attempts < 100) {
        // 1. Generăm harta folosind strategia aleasă (Procedural sau File)
        grid = generator->generate(rows, cols, numStations, numClients);
        
        // Actualizăm dimensiunile reale (în caz că strategia le-a schimbat, ex: FileLoader)
        if (!grid.empty()) {
            rows = grid.size();
            cols = grid[0].size();
        }

        // 2. Căutăm coordonatele Bazei [B] pentru a porni BFS-ul de acolo
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

        // 3. Validăm harta folosind Flood Fill / BFS
        valid = isValid();
        
        if (!valid) {
            // Dacă e generare procedurală, mai încercăm o dată
            // std::cout << "Harta invalida (zone inaccesibile). Se regenereaza... (Incercarea " << attempts + 1 << ")\n";
            attempts++;
        }
    }
    
    if (!valid) {
        std::cerr << "CRITIC: Nu s-a putut genera o harta valida dupa 100 incercari.\n";
    } else {
        std::cout << "[Succes] Harta generata si validata corect!\n";
    }
}

// Algoritmul BFS (Breadth-First Search) pentru a verifica accesibilitatea
bool Map::isValid() {
    // 1. Numărăm câte ținte (Clienți + Stații) există pe hartă
    int totalTargets = 0;
    for(const auto& row : grid) {
        for(char cell : row) {
            if(cell == CellType::CLIENT || cell == CellType::STATION) {
                totalTargets++;
            }
        }
    }

    // 2. Pregătim BFS
    std::vector<std::vector<bool>> visited(rows, std::vector<bool>(cols, false));
    std::queue<std::pair<int, int>> q;
    
    // Pornim de la Bază
    q.push(baseLocation);
    visited[baseLocation.first][baseLocation.second] = true;
    
    int foundTargets = 0;
    
    // Vectori de direcție: Sus, Jos, Stânga, Dreapta
    int dx[] = {-1, 1, 0, 0};
    int dy[] = {0, 0, -1, 1};

    while(!q.empty()) {
        std::pair<int, int> current = q.front();
        q.pop();
        
        int x = current.first;
        int y = current.second;

        // Verificăm vecinii
        for(int i = 0; i < 4; ++i) {
            int nx = x + dx[i];
            int ny = y + dy[i];

            // Verificăm dacă suntem în harta
            if(nx >= 0 && nx < rows && ny >= 0 && ny < cols) {
                // Verificăm dacă nu e zid și nu e vizitat
                if(!visited[nx][ny] && grid[nx][ny] != CellType::WALL) {
                    visited[nx][ny] = true;
                    q.push({nx, ny});
                    
                    // Dacă am găsit o țintă, o numărăm
                    char cell = grid[nx][ny];
                    if(cell == CellType::CLIENT || cell == CellType::STATION) {
                        foundTargets++;
                    }
                }
            }
        }
    }

    // Harta e validă doar dacă am putut ajunge la TOATE țintele
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