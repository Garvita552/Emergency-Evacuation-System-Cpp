#ifndef SENSOR_GRID_HPP
#define SENSOR_GRID_HPP

#include "BuildingMap.hpp"
#include <vector>
#include <string>

namespace Futura {

enum class SensorType {
    SMOKE_DETECTOR,
    HEAT_SENSOR,
    AUTOMATED_SPRINKLER,
    DYNAMIC_EXIT_SIGN
};

struct IoTSensor {
    int id;
    int x;
    int y;
    int floor;
    SensorType type;
    bool triggered;
    bool active;
    float readingValue; // e.g. PPM or Temperature in Celsius
    std::string statusMsg;
};

class SensorGrid {
public:
    std::vector<IoTSensor> sensors;
    bool autoSprinklersEnabled;
    bool dynamicReroutingEnabled;

    SensorGrid();

    void initializeDefaultSensors(const BuildingMap& map);
    void update(BuildingMap& map, float deltaTime);
    void triggerSprinkler(int sensorId, BuildingMap& map);
    void resetSensors();
};

} // namespace Futura

#endif // SENSOR_GRID_HPP
