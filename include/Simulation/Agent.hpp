#ifndef AGENT_HPP
#define AGENT_HPP

#include "../Core/Vector3.hpp"
#include <vector>
#include <string>

namespace Futura {

enum class AgentType {
    NORMAL,
    STAFF,
    CHILD,
    ELDERLY,
    MOBILITY_RESTRICTED
};

enum class AgentState {
    IDLE,
    EVACUATING,
    EVACUATED,
    PANICKED,
    INJURED,
    TRAPPED
};

class Agent {
public:
    int id;
    Vector3 position;
    Vector3 velocity;
    Vector3 targetExitPos;
    int currentFloor;
    int targetExitId;

    float speed;
    float maxSpeed;
    float panicLevel;       // 0.0 to 1.0
    float health;           // 100.0 max
    float smokeInhalation;  // 0.0 to 100.0
    float radius;           // Physical radius (m)
    float awarenessLevel;   // 0.0 to 1.0 (knowledge of building/exits)

    AgentType type;
    AgentState state;
    std::vector<Vector3> currentPath;
    size_t pathIndex;

    Agent();
    Agent(int agentId, const Vector3& startPos, int startFloor, AgentType agentType = AgentType::NORMAL);
    virtual ~Agent() = default;

    virtual void update(float deltaTime, const std::vector<Agent>& allAgents, class BuildingMap& map);
    virtual Vector3 computeSocialForce(const std::vector<Agent>& allAgents) const;
    virtual void applyHazardEffects(float hazardLevel, int hazardType, float deltaTime);
    virtual std::string getTypeName() const;
    
    bool hasReachedExit(const Vector3& exitPos, float threshold = 0.8f) const;
    static AgentType getRandomAgentType();
};

class NormalPerson : public Agent {
public:
    NormalPerson(int agentId, const Vector3& startPos, int startFloor);
    std::string getTypeName() const override { return "Normal Person"; }
};

class StaffPerson : public Agent {
public:
    StaffPerson(int agentId, const Vector3& startPos, int startFloor);
    void update(float deltaTime, const std::vector<Agent>& allAgents, class BuildingMap& map) override;
    std::string getTypeName() const override { return "Staff / First Responder"; }
};

class ChildPerson : public Agent {
public:
    ChildPerson(int agentId, const Vector3& startPos, int startFloor);
    void update(float deltaTime, const std::vector<Agent>& allAgents, class BuildingMap& map) override;
    std::string getTypeName() const override { return "Child"; }
};

class ElderlyPerson : public Agent {
public:
    ElderlyPerson(int agentId, const Vector3& startPos, int startFloor);
    void applyHazardEffects(float hazardLevel, int hazardType, float deltaTime) override;
    std::string getTypeName() const override { return "Elderly Person"; }
};

class MobilityRestrictedPerson : public Agent {
public:
    MobilityRestrictedPerson(int agentId, const Vector3& startPos, int startFloor);
    std::string getTypeName() const override { return "Mobility Restricted"; }
};

} // namespace Futura

#endif // AGENT_HPP
