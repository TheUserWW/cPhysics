# Physics Formulas Reference

## Classical Mechanics

### Kinematics
- **Displacement**: Δx = x₂ - x₁
- **Average Velocity**: v̄ = Δx / Δt
- **Instantaneous Velocity**: v = dx/dt
- **Average Acceleration**: ā = Δv / Δt
- **Instantaneous Acceleration**: a = dv/dt

### Constant Acceleration Equations
- **Velocity**: v = v₀ + at
- **Displacement**: x = x₀ + v₀t + ½at²
- **Velocity-Displacement**: v² = v₀² + 2a(x - x₀)
- **Average Velocity**: v̄ = (v₀ + v)/2

### Newton's Laws
- **First Law**: ΣF = 0 ⇒ a = 0
- **Second Law**: F = ma
- **Third Law**: F₁₂ = -F₂₁

### Universal Gravitation (Implemented)
- **Gravitational Force**: F = G × (m₁ × m₂) / r²
    - G = 6.67430 × 10⁻¹¹ N·m²/kg² (Gravitational constant)
- **Gravitational Potential Energy**: U = -G × (m₁ × m₂) / r

### Electrostatics (Implemented)
- **Coulomb's Law**: F = K × (q₁ × q₂) / r²
    - K = 8.987551787 × 10⁹ N·m²/C² (Coulomb's constant)
- **Electric Field**: E = F / q

### Magnetic Force (Implemented)
- **Lorentz Force**: F = q(v × B)
- **Magnetic Force Magnitude**: F = qvB sin(θ)

## Numerical Integration Methods (Implemented)

### Euler Integration
- **Position Update**: x(t+Δt) = x(t) + v(t)Δt
- **Velocity Update**: v(t+Δt) = v(t) + a(t)Δt
- **Orientation Update**: q(t+Δt) = q(t) + ½ω(t)q(t)Δt (quaternion integration)

### Time Integration
- **Simulation Time**: t_simulation = t_real × time_scale + time_offset
- **Time Step Control**: Δt = time_step × time_scale

## Physical Constants (Implemented)

### Fundamental Constants
Defined in `include/constant.h`:
```c
#define PI 3.14159265358979323846f
#define G 6.67430e-11f      // Gravitational constant (N·m²/kg²)
#define K 8.987551787e9f    // Coulomb's constant (N·m²/C²)
#define SPEED_OF_LIGHT 3e8  // Speed of light (m/s)
```

### Astronomical Constants
- **Earth Mass**: M_earth = 5.972 × 10²⁴ kg
- **Moon Mass**: M_moon = 7.348 × 10²² kg
- **Earth-Moon Distance**: d_earth_moon = 3.844 × 10⁸ m
- **Earth Rotation Rate**: ω_earth = 7.292 × 10⁻⁵ rad/s

## Collision Physics (Implemented)

### Elastic Collisions
- **Coefficient of Restitution**: e = (v₂' - v₁') / (v₁ - v₂)
- **Velocity After Collision**: 
  - v₁' = (m₁ - e·m₂)v₁/(m₁ + m₂) + (1 + e)m₂v₂/(m₁ + m₂)
  - v₂' = (1 + e)m₁v₁/(m₁ + m₂) + (m₂ - e·m₁)v₂/(m₁ + m₂)

### Impulse-Based Collision Response
- **Impulse Magnitude**: J = -(1 + e) × v_rel · n / (1/m₁ + 1/m₂)
- **Velocity Update**: 
  - v₁' = v₁ - (J/m₁) × n
  - v₂' = v₂ + (J/m₂) × n

### Rigid Body Properties
- **Moment of Inertia**: I = ∫r²dm (for continuous bodies)
- **Angular Momentum**: L = Iω
- **Rotational Kinetic Energy**: K_rot = ½Iω²

## Vector Operations (Implemented)

### Vector Structure
```c
typedef struct Vector {
    double x;
    double y;
    double z;
} Vector;
```

### 3D Vector Operations
- **Dot Product**: A·B = AₓBₓ + AᵧBᵧ + A_zB_z
- **Cross Product**: A×B = (AᵧB_z - A_zBᵧ, A_zBₓ - AₓB_z, AₓBᵧ - AᵧBₓ)
- **Magnitude**: ‖A‖ = √(Aₓ² + Aᵧ² + A_z²)

### Vector Functions
```c
double dot_product(const Vector a, const Vector b);
Vector cross_product(const Vector a, const Vector b);
double normalize(const Vector a);
```

## Quaternion Operations (Implemented)

### Quaternion Structure
```c
typedef struct {
    float w;  // Scalar (real) component
    float x;  // i-component
    float y;  // j-component
    float z;  // k-component
} Quaternion;
```

### Quaternion Mathematics
- **Quaternion Multiplication**: q₁q₂ = (w₁w₂ - v₁·v₂, w₁v₂ + w₂v₁ + v₁×v₂)
- **Quaternion Conjugate**: q* = [w, -x, -y, -z]
- **Quaternion Normalization**: q̂ = q / ‖q‖
- **Quaternion Integration**: dq/dt = ½ωq (for orientation updates)

### Quaternion Functions
```c
Quaternion quat_identity();
Quaternion quat_create(float w, float x, float y, float z);
Quaternion quat_from_axis_angle(float angle, float axis_x, float axis_y, float axis_z);
Quaternion quat_from_euler(float yaw, float pitch, float roll);
Quaternion quat_normalize(Quaternion q);
Quaternion quat_multiply(Quaternion q1, Quaternion q2);
Quaternion quat_conjugate(Quaternion q);
Quaternion quat_inverse(Quaternion q);
void quat_rotate_vector(Quaternion q, float vx, float vy, float vz, 
                       float* out_x, float* out_y, float* out_z);
void quat_to_axis_angle(Quaternion q, float* angle, 
                       float* axis_x, float* axis_y, float* axis_z);
void quat_to_euler(Quaternion q, float* yaw, float* pitch, float* roll);
Quaternion quat_lerp(Quaternion q1, Quaternion q2, float t);
Quaternion quat_slerp(Quaternion q1, Quaternion q2, float t);
float quat_dot(Quaternion q1, Quaternion q2);
float quat_magnitude(Quaternion q);
```

### Rotation Conversions
- **Axis-Angle to Quaternion**: q = [cos(θ/2), axis × sin(θ/2)]
- **Euler Angles to Quaternion**: Conversion from pitch, yaw, roll
- **Vector Rotation**: v' = q ⊗ v ⊗ q*

## Electromagnetism

### Electrostatics (Implemented)
- **Coulomb's Law**: F = K × (q₁ × q₂) / r²
    - K = 8.987551787 × 10⁹ N·m²/C² (Coulomb's constant)
- **Electric Field**: E = F/q = K × q / r²
- **Electric Potential**: V = K × q / r

### Magnetic Fields (Implemented)
- **Lorentz Force**: F = q(E + v × B)
- **Magnetic Force Only**: F = q(v × B)
- **Cyclotron Radius**: r = mv / (qB)

### Electromagnetic Waves
- **Speed of Light**: c = 3 × 10⁸ m/s

## Energy and Work

### Mechanical Energy
- **Kinetic Energy**: K = ½mv²
- **Potential Energy**: U = mgh (gravitational near Earth)
- **Work**: W = F·d = Fd cosθ
- **Work-Energy Theorem**: W = ΔK

### Power
- **Average Power**: P = W/Δt
- **Instantaneous Power**: P = F·v

## Rotational Dynamics

### Torque and Angular Motion
- **Torque**: τ = r × F
- **Angular Momentum**: L = Iω
- **Rotational Newton's Second Law**: τ = Iα
- **Moment of Inertia**: I = Σmr² (for discrete masses)

### Rotational Energy
- **Rotational Kinetic Energy**: K_rot = ½Iω²
- **Angular Work**: W = τθ

## Units and Conventions

### SI Units Used
- **Length**: meters (m)
- **Mass**: kilograms (kg)
- **Time**: seconds (s)
- **Charge**: coulombs (C)
- **Force**: newtons (N)
- **Energy**: joules (J)
- **Magnetic Field**: tesla (T)
- **Electric Field**: newtons per coulomb (N/C)

### Coordinate System
- Right-handed coordinate system
- Positive x: right
- Positive y: up
- Positive z: forward

## Useful Derivations

### Orbital Mechanics
- **Circular Orbit Velocity**: v = √(GM/r)
- **Escape Velocity**: vₑ = √(2GM/r)
- **Orbital Period**: T = 2π√(r³/GM)

### Projectile Motion
- **Range**: R = (v₀² sin 2θ)/g
- **Maximum Height**: h = (v₀² sin² θ)/(2g)
- **Time of Flight**: T = (2v₀ sin θ)/g

## Implementation Notes

### Entity Structure (from entity.h)
Entities in CPhysics use Vector and Quaternion types:
```c
typedef struct Entity {
    char name[256];
    double mass;
    double charge;
    Vector position;
    Vector velocity;
    Vector acceleration;
    Quaternion quaternion;
    Vector angular_velocity;
    Vector angular_acceleration;
    double moment_of_inertia;
    double coefficient_of_restitution;
    bool rigid_body;
    bool is_static;
} Entity;
```

### Force Application
- **apply_force()**: Direct force application using F = ma
- **apply_electric_force()**: Implements Coulomb's Law
- **apply_universal_gravitation()**: Implements Newton's Law of Universal Gravitation

### Field Application
- **apply_gravitational_field()**: Uniform gravitational field
- **apply_electric_field()**: Uniform electric field
- **apply_magnetic_field()**: Magnetic field using Lorentz force

### Collision Detection
- **check_sphere_collision()**: Sphere-sphere collision detection
- **resolve_sphere_collision()**: Impulse-based collision response

This formula reference covers the core physics principles implemented in the CPhysics engine and provides a comprehensive reference for physical calculations.
