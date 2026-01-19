# Entity System Documentation

## Overview

The Entity system is the core component of the CPhysics engine, representing physical objects in 3D space with comprehensive physical properties and behaviors. Each Entity encapsulates mass, charge, position, velocity, acceleration, rotational dynamics, and collision properties for realistic physics simulations. The system supports both translational and rotational motion, rigid body dynamics, and integrates with time flow management for accurate simulations.

## Entity Structure

### Data Structure Definition

```c
typedef struct Entity {
    char name[256];                    // Entity identifier (max 255 characters + null terminator)
    double mass;                       // Mass in kilograms (kg)
    double charge;                     // Electric charge in coulombs (C)
    double position[3];                // 3D position vector (x, y, z) in meters
    double velocity[3];                // 3D velocity vector (vx, vy, vz) in m/s
    double acceleration[3];            // 3D acceleration vector (ax, ay, az) in m/s²
    double quaternion[4];              // Orientation quaternion (w, x, y, z) - unit quaternion
    double angular_velocity[3];        // Angular velocity vector (ωx, ωy, ωz) in rad/s
    double angular_acceleration[3];    // Angular acceleration vector (αx, αy, αz) in rad/s²
    double moment_of_inertia;          // Moment of inertia scalar in kg·m²
    double coefficient_of_restitution; // Elasticity coefficient (0.0-1.0) for collisions
    bool rigid_body;                   // Rigid body flag (true for rigid body physics)
    bool is_static;                    // Static object flag (true for immovable objects)
} Entity;
```

### Field Descriptions

#### Basic Properties
- **name**: Human-readable identifier for the entity (e.g., "Earth", "Proton")
- **mass**: Mass of the entity in kilograms - fundamental for force calculations
- **charge**: Electric charge in coulombs - used for electromagnetic interactions

#### Translational Motion
- **position**: 3D coordinates in Cartesian space (x, y, z) in meters
- **velocity**: Rate of change of position in meters per second (vx, vy, vz)
- **acceleration**: Rate of change of velocity in meters per second squared (ax, ay, az)

#### Rotational Dynamics
- **quaternion**: 4-component quaternion representing 3D orientation (w, x, y, z)
  - Uses unit quaternions to avoid gimbal lock and ensure numerical stability
  - w component represents the scalar (real) part
  - x, y, z components represent the vector (imaginary) part
- **angular_velocity**: Rotational speed around each axis in radians per second
- **angular_acceleration**: Rate of change of angular velocity in rad/s²
- **moment_of_inertia**: Resistance to rotational acceleration (scalar approximation)

#### Physical Properties
- **coefficient_of_restitution**: Bounciness factor (0.0 = perfectly inelastic, 1.0 = perfectly elastic)
- **rigid_body**: Flag indicating whether entity follows rigid body dynamics
- **is_static**: Flag indicating whether entity is fixed in space (immovable)

## Entity Creation and Management

### Creating Entities

Use the `new_entity()` function to create new entities with specified properties:

```c
Entity new_entity(const char* name, double mass, double charge, 
                 double position[3], double velocity[3], double acceleration[3],
                 double quaternion[4], double angular_velocity[3], 
                 double moment_of_inertia, double coefficient_of_restitution,
                 bool rigid_body, bool is_static);
```

Example usage:
```c
double earth_pos[] = {0.0, 0.0, 0.0};
double earth_vel[] = {0.0, 0.0, 0.0};
double earth_acc[] = {0.0, 0.0, 0.0};
double earth_quat[] = {1.0, 0.0, 0.0, 0.0}; // Identity orientation
double earth_ang_vel[] = {0.0, 0.0, 7.292e-5}; // Earth's rotation rate

Entity earth = new_entity("Earth", 5.972e24, 0.0, earth_pos, earth_vel, earth_acc,
                         earth_quat, earth_ang_vel, 8.034e37, 0.3, true, false);
```

### Entity Operations

#### Position Updates
```c
void update_entity_position(Entity* entity, TimeFlow* time_flow);
```
Updates entity position and orientation based on current velocity, acceleration, and time step.

