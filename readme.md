# CPhysics Engine

A lightweight physics simulation engine written in C. This library provides fundamental physics calculations including gravitational forces, electric forces, and basic entity movement simulations.

## Features

- **Entity System**: Create and manage physical entities with mass, charge, position, velocity, and acceleration
- **Universal Gravitation**: Calculate gravitational forces between celestial bodies
- **Electrostatic Forces**: Compute electric forces between charged particles
- **3D Physics**: Support for 3-dimensional position, velocity, and acceleration vectors
- **CMake Build System**: Easy compilation and integration with other projects
- **Cross-Platform**: Compatible with Windows, Linux, and macOS

## Project Structure
```
CPhysics/
├── include/              # Header files
│   ├── cphysics.h       # Main library header
│   ├── entity.h         # Entity definitions and functions
│   ├── field.h          # Field calculations
│   ├── movement.h       # Movement and kinematics
│   └── constant.h       # Physical constants
├── src/                 # Source files
│   ├── entity.c         # Entity implementation
│   ├── field.c          # Field calculations
│   └── movement.c       # Movement implementation
├── main.c               # Example usage and test suite
└── CMakeLists.txt       # Build configuration

```

## Quick Start

### Prerequisites

- CMake (version 4.1 or higher)
- C compiler (GCC, Clang, or MSVC)
- Make or Ninja build system

### Building the Project

```bash
# Clone or navigate to the project directory
cd CPhysics

# Create build directory
mkdir build && cd build

# Configure with CMake
cmake ..

# Build the project
cmake --build .

# Run the test suite
./CPhysics
```

### Example Usage

```c
#include "include/cphysics.h"

int main(void) {
    // Create celestial bodies
    double earth_pos[] = {0.0, 0.0, 0.0};
    double earth_vel[] = {0.0, 0.0, 0.0};
    double earth_acc[] = {0.0, 0.0, 0.0};
    
    Entity earth = new_entity("Earth", 5.972e24, 0.0, earth_pos, earth_vel, earth_acc, true, false, 0.8);
    
    double moon_pos[] = {3.844e8, 0.0, 0.0};
    double moon_vel[] = {0.0, 1022.0, 0.0};
    Entity moon = new_entity("Moon", 7.348e22, 0.0, moon_pos, moon_vel, earth_acc, true, false, 0.8);
    
    // Calculate gravitational force
    apply_universal_gravitation(&earth, &moon);
    
    // Get distance between entities
    double distance = get_euclidean_distance(&earth, &moon);
    
    return 0;
}
```

## API Reference

### Core Functions

- `new_entity()`: Create a new physical entity
- `apply_universal_gravitation()`: Apply gravitational force between two entities
- `apply_electric_force()`: Apply electric force between charged particles
- `get_euclidean_distance()`: Calculate distance between two entities
- `update_entity_position()`: Update entity position based on velocity and acceleration

### Entity Structure

```c
typedef struct {
    char name[50];
    double mass;           // kg
    double charge;         // C
    double position[3];    // m
    double velocity[3];    // m/s
    double acceleration[3];// m/s²
    bool is_active;
    bool is_static;
    double radius;         // m
} Entity;
```

## Physical Constants

The library includes commonly used physical constants:

- Gravitational constant (G): 6.67430e-11 m³/kg/s²
- Coulomb's constant (k): 8.987551787e9 N·m²/C²
- Elementary charge (e): 1.602176634e-19 C

## Testing

The project includes a comprehensive test suite in `main.c` that demonstrates:

1. Entity creation and management
2. Gravitational force calculations (Earth-Moon system)
3. Electrostatic force calculations (proton-electron system)
4. Distance calculations

Run the tests with:
```bash
./CPhysics
```

## License

This project is open source and available under the MIT License.

## Contributing

Contributions are welcome! Please feel free to submit issues, feature requests, or pull requests.

## Future Development

Planned features include:
- Magnetic field calculations
- Collision detection and response
- Multi-body simulations
- Numerical integration methods
- Visualization support


