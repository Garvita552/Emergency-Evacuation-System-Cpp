#include "../../include/Simulation/BuildingMap.hpp"
#include <algorithm>
#include <cmath>

namespace Futura {

BuildingMap::BuildingMap() : width(32), height(24), numFloors(3), floorHeight(4.0f) {
    initializeGrid();
}

BuildingMap::BuildingMap(int w, int h, int floors)
    : width(w), height(h), numFloors(floors), floorHeight(4.0f) {
    initializeGrid();
}

void BuildingMap::initializeGrid() {
    grid.resize(numFloors);
    for (int f = 0; f < numFloors; ++f) {
        grid[f].resize(height);
        for (int y = 0; y < height; ++y) {
            grid[f][y].resize(width);
            for (int x = 0; x < width; ++x) {
                grid[f][y][x] = MapCell(x, y, f, CellType::EMPTY);
                if (x == 0 || x == width - 1 || y == 0 || y == height - 1) {
                    grid[f][y][x].type = CellType::WALL;
                }
            }
        }
    }
}

bool BuildingMap::isValid(int x, int y, int floor) const {
    return floor >= 0 && floor < numFloors && y >= 0 && y < height && x >= 0 && x < width;
}

bool BuildingMap::isWalkable(int x, int y, int floor) const {
    if (!isValid(x, y, floor)) return false;
    const auto& cell = grid[floor][y][x];
    if (cell.type == CellType::WALL || cell.type == CellType::DOOR_CLOSED) return false;
    if (cell.hazardType == 4) return false;
    return true;
}

MapCell& BuildingMap::getCell(int x, int y, int floor) {
    return grid[floor][y][x];
}

const MapCell& BuildingMap::getCellConst(int x, int y, int floor) const {
    return grid[floor][y][x];
}

void BuildingMap::setCellType(int x, int y, int floor, CellType type) {
    if (isValid(x, y, floor)) grid[floor][y][x].type = type;
}

void BuildingMap::addExit(int x, int y, int floor, const std::string& label, float widthMeters, int capacity) {
    if (isValid(x, y, floor)) {
        grid[floor][y][x].type = CellType::EXIT;
        int exitId = static_cast<int>(exits.size()) + 1;
        Vector3 worldPos = gridToWorld(x, y, floor);
        exits.push_back({exitId, worldPos, floor, label, true, capacity, widthMeters});
    }
}

void BuildingMap::toggleExitState(int exitId, bool isBlocked) {
    for (auto& exit : exits) {
        if (exit.id == exitId) {
            exit.isActive = !isBlocked;
            int ex, ey, ef;
            worldToGrid(exit.position, ex, ey, ef);
            if (isValid(ex, ey, ef)) {
                grid[ef][ey][ex].type = isBlocked ? CellType::WALL : CellType::EXIT;
            }
            break;
        }
    }
}

void BuildingMap::removeExit(int exitId) {
    exits.erase(std::remove_if(exits.begin(), exits.end(), [exitId](const ExitLocation& e) {
        return e.id == exitId;
    }), exits.end());
}

void BuildingMap::addStairwell(int x1, int y1, int floor1, int x2, int y2, int floor2) {
    if (isValid(x1, y1, floor1) && isValid(x2, y2, floor2)) {
        grid[floor1][y1][x1].type = CellType::STAIRS_UP;
        grid[floor2][y2][x2].type = CellType::STAIRS_DOWN;

        Vector3 pos1 = gridToWorld(x1, y1, floor1);
        Vector3 pos2 = gridToWorld(x2, y2, floor2);
        stairwells.push_back({pos1, pos2, floor1, floor2, pos1.distanceTo(pos2)});
    }
}

Vector3 BuildingMap::gridToWorld(int x, int y, int floor) const {
    return Vector3(static_cast<float>(x) + 0.5f, static_cast<float>(floor) * floorHeight + 0.5f, static_cast<float>(y) + 0.5f);
}

void BuildingMap::worldToGrid(const Vector3& pos, int& x, int& y, int& floor) const {
    floor = std::clamp(static_cast<int>(pos.y / floorHeight), 0, numFloors - 1);
    x = std::clamp(static_cast<int>(pos.x), 0, width - 1);
    y = std::clamp(static_cast<int>(pos.z), 0, height - 1);
}

void BuildingMap::loadPreset(const std::string& presetName, int floorCount) {
    if (presetName == "school") loadSchoolPreset(floorCount);
    else if (presetName == "hospital") loadHospitalPreset(floorCount);
    else if (presetName == "highrise") loadHighRisePreset(floorCount);
    else if (presetName == "mall") loadMallPreset(floorCount);
    else if (presetName == "airport") loadAirportPreset(floorCount);
    else if (presetName == "factory") loadFactoryPreset(floorCount);
    else if (presetName == "hotel") loadHotelPreset(floorCount);
    else if (presetName == "government") loadGovernmentPreset(floorCount);
    else if (presetName == "arena") loadArenaPreset(floorCount);
    else loadResidentialPreset(floorCount);
}

void BuildingMap::loadSchoolPreset(int floors) {
    numFloors = std::clamp(floors, 1, 10); width = 32; height = 20;
    initializeGrid(); exits.clear(); stairwells.clear();
    addExit(0, 10, 0, "Main Entrance Gate", 3.0f, 150);
    addExit(31, 10, 0, "East Fire Exit", 2.0f, 100);
    for (int f = 0; f < numFloors - 1; ++f) {
        addStairwell(2, 9, f, 2, 9, f + 1);
        addStairwell(29, 9, f, 29, 9, f + 1);
    }
}

void BuildingMap::loadHospitalPreset(int floors) {
    numFloors = std::clamp(floors, 1, 10); width = 36; height = 24;
    initializeGrid(); exits.clear(); stairwells.clear();
    addExit(0, 12, 0, "ER Ambulance Bay", 3.5f, 200);
    addExit(35, 12, 0, "Lobby West Exit", 3.0f, 150);
    for (int f = 0; f < numFloors - 1; ++f) {
        addStairwell(5, 5, f, 5, 5, f + 1);
        addStairwell(30, 5, f, 30, 5, f + 1);
    }
}

void BuildingMap::loadHighRisePreset(int floors) {
    numFloors = std::clamp(floors, 1, 10); width = 30; height = 30;
    initializeGrid(); exits.clear(); stairwells.clear();
    addExit(0, 15, 0, "Tower South Lobby Exit", 3.0f, 200);
    addExit(29, 15, 0, "Tower North Gate", 2.5f, 150);
    for (int f = 0; f < numFloors - 1; ++f) {
        addStairwell(13, 13, f, 13, 13, f + 1);
        addStairwell(17, 17, f, 17, 17, f + 1);
    }
}

void BuildingMap::loadMallPreset(int floors) {
    numFloors = std::clamp(floors, 1, 10); width = 40; height = 30;
    initializeGrid(); exits.clear(); stairwells.clear();
    addExit(0, 15, 0, "Mall Main Entrance", 4.0f, 300);
    addExit(39, 15, 0, "East Food Court Exit", 3.0f, 200);
    addExit(20, 0, 0, "North Retail Gate", 2.5f, 150);
    for (int f = 0; f < numFloors - 1; ++f) {
        addStairwell(10, 10, f, 10, 10, f + 1);
        addStairwell(30, 20, f, 30, 20, f + 1);
    }
}

void BuildingMap::loadAirportPreset(int floors) {
    numFloors = std::clamp(floors, 1, 10); width = 45; height = 22;
    initializeGrid(); exits.clear(); stairwells.clear();
    addExit(0, 11, 0, "Departures Main Terminal", 4.5f, 400);
    addExit(44, 11, 0, "Concourse B Fire Exit", 3.0f, 200);
    for (int f = 0; f < numFloors - 1; ++f) {
        addStairwell(15, 11, f, 15, 11, f + 1);
        addStairwell(30, 11, f, 30, 11, f + 1);
    }
}

void BuildingMap::loadFactoryPreset(int floors) {
    numFloors = std::clamp(floors, 1, 10); width = 38; height = 26;
    initializeGrid(); exits.clear(); stairwells.clear();
    addExit(0, 13, 0, "Factory Loading Dock 1", 4.0f, 250);
    addExit(37, 13, 0, "Assembly Yard Exit", 3.5f, 200);
    for (int f = 0; f < numFloors - 1; ++f) {
        addStairwell(4, 4, f, 4, 4, f + 1);
    }
}

void BuildingMap::loadHotelPreset(int floors) {
    numFloors = std::clamp(floors, 1, 10); width = 32; height = 26;
    initializeGrid(); exits.clear(); stairwells.clear();
    addExit(0, 13, 0, "Grand Hotel Lobby Exit", 3.5f, 200);
    addExit(31, 13, 0, "Pool Terrace Emergency Gate", 2.5f, 150);
    for (int f = 0; f < numFloors - 1; ++f) {
        addStairwell(3, 13, f, 3, 13, f + 1);
        addStairwell(28, 13, f, 28, 13, f + 1);
    }
}

void BuildingMap::loadGovernmentPreset(int floors) {
    numFloors = std::clamp(floors, 1, 10); width = 34; height = 24;
    initializeGrid(); exits.clear(); stairwells.clear();
    addExit(17, 0, 0, "Civic Palace Portico Gate", 4.0f, 300);
    addExit(17, 23, 0, "Rear Security Exit", 2.5f, 150);
    for (int f = 0; f < numFloors - 1; ++f) {
        addStairwell(8, 12, f, 8, 12, f + 1);
        addStairwell(25, 12, f, 25, 12, f + 1);
    }
}

void BuildingMap::loadArenaPreset(int floors) {
    numFloors = std::clamp(floors, 1, 10); width = 36; height = 36;
    initializeGrid(); exits.clear(); stairwells.clear();
    addExit(0, 18, 0, "Stadium Gate North", 4.0f, 400);
    addExit(35, 18, 0, "Stadium Gate South", 4.0f, 400);
    addExit(18, 0, 0, "Stadium Gate West", 4.0f, 400);
    addExit(18, 35, 0, "Stadium Gate East", 4.0f, 400);
    for (int f = 0; f < numFloors - 1; ++f) {
        addStairwell(6, 6, f, 6, 6, f + 1);
        addStairwell(29, 29, f, 29, 29, f + 1);
    }
}

void BuildingMap::loadResidentialPreset(int floors) {
    numFloors = std::clamp(floors, 1, 10); width = 30; height = 22;
    initializeGrid(); exits.clear(); stairwells.clear();
    addExit(0, 11, 0, "Courtyard Entrance", 2.5f, 150);
    addExit(29, 11, 0, "Garden Emergency Door", 2.0f, 100);
    for (int f = 0; f < numFloors - 1; ++f) {
        addStairwell(2, 11, f, 2, 11, f + 1);
        addStairwell(27, 11, f, 27, 11, f + 1);
    }
}

} // namespace Futura
