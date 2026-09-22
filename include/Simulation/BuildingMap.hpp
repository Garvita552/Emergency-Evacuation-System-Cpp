#ifndef BUILDING_MAP_HPP
#define BUILDING_MAP_HPP

#include "../Core/Vector3.hpp"
#include <vector>
#include <string>

namespace Futura {

enum class CellType {
    EMPTY = 0,
    WALL = 1,
    EXIT = 2,
    STAIRS_UP = 3,
    STAIRS_DOWN = 4,
    ELEVATOR = 5,
    DOOR_OPEN = 6,
    DOOR_CLOSED = 7
};

struct MapCell {
    int x;
    int y;
    int floor;
    CellType type;
    float hazardLevel;
    int hazardType;
    int agentCount;

    MapCell() : x(0), y(0), floor(0), type(CellType::EMPTY), hazardLevel(0.0f), hazardType(0), agentCount(0) {}
    MapCell(int xVal, int yVal, int floorVal, CellType t)
        : x(xVal), y(yVal), floor(floorVal), type(t), hazardLevel(0.0f), hazardType(0), agentCount(0) {}
};

struct ExitLocation {
    int id;
    Vector3 position;
    int floor;
    std::string label;
    bool isActive;
    int capacity;
    float widthMeters;
};

struct StairwellConnection {
    Vector3 bottomPos;
    Vector3 topPos;
    int fromFloor;
    int toFloor;
    float length;
};

struct RoomZone {
    std::string name;
    int x1, y1, x2, y2;
    int floor;
    std::string zoneType;
};

class BuildingMap {
public:
    int width;
    int height;
    int numFloors;
    float floorHeight;

    std::vector<std::vector<std::vector<MapCell>>> grid;
    std::vector<ExitLocation> exits;
    std::vector<StairwellConnection> stairwells;
    std::vector<RoomZone> rooms;

    BuildingMap();
    BuildingMap(int w, int h, int floors);

    void initializeGrid();
    bool isValid(int x, int y, int floor) const;
    bool isWalkable(int x, int y, int floor) const;
    
    MapCell& getCell(int x, int y, int floor);
    const MapCell& getCellConst(int x, int y, int floor) const;

    void setCellType(int x, int y, int floor, CellType type);
    void addExit(int x, int y, int floor, const std::string& label, float widthMeters = 2.0f, int capacity = 100);
    void toggleExitState(int exitId, bool isBlocked);
    void removeExit(int exitId);
    void addStairwell(int x1, int y1, int floor1, int x2, int y2, int floor2);

    void loadPreset(const std::string& presetName, int floorCount = 3);
    void loadSchoolPreset(int floors = 3);
    void loadHospitalPreset(int floors = 4);
    void loadHighRisePreset(int floors = 5);
    void loadMallPreset(int floors = 3);
    void loadAirportPreset(int floors = 2);
    void loadFactoryPreset(int floors = 1);
    void loadHotelPreset(int floors = 6);
    void loadGovernmentPreset(int floors = 3);
    void loadArenaPreset(int floors = 2);
    void loadResidentialPreset(int floors = 4);

    Vector3 gridToWorld(int x, int y, int floor) const;
    void worldToGrid(const Vector3& pos, int& x, int& y, int& floor) const;
};

} // namespace Futura

#endif // BUILDING_MAP_HPP
