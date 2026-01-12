#pragma once
#include <utility>
#include <string>
#include <vector>

enum AgentState { IDLE, MOVING, CHARGING, DEAD };

struct AgentStats {
    std::string type;
    char symbol;
    int speed;
    float maxBattery;
    float batteryDrain;
    int costPerTick;
    int capacity;
};

class Agent {
protected:
    int id;
    std::pair<int, int> position;
    float currentBattery;
    AgentState state;
    AgentStats stats;
    std::vector<std::pair<int, int>> currentPath;

public:
    Agent(int id, std::pair<int, int> startPos, AgentStats stats);
    virtual ~Agent() = default;
    int getCostPerTick() const { return stats.costPerTick; }
    
    // POLIMORFISM: metoda virtuala pura. Agent este clasa abstracta (interfata).
    // Fiecare clasa copil (Drona, Robot) trebuie sa implementeze propria logica.
    virtual bool canPassWall() const = 0;
    
    int getSpeed() const { return stats.speed; }

    void update(); 
    void move();
    void charge();

    float getBatteryPercentage() const { return currentBattery / stats.maxBattery; }
    float getBatteryDrain() const { return stats.batteryDrain; }
    float getMaxBattery() const { return stats.maxBattery; }
    void startCharging() { state = CHARGING; }
    bool isAvailable() const { return state == IDLE; }
    bool isDead() const { return state == DEAD; }
    int getId() const { return id; }
    char getSymbol() const { return stats.symbol; }
    std::pair<int, int> getPosition() const { return position; }
    std::string getType() const { return stats.type; }
    AgentState getState() const { return state; }
    
    void setPath(const std::vector<std::pair<int, int>>& path);
};