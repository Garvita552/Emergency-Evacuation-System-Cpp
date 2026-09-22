#ifndef HAZARD_MODEL_HPP
#define HAZARD_MODEL_HPP

#include "BuildingMap.hpp"
#include <vector>

namespace Futura {

struct HazardSource {
    int id;
    int x;
    int y;
    int floor;
    int type; // 1: Fire, 2: Smoke, 3: Gas, 4: Structural Collapse
    float intensity; // 0.0 to 1.0
    float spreadRate;
    bool active;
};

class HazardModel {
public:
    std::vector<HazardSource> activeHazards;
    float globalSpreadSpeed;

    HazardModel();

    void addHazard(int x, int y, int floor, int hazardType, float initialIntensity = 0.5f);
    void update(BuildingMap& map, float deltaTime);
    void clearAll(BuildingMap& map);
};

} // namespace Futura

#endif // HAZARD_MODEL_HPP
