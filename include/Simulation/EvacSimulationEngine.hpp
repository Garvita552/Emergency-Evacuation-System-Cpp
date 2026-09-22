#ifndef EVAC_SIMULATION_ENGINE_HPP
#define EVAC_SIMULATION_ENGINE_HPP

#include "BuildingMap.hpp"
#include "Agent.hpp"
#include "HazardModel.hpp"
#include "SensorGrid.hpp"
#include "../Analytics/EvacMetrics.hpp"
#include "../Core/Camera3D.hpp"
#include <vector>
#include <memory>
#include <mutex>

namespace Futura {

enum class SimState {
    STOPPED,
    RUNNING,
    PAUSED,
    COMPLETED
};

class EvacSimulationEngine {
public:
    BuildingMap map;
    HazardModel hazardModel;
    SensorGrid sensorGrid;
    EvacMetrics metrics;
    Camera3D camera;

    std::vector<Agent> agents;
    SimState state;
    float simTime;
    float tickSpeedMs; // e.g. 50ms per tick
    int targetAgentCount;

    std::mutex engineMutex;

    EvacSimulationEngine();

    void initialize(int agentCount, int layoutPreset);
    void spawnAgents(int count);
    void startSim();
    void pauseSim();
    void resetSim();
    void step(float deltaTime);

    void calculatePathsForAllAgents();
    void selectOptimalExitForAgent(Agent& agent);

    int getEvacuatedCount() const;
    int getInjuredCount() const;
    int getRemainingCount() const;
};

} // namespace Futura

#endif // EVAC_SIMULATION_ENGINE_HPP
