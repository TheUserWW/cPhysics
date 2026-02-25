# Movement Module Documentation

## Overview
The `movement.h` module provides physics simulation capabilities for applying forces and handling rotational dynamics in a 3D space. This module implements classical mechanics principles including Newtonian forces, Coulomb's law, universal gravitation, and quaternion-based rotation systems.

## Module Structure
- **Header File**: `include/core/movement.h`
- **Implementation**: `src/core/movement.c`
- **Dependencies**: `include/core/entity.h`, `include/mathlib/Vector.h`, `include/mathlib/Quaternion.h`

## Data Types Used

### Vector
```c
typedef struct Vector {
    double x;
    double y;
    double z;
} Vector;
```

### Quaternion
```c
typedef struct {
    float w;  // Scalar (real) component
    float x;  // i-component (imaginary)
    float y;  // j-component (imaginary)
    float z;  // k-component (imaginary)
} Quaternion;
```

## Force Application Functions

### `apply_force(Entity* obj, const Vector* acceleration_vector)`
Applies an arbitrary force to an entity by directly modifying its acceleration.

**Parameters:**
- `obj`: Pointer to the target Entity
- `acceleration_vector`: Pointer to Vector containing acceleration components

**Usage:**
```c
Vector force = {10.0, 0.0, 0.0};
apply_force(&entity, &force);
```

### `apply_electric_force(Entity* obj_1, Entity* obj_2)`
Calculates and applies electrostatic force between two charged entities according to Coulomb's law:
```
F = k × (q₁ × q₂) / r²
```

**Parameters:**
- `obj_1`: First charged entity
- `obj_2`: Second charged entity

**Note:** Entities must have charge properties defined for this function to work correctly.

### `apply_universal_gravitation(Entity* obj_1, Entity* obj_2)`
Applies gravitational attraction between two entities according to Newton's law of universal gravitation:
```
F = G × (m₁ × m₂) / r²
```

**Parameters:**
- `obj_1`: First entity with mass
- `obj_2`: Second entity with mass

## Quaternion Rotation System

The module implements a complete quaternion-based rotation system for handling 3D rotations without gimbal lock.

### Core Quaternion Operations

#### `quaternion_multiply(Quaternion q1, Quaternion q2)`
Multiplies two quaternions together. Quaternion multiplication is non-commutative and represents combined rotations.

**Parameters:**
- `q1`: First quaternion
- `q2`: Second quaternion

**Return:** Product quaternion

#### `quaternion_conjugate(Quaternion q)`
Computes the conjugate of a quaternion, which represents the inverse rotation.

**Parameters:**
- `q`: Input quaternion

**Return:** Conjugate quaternion

#### `quaternion_normalize(Quaternion q)`
Normalizes a quaternion to ensure it represents a valid rotation (unit quaternion).

**Parameters:**
- `q`: Quaternion to normalize

**Return:** Normalized quaternion

### Rotation Conversion Functions

#### `axis_angle_to_quaternion(const Vector* axis, double angle, Quaternion* result)`
Converts an axis-angle rotation representation to a quaternion.

**Parameters:**
- `axis`: Pointer to rotation axis vector (should be normalized)
- `angle`: Rotation angle in radians
- `result`: Output quaternion

#### `euler_to_quaternion(double pitch, double yaw, double roll)`
Converts Euler angles to a quaternion representation.

**Parameters:**
- `pitch`: Rotation around x-axis (radians)
- `yaw`: Rotation around y-axis (radians)
- `roll`: Rotation around z-axis (radians)

**Return:** Quaternion representing the rotation

### Vector Rotation

#### `rotate_vector_by_quaternion(const Vector* v, Quaternion q, Vector* result)`
Rotates a 3D vector using a quaternion rotation.

**Parameters:**
- `v`: Input vector to rotate
- `q`: Rotation quaternion
- `result`: Output rotated vector

### Dynamic Rotation Updates

#### `update_quaternion_with_angular_velocity(Quaternion* q, const Vector* omega, double dt)`
Updates a quaternion based on angular velocity over a time step.

**Parameters:**
- `q`: Current orientation quaternion (updated in-place)
- `omega`: Angular velocity vector (radians per second)
- `dt`: Time step (seconds)

#### `apply_torque(Entity* obj, const Vector* torque)`
Applies torque to an entity, affecting its angular acceleration.

**Parameters:**
- `obj`: Target entity
- `torque`: Torque vector

#### `update_rotation(Entity* obj, double dt)`
Updates an entity's rotation based on its current angular velocity and acceleration.

**Parameters:**
- `obj`: Entity to update
- `dt`: Time step (seconds)

