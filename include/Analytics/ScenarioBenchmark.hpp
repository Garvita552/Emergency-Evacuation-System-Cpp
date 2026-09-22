#ifndef SCENARIO_BENCHMARK_HPP
#define SCENARIO_BENCHMARK_HPP

#include "../Simulation/EvacSimulationEngine.hpp"
#include <vector>
#include <string>

namespace Futura {

struct ScenarioResult {
    std::string scenarioName;
    std::string description;
    float totalEvacTime;
    int totalAgents;
    int evacuatedCount;
    int injuredCount;
    float systemEfficiency;
    std::string primaryBottleneck;
};

class ScenarioBenchmark {
public:
    static std::vector<ScenarioResult> runAllScenarios();
    static ScenarioResult runScenario(int scenarioId, const std::string& name, const std::string& desc);
};

} // namespace Futura

#endif // SCENARIO_BENCHMARK_HPP
