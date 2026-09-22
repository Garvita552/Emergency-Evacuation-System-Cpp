#include "../../include/Analytics/HeatmapGenerator.hpp"
#include <algorithm>

namespace Futura {

std::vector<std::vector<HeatmapCell>> HeatmapGenerator::generateFloorHeatmap(
    const BuildingMap& map,
    const std::vector<Agent>& agents,
    int floor
) {
    std::vector<std::vector<HeatmapCell>> heatmap(map.height, std::vector<HeatmapCell>(map.width, {0.0f, 0.0f}));

    // Accumulate agent positions
    for (const auto& agent : agents) {
        if (agent.currentFloor == floor && agent.state == AgentState::EVACUATING) {
            int gx, gy, gf;
            map.worldToGrid(agent.position, gx, gy, gf);
            if (map.isValid(gx, gy, floor)) {
                heatmap[gy][gx].density += 0.25f; // Each agent adds density
            }
        }
    }

    // Normalize density and compute bottleneck risk
    for (int y = 0; y < map.height; ++y) {
        for (int x = 0; x < map.width; ++x) {
            float d = std::min(1.0f, heatmap[y][x].density);
            float hazardLevel = map.getCellConst(x, y, floor).hazardLevel;
            
            heatmap[y][x].density = d;
            // Bottleneck risk combines high crowd density with hazard proximity
            heatmap[y][x].riskFactor = std::min(1.0f, d * 0.7f + hazardLevel * 0.5f);
        }
    }

    return heatmap;
}

} // namespace Futura
