#include "scooter.h"

Scooter::Scooter(int id, std::pair<int, int> startPos) 
    : Agent(id, startPos, {"Scooter", 'S', 2, 200.0f, 5.0f, 4, 2}) {}