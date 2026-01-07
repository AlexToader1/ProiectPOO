#include "filemaploader.h"
#include <fstream>
#include <iostream>

FileMapLoader::FileMapLoader(const std::string& file) : filename(file) {}

std::vector<std::vector<char>> FileMapLoader::generate(int rows, int cols, int numStations, int numClients) {
    std::ifstream fin(filename);
    std::vector<std::vector<char>> map;
    
    if (!fin.is_open()) {
        std::cerr << "Eroare: Nu s-a putut deschide fisierul hartii: " << filename << std::endl;
        return map; 
    }

    std::string line;
    while (std::getline(fin, line)) {
        std::vector<char> row;
        for (char c : line) {
            row.push_back(c);
        }
        if (!row.empty()) map.push_back(row);
    }
    
    return map;
}