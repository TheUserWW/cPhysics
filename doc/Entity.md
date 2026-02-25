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
    Vector position;                   // 3D position vector
    Vector velocity;                   // 3D velocity vector
    Vector acceleration;               // 3D acceleration vector
    Quaternion quaternion;             // Orientation quaternion (w, x, y, z)
    Vector angular_velocity;           // Angular velocity vector (rad/s)
    Vector angular_acceleration;       // Angular acceleration vector (rad/s²)
    double moment_of_inertia;          // Moment of inertia scalar in kg·m²
    double coefficient_of_restitution; // Elasticity coefficient (0.0-1.0) for collisions
    bool rigid_body;                   // Rigid body flag (true for rigid body physics)
    bool is_static;                    // Static object flag (true for immovable objects)
} Entity;
```

### Supporting Types

#### Vector Structure

```c
typedef struct Vector {
    double x;
    double y;
    double z;
} Vector;
```

#### Quaternion Structure

```c
typedef struct {
    float w;  // Scalar (real) component
    float x;  // i-component (imaginary)
    float y;  // j-component (imaginary)
    float z;  // k-component (imaginary)
} Quaternion;
```

### Field Descriptions

#### Basic Properties
- **name**: Human-readable identifier for the entity (e.g., "Earth", "Proton")
- **mass**: Mass of the entity in kilograms - fundamental for force calculations
- **charge**: Electric charge in coulombs - used for electromagnetic interactions

#### Translational Motion
- **position**: 3D coordinates in Cartesian space using Vector type
- **velocity**: Rate of change of position in meters per second
- **acceleration**: Rate of change of velocity in meters per second squared

#### Rotational Dynamics
- **quaternion**: 4-component quaternion representing 3D orientation
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
struct Entity new_entity(const char* n, double m, double c,
                        const Vector* d, const Vector* v,
                        const Vector* a, double cor, bool rigid, bool s);
```

**Parameters:**
- `n`: Entity name string
- `m`: Mass value
- `c`: Charge value
- `d`: Position vector pointer (can be NULL for origin)
- `v`: Velocity vector pointer (can be NULL for stationary)
- `a`: Acceleration vector pointer (can be NULL for zero acceleration)
- `cor`: Coefficient of restitution
- `rigid`: Rigid body flag
- `s`: Static object flag

Example usage:
```c
Vector earth_pos = {0.0, 0.0, 0.0};
Vector earth_vel = {0.0, 0.0, 0.0};
Vector earth_acc = {0.0, 0.0, 0.0};

Entity earth = new_entity("Earth", 5.972e24, 0.0, 
                         &earth_pos, &earth_vel, &earth_acc,
                         0.3, true, false);
```

### Accessor Functions

#### Position Access
```c
Vector* get_position(Entity* obj);
```
Returns pointer to the position Vector for direct modification.

#### Velocity Access
```c
Vector* get_velocity(Entity* obj);
```
Returns pointer to the velocity Vector for direct modification.

#### Acceleration Access
```c
Vector* get_acceleration(Entity* obj);
```
Returns pointer to the acceleration Vector for direct modification.

### Setter Functions

#### Position Setter
```c
ErrorCode set_entity_position(Entity* obj, double x, double y, double z);
```

#### Velocity Setter
```c
ErrorCode set_entity_velocity(Entity* obj, double x, double y, double z);
```

#### Acceleration Setter
```c
ErrorCode set_entity_acceleration(Entity* obj, double x, double y, double z);
```

#### Angular Velocity Setter
```c
ErrorCode set_entity_angular_velocity(Entity* obj, double x, double y, double z);
```

#### Angular Acceleration Setter
```c
ErrorCode set_entity_angular_acceleration(Entity* obj, double x, double y, double z);
```

### Utility Functions

#### `get_euclidean_distance()`
```c
double get_euclidean_distance(const Entity* obj_1, const Entity* obj_2);
```
Calculates the straight-line distance between two entities:

```
distance = √((x₂-x₁)² + (y₂-y₁)² + (z₂-z₁)²)
```

#### `get_linear_momentum()`
```c
void get_linear_momentum(const Entity* obj, Vector* result);
```
Computes the linear momentum vector:

```
p = m × v
```

where `p` is momentum, `m` is mass, and `v` is velocity.

## EntityManager Class (C++)

The EntityManager provides a high-level C++ interface for managing multiple entities.

### Class Definition

```cpp
class EntityManager {
private:
    std::vector<Entity*> entities;
    
public:
    EntityManager();
    ~EntityManager();
    
    // Entity management
    void addEntity(Entity* entity);
    void addEntity(const std::string& name, double mass, double charge, 
                   const Vector& position, const Vector& velocity,
                   double radius = 0.5, bool is_static = false);
    
    bool removeEntity(const std::string& name);
    bool removeEntity(Entity* entity);
    void clearAll();
    
    // Entity lookup
    Entity* findEntity(const std::string& name);
    Entity* getEntity(size_t index);
    size_t getEntityCount() const;
    
    // Batch operations
    void updateAllPhysics(double delta_time);
    void applyGravityField(double magnitude, const Vector& direction);
    void applyElectricField(double magnitude, const Vector& direction);
    void applyMagneticField(double magnitude, const Vector& direction);
    
    // Collision detection
    void checkCollisions();
    
    // Serialization
    bool saveToFile(const std::string& filename);
    bool loadFromFile(const std::string& filename);
    
    // Iterator support
    std::vector<Entity*>::iterator begin();
    std::vector<Entity*>::iterator end();
};
```

