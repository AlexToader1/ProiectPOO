#pragma once
#include "imapgen.h"
#include <string>
#include <vector>

class FileMapLoader : public IMapGenerator {
private:
    std::string filename;
public:
    FileMapLoader(const std::string& file);
    
    std::vector<std::vector<char>> generate(int rows, int cols, int numStations, int numClients) override;
};