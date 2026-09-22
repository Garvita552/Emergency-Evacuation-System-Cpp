#ifndef HEATMAP_GENERATOR_HPP
#define HEATMAP_GENERATOR_HPP

#include "../Simulation/BuildingMap.hpp"
#include "../Simulation/Agent.hpp"
#include <vector>

namespace Futura {

struct HeatmapCell {
    float density;    // Crowd congestion 0.0 to 1.0
    float riskFactor; // Combined hazard + congestion bottleneck score
};

class HeatmapGenerator {
public:
    static std::vector<std::vector<HeatmapCell>> generateFloorHeatmap(
        const BuildingMap& map,
        const std::vector<Agent>& agents,
        int floor
    );
};

} // namespace Futura

#endif // HEATMAP_GENERATOR_HPP
