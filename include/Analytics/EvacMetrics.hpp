#ifndef EVAC_METRICS_HPP
#define EVAC_METRICS_HPP

#include <vector>
#include <string>

namespace Futura {

struct TimeSeriesPoint {
    float timeSeconds;
    int evacuatedCount;
    int remainingCount;
    int injuredCount;
    float systemEfficiency;
};

class EvacMetrics {
public:
    std::vector<TimeSeriesPoint> history;
    float totalEvacuationTime;
    float systemEfficiencyScore;
    int peakBottleneckCount;

    EvacMetrics();

    void reset();
    void recordEvacuation(int agentId, float time);
    void update(float currentTime, int evacuated, int injured, int totalAgents);
    
    std::string exportToCSV() const;
};

} // namespace Futura

#endif // EVAC_METRICS_HPP
