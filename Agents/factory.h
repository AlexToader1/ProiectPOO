#pragma once
#include "agent.h"
#include "drone.h"
#include "robot.h"
#include "scooter.h"
#include <string>
#include <stdexcept>

// FACTORY PATTERN: clasa care se ocupa exclusiv cu crearea obiectelor (agentilor)
// Ascunde logica de instantiere 
class AgentFactory {
public:

    static Agent* createAgent(const std::string& type, int id, std::pair<int, int> startPos) {
        if (type == "Drona" || type == "DRONES") {
            return new Drone(id, startPos);
        } 
        else if (type == "Robot" || type == "ROBOTS") {
            return new Robot(id, startPos);
        } 
        else if (type == "Scuter" || type == "SCOOTERS") {
            return new Scooter(id, startPos);
        } 
        else {
            return nullptr; 
        }
    }
};