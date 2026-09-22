# FUTURA 3D E-VAC ENGINE (v3.0 Commercial Edition)
> **Real-World Multi-Floor Emergency Evacuation Simulation & Analytics Platform in Modern C++**

---

## 🌟 Overview
**FUTURA 3D E-VAC ENGINE** is a commercial-grade emergency evacuation simulation and spatial telemetry platform rewritten from Java Swing into modern high-performance **C++ (C++17/20)**.

Designed for real-world architectural safety analysis, smart building emergency management, and disaster response planning, FUTURA 3D provides full multi-floor 3D spatial simulation, multi-hazard propagation, IoT sensor grid response, crowd dynamics via Social Force Model and 3D A* pathfinding, and interactive multi-tab telemetry dashboards.

---

## 🚀 Key Features & Upgrades over Java Swing

| Feature | Java Swing Version (v2.0) | FUTURA 3D C++ Engine (v3.0) |
|---|---|---|
| **Spatial Graphics** | 2D Grid Layout | **Full 3D Multi-Floor Building Rendering** (Orbit Camera, Floor Slices) |
| **Building Support** | Single Floor | **Multi-Story Buildings** with vertical Stairwell & Elevator transitions |
| **Pathfinding** | 2D A* | **3D A* Algorithm** with crowd density penalties & hazard cost rerouting |
| **Agent Dynamics** | Simple Grid Movement | **Social Force Model** (Repulsion, Panic Speeds, Smoke Inhalation) |
| **Hazard Models** | Static 2D Tiles | **Cellular Fire, Thermal Smoke Rise, Toxic Gas & Structural Collapse** |
| **Smart Systems** | Manual Controls | **IoT Sensor Grid** (Smoke Detectors, Automated Sprinklers, Dynamic LED Exit Signs) |
| **User Interface** | Single View Console | **Futuristic 5-Tab Command Center** (Control, Architect, Telemetry, Sensors, Settings) |

---

## 📂 Project Architecture

```
Emergency_Evac_Sys_Cpp/
├── CMakeLists.txt              # Build configuration for GCC / Clang / MSVC
├── README.md                   # System documentation & manual
├── include/
│   ├── Core/
│   │   ├── Vector3.hpp         # 3D spatial math vector
│   │   ├── Camera3D.hpp        # 3D Orbit camera controller
│   │   └── Logger.hpp          # Thread-safe system event logger
│   ├── Simulation/
│   │   ├── BuildingMap.hpp     # Multi-floor 3D spatial grid & presets
│   │   ├── Agent.hpp           # 3D Agent physics & social force model
│   │   ├── Pathfinding.hpp     # 3D A* vertical pathfinding algorithm
│   │   ├── HazardModel.hpp     # Multi-hazard cellular spread model
│   │   ├── SensorGrid.hpp      # Smart IoT disaster response system
│   │   └── EvacSimulationEngine.hpp # Master multi-threaded simulation controller
│   ├── Analytics/
│   │   ├── HeatmapGenerator.hpp# Crowd congestion & bottleneck heatmap generator
│   │   └── EvacMetrics.hpp     # Real-time evacuation curves & efficiency calculator
│   └── UI/
│       └── UIStyles.hpp        # Dark cyber styling & color constants
├── src/                        # Complete C++ implementations
│   ├── Core/
│   ├── Simulation/
│   ├── Analytics/
│   ├── UI/
│   └── main.cpp                # Native C++ entry point & CLI telemetry
├── scenarios/                  # Preset JSON scenarios
│   ├── school_multi_floor.json
│   ├── hospital_complex.json
│   └── highrise_office.json
└── web_preview/
    └── index.html              # Standalone Interactive 3D WebGL Command Center
```

---

## 🖥️ How to Run the 3D Interactive Web App (Instant Preview)

To immediately launch and test the interactive 3D simulation suite with full orbit controls, tabs, and interactive hazard painting:

1. Open `web_preview/index.html` in any web browser (Chrome, Edge, Firefox, Safari).
2. Enjoy the 3D view:
   - **Rotate Camera**: Left-Click + Drag.
   - **Pan**: Right-Click + Drag.
   - **Zoom**: Mouse Scroll Wheel.
   - **Filter Floors**: Click `Floor 1`, `Floor 2`, or `All Floors` buttons.
   - **Paint 3D Hazards**: Click `Paint Fire`, `Paint Smoke`, or `Toxic Gas` in Module 2 and click inside the 3D building.
   - **Start Evacuation**: Click `START SIM` in Module 3.
   - **Explore Tabs**: Click between **Command & Control**, **Building Architect**, **Telemetry & Heatmaps**, **IoT Sensor Grid**, and **Scenario Manager**.

---

## 🛠️ How to Compile & Run the Native C++ Engine

### Prerequisites
- Modern C++ Compiler (`g++`, `clang++`, or MSVC with C++17 support)
- `cmake` (version 3.14 or higher)

### Build Instructions
```bash
# 1. Navigate to the project directory
cd Emergency_Evac_Sys_Cpp

# 2. Create build directory
mkdir build && cd build

# 3. Configure with CMake
cmake ..

# 4. Compile the project
cmake --build .

# 5. Run the FUTURA 3D C++ Engine executable
./FuturaEvac3D
```

---

## 📊 Analytics & CSV Export
The engine continuously records:
- **Evacuation Throughput Curve**
- **Casualty & Smoke Inhalation Index**
- **System Efficiency Score (%)**
- **Bottleneck Congestion Heatmaps**

Telemetry data can be exported directly to standard CSV format via the Scenario Manager tab or C++ `metrics.exportToCSV()`.
