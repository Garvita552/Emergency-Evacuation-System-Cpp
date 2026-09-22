#include "../../include/Simulation/Agent.hpp"
#include "../../include/Simulation/BuildingMap.hpp"
#include <algorithm>
#include <cmath>
#include <random>

namespace Futura {

Agent::Agent()
    : id(0), position(0, 0, 0), velocity(0, 0, 0), targetExitPos(0, 0, 0),
      currentFloor(0), targetExitId(-1), speed(1.4f), maxSpeed(2.2f),
      panicLevel(0.0f), health(100.0f), smokeInhalation(0.0f), radius(0.25f),
      awarenessLevel(0.7f), type(AgentType::NORMAL), state(AgentState::IDLE), pathIndex(0) {}

Agent::Agent(int agentId, const Vector3& startPos, int startFloor, AgentType agentType)
    : id(agentId), position(startPos), velocity(0, 0, 0), targetExitPos(0, 0, 0),
      currentFloor(startFloor), targetExitId(-1), speed(1.4f), maxSpeed(2.2f),
      panicLevel(0.0f), health(100.0f), smokeInhalation(0.0f), radius(0.25f),
      awarenessLevel(0.7f), type(agentType), state(AgentState::IDLE), pathIndex(0) {}

AgentType Agent::getRandomAgentType() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::discrete_distribution<> dist({70, 15, 8, 5, 2}); // 70% Normal, 15% Staff, 8% Child, 5% Elderly, 2% Mobility
    int val = dist(gen);
    return static_cast<AgentType>(val);
}

std::string Agent::getTypeName() const {
    return "Generic Agent";
}

Vector3 Agent::computeSocialForce(const std::vector<Agent>& allAgents) const {
    Vector3 repulsionForce(0, 0, 0);
    float neighborRadius = 1.5f;

    for (const auto& other : allAgents) {
        if (other.id == id || other.state == AgentState::EVACUATED || other.currentFloor != currentFloor) {
            continue;
        }

        float dist = position.distanceTo(other.position);
        if (dist > 0.001f && dist < neighborRadius) {
            Vector3 diff = (position - other.position).normalized();
            float forceMagnitude = std::exp((radius * 2.0f - dist) / 0.08f);
            repulsionForce += diff * forceMagnitude;
        }
    }
    return repulsionForce;
}

void Agent::update(float deltaTime, const std::vector<Agent>& allAgents, BuildingMap& map) {
    if (state == AgentState::EVACUATED || state == AgentState::INJURED) return;
    if (currentPath.empty() || pathIndex >= currentPath.size()) return;

    Vector3 targetWaypoint = currentPath[pathIndex];
    Vector3 desiredDir = (targetWaypoint - position).normalized();

    float effectiveSpeed = speed * (health / 100.0f) * (1.0f - smokeInhalation / 200.0f);
    if (panicLevel > 0.5f) effectiveSpeed *= 1.15f;
    effectiveSpeed = std::min(effectiveSpeed, maxSpeed);

    Vector3 socialForce = computeSocialForce(allAgents);
    Vector3 netDir = (desiredDir * 2.0f + socialForce * 0.5f).normalized();

    velocity = netDir * effectiveSpeed;
    position += velocity * deltaTime;

    if (position.distanceTo(targetWaypoint) < 0.6f) {
        pathIndex++;
    }

    int gridX, gridY, gridFloor;
    map.worldToGrid(position, gridX, gridY, gridFloor);
    currentFloor = gridFloor;

    if (map.isValid(gridX, gridY, gridFloor)) {
        const auto& cell = map.getCellConst(gridX, gridY, gridFloor);
        if (cell.hazardLevel > 0.05f) {
            applyHazardEffects(cell.hazardLevel, cell.hazardType, deltaTime);
        }
    }
}

void Agent::applyHazardEffects(float hazardLevel, int hazardType, float deltaTime) {
    switch (hazardType) {
        case 1: // Fire
            health -= hazardLevel * 45.0f * deltaTime;
            panicLevel = std::min(1.0f, panicLevel + 0.3f * deltaTime);
            break;
        case 2: // Smoke
            smokeInhalation += hazardLevel * 25.0f * deltaTime;
            panicLevel = std::min(1.0f, panicLevel + 0.15f * deltaTime);
            break;
        case 3: // Toxic Gas
            health -= hazardLevel * 30.0f * deltaTime;
            smokeInhalation += hazardLevel * 35.0f * deltaTime;
            panicLevel = std::min(1.0f, panicLevel + 0.4f * deltaTime);
            break;
        default:
            break;
    }

    if (health <= 0.0f) {
        health = 0.0f;
        state = AgentState::INJURED;
    }
}

bool Agent::hasReachedExit(const Vector3& exitPos, float threshold) const {
    return position.distanceTo(exitPos) <= threshold;
}

/* Derived Agent Implementations */
NormalPerson::NormalPerson(int agentId, const Vector3& startPos, int startFloor)
    : Agent(agentId, startPos, startFloor, AgentType::NORMAL) {
    speed = 1.4f;
    maxSpeed = 2.2f;
    awarenessLevel = 0.75f;
}

StaffPerson::StaffPerson(int agentId, const Vector3& startPos, int startFloor)
    : Agent(agentId, startPos, startFloor, AgentType::STAFF) {
    speed = 1.8f;
    maxSpeed = 2.6f;
    awarenessLevel = 1.0f; // Complete exit awareness
}

void StaffPerson::update(float deltaTime, const std::vector<Agent>& allAgents, BuildingMap& map) {
    Agent::update(deltaTime, allAgents, map);
}

ChildPerson::ChildPerson(int agentId, const Vector3& startPos, int startFloor)
    : Agent(agentId, startPos, startFloor, AgentType::CHILD) {
    speed = 1.0f;
    maxSpeed = 1.6f;
    awarenessLevel = 0.4f;
}

void ChildPerson::update(float deltaTime, const std::vector<Agent>& allAgents, BuildingMap& map) {
    // Follow nearest adult/staff if panic is high
    if (panicLevel > 0.4f) {
        speed = 1.2f;
    }
    Agent::update(deltaTime, allAgents, map);
}

ElderlyPerson::ElderlyPerson(int agentId, const Vector3& startPos, int startFloor)
    : Agent(agentId, startPos, startFloor, AgentType::ELDERLY) {
    speed = 0.8f;
    maxSpeed = 1.3f;
    awarenessLevel = 0.8f;
}

void ElderlyPerson::applyHazardEffects(float hazardLevel, int hazardType, float deltaTime) {
    // Elderly people are twice as sensitive to smoke & toxic gas
    float multiplier = (hazardType == 2 || hazardType == 3) ? 2.0f : 1.0f;
    Agent::applyHazardEffects(hazardLevel * multiplier, hazardType, deltaTime);
}

MobilityRestrictedPerson::MobilityRestrictedPerson(int agentId, const Vector3& startPos, int startFloor)
    : Agent(agentId, startPos, startFloor, AgentType::MOBILITY_RESTRICTED) {
    speed = 0.5f;
    maxSpeed = 0.9f;
    awarenessLevel = 0.6f;
}

} // namespace Futura