#### Force Application
```c
void apply_force_to_entity(Entity* entity, double force[3], double torque[3]);
```
Applies both linear force and rotational torque to an entity.

#### Distance Calculations
```c
double get_euclidean_distance(Entity* entity1, Entity* entity2);
```
Calculates the Euclidean distance between two entities in 3D space.

## Rotational Dynamics

### Quaternion Operations

The entity system uses quaternions for orientation representation to avoid gimbal lock and provide smooth interpolation:

- **Identity Quaternion**: [1.0, 0.0, 0.0, 0.0] represents no rotation
- **Normalization**: All orientation quaternions are automatically normalized
- **Integration**: Angular velocity is integrated to update orientation over time

### Angular Motion Integration

Angular motion follows the rotational analogs of Newton's laws:
- τ = Iα (Torque = Moment of Inertia × Angular Acceleration)
- ω = ω₀ + αΔt (Angular velocity integration)
- θ updated via quaternion integration

## Integration with Time Flow

Entities are designed to work seamlessly with the time flow system:

```c
TimeFlow tf = new_time_flow(1.0, 0.0, 0.0); // Real-time simulation

// Update entity state based on time flow
update_entity_position(&entity, &tf);

// Advance simulation time
advance_time(&tf, time_step);
```

This integration ensures consistent time scaling across all physics calculations.

#### Basic Properties
- **name**: Human-readable identifier (max 255 characters)
- **mass**: Object mass in kilograms (must be positive)
- **charge**: Electric charge in coulombs (can be positive, negative, or zero)

#### Kinematic Properties
- **position**: 3D Cartesian coordinates in meters
- **velocity**: Velocity components in meters/second
- **acceleration**: Acceleration components in meters/second²

#### Rotational Properties
- **quaternion**: Orientation represented as [w, x, y, z] quaternion
- **angular_velocity**: Angular velocity in radians/second
- **angular_acceleration**: Angular acceleration in radians/second²
- **moment_of_inertia**: Rotational inertia scalar

#### Physical Behavior
- **coefficient_of_restitution**: Elasticity coefficient (0.0 = perfectly inelastic, 1.0 = perfectly elastic)
- **rigid_body**: Flag indicating rigid body behavior
- **is_static**: Flag for immovable objects (infinite mass)

## API Functions

### Entity Creation

#### `new_entity()`
```c
struct Entity new_entity(const char* n, double m, double c,
                        const double d[3], const double v[3],
                        const double a[3], double cor, bool rigid, bool s);
```

**Parameters:**
- `n`: Entity name string
- `m`: Mass value
- `c`: Charge value
- `d`: Position vector [x, y, z] (can be NULL for origin)
- `v`: Velocity vector [vx, vy, vz] (can be NULL for stationary)
- `a`: Acceleration vector [ax, ay, az] (can be NULL for zero acceleration)
- `cor`: Coefficient of restitution
- `rigid`: Rigid body flag
- `s`: Static object flag

**Initialization Details:**
- Quaternion initialized to identity quaternion [1, 0, 0, 0]
- Angular velocity and acceleration set to zero
- Moment of inertia defaults to 1.0
- Arrays are safely copied with null checks

### Accessor Functions

#### Position Access
```c
inline double* get_position(Entity* obj);
```
Returns pointer to the position array for direct modification.

#### Velocity Access
```c
inline double* get_velocity(Entity* obj);
```
Returns pointer to the velocity array for direct modification.

#### Acceleration Access
```c
inline double* get_acceleration(Entity* obj);
```
Returns pointer to the acceleration array for direct modification.

### Utility Functions

#### `get_euclidean_distance()`
```c
double get_euclidean_distance(const Entity* obj_1, const Entity* obj_2);
```
Calculates the straight-line distance between two entities using the Euclidean distance formula:

distance = √((x₂-x₁)² + (y₂-y₁)² + (z₂-z₁)²)



