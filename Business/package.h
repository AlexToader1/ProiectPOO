#pragma once
#include <utility>

struct Package {
    int id;
    std::pair<int, int> destination; 
    int reward;                    
    int creationTick;
    int deadlineTicks;
    
    bool assigned = false;        
    bool delivered = false;        
};