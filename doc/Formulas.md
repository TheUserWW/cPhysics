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
- **Coulomb's Law**: F = k × (q₁ × q₂) / r²
    - k = 8.987551787 × 10⁹ N·m²/C² (Coulomb's constant)
- **Electric Field**: E = F / q

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
- **Gravitational Constant**: G = 6.67430 × 10⁻¹¹ N·m²/kg²
- **Coulomb's Constant**: k = 8.987551787 × 10⁹ N·m²/C²
- **Elementary Charge**: e = 1.602176634 × 10⁻¹⁹ C

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

### Rigid Body Properties
- **Moment of Inertia**: I = ∫r²dm (for continuous bodies)
- **Angular Momentum**: L = Iω
- **Rotational Kinetic Energy**: K_rot = ½Iω²

## Implementation Details

### Vector Operations
All 3D vector operations (position, velocity, acceleration) are implemented using array operations with proper dimensionality handling.

### Quaternion Implementation
The engine uses unit quaternions for orientation representation to avoid gimbal lock and ensure numerical stability during integration.

### Time Flow Integration
The time flow system allows for flexible time scaling, enabling both real-time and accelerated simulations with consistent physics behavior.

### Force Calculations
Forces are calculated using vector mathematics with proper unit handling and dimensional consistency.
- **Torque**: τ = r × F
- **Rotational Newton's Second Law**: τ = Iα
- **Moment of Inertia**: I = Σmr² (for discrete masses)

### Quaternion Operations (Implemented)
- **Quaternion Multiplication**: q₁q₂ = (w₁w₂ - v₁·v₂, w₁v₂ + w₂v₁ + v₁×v₂)
- **Quaternion Normalization**: q̂ = q / ||q||
- **Quaternion Integration**: dq/dt = ½ωq (for orientation updates)
- **Torque**: τ = r × F
- **Moment of Inertia**: I = Σmr²

### Quaternion Operations (Implemented)
- **Quaternion Multiplication**: q₁ ⊗ q₂
- **Quaternion Conjugate**: q* = [w, -x, -y, -z]
- **Quaternion Normalization**: q/‖q‖
- **Axis-Angle to Quaternion**: q = [cos(θ/2), axis × sin(θ/2)]
- **Euler Angles to Quaternion**: Conversion from pitch, yaw, roll
- **Vector Rotation**: v' = q ⊗ v ⊗ q*

## Electromagnetism

### Electrostatics (Implemented)
- **Coulomb's Law**: F = k × (q₁ × q₂) / r²
    - k = 8.987551787 × 10⁹ N·m²/C² (Coulomb's constant)
- **Electric Field**: E = F/q = k × q / r²
- **Electric Potential**: V = k × q / r

### Electromagnetic Waves
- **Speed of Light**: c = 3 × 10⁸ m/s

## Constants (Defined in constant.h)

### Fundamental Constants
- **π (Pi)**: 3.14159265358979323846
- **G (Gravitational Constant)**: 6.67430 × 10⁻¹¹ N·m²/kg²
- **k (Coulomb's Constant)**: 8.987551787 × 10⁹ N·m²/C²
- **c (Speed of Light)**: 3 × 10⁸ m/s

## Vector Operations

### 3D Vector Operations
- **Dot Product**: A·B = AₓBₓ + AᵧBᵧ + A₂B₂
- **Cross Product**: A×B = (AᵧB₂ - A₂Bᵧ, A₂Bₓ - AₓB₂, AₓBᵧ - AᵧBₓ)
- **Magnitude**: ‖A‖ = √(Aₓ² + Aᵧ² + A₂²)

### Vector Kinematics
- **Position**: r = (x, y, z)
- **Velocity**: v = dr/dt = (vₓ, vᵧ, v₂)
- **Acceleration**: a = dv/dt = (aₓ, aᵧ, a₂)

## Energy and Work

### Mechanical Energy
- **Kinetic Energy**: K = ½mv²
- **Potential Energy**: U = mgh (gravitational near Earth)
- **Work**: W = F·d = Fd cosθ
- **Work-Energy Theorem**: W = ΔK

### Power
- **Average Power**: P = W/Δt
- **Instantaneous Power**: P = F·v

## Implementation Notes

### Entity Structure (from entity.h)
Entities in CPhysics contain:
- Mass (m)
- Charge (q)
- Position vector (x, y, z)
- Velocity vector (vₓ, vᵧ, v₂)
- Acceleration vector (aₓ, aᵧ, a₂)

### Force Application
- **apply_force()**: Direct force application using F = ma
- **apply_electric_force()**: Implements Coulomb's Law
- **apply_universal_gravitation()**: Implements Newton's Law of Universal Gravitation

### Coordinate Systems
The engine supports:
- Cartesian coordinates (x, y, z)
- Quaternion rotations for 3D orientation
- Euler angles (pitch, yaw, roll) conversion

## Units and Conventions

### SI Units Used
- **Length**: meters (m)
- **Mass**: kilograms (kg)
- **Time**: seconds (s)
- **Charge**: coulombs (C)
- **Force**: newtons (N)
- **Energy**: joules (J)

### Coordinate System
- Right-handed coordinate system
- Positive x: right
- Positive y: up
- Positive z: forward

## Useful Derivations

### Orbital Mechanics
- **Circular Orbit Velocity**: v = √(GM/r)
- **Escape Velocity**: vₑ = √(2GM/r)

### Projectile Motion
- **Range**: R = (v₀² sin 2θ)/g
- **Maximum Height**: h = (v₀² sin² θ)/(2g)
- **Time of Flight**: T = (2v₀ sin θ)/g

This formula table covers the core physics principles implemented in the CPhysics engine and provides a comprehensive reference for physical calculations.