#pragma once
#include "imapgen.h" // Legătura cu părintele
#include <string>
#include <vector>

class FileMapLoader : public IMapGenerator {
private:
    std::string filename;
public:
    // Constructorul
    FileMapLoader(const std::string& file);
    
    // Metoda de generare (override)
    std::vector<std::vector<char>> generate(int rows, int cols, int numStations, int numClients) override;
};