# CPhysics Engine

A lightweight physics simulation engine written in C. This library provides fundamental physics calculations including gravitational forces, electric forces, and basic entity movement simulations.

## Features

- **Advanced Entity System**: Create and manage physical entities with mass, charge, position, velocity, acceleration, and rotational properties
- **Universal Gravitation**: Calculate gravitational forces between celestial bodies
- **Electrostatic Forces**: Compute electric forces between charged particles
- **3D Physics**: Support for 3-dimensional position, velocity, and acceleration vectors
- **CMake Build System**: Easy compilation and integration with other projects
- **Cross-Platform**: Compatible with Windows, Linux, and macOS

## Project Structure
```
CPhysics/
├── include/              # Header files
│   ├── basic_obj/       # Basic 3D object definitions
│   │   ├── cube.h       # Cube object
│   │   ├── cylinder.h   # Cylinder object
│   │   ├── pyramid.h    # Pyramid object
│   │   └── sphere.h     # Sphere object
│   ├── graphics/        # Graphics components
│   │   └── camera.h     # Camera system
│   ├── cphysics.h       # Main library header
│   ├── entity.h         # Entity definitions and functions
│   ├── field.h          # Field calculations
│   ├── movement.h       # Movement and kinematics
│   ├── time_flow.h      # Time flow management
│   ├── plog.h           # Physics logging system
│   ├── collider.h       # Collision detection
│   ├── constant.h       # Physical constants
│   └── error_codes.h    # Error code definitions
├── src/                 # Source files
│   ├── graphics/        # Graphics implementations
│   │   └── camera.c     # Camera implementation
│   ├── entity.c         # Entity implementation
│   ├── field.c          # Field calculations
│   ├── movement.c       # Movement implementation
│   ├── time_flow.c      # Time flow implementation
│   ├── plog.c           # Physics logging implementation
│   ├── collider.c       # Collision detection
│   ├── cube.c           # Cube implementation
│   ├── cylinder.c       # Cylinder implementation
│   ├── pyramid.c        # Pyramid implementation
│   └── sphere.c         # Sphere implementation
├── doc/                 # Documentation
│   ├── Entity.md        # Entity system documentation
│   ├── Field.md         # Field calculations documentation
│   ├── Formulas.md      # Physics formulas reference
│   └── Movement.md      # Movement system documentation
├── main.c               # Example usage and test suite
├── CMakeLists.txt       # Build configuration
└── LICENSE              # MIT License
```

## Quick Start

### Prerequisites

- CMake (version 3.10 or higher)
- C compiler (GCC, Clang, or MSVC)
- Make or Ninja build system

### Building the Project

```bash
# Clone or navigate to the project directory
cd cPhysics

# Create build directory
mkdir build && cd build

# Configure with CMake
cmake ..

# Build the project
cmake --build .

# Run the test suite
./cPhysics
```

### Example Usage

```c
#include "include/cphysics.h"

int main(void) {
    // Initialize physics logging
    plog_init("simulation.log");
    
    // Create celestial bodies with rotational properties
    double earth_pos[] = {0.0, 0.0, 0.0};
    double earth_vel[] = {0.0, 0.0, 0.0};
    double earth_acc[] = {0.0, 0.0, 0.0};
    double earth_quat[] = {1.0, 0.0, 0.0, 0.0}; // Identity quaternion
    double earth_ang_vel[] = {0.0, 0.0, 7.292e-5}; // Earth's rotation
    
    Entity earth = new_entity("Earth", 5.972e24, 0.0, earth_pos, earth_vel, earth_acc, 
                             earth_quat, earth_ang_vel, 0.0, 1.0, true, false);
    
    double moon_pos[] = {3.844e8, 0.0, 0.0};
    double moon_vel[] = {0.0, 1022.0, 0.0};
    double moon_quat[] = {1.0, 0.0, 0.0, 0.0};
    
    Entity moon = new_entity("Moon", 7.348e22, 0.0, moon_pos, moon_vel, earth_acc,
                            moon_quat, earth_ang_vel, 0.0, 1.0, true, false);
    
    // Set up time flow with 1:1 time scaling
    TimeFlow tf = new_time_flow(1.0, 0.0, 0.0);
    
    // Simulation loop
    for (int step = 0; step < 1000; step++) {
        // Calculate gravitational force
        apply_universal_gravitation(&earth, &moon);
        
        // Update positions based on forces
        update_entity_position(&earth, &tf);
        update_entity_position(&moon, &tf);
        
        // Log simulation state
        plog_entity_state(&earth, step);
        plog_entity_state(&moon, step);
        
        // Advance time
        advance_time(&tf, 1.0);
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
- `update_entity_position()`: Update entity position and orientation based on forces and time
- `get_euclidean_distance()`: Calculate distance between two entities
- `apply_force_to_entity()`: Apply external force to an entity

#### Physics Calculations
- `apply_universal_gravitation()`: Apply gravitational force between two entities
- `apply_electric_force()`: Apply electric force between charged particles
- `calculate_net_force()`: Calculate net force acting on an entity

#### Time Management
- `new_time_flow()`: Create a new time flow configuration
- `advance_time()`: Advance simulation time
- `get_simulation_time()`: Get current simulation time

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
    double position[3];                // 3D position vector (x, y, z)
    double velocity[3];                // 3D velocity vector (vx, vy, vz)
    double acceleration[3];            // 3D acceleration vector (ax, ay, az)
    double quaternion[4];              // Orientation quaternion (w, x, y, z)
    double angular_velocity[3];        // Angular velocity vector (ωx, ωy, ωz)
    double angular_acceleration[3];    // Angular acceleration vector (αx, αy, αz)
    double moment_of_inertia;          // Moment of inertia scalar
    double coefficient_of_restitution; // Elasticity coefficient (0.0-1.0)
    bool rigid_body;                   // Rigid body flag
    bool is_static;                    // Static object flag
} Entity;
```

## Physical Constants

The library includes commonly used physical constants:

- Gravitational constant (G): 6.67430e-11 m³/kg/s²
- Coulomb's constant (k): 8.987551787e9 N·m²/C²
- Elementary charge (e): 1.602176634e-19 C

## Testing

The project includes a comprehensive test suite in `main.c` that demonstrates:

1. Entity creation and management with rotational properties
2. Gravitational force calculations (Earth-Moon system)
3. Electrostatic force calculations (proton-electron system)
4. Rotational dynamics and quaternion operations
5. Time flow management and simulation control
6. Physics logging and data output
7. Distance calculations and collision detection

Run the tests with:
```bash
./CPhysics
```

## Documentation

Detailed documentation is available in the `doc/` directory:

- [Entity System Documentation](doc/Entity.md) - Complete guide to entity management
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

## License

This project is open source and available under the MIT License.

## Contributing

Contributions are welcome! Please feel free to submit issues, feature requests, or pull requests.

## Future Development

Planned features include:
- Magnetic field calculations
- Advanced collision detection and response
- Multi-body simulations with N-body problem solvers
- Numerical integration methods (Runge-Kutta, Verlet)
- Visualization support and real-time rendering
- Fluid dynamics simulations
- Thermodynamic systems
- Quantum mechanics extensions