### Usage Example

```cpp
#include "include/core/entity_manager.h"

int main() {
    EntityManager manager;
    
    // Add entities
    manager.addEntity("Ball1", 1.0, 0.0, {0.0, 0.0, 0.0}, {1.0, 0.0, 0.0}, 0.5, false);
    manager.addEntity("Ball2", 2.0, 0.0, {5.0, 0.0, 0.0}, {-1.0, 0.0, 0.0}, 0.5, false);
    
    // Simulation loop
    double dt = 0.016;
    for (int i = 0; i < 1000; i++) {
        manager.updateAllPhysics(dt);
        manager.checkCollisions();
    }
    
    // Iterate over entities
    for (Entity* entity : manager) {
        printf("%s: pos=(%.2f, %.2f, %.2f)\n", 
               entity->name, 
               entity->position.x, 
               entity->position.y, 
               entity->position.z);
    }
    
    return 0;
}
```

## Rotational Dynamics

### Quaternion Operations

The entity system uses quaternions for orientation representation to avoid gimbal lock and provide smooth interpolation:

- **Identity Quaternion**: `{w=1.0, x=0.0, y=0.0, z=0.0}` represents no rotation
- **Normalization**: All orientation quaternions are automatically normalized
- **Integration**: Angular velocity is integrated to update orientation over time

### Quaternion Functions (from Quaternion.h)

| Function | Description |
|----------|-------------|
| `quat_identity()` | Create identity quaternion |
| `quat_create(w, x, y, z)` | Create quaternion from components |
| `quat_from_axis_angle(angle, x, y, z)` | Create from axis-angle |
| `quat_from_euler(yaw, pitch, roll)` | Create from Euler angles |
| `quat_normalize(q)` | Normalize to unit length |
| `quat_multiply(q1, q2)` | Compose two rotations |
| `quat_conjugate(q)` | Get inverse rotation |
| `quat_rotate_vector(q, vx, vy, vz, ...)` | Rotate a vector |
| `quat_slerp(q1, q2, t)` | Spherical interpolation |

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

## Collision System

### Collision Detection

The system provides sphere-based collision detection:

```c
int check_sphere_collision(const Entity* obj_1, const Entity* obj_2);
```

### Collision Response

```c
void resolve_sphere_collision(Entity* obj_1, Entity* obj_2);
void apply_collision_response(Entity* entities, int count);
```

### Entity Radius

```c
double get_entity_radius(const Entity* entity);
void set_entity_radius(Entity* entity, double radius);
```

### Collision Algorithm

1. **Normal Vector Calculation**: Computes collision normal from position difference
2. **Relative Velocity**: Calculates velocity along collision normal
3. **Impulse Calculation**: Uses coefficient of restitution to compute collision impulse
4. **Velocity Update**: Applies impulse to update velocities
5. **Energy Loss**: Optional energy loss calculation for inelastic collisions

**Collision Formulas:**
- **Impulse Magnitude**: `J = -(1 + e) × v_rel / (1/m₁ + 1/m₂)`
- **Velocity Update**: `v₁' = v₁ - J/m₁ × n`, `v₂' = v₂ + J/m₂ × n`

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
Vector pos = {0.0, 10.0, 0.0};
Vector vel = {5.0, 0.0, 0.0};
Entity ball = new_entity("Ball", 1.0, 0.0, &pos, &vel, NULL, 0.8, true, false);
```

### Calculating Distance Between Entities
```c
double distance = get_euclidean_distance(&entity1, &entity2);
```

### Processing Collisions
```c
if (check_sphere_collision(&ball1, &ball2)) {
    resolve_sphere_collision(&ball1, &ball2);
}
```

### Using EntityManager
```cpp
EntityManager manager;
manager.addEntity("Particle", 1.0, 1.6e-19, {0, 0, 0}, {100, 0, 0});
manager.updateAllPhysics(0.001);
```

## Coordinate System

### 3D Cartesian Coordinates
- **X-axis**: Right direction (positive values)
- **Y-axis**: Up direction (positive values)
- **Z-axis**: Forward direction (positive values)

### Quaternion Orientation
- **Quaternion Format**: {w, x, y, z} where w is the scalar part
- **Identity Quaternion**: {1, 0, 0, 0} represents no rotation
- **Normalized**: All quaternions are automatically normalized

## Performance Considerations

### Memory Layout
- Vector and Quaternion types use simple structs for efficient memory layout
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
- Magnetic field calculations based on charge and velocity

## Error Handling

The entity system uses the ErrorCode enum defined in error_codes.h:

```c
typedef enum {
    SUCCESS = 0,
    ERROR_NULL_POINTER,
    ERROR_INVALID_PARAMETER,
    // ... other error codes
} ErrorCode;
```

Always check return values from setter functions to ensure operations completed successfully.

This documentation provides a comprehensive reference for working with the Entity system in the CPhysics engine.
