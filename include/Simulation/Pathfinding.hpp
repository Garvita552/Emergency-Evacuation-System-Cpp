#ifndef PATHFINDING_HPP
#define PATHFINDING_HPP

#include "../Core/Vector3.hpp"
#include "BuildingMap.hpp"
#include <vector>
#include <queue>
#include <unordered_map>

namespace Futura {

struct PathNode {
    int x, y, floor;
    float gCost;
    float hCost;
    float fCost() const { return gCost + hCost; }

    bool operator>(const PathNode& other) const {
        return fCost() > other.fCost();
    }
};

class Pathfinding {
public:
    static std::vector<Vector3> find3DPath(const BuildingMap& map, const Vector3& start, const Vector3& target);
    static float heuristic(int x1, int y1, int f1, int x2, int y2, int f2);
};

} // namespace Futura

#endif // PATHFINDING_HPP
