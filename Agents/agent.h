#pragma once
#include <string>
#include <vector>
#include <utility>

enum State {
    IDLE,
    MOVING,
    CHARGING,
    DEAD
};

struct Stats {
    std::string type;
    char symbol;
    int speed;
    float maxBattery;
    float batteryConsumption;
    int costPerMove;
    int capacity;
};

class Agent {
    protected:
        int id;
        std::pair<int, int> position;
        float currentBattery;
        State state;

        Stats stats;
        std::vector<std::pair<int, int>> currPath;

    public:
        Agent(int id, std::pair<int, int> startPos, const Stats& stats);
        virtual ~Agent() = default;

        virtual bool canPassWall() const = 0;

        void updateState();
        void move();
        void charge();

        bool isDead() const { return state == DEAD; }
        int getId() const { return id; }
        char getSymbol() const { return stats.symbol; }
        std::pair<int, int> getPosition() const { return position; }
        std::string getType() const { return stats.type; }

        void setPath(const std::vector<std::pair<int, int>>& path);
};