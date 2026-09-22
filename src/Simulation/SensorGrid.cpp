#include "../../include/Simulation/SensorGrid.hpp"
#include "../../include/Core/Logger.hpp"
#include <algorithm>
#include <iostream>

namespace Futura {

SensorGrid::SensorGrid() : autoSprinklersEnabled(true), dynamicReroutingEnabled(true) {}

void SensorGrid::initializeDefaultSensors(const BuildingMap& map) {
    sensors.clear();
    int sensorId = 1;

    for (int f = 0; f < map.numFloors; ++f) {
        // Space sensors every 8 units
        for (int y = 4; y < map.height; y += 8) {
            for (int x = 4; x < map.width; x += 8) {
                // Smoke Detector
                sensors.push_back({
                    sensorId++, x, y, f, SensorType::SMOKE_DETECTOR, false, true, 0.0f, "Normal"
                });
                // Automated Sprinkler
                sensors.push_back({
                    sensorId++, x + 1, y, f, SensorType::AUTOMATED_SPRINKLER, false, false, 0.0f, "Standby"
                });
            }
        }

        // Dynamic Exit Signs near exits
        for (const auto& exit : map.exits) {
            if (exit.floor == f) {
                int ex, ey, ef;
                map.worldToGrid(exit.position, ex, ey, ef);
                sensors.push_back({
                    sensorId++, ex, ey, f, SensorType::DYNAMIC_EXIT_SIGN, true, true, 100.0f, "GUIDING: ACTIVE"
                });
            }
        }
    }
}

void SensorGrid::update(BuildingMap& map, float deltaTime) {
    for (auto& s : sensors) {
        if (!s.active) continue;

        if (map.isValid(s.x, s.y, s.floor)) {
            const auto& cell = map.getCellConst(s.x, s.y, s.floor);

            if (s.type == SensorType::SMOKE_DETECTOR) {
                s.readingValue = cell.hazardLevel * 300.0f; // PPM
                if (cell.hazardLevel > 0.2f && !s.triggered) {
                    s.triggered = true;
                    s.statusMsg = "ALARM: Smoke Detected!";
                    Logger::getInstance().log(LogLevel::SENSOR, "IoT Smoke Alarm [ID " + std::to_string(s.id) + "] triggered on Floor " + std::to_string(s.floor));
                    
                    if (autoSprinklersEnabled) {
                        triggerSprinkler(s.id + 1, map); // Activate nearby sprinkler
                    }
                }
            } else if (s.type == SensorType::AUTOMATED_SPRINKLER && s.triggered) {
                // Suppress hazard in a 3x3 radius
                for (int dy = -1; dy <= 1; ++dy) {
                    for (int dx = -1; dx <= 1; ++dx) {
                        if (map.isValid(s.x + dx, s.y + dy, s.floor)) {
                            auto& targetCell = map.getCell(s.x + dx, s.y + dy, s.floor);
                            targetCell.hazardLevel = std::max(0.0f, targetCell.hazardLevel - 0.5f * deltaTime);
                        }
                    }
                }
            }
        }
    }
}

void SensorGrid::triggerSprinkler(int sensorId, BuildingMap& map) {
    for (auto& s : sensors) {
        if (s.id == sensorId && s.type == SensorType::AUTOMATED_SPRINKLER) {
            s.triggered = true;
            s.active = true;
            s.statusMsg = "SPRINKLER: ACTIVATED";
            Logger::getInstance().log(LogLevel::SENSOR, "Automated Fire Sprinkler [ID " + std::to_string(s.id) + "] deployed water suppression.");
            break;
        }
    }
}

void SensorGrid::resetSensors() {
    for (auto& s : sensors) {
        s.triggered = false;
        s.readingValue = 0.0f;
        if (s.type == SensorType::AUTOMATED_SPRINKLER) {
            s.active = false;
            s.statusMsg = "Standby";
        } else {
            s.statusMsg = "Normal";
        }
    }
}

} // namespace Futura
