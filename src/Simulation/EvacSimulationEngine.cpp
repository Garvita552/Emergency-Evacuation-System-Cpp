#include "../../include/Simulation/EvacSimulationEngine.hpp"
#include "../../include/Simulation/Pathfinding.hpp"
#include "../../include/Core/Logger.hpp"
#include <random>
#include <iostream>

namespace Futura {

EvacSimulationEngine::EvacSimulationEngine()
    : state(SimState::STOPPED), simTime(0.0f), tickSpeedMs(50.0f), targetAgentCount(300) {
    initialize(300, 1);
}

void EvacSimulationEngine::initialize(int agentCount, int layoutPreset) {
    std::lock_guard<std::mutex> lock(engineMutex);
    targetAgentCount = agentCount;

    if (layoutPreset == 1) {
        map.loadSchoolPreset();
    } else if (layoutPreset == 2) {
        map.loadHospitalPreset();
    } else {
        map.loadHighRisePreset();
    }

    sensorGrid.initializeDefaultSensors(map);
    hazardModel.clearAll(map);
    spawnAgents(targetAgentCount);
    metrics.reset();
    simTime = 0.0f;
    state = SimState::STOPPED;

    Logger::getInstance().log(LogLevel::INFO, "Simulation engine initialized with " + std::to_string(agents.size()) + " agents across " + std::to_string(map.numFloors) + " floors.");
}

void EvacSimulationEngine::spawnAgents(int count) {
    agents.clear();
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> floorDist(0, map.numFloors - 1);
    std::uniform_int_distribution<int> xDist(2, map.width - 3);
    std::uniform_int_distribution<int> yDist(2, map.height - 3);

    for (int i = 0; i < count; ++i) {
        int f = floorDist(gen);
        int x = xDist(gen);
        int y = yDist(gen);

        int attempts = 0;
        while (!map.isWalkable(x, y, f) && attempts < 50) {
            x = xDist(gen);
            y = yDist(gen);
            attempts++;
        }

        Vector3 startPos = map.gridToWorld(x, y, f);
        Agent agent(i + 1, startPos, f);
        selectOptimalExitForAgent(agent);
        agents.push_back(agent);
    }
}

void EvacSimulationEngine::selectOptimalExitForAgent(Agent& agent) {
    if (map.exits.empty()) return;

    float minDistance = 1e9f;
    Vector3 bestExitPos = map.exits[0].position;
    int bestExitId = map.exits[0].id;

    for (const auto& exit : map.exits) {
        if (!exit.isActive) continue;
        float dist = agent.position.distanceTo(exit.position);
        if (exit.floor != agent.currentFloor) {
            dist += 15.0f * std::abs(exit.floor - agent.currentFloor);
        }

        if (dist < minDistance) {
            minDistance = dist;
            bestExitPos = exit.position;
            bestExitId = exit.id;
        }
    }

    agent.targetExitPos = bestExitPos;
    agent.targetExitId = bestExitId;
    agent.currentPath = Pathfinding::find3DPath(map, agent.position, bestExitPos);
    agent.pathIndex = 0;
    agent.state = AgentState::EVACUATING;
}

void EvacSimulationEngine::calculatePathsForAllAgents() {
    for (auto& agent : agents) {
        if (agent.state == AgentState::EVACUATING) {
            selectOptimalExitForAgent(agent);
        }
    }
}

void EvacSimulationEngine::startSim() {
    if (state == SimState::STOPPED || state == SimState::PAUSED) {
        state = SimState::RUNNING;
        calculatePathsForAllAgents();
        Logger::getInstance().log(LogLevel::INFO, "Evacuation simulation STARTED.");
    }
}

void EvacSimulationEngine::pauseSim() {
    if (state == SimState::RUNNING) {
        state = SimState::PAUSED;
        Logger::getInstance().log(LogLevel::INFO, "Evacuation simulation PAUSED.");
    }
}

void EvacSimulationEngine::resetSim() {
    initialize(targetAgentCount, 1);
}

void EvacSimulationEngine::step(float deltaTime) {
    if (state != SimState::RUNNING) return;

    std::lock_guard<std::mutex> lock(engineMutex);
    simTime += deltaTime;

    // 1. Update Hazards
    hazardModel.update(map, deltaTime);

    // 2. Update Sensor Grid
    sensorGrid.update(map, deltaTime);

    // 3. Update Agents
    int activeEvacuating = 0;
    int trappedOrInjured = 0;

    for (auto& agent : agents) {
        if (agent.state == AgentState::EVACUATING) {
            agent.update(deltaTime, agents, map);

            if (agent.state == AgentState::INJURED) {
                trappedOrInjured++;
            } else if (agent.hasReachedExit(agent.targetExitPos)) {
                agent.state = AgentState::EVACUATED;
                metrics.recordEvacuation(agent.id, simTime);
                Logger::getInstance().log(LogLevel::INFO, "Agent #" + std::to_string(agent.id) + " successfully EVACUATED!");
            } else {
                activeEvacuating++;
            }
        } else if (agent.state == AgentState::INJURED) {
            trappedOrInjured++;
        }
    }

    // 4. Update Metrics
    metrics.update(simTime, getEvacuatedCount(), getInjuredCount(), static_cast<int>(agents.size()));

    // 5. Intelligent Termination:
    // If all remaining active agents are either evacuated, injured, or blocked from further progress, finish simulation!
    if (activeEvacuating == 0 || (getEvacuatedCount() + getInjuredCount() >= agents.size())) {
        state = SimState::COMPLETED;
        Logger::getInstance().log(LogLevel::INFO, "EVACUATION FINISHED! Maximum evacuable spawns reached (" + std::to_string(getEvacuatedCount()) + "/" + std::to_string(agents.size()) + ") in " + std::to_string(simTime) + "s.");
    }
}

int EvacSimulationEngine::getEvacuatedCount() const {
    int count = 0;
    for (const auto& a : agents) if (a.state == AgentState::EVACUATED) count++;
    return count;
}

int EvacSimulationEngine::getInjuredCount() const {
    int count = 0;
    for (const auto& a : agents) if (a.state == AgentState::INJURED) count++;
    return count;
}

int EvacSimulationEngine::getRemainingCount() const {
    return static_cast<int>(agents.size()) - getEvacuatedCount() - getInjuredCount();
}

} // namespace Futura