#### `rotate_entity(Entity* obj, const Vector* axis, double angle)`
Rotates an entity by a specified angle around a given axis.

**Parameters:**
- `obj`: Entity to rotate
- `axis`: Rotation axis vector
- `angle`: Rotation angle in radians

## Collision Detection and Response

### `get_entity_radius(const Entity* entity)`
Gets the collision radius of an entity.

**Parameters:**
- `entity`: Target entity

**Return:** Collision radius

### `set_entity_radius(Entity* entity, double radius)`
Sets the collision radius of an entity.

**Parameters:**
- `entity`: Target entity
- `radius`: New collision radius

### `check_sphere_collision(const Entity* obj_1, const Entity* obj_2)`
Checks if two spherical entities are colliding.

**Parameters:**
- `obj_1`: First entity
- `obj_2`: Second entity

**Return:** Non-zero if collision detected, 0 otherwise

### `resolve_sphere_collision(Entity* obj_1, Entity* obj_2)`
Resolves collision between two spherical entities by updating their velocities.

**Parameters:**
- `obj_1`: First entity (modified)
- `obj_2`: Second entity (modified)

### `apply_collision_response(Entity* entities, int count)`
Applies collision response to an array of entities.

**Parameters:**
- `entities`: Array of entity pointers
- `count`: Number of entities in the array

## Physics Constants

Defined in `include/constant.h`:
- **G**: Gravitational constant (6.67430 × 10⁻¹¹ m³ kg⁻¹ s²)
- **K**: Coulomb's constant (8.987551787 × 10⁹ N m² C⁻²)
- **PI**: π (3.14159265358979323846)
- **SPEED_OF_LIGHT**: c (3 × 10⁸ m/s)

## Usage Example

```c
#include "include/core/movement.h"
#include "include/core/entity.h"

int main() {
    // Create entities with mass and charge
    Vector pos1 = {0, 0, 0};
    Vector pos2 = {10, 0, 0};
    Vector vel1 = {0, 0, 0};
    Vector vel2 = {0, 0, 0};
    
    Entity obj1 = new_entity("Object1", 10.0, 1.0e-6, &pos1, &vel1, NULL, 0.8, true, false);
    Entity obj2 = new_entity("Object2", 5.0, -1.0e-6, &pos2, &vel2, NULL, 0.8, true, false);
    
    // Apply gravitational force
    apply_universal_gravitation(&obj1, &obj2);
    
    // Apply electrostatic force
    apply_electric_force(&obj1, &obj2);
    
    // Rotate an entity
    Vector axis = {0.0, 1.0, 0.0}; // Y-axis
    rotate_entity(&obj1, &axis, M_PI / 4.0); // 45 degrees
    
    // Update rotation over time
    update_rotation(&obj1, 0.016); // 16ms time step
    
    // Check and resolve collisions
    if (check_sphere_collision(&obj1, &obj2)) {
        resolve_sphere_collision(&obj1, &obj2);
    }
    
    return 0;
}
```

## Using with EntityManager

```cpp
#include "include/core/entity_manager.h"

EntityManager manager;

// Add entities
manager.addEntity("Ball1", 1.0, 0.0, {0, 0, 0}, {1, 0, 0}, 0.5);
manager.addEntity("Ball2", 1.0, 0.0, {2, 0, 0}, {-1, 0, 0}, 0.5);

// Simulation loop
double dt = 0.016;
for (int i = 0; i < 1000; i++) {
    manager.updateAllPhysics(dt);
    manager.checkCollisions();
}
```

## Implementation Notes

- All vector operations use the Vector struct type with x, y, z components
- Quaternions are stored as {w, x, y, z} where w is the scalar component
- Angles are expected in radians
- Time steps should be consistent throughout the simulation
- Entity structures must contain appropriate physics properties (mass, charge, position, velocity, etc.)

## Error Handling

- Functions assume valid input parameters
- Quaternion operations include normalization to prevent numerical instability
- Null pointer checks are performed where appropriate

## Performance Considerations

- Quaternion operations are optimized for real-time simulation
- Force calculations use direct mathematical operations for efficiency
- Consider caching frequently used values in large-scale simulations
- Use EntityManager for batch operations on multiple entities

## Related Modules

- **Entity System**: Defines the base entity structure used by all movement functions
- **Field System**: Applies uniform fields (gravitational, electric, magnetic)
- **Collision Detection**: Works alongside movement for complete physics simulation
- **Time Flow**: Handles simulation time management
- **Vector Library**: Provides vector mathematical operations
- **Quaternion Library**: Provides quaternion mathematical operations
