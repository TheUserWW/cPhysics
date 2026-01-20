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
    double direction[3];   // Unit direction vector (x, y, z)
} gravitational_field;

typedef struct electric_field {
    double magnitude;      // Electric field strength in N/C (Newtons per Coulomb)
    double direction[3];   // Unit direction vector (x, y, z)
} electric_field;

typedef struct magnetic_field {
    double magnitude;      // Magnetic field strength in Tesla (T)
    double direction[3];   // Unit direction vector (x, y, z)
    double position[3];    // Field source position in meters (for future use)
} magnetic_field;
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

**Physics Formula**:

a = g * direction


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
Entity ball = new_entity("Ball", 1.0, 0.0, /* ... */);
FieldErrorCode result = apply_gravitational_field(&ball, &gravity);
```

### `apply_electric_field`

**Purpose**: Applies electric force acceleration to a charged entity.

**Physics Formula**:

a = (q * E) / m * direction


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
Entity electron = new_entity("Electron", 9.1e-31, -1.6e-19, /* ... */);
FieldErrorCode result = apply_electric_field(&electron, &e_field);
```

### `apply_magnetic_field`

**Purpose**: Applies magnetic force acceleration using Lorentz force law.

**Physics Formula**:

F = q(v × B)
a = F / m = (q * (v × B)) / m


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
cross_product[0] = vy * Bz - vz * By
cross_product[1] = vz * Bx - vx * Bz
cross_product[2] = vx * By - vy * Bx
```

**Usage Example**:
```c
magnetic_field b_field = {0.5, {0, 0, 1}, {0, 0, 0}};  // 0.5T upward field
Entity proton = new_entity("Proton", 1.67e-27, 1.6e-19, /* ... */);
set_entity_velocity(&proton, 1000.0, 0.0, 0.0);  // Moving right at 1000 m/s
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
- Fields modify Entity's acceleration vectors
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
        // Handle other error cases
    }
}
```

This documentation provides comprehensive guidance for using the field system in CPhysics engine, ensuring proper implementation of physical field interactions in your simulations.