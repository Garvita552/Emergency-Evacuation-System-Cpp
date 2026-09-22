#include "../include/Simulation/EvacSimulationEngine.hpp"
#include "../include/Core/Logger.hpp"
#include "../include/Analytics/ScenarioBenchmark.hpp"
#include "../include/Analytics/RiskAnalyzer.hpp"
#include <iostream>
#include <iomanip>
#include <thread>
#include <chrono>

using namespace Futura;

int main(int argc, char* argv[]) {
    std::cout << "======================================================================\n";
    std::cout << "    EMERGENCY EVACUATION SIMULATOR — NEXT-GEN 3D PLATFORM (C++ Core)\n";
    std::cout << "    Real-World Multi-Floor Evacuation & Decision-Support System\n";
    std::cout << "======================================================================\n\n";

    EvacSimulationEngine engine;

    std::cout << "[SYSTEM] Initializing 3D Multi-Floor Facility with Polymorphic Demographics...\n";
    engine.initialize(300, 1);

    std::cout << "[SYSTEM] Spawning Dynamic Emergency Hazards (Fire, Smoke, Structural Collapse)...\n";
    engine.hazardModel.addHazard(10, 10, 0, 1, 0.6f); // Fire
    engine.hazardModel.addHazard(15, 10, 1, 2, 0.4f); // Smoke

    std::cout << "[SYSTEM] Running Real-Time 3D Simulation Loop...\n";
    engine.startSim();

    int stepCount = 0;
    while (engine.state == SimState::RUNNING && stepCount < 200) {
        engine.step(0.1f);
        stepCount++;

        if (stepCount % 25 == 0) {
            std::cout << "[TELEMETRY @ " << std::fixed << std::setprecision(1) << engine.simTime << "s] "
                      << "Evacuated: " << engine.getEvacuatedCount() << " | "
                      << "Trapped: " << engine.getInjuredCount() << " | "
                      << "Remaining: " << engine.getRemainingCount() << " | "
                      << "Efficiency: " << engine.metrics.systemEfficiencyScore << "%\n";
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(15));
    }

    std::cout << "\n======================================================================\n";
    std::cout << "    AUTOMATED FACILITY RISK ANALYSIS & RECOMMENDATIONS REPORT\n";
    std::cout << "======================================================================\n";
    auto recs = RiskAnalyzer::analyzeFacility(engine.map, engine.agents, engine.simTime);
    for (const auto& rec : recs) {
        std::string tag = (rec.severity == SeverityLevel::CRITICAL) ? "[CRITICAL RISK]" : "[WARNING]";
        std::cout << tag << " Location: " << rec.location << "\n";
        std::cout << "   Problem: " << rec.riskDescription << "\n";
        std::cout << "   Action:  " << rec.recommendationText << "\n\n";
    }

    std::cout << "======================================================================\n";
    std::cout << "    WHAT-IF SCENARIO BENCHMARKING MATRIX\n";
    std::cout << "======================================================================\n";
    auto benchmarkResults = ScenarioBenchmark::runAllScenarios();
    std::cout << std::left << std::setw(32) << "Scenario"
              << std::setw(12) << "Evac Time"
              << std::setw(12) << "Evacuated"
              << std::setw(12) << "Trapped"
              << std::setw(14) << "Efficiency"
              << "Primary Bottleneck\n";
    std::cout << "----------------------------------------------------------------------------------------\n";
    for (const auto& res : benchmarkResults) {
        std::cout << std::left << std::setw(32) << res.scenarioName.substr(0, 30)
                  << std::setw(12) << (std::to_string((int)res.totalEvacTime) + "s")
                  << std::setw(12) << res.evacuatedCount
                  << std::setw(12) << res.injuredCount
                  << std::setw(14) << (std::to_string((int)res.systemEfficiency) + "%")
                  << res.primaryBottleneck << "\n";
    }

    std::cout << "\n[COMPLETED] Next-Generation C++ Core Engine execution finished cleanly.\n";
    return 0;
}
