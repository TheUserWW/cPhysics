# CPhysics Engine

A lightweight physics simulation engine written in C/C++. This library provides fundamental physics calculations including gravitational forces, electric forces, and basic entity movement simulations.

## Features

- **Advanced Entity System**: Create and manage physical entities with mass, charge, position, velocity, acceleration, and rotational properties
- **Universal Gravitation**: Calculate gravitational forces between celestial bodies
- **Electrostatic Forces**: Compute electric forces between charged particles
- **Magnetic Field Support**: Apply magnetic forces using Lorentz force law
- **3D Physics**: Support for 3-dimensional position, velocity, and acceleration vectors using Vector and Quaternion types
- **Entity Manager**: C++ class for managing multiple entities with batch operations and collision detection
- **OpenGL Graphics Support**: Real-time 3D visualization with camera, lighting, and texture support
- **Model Loading**: OBJ model loading support via custom Model class
- **Sphere Rendering**: Built-in sphere renderer for physics visualization
- **Mathematical Library**: Comprehensive vector operations and quaternion utilities
- **Skybox Rendering**: High dynamic range (HDR) skybox support for realistic environments
- **Collision Detection**: Sphere collision detection and response between entities
- **Physics Logging**: Comprehensive logging system for simulation data
- **CMake Build System**: Easy compilation and integration with other projects
- **vcpkg Integration**: Modern dependency management with vcpkg

## Project Structure
```
CPhysics/
├── include/             # Header files
│   ├── core/           # Core physics components
│   │   ├── collider.h  # Collision detection
│   │   ├── entity.h    # Entity definitions
│   │   ├── entity_manager.h  # Entity management (C++)
│   │   ├── field.h     # Field calculations
│   │   ├── movement.h  # Movement and kinematics
│   │   └── time_flow.h # Time flow management
│   ├── graphics/        # Graphics components
│   │   └── OpenGL/     # OpenGL implementations
│   │       ├── gl_camera.h  # Camera system
│   │       ├── gl_cube.h    # Cube rendering
│   │       ├── gl_model.h   # OBJ model loading (C++)
│   │       ├── gl_skybox.h  # Skybox rendering
│   │       ├── gl_sphere.hpp # Sphere rendering (C++)
│   │       └── gl_texture.h # Texture management
│   ├── mathlib/        # Mathematical utilities
│   │   ├── Vector.h    # Vector operations
│   │   └── Quaternion.h # Quaternion operations
│   ├── cphysics.h      # Main library header
│   ├── constant.h      # Physical constants
│   ├── error_codes.h   # Error code definitions
│   └── plog.h          # Physics logging system
├── src/                # Source files
│   ├── core/           # Core physics implementations
│   ├── graphics/       # Graphics implementations
│   │   └── OpenGL/     # OpenGL implementations
│   ├── mathlib/        # Mathematical utilities
│   └── plog.c         # Physics logging implementation
├── resources/          # Resource files
│   ├── models/        # 3D models (OBJ format)
│   ├── skybox/        # Skybox HDR textures
│   └── textures/      # Texture files
├── doc/               # Documentation
│   ├── Entity.md      # Entity system documentation
│   ├── Field.md       # Field calculations documentation
│   ├── Formulas.md    # Physics formulas reference
│   └── Movement.md    # Movement system documentation
├── vcpkg_installed/   # vcpkg dependencies
├── CMakeLists.txt     # Build configuration
├── vcpkg.json         # vcpkg manifest
├── vcpkg-configuration.json # vcpkg configuration
├── LICENSE            # MIT License
├── main.cpp           # Demo application
└── readme.md          # This file
```

## Quick Start

### Prerequisites

- CMake (version 3.10 or higher)
- C/C++ compiler (GCC, Clang, or MSVC)
- vcpkg package manager
- OpenGL libraries (for graphics features)

### Dependencies (via vcpkg)

The project uses vcpkg for dependency management. Required packages:
- GLFW (window and input management)
- GLEW (OpenGL extensions)

```bash
# Install vcpkg if not already installed
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.bat  # Windows
# or ./bootstrap-vcpkg.sh  # Linux/macOS

# Install dependencies
./vcpkg install glfw3:x64-windows
./vcpkg install glew:x64-windows
```

### Building the Project

```bash
# Clone or navigate to the project directory
cd cPhysics

# Create build directory
mkdir build && cd build

# Configure with CMake (specify vcpkg toolchain)
cmake .. -DCMAKE_TOOLCHAIN_FILE=[vcpkg-root]/scripts/buildsystems/vcpkg.cmake

# Build the project
cmake --build .

# Run the demo
./cPhysics
```

### Example Usage

```cpp
#include "include/cphysics.h"
#include "include/core/entity_manager.h"

int main(void) {
    // Initialize physics logging
    plog_init("simulation.log");
    
    // Create entity manager
    EntityManager entity_manager;
    
    // Add entities using the manager
    entity_manager.addEntity("Earth", 5.972e24, 0.0, 
                            {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0},
                            6371000.0, false);
    
    entity_manager.addEntity("Moon", 7.348e22, 0.0,
                            {3.844e8, 0.0, 0.0}, {0.0, 1022.0, 0.0},
                            1737400.0, false);
    
    // Simulation loop
    double dt = 0.001;
    for (int step = 0; step < 1000; step++) {
        // Update all physics
        entity_manager.updateAllPhysics(dt);
        
        // Check for collisions
        entity_manager.checkCollisions();
        
        // Log state
        for (Entity* entity : entity_manager) {
            plog_entity_state(entity, step);
        }
    }
    
    // Clean up
    plog_close();
    
    return 0;
}
```