#### `get_linear_momentum()`
```c
void get_linear_momentum(const Entity* obj, double result[3]);
```
Computes the linear momentum vector:

p = m × v

where `p` is momentum, `m` is mass, and `v` is velocity.

### Collision System

#### `process_collision()`
```c
void process_collision(Entity* obj_1, Entity* obj_2, double* loss);
```

**Collision Types Handled:**
1. **Static-Dynamic Collision**: One object is static, the other is dynamic
2. **Dynamic-Dynamic Collision**: Both objects are movable
3. **Static-Static Collision**: No collision processing (returns immediately)

**Collision Algorithm:**
1. **Normal Vector Calculation**: Computes collision normal from position difference
2. **Relative Velocity**: Calculates velocity along collision normal
3. **Impulse Calculation**: Uses coefficient of restitution to compute collision impulse
4. **Velocity Update**: Applies impulse to update velocities
5. **Energy Loss**: Optional energy loss calculation for inelastic collisions

**Collision Formulas:**
- **Impulse Magnitude**: `J = -(1 + e) × v_rel / (1/m₁ + 1/m₂)`
- **Velocity Update**: `v₁' = v₁ - J/m₁ × n`, `v₂' = v₂ + J/m₂ × n`
- **Energy Loss**: `ΔE = KE_before - KE_after`

## Physical Properties and Behaviors

### Mass and Inertia
- **Mass**: Affects response to forces (F = ma)
- **Static Objects**: Infinite mass, unaffected by forces
- **Moment of Inertia**: Affects rotational dynamics (τ = Iα)

### Charge and Electromagnetism
- **Positive Charge**: Repels other positive charges
- **Negative Charge**: Attracts positive charges
- **Neutral Objects**: Unaffected by electric forces

### Elasticity
- **Coefficient of Restitution**: Controls energy conservation in collisions
- **Perfectly Elastic (e=1.0)**: No energy loss
- **Perfectly Inelastic (e=0.0)**: Maximum energy loss

### Rigid Body Dynamics
- **Rigid Bodies**: Maintain shape during collisions
- **Non-rigid Bodies**: May deform (future implementation)

## Usage Examples

### Creating a Basic Entity
```c
double pos[3] = {0.0, 10.0, 0.0};
double vel[3] = {5.0, 0.0, 0.0};
Entity ball = new_entity("Ball", 1.0, 0.0, pos, vel, NULL, 0.8, true, false);
```

### Calculating Distance Between Entities
```c
double distance = get_euclidean_distance(&entity1, &entity2);
```

### Processing Collisions
```c
double energy_loss;
process_collision(&ball1, &ball2, &energy_loss);
```

## Coordinate System

### 3D Cartesian Coordinates
- **X-axis**: Right direction (positive values)
- **Y-axis**: Up direction (positive values)
- **Z-axis**: Forward direction (positive values)

### Quaternion Orientation
- **Quaternion Format**: [w, x, y, z] where w is the scalar part
- **Identity Quaternion**: [1, 0, 0, 0] represents no rotation
- **Normalized**: All quaternions are automatically normalized

## Performance Considerations

### Memory Layout
- Contiguous array storage for vector data
- Inline accessor functions for performance
- Stack allocation for entity creation

### Collision Optimization
- Early termination for static-static collisions
- Efficient distance calculations using squared distances
- Vectorized operations where possible

## Integration with Other Systems

### Movement System
- `apply_force()`: Applies forces to update acceleration
- `apply_electric_force()`: Handles electrostatic interactions
- `apply_universal_gravitation()`: Handles gravitational forces

### Field System
- Electric field calculations based on charge
- Gravitational field calculations based on mass

## Error Handling and Validation

### Input Validation
- Null pointer checks for array parameters
- Buffer overflow protection for name copying
- Safe array initialization with fallbacks

### Edge Cases
- Zero-mass objects handled appropriately
- Infinite distances (division by zero protection)
- Collision with static objects

This documentation provides a comprehensive reference for working with the Entity system in the CPhysics engine.