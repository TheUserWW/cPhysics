# Field System Documentation

## Overview

The Field system in CPhysics engine handles the application of various physical fields (gravitational, electric, and magnetic) to Entity objects. These field interactions are fundamental to simulating realistic physics behaviors in 3D space, including classical mechanics and electromagnetism.

## Field Types

### Gravitational Field
Applies uniform gravitational acceleration to objects based on Newtonian gravity principles.

### Electric Field
Applies electric forces to charged objects based on Coulomb's law.

### Magnetic Field
Applies magnetic forces to moving charged objects based on the Lorentz force law.

## Data Structures

### Field Type Definitions

```c
typedef struct gravitational_field {
    double magnitude;      // Gravitational field strength in m/s²
    Vector direction;      // Unit direction vector
} gravitational_field;

typedef struct electric_field {
    double magnitude;      // Electric field strength in N/C (Newtons per Coulomb)
    Vector direction;      // Unit direction vector
} electric_field;

typedef struct magnetic_field {
    double magnitude;      // Magnetic field strength in Tesla (T)
    Vector direction;      // Unit direction vector
    Vector position;       // Field source position in meters (for future use)
} magnetic_field;
```

### Vector Type

```c
typedef struct Vector {
    double x;
    double y;
    double z;
} Vector;
```

### Error Handling

```c
typedef enum {
    FIELD_SUCCESS = 0,              // Operation completed successfully
    FIELD_ERROR_NULL_POINTER,       // Null pointer detected
    FIELD_ERROR_INVALID_MASS,       // Mass is zero or too small
    FIELD_ERROR_INVALID_CHARGE,     // Charge is zero or too small
    FIELD_ERROR_STATIC_OBJECT       // Object is marked as static
} FieldErrorCode;
```

## Core Functions

### `apply_gravitational_field`

**Purpose**: Applies gravitational acceleration to an entity.

**Signature**:
```c
FieldErrorCode apply_gravitational_field(Entity* obj, const gravitational_field* g);
```

**Physics Formula**:
```
a = g * direction
```

**Parameters**:
- `obj`: Pointer to target Entity object
- `g`: Pointer to gravitational field parameters

**Return Value**: FieldErrorCode indicating operation status

**Error Conditions**:
- Returns `FIELD_ERROR_NULL_POINTER` if obj or g is NULL
- Returns `FIELD_ERROR_STATIC_OBJECT` if object is static

**Usage Example**:
```c
gravitational_field gravity = {9.8, {0, -1, 0}};  // Downward gravity
Vector pos = {0, 0, 0};
Vector vel = {0, 0, 0};
Entity ball = new_entity("Ball", 1.0, 0.0, &pos, &vel, NULL, 0.8, true, false);
FieldErrorCode result = apply_gravitational_field(&ball, &gravity);
```

### `apply_electric_field`

**Purpose**: Applies electric force acceleration to a charged entity.

**Signature**:
```c
FieldErrorCode apply_electric_field(Entity* obj, const electric_field* e);
```

**Physics Formula**:
```
a = (q * E) / m * direction
```

**Parameters**:
- `obj`: Pointer to target Entity object
- `e`: Pointer to electric field parameters

**Return Value**: FieldErrorCode indicating operation status

**Error Conditions**:
- Returns `FIELD_ERROR_NULL_POINTER` if obj or e is NULL
- Returns `FIELD_ERROR_STATIC_OBJECT` if object is static
- Returns `FIELD_ERROR_INVALID_MASS` if mass is too small

**Usage Example**:
```c
electric_field e_field = {1000.0, {1, 0, 0}};  // 1000 N/C rightward field
Vector pos = {0, 0, 0};
Vector vel = {0, 0, 0};
Entity electron = new_entity("Electron", 9.1e-31, -1.6e-19, &pos, &vel, NULL, 1.0, true, false);
FieldErrorCode result = apply_electric_field(&electron, &e_field);
```

### `apply_magnetic_field`

**Purpose**: Applies magnetic force acceleration using Lorentz force law.

**Signature**:
```c
FieldErrorCode apply_magnetic_field(Entity* obj, const magnetic_field* b);
```

**Physics Formula**:
```
F = q(v × B)
a = F / m = (q * (v × B)) / m
```

**Parameters**:
- `obj`: Pointer to target Entity object
- `b`: Pointer to magnetic field parameters

**Return Value**: FieldErrorCode indicating operation status

**Error Conditions**:
- Returns `FIELD_ERROR_NULL_POINTER` if obj or b is NULL
- Returns `FIELD_ERROR_STATIC_OBJECT` if object is static
- Returns `FIELD_ERROR_INVALID_MASS` if mass is too small
- Returns `FIELD_ERROR_INVALID_CHARGE` if charge is too small

**Cross Product Calculation**:
```c
cross_product.x = vy * Bz - vz * By
cross_product.y = vz * Bx - vx * Bz
cross_product.z = vx * By - vy * Bx
```

**Usage Example**:
```c
magnetic_field b_field = {0.5, {0, 0, 1}, {0, 0, 0}};  // 0.5T upward field
Vector pos = {0, 0, 0};
Vector vel = {1000, 0, 0};
Entity proton = new_entity("Proton", 1.67e-27, 1.6e-19, &pos, &vel, NULL, 1.0, true, false);
FieldErrorCode result = apply_magnetic_field(&proton, &b_field);
```

## Implementation Details

### Numerical Stability
- Uses `DBL_EPSILON` from `<float.h>` for floating-point comparisons
- Prevents division by zero and numerical instability
- Handles edge cases with very small mass or charge values

### Performance Optimizations
- Pointer-based parameter passing reduces memory copying
- Inline calculations minimize function call overhead
- Early error returns prevent unnecessary computations

### Safety Features
- Comprehensive null pointer checking
- Validation of physical constraints
- Protection against invalid operations on static objects

## Integration with Other Systems

### Entity System
- Fields modify Entity's acceleration vectors (Vector type)
- Works with both static and dynamic entities
- Respects entity properties (mass, charge, static flag)

### Movement System
- Field-applied accelerations are integrated by movement functions
- Supports both linear and rotational dynamics
- Compatible with time flow management

### Collision System
- Field forces are applied before collision detection
- Acceleration modifications affect subsequent motion
- Works with rigid body physics

## Physical Units

All field calculations use standard SI units:
- **Mass**: kilograms (kg)
- **Charge**: coulombs (C)
- **Position**: meters (m)
- **Velocity**: meters per second (m/s)
- **Acceleration**: meters per second squared (m/s²)
- **Electric Field**: newtons per coulomb (N/C)
- **Magnetic Field**: tesla (T)

## Error Handling Best Practices

When using field functions, always check return codes:

```c
FieldErrorCode result = apply_gravitational_field(&obj, &field);
if (result != FIELD_SUCCESS) {
    switch (result) {
        case FIELD_ERROR_NULL_POINTER:
            printf("Error: Null pointer detected\n");
            break;
        case FIELD_ERROR_STATIC_OBJECT:
            printf("Warning: Cannot apply field to static object\n");
            break;
        case FIELD_ERROR_INVALID_MASS:
            printf("Error: Invalid mass value\n");
            break;
        case FIELD_ERROR_INVALID_CHARGE:
            printf("Error: Invalid charge value\n");
            break;
    }
}
```

## Using with EntityManager

The EntityManager class provides batch field operations:

```cpp
#include "include/core/entity_manager.h"

EntityManager manager;

// Add entities
manager.addEntity("Particle1", 1.0, 1.6e-19, {0, 0, 0}, {100, 0, 0});
manager.addEntity("Particle2", 1.0, -1.6e-19, {1, 0, 0}, {-100, 0, 0});

// Apply fields to all entities
manager.applyGravityField(9.8, {0, -1, 0});
manager.applyElectricField(1000.0, {1, 0, 0});
manager.applyMagneticField(0.5, {0, 0, 1});
```

## Vector Operations

The field system uses Vector operations from mathlib/Vector.h:

```c
double dot_product(const Vector a, const Vector b);
Vector cross_product(const Vector a, const Vector b);
double normalize(const Vector a);
```

## Complete Example

```c
#include "include/cphysics.h"
#include "include/core/field.h"

int main() {
    // Create a charged particle
    Vector pos = {0, 0, 0};
    Vector vel = {1000, 0, 0};
    Entity particle = new_entity("Proton", 1.67e-27, 1.6e-19, 
                                 &pos, &vel, NULL, 1.0, true, false);
    
    // Define fields
    gravitational_field gravity = {9.8, {0, -1, 0}};
    electric_field e_field = {1000.0, {1, 0, 0}};
    magnetic_field b_field = {0.5, {0, 0, 1}, {0, 0, 0}};
    
    // Apply fields
    apply_gravitational_field(&particle, &gravity);
    apply_electric_field(&particle, &e_field);
    apply_magnetic_field(&particle, &b_field);
    
    // The particle's acceleration is now modified
    printf("Acceleration: (%.2e, %.2e, %.2e)\n", 
           particle.acceleration.x,
           particle.acceleration.y,
           particle.acceleration.z);
    
    return 0;
}
```

This documentation provides comprehensive guidance for using the field system in CPhysics engine, ensuring proper implementation of physical field interactions in your simulations.
