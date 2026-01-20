# Movement Module Documentation

## Overview
The `movement.h` module provides physics simulation capabilities for applying forces and handling rotational dynamics in a 3D space. This module implements classical mechanics principles including Newtonian forces, Coulomb's law, universal gravitation, and quaternion-based rotation systems.

## Module Structure
- **Header File**: `include/movement.h`
- **Implementation**: `src/movement.c`
- **Dependencies**: `include/entity.h`

## Force Application Functions

### `apply_force(Entity* obj, const double* acceleration_vector)`
Applies an arbitrary force to an entity by directly modifying its acceleration.

**Parameters:**
- `obj`: Pointer to the target Entity
- `acceleration_vector`: 3D acceleration vector (x, y, z components)

**Usage:**
```c
// Apply force in the positive x-direction
double force[3] = {10.0, 0.0, 0.0};
apply_force(&entity, force);
```

### `apply_electric_force(const Entity* obj_1, const Entity* obj_2)`
Calculates and applies electrostatic force between two charged entities according to Coulomb's law:
\[ F = \frac{k \cdot q_1 \cdot q_2}{r^2} \]

**Parameters:**
- `obj_1`: First charged entity
- `obj_2`: Second charged entity

**Note:** Entities must have charge properties defined for this function to work correctly.

### `apply_universal_gravitation(Entity* obj_1, Entity* obj_2)`
Applies gravitational attraction between two entities according to Newton's law of universal gravitation:
\[ F = \frac{G \cdot m_1 \cdot m_2}{r^2} \]

**Parameters:**
- `obj_1`: First entity with mass
- `obj_2`: Second entity with mass

## Quaternion Rotation System

The module implements a complete quaternion-based rotation system for handling 3D rotations without gimbal lock.

### Core Quaternion Operations

#### `quaternion_multiply(const double q1[4], const double q2[4], double result[4])`
Multiplies two quaternions together. Quaternion multiplication is non-commutative and represents combined rotations.

**Parameters:**
- `q1`: First quaternion (w, x, y, z)
- `q2`: Second quaternion (w, x, y, z)
- `result`: Output quaternion storing the product

#### `quaternion_conjugate(const double q[4], double result[4])`
Computes the conjugate of a quaternion, which represents the inverse rotation.

**Parameters:**
- `q`: Input quaternion
- `result`: Output quaternion storing the conjugate

#### `quaternion_normalize(double q[4])`
Normalizes a quaternion to ensure it represents a valid rotation (unit quaternion).

**Parameters:**
- `q`: Quaternion to normalize (modified in-place)

### Rotation Conversion Functions

#### `axis_angle_to_quaternion(const double axis[3], double angle, double q[4])`
Converts an axis-angle rotation representation to a quaternion.

**Parameters:**
- `axis`: 3D rotation axis vector (normalized)
- `angle`: Rotation angle in radians
- `q`: Output quaternion

#### `euler_to_quaternion(double pitch, double yaw, double roll, double q[4])`
Converts Euler angles to a quaternion representation.

**Parameters:**
- `pitch`: Rotation around x-axis (radians)
- `yaw`: Rotation around y-axis (radians)
- `roll`: Rotation around z-axis (radians)
- `q`: Output quaternion

### Vector Rotation

#### `rotate_vector_by_quaternion(const double v[3], const double q[4], double result[3])`
Rotates a 3D vector using a quaternion rotation.

**Parameters:**
- `v`: Input vector to rotate
- `q`: Rotation quaternion
- `result`: Output rotated vector

### Dynamic Rotation Updates

#### `update_quaternion_with_angular_velocity(double q[4], const double omega[3], double dt)`
Updates a quaternion based on angular velocity over a time step.

**Parameters:**
- `q`: Current orientation quaternion (updated in-place)
- `omega`: Angular velocity vector (radians per second)
- `dt`: Time step (seconds)

#### `apply_torque(Entity* obj, const double torque[3])`
Applies torque to an entity, affecting its angular acceleration.

**Parameters:**
- `obj`: Target entity
- `torque`: Torque vector (x, y, z components)

#### `update_rotation(Entity* obj, double dt)`
Updates an entity's rotation based on its current angular velocity and acceleration.

**Parameters:**
- `obj`: Entity to update
- `dt`: Time step (seconds)

#### `rotate_entity(Entity* obj, const double axis[3], double angle)`
Rotates an entity by a specified angle around a given axis.

**Parameters:**
- `obj`: Entity to rotate
- `axis`: Rotation axis vector
- `angle`: Rotation angle in radians

## Physics Constants

For proper functioning, ensure these constants are defined in your physics system:
- **G**: Gravitational constant (6.67430 × 10⁻¹¹ m³ kg⁻¹ s⁻²)
- **k**: Coulomb's constant (8.987551787 × 10⁹ N m² C⁻²)

## Usage Example

```c
#include "include/movement.h"

// Create entities with mass and charge
Entity obj1, obj2;
obj1.mass = 10.0;
obj2.mass = 5.0;
obj1.charge = 1.0e-6;
obj2.charge = -1.0e-6;

// Apply gravitational force
apply_universal_gravitation(&obj1, &obj2);

// Apply electrostatic force
apply_electric_force(&obj1, &obj2);

// Rotate an entity
double axis[3] = {0.0, 1.0, 0.0}; // Y-axis
double angle = M_PI / 4.0; // 45 degrees
rotate_entity(&obj1, axis, angle);

// Update rotation over time
update_rotation(&obj1, 0.016); // 16ms time step
```

## Implementation Notes

- All vector operations assume 3D vectors with x, y, z components
- Quaternions are stored as [w, x, y, z] where w is the scalar component
- Angles are expected in radians
- Time steps should be consistent throughout the simulation
- Entity structures must contain appropriate physics properties (mass, charge, position, velocity, etc.)

## Error Handling

- Functions assume valid input parameters
- Quaternion operations include normalization to prevent numerical instability
- No explicit error checking for null pointers or invalid entity states

## Performance Considerations

- Quaternion operations are optimized for real-time simulation
- Force calculations use direct mathematical operations for efficiency
- Consider caching frequently used values in large-scale simulations

## Related Modules

- **Entity System**: Defines the base entity structure used by all movement functions
- **Collision Detection**: Works alongside movement for complete physics simulation
- **Integration**: Handles numerical integration of forces and velocities over time