#include "../include/field.h"
#include <math.h>

/**
 * Apply gravitational field to an object
 * Calculate acceleration based on gravitational field strength and direction
 * Acceleration = gravitational field strength * direction vector
 */
void apply_gravitational_field(Entity* obj, gravitational_field g) {
    if (obj == NULL || obj->is_static) return;
    
    // Calculate gravitational acceleration: a = g * direction
    obj->acceleration[0] += g.magnitude * g.direction[0];
    obj->acceleration[1] += g.magnitude * g.direction[1];
    obj->acceleration[2] += g.magnitude * g.direction[2];
}

/**
 * Apply electric field to a charged object
 * Calculate acceleration based on electric field strength and direction
 * Acceleration = (charge * electric field strength) / mass * direction vector
 */
void apply_electric_field(Entity* obj, electric_field e) {
    if (obj == NULL || obj->is_static || obj->mass == 0) return;
    
    // Calculate acceleration from electric force: a = (q * E) / m * direction
    double force_magnitude = obj->charge * e.magnitude;
    obj->acceleration[0] += (force_magnitude / obj->mass) * e.direction[0];
    obj->acceleration[1] += (force_magnitude / obj->mass) * e.direction[1];
    obj->acceleration[2] += (force_magnitude / obj->mass) * e.direction[2];
}

/**
 * Apply magnetic field to a charged object
 * Calculate acceleration from Lorentz force based on magnetic field strength and direction
 * Acceleration = (charge * (velocity × magnetic field)) / mass
 */
void apply_magnetic_field(Entity* obj, magnetic_field b) {
    if (obj == NULL || obj->is_static || obj->mass == 0 || obj->charge == 0) return;
    
    // Calculate cross product of velocity vector and magnetic field vector (v × B)
    double cross_product[3];
    cross_product[0] = obj->velocity[1] * b.direction[2] - obj->velocity[2] * b.direction[1];
    cross_product[1] = obj->velocity[2] * b.direction[0] - obj->velocity[0] * b.direction[2];
    cross_product[2] = obj->velocity[0] * b.direction[1] - obj->velocity[1] * b.direction[0];
    
    // Calculate acceleration from Lorentz force: a = (q * (v × B)) / m
    double force_factor = (obj->charge * b.magnitude) / obj->mass;
    obj->acceleration[0] += force_factor * cross_product[0];
    obj->acceleration[1] += force_factor * cross_product[1];
    obj->acceleration[2] += force_factor * cross_product[2];
}