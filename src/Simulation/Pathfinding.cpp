#include "../../include/Simulation/Pathfinding.hpp"
#include <cmath>
#include <algorithm>
#include <tuple>

namespace Futura {

float Pathfinding::heuristic(int x1, int y1, int f1, int x2, int y2, int f2) {
    float dx = static_cast<float>(x1 - x2);
    float dy = static_cast<float>(y1 - y2);
    float df = static_cast<float>(f1 - f2) * 4.0f; // Height weight per floor
    return std::sqrt(dx * dx + dy * dy + df * df);
}

struct NodeKey {
    int x, y, floor;
    bool operator==(const NodeKey& other) const {
        return x == other.x && y == other.y && floor == other.floor;
    }
};

struct NodeKeyHash {
    std::size_t operator()(const NodeKey& k) const {
        return (std::hash<int>()(k.x) ^ (std::hash<int>()(k.y) << 1)) ^ (std::hash<int>()(k.floor) << 2);
    }
};

std::vector<Vector3> Pathfinding::find3DPath(const BuildingMap& map, const Vector3& start, const Vector3& target) {
    int startX, startY, startFloor;
    int targetX, targetY, targetFloor;

    map.worldToGrid(start, startX, startY, startFloor);
    map.worldToGrid(target, targetX, targetY, targetFloor);

    if (!map.isValid(startX, startY, startFloor) || !map.isValid(targetX, targetY, targetFloor)) {
        return {target};
    }

    std::priority_queue<PathNode, std::vector<PathNode>, std::greater<PathNode>> openSet;
    std::unordered_map<NodeKey, float, NodeKeyHash> gCosts;
    std::unordered_map<NodeKey, NodeKey, NodeKeyHash> cameFrom;

    NodeKey startKey{startX, startY, startFloor};
    NodeKey targetKey{targetX, targetY, targetFloor};

    openSet.push({startX, startY, startFloor, 0.0f, heuristic(startX, startY, startFloor, targetX, targetY, targetFloor)});
    gCosts[startKey] = 0.0f;

    // 8-directional neighbors on same floor + stairwells
    const int dx[8] = {-1, 1, 0, 0, -1, -1, 1, 1};
    const int dy[8] = {0, 0, -1, 1, -1, 1, -1, 1};

    while (!openSet.empty()) {
        PathNode current = openSet.top();
        openSet.pop();

        NodeKey currKey{current.x, current.y, current.floor};

        if (currKey == targetKey) {
            std::vector<Vector3> path;
            NodeKey step = targetKey;
            while (!(step == startKey)) {
                path.push_back(map.gridToWorld(step.x, step.y, step.floor));
                step = cameFrom[step];
            }
            path.push_back(start);
            std::reverse(path.begin(), path.end());
            return path;
        }

        if (current.gCost > gCosts[currKey]) continue;

        // Same floor neighbors
        for (int i = 0; i < 8; ++i) {
            int nx = current.x + dx[i];
            int ny = current.y + dy[i];
            int nf = current.floor;

            if (map.isWalkable(nx, ny, nf)) {
                const auto& cell = map.getCellConst(nx, ny, nf);
                float stepCost = (dx[i] != 0 && dy[i] != 0) ? 1.414f : 1.0f;
                // Add hazard penalty cost to avoid fire/smoke
                stepCost += cell.hazardLevel * 15.0f;

                float tentativeG = current.gCost + stepCost;
                NodeKey neighborKey{nx, ny, nf};

                if (gCosts.find(neighborKey) == gCosts.end() || tentativeG < gCosts[neighborKey]) {
                    gCosts[neighborKey] = tentativeG;
                    cameFrom[neighborKey] = currKey;
                    float h = heuristic(nx, ny, nf, targetX, targetY, targetFloor);
                    openSet.push({nx, ny, nf, tentativeG, h});
                }
            }
        }

        // Stairwell floor transitions
        const auto& cell = map.getCellConst(current.x, current.y, current.floor);
        if (cell.type == CellType::STAIRS_UP || cell.type == CellType::STAIRS_DOWN) {
            int targetF = (cell.type == CellType::STAIRS_UP) ? current.floor + 1 : current.floor - 1;
            if (targetF >= 0 && targetF < map.numFloors) {
                NodeKey neighborKey{current.x, current.y, targetF};
                float stairCost = 3.5f; // Stair climbing cost
                float tentativeG = current.gCost + stairCost;

                if (gCosts.find(neighborKey) == gCosts.end() || tentativeG < gCosts[neighborKey]) {
                    gCosts[neighborKey] = tentativeG;
                    cameFrom[neighborKey] = currKey;
                    float h = heuristic(current.x, current.y, targetF, targetX, targetY, targetFloor);
                    openSet.push({current.x, current.y, targetF, tentativeG, h});
                }
            }
        }
    }

    // Direct path fallback if pathfinding gets blocked
    return {target};
}

} // namespace Futura