## API Reference

### Core Functions

#### Entity Management
- `new_entity()`: Create a new physical entity with rotational properties
- `get_position()`, `get_velocity()`, `get_acceleration()`: Access entity state
- `set_entity_position()`, `set_entity_velocity()`: Modify entity state
- `get_euclidean_distance()`: Calculate distance between two entities
- `get_linear_momentum()`: Calculate linear momentum vector

#### Physics Calculations
- `apply_universal_gravitation()`: Apply gravitational force between two entities
- `apply_electric_force()`: Apply electric force between charged particles
- `apply_force()`: Apply arbitrary force to an entity
- `apply_torque()`: Apply torque to an entity

#### Field System
- `apply_gravitational_field()`: Apply uniform gravitational field
- `apply_electric_field()`: Apply uniform electric field
- `apply_magnetic_field()`: Apply magnetic field using Lorentz force

#### Time Management
- `new_time_flow()`: Create a new time flow configuration
- `advance_time()`: Advance simulation time
- `get_simulation_time()`: Get current simulation time

#### Collision System
- `check_sphere_collision()`: Check collision between two spherical entities
- `resolve_sphere_collision()`: Resolve collision between entities
- `apply_collision_response()`: Apply collision response to entity array

#### Logging System
- `plog_init()`: Initialize physics logging system
- `plog_entity_state()`: Log entity state to file
- `plog_close()`: Close logging system

### Entity Structure

```c
typedef struct Entity {
    char name[256];                    // Entity identifier
    double mass;                       // Mass in kilograms (kg)
    double charge;                     // Electric charge in coulombs (C)
    Vector position;                   // 3D position vector
    Vector velocity;                   // 3D velocity vector
    Vector acceleration;               // 3D acceleration vector
    Quaternion quaternion;             // Orientation quaternion
    Vector angular_velocity;           // Angular velocity vector
    Vector angular_acceleration;       // Angular acceleration vector
    double moment_of_inertia;          // Moment of inertia scalar
    double coefficient_of_restitution; // Elasticity coefficient (0.0-1.0)
    bool rigid_body;                   // Rigid body flag
    bool is_static;                    // Static object flag
} Entity;
```

### Vector Structure

```c
typedef struct Vector {
    double x;
    double y;
    double z;
} Vector;
```

### Quaternion Structure

```c
typedef struct {
    float w;  // Scalar (real) component
    float x;  // i-component
    float y;  // j-component
    float z;  // k-component
} Quaternion;
```

### EntityManager Class (C++)

```cpp
class EntityManager {
public:
    void addEntity(const std::string& name, double mass, double charge, 
                   const Vector& position, const Vector& velocity,
                   double radius = 0.5, bool is_static = false);
    bool removeEntity(const std::string& name);
    Entity* findEntity(const std::string& name);
    size_t getEntityCount() const;
    
    void updateAllPhysics(double delta_time);
    void applyGravityField(double magnitude, const Vector& direction);
    void applyElectricField(double magnitude, const Vector& direction);
    void applyMagneticField(double magnitude, const Vector& direction);
    void checkCollisions();
    
    bool saveToFile(const std::string& filename);
    bool loadFromFile(const std::string& filename);
    
    // Iterator support
    std::vector<Entity*>::iterator begin();
    std::vector<Entity*>::iterator end();
};
```

## Physical Constants

The library includes commonly used physical constants:

- Gravitational constant (G): 6.67430e-11 m³/kg/s²
- Coulomb's constant (K): 8.987551787e9 N·m²/C²
- Speed of light (c): 3e8 m/s
- Pi (π): 3.14159265358979323846

## Demo Application

The project includes a demo application in `main.cpp` that demonstrates:

1. OpenGL window creation and rendering
2. Entity creation and management with EntityManager
3. Sphere collision simulation
4. Camera controls (WASD + mouse)
5. HDR skybox rendering
6. OBJ model loading and rendering
7. Real-time physics updates

### Demo Controls
- **WASD** - Move camera
- **Q/E** - Move up/down
- **Mouse** - Look around
- **SPACE** - Pause/Resume simulation
- **R** - Reset simulation
- **ESC** - Exit

## Documentation

Detailed documentation is available in the `doc/` directory:

- [Entity System Documentation](doc/Entity.md) - Complete guide to entity management
- [Field System Documentation](doc/Field.md) - Field calculations and applications
- [Movement System Documentation](doc/Movement.md) - Movement and rotation dynamics
- [Physics Formulas Reference](doc/Formulas.md) - Mathematical foundations of the engine

## Advanced Features

### Rotational Dynamics
- Quaternion-based orientation representation
- Angular velocity and acceleration calculations
- Moment of inertia support
- Rigid body physics simulation

### Time Flow Management
- Configurable time scaling
- Simulation time tracking
- Time step control for numerical stability

### Physics Logging
- Comprehensive state logging
- Simulation data export
- Debugging and analysis support

### Graphics System
- Camera system with mouse look
- Sphere rendering for physics visualization
- OBJ model loading support
- HDR skybox rendering

## License

This project is open source and available under the MIT License.

## Contributing

Contributions are welcome! Please feel free to submit issues, feature requests, or pull requests.

## Future Development

Planned features include:
- Advanced collision detection (AABB, OBB)
- Multi-body simulations with N-body problem solvers
- Numerical integration methods (Runge-Kutta, Verlet)
- Fluid dynamics simulations
- Thermodynamic systems
- Quantum mechanics extensions
