#include "../../include/Analytics/EvacMetrics.hpp"
#include <sstream>
#include <iomanip>
#include <algorithm>

namespace Futura {

EvacMetrics::EvacMetrics() : totalEvacuationTime(0.0f), systemEfficiencyScore(100.0f), peakBottleneckCount(0) {
    reset();
}

void EvacMetrics::reset() {
    history.clear();
    totalEvacuationTime = 0.0f;
    systemEfficiencyScore = 100.0f;
    peakBottleneckCount = 0;
}

void EvacMetrics::recordEvacuation(int agentId, float time) {
    totalEvacuationTime = std::max(totalEvacuationTime, time);
}

void EvacMetrics::update(float currentTime, int evacuated, int injured, int totalAgents) {
    int remaining = totalAgents - evacuated - injured;
    float eff = (totalAgents > 0) ? (static_cast<float>(evacuated) / totalAgents) * 100.0f : 100.0f;
    
    // Deduct for injuries
    if (totalAgents > 0) {
        eff -= (static_cast<float>(injured) / totalAgents) * 50.0f;
    }
    systemEfficiencyScore = std::max(0.0f, eff);

    history.push_back({currentTime, evacuated, remaining, injured, systemEfficiencyScore});
}

std::string EvacMetrics::exportToCSV() const {
    std::stringstream ss;
    ss << "Time (s),Evacuated,Remaining,Injured,System Efficiency (%)\n";
    for (const auto& pt : history) {
        ss << std::fixed << std::setprecision(2)
           << pt.timeSeconds << ","
           << pt.evacuatedCount << ","
           << pt.remainingCount << ","
           << pt.injuredCount << ","
           << pt.systemEfficiency << "\n";
    }
    return ss.str();
}

} // namespace Futura
