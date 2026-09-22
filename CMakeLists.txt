cmake_minimum_required(VERSION 3.14)
project(FuturaEvac3D VERSION 3.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Source files
set(SOURCES
    src/Core/Camera3D.cpp
    src/Simulation/BuildingMap.cpp
    src/Simulation/Agent.cpp
    src/Simulation/Pathfinding.cpp
    src/Simulation/HazardModel.cpp
    src/Simulation/SensorGrid.cpp
    src/Simulation/EvacSimulationEngine.cpp
    src/Analytics/HeatmapGenerator.cpp
    src/Analytics/EvacMetrics.cpp
    src/Analytics/ScenarioBenchmark.cpp
    src/Analytics/RiskAnalyzer.cpp
    src/UI/UIStyles.cpp
    src/main.cpp
)

# Executable target
add_executable(FuturaEvac3D ${SOURCES})

# Target properties
target_compile_features(FuturaEvac3D PRIVATE cxx_std_17)

message(STATUS "FUTURA 3D Next-Gen E-VAC Engine C++ configured successfully.")
