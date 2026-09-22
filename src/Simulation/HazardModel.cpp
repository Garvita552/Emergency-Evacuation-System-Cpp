#include "../../include/Simulation/HazardModel.hpp"
#include "../../include/Core/Logger.hpp"
#include <algorithm>
#include <cmath>

namespace Futura {

HazardModel::HazardModel() : globalSpreadSpeed(0.15f) {}

void HazardModel::addHazard(int x, int y, int floor, int hazardType, float initialIntensity) {
    if (hazardType <= 0) return;
    int newId = static_cast<int>(activeHazards.size()) + 1;
    activeHazards.push_back({newId, x, y, floor, hazardType, initialIntensity, globalSpreadSpeed, true});

    std::string typeName = "Hazard";
    if (hazardType == 1) typeName = "Fire Ignition";
    else if (hazardType == 2) typeName = "Smoke Outbreak";
    else if (hazardType == 3) typeName = "Toxic Gas Leak";
    else if (hazardType == 4) typeName = "Structural Collapse";

    Logger::getInstance().log(LogLevel::CRITICAL, typeName + " detected on Floor " + std::to_string(floor) + " at (" + std::to_string(x) + ", " + std::to_string(y) + ")");
}

void HazardModel::update(BuildingMap& map, float deltaTime) {
    if (activeHazards.empty()) return;

    const int dx[4] = {-1, 1, 0, 0};
    const int dy[4] = {0, 0, -1, 1};

    for (auto& haz : activeHazards) {
        if (!haz.active) continue;

        // Increase intensity at core
        haz.intensity = std::min(1.0f, haz.intensity + haz.spreadRate * deltaTime);

        // Update grid cell hazard level
        if (map.isValid(haz.x, haz.y, haz.floor)) {
            auto& cell = map.getCell(haz.x, haz.y, haz.floor);
            cell.hazardLevel = haz.intensity;
            cell.hazardType = haz.type;
        }

        // Cellular spread to neighbor cells
        for (int i = 0; i < 4; ++i) {
            int nx = haz.x + dx[i];
            int ny = haz.y + dy[i];

            if (map.isValid(nx, ny, haz.floor) && map.getCellConst(nx, ny, haz.floor).type != CellType::WALL) {
                auto& nCell = map.getCell(nx, ny, haz.floor);
                if (nCell.hazardType == 0 || nCell.hazardType == haz.type) {
                    nCell.hazardType = haz.type;
                    nCell.hazardLevel = std::min(1.0f, nCell.hazardLevel + haz.spreadRate * 0.4f * deltaTime);
                }
            }
        }

        // Smoke rises vertically to floor above!
        if (haz.type == 2 && haz.floor + 1 < map.numFloors) {
            auto& upperCell = map.getCell(haz.x, haz.y, haz.floor + 1);
            if (upperCell.hazardType == 0) {
                upperCell.hazardType = 2; // Smoke
                upperCell.hazardLevel = std::min(1.0f, upperCell.hazardLevel + haz.spreadRate * 0.2f * deltaTime);
            }
        }
    }
}

void HazardModel::clearAll(BuildingMap& map) {
    activeHazards.clear();
    for (int f = 0; f < map.numFloors; ++f) {
        for (int y = 0; y < map.height; ++y) {
            for (int x = 0; x < map.width; ++x) {
                auto& cell = map.getCell(x, y, f);
                cell.hazardLevel = 0.0f;
                cell.hazardType = 0;
            }
        }
    }
    Logger::getInstance().log(LogLevel::INFO, "All active hazards cleared.");
}

} // namespace Futura
