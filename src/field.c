#include "../include/field.h"
#include <math.h>
#include <float.h>

/**
 * Apply gravitational field to an object
 * Calculate acceleration based on gravitational field strength and direction
 * Acceleration = gravitational field strength * direction vector
 * @param obj Target entity object
 * @param g Gravitational field parameters
 * @return FIELD_SUCCESS on success, error code on failure
 */
FieldErrorCode apply_gravitational_field(Entity* obj, const gravitational_field* g) {
    if (obj == NULL || g == NULL) return FIELD_ERROR_NULL_POINTER;
    if (obj->is_static) return FIELD_ERROR_STATIC_OBJECT;
    
    // Calculate gravitational acceleration: a = g * direction
    obj->acceleration[0] += g->magnitude * g->direction[0];
    obj->acceleration[1] += g->magnitude * g->direction[1];
    obj->acceleration[2] += g->magnitude * g->direction[2];
    
    return FIELD_SUCCESS;
}

/**
 * Apply electric field to a charged object
 * Calculate acceleration based on electric field strength and direction
 * Acceleration = (charge * electric field strength) / mass * direction vector
 * @param obj Target entity object
 * @param e Electric field parameters
 * @return FIELD_SUCCESS on success, error code on failure
 */
FieldErrorCode apply_electric_field(Entity* obj, const electric_field* e) {
    if (obj == NULL || e == NULL) return FIELD_ERROR_NULL_POINTER;
    if (obj->is_static) return FIELD_ERROR_STATIC_OBJECT;
    if (obj->mass < DBL_EPSILON) return FIELD_ERROR_INVALID_MASS;
    
    // Calculate acceleration from electric force: a = (q * E) / m * direction
    double force_magnitude = obj->charge * e->magnitude;
    obj->acceleration[0] += (force_magnitude / obj->mass) * e->direction[0];
    obj->acceleration[1] += (force_magnitude / obj->mass) * e->direction[1];
    obj->acceleration[2] += (force_magnitude / obj->mass) * e->direction[2];
    
    return FIELD_SUCCESS;
}

/**
 * Apply magnetic field to a charged object
 * Calculate acceleration from Lorentz force based on magnetic field strength and direction
 * Acceleration = (charge * (velocity × magnetic field)) / mass
 * @param obj Target entity object
 * @param b Magnetic field parameters
 * @return FIELD_SUCCESS on success, error code on failure
 */
FieldErrorCode apply_magnetic_field(Entity* obj, const magnetic_field* b) {
    if (obj == NULL || b == NULL) return FIELD_ERROR_NULL_POINTER;
    if (obj->is_static) return FIELD_ERROR_STATIC_OBJECT;
    if (obj->mass < DBL_EPSILON) return FIELD_ERROR_INVALID_MASS;
    if (fabs(obj->charge) < DBL_EPSILON) return FIELD_ERROR_INVALID_CHARGE;
    
    // Calculate cross product of velocity vector and magnetic field vector (v × B)
    double cross_product[3];
    cross_product[0] = obj->velocity[1] * b->direction[2] - obj->velocity[2] * b->direction[1];
    cross_product[1] = obj->velocity[2] * b->direction[0] - obj->velocity[0] * b->direction[2];
    cross_product[2] = obj->velocity[0] * b->direction[1] - obj->velocity[1] * b->direction[0];
    
    // Calculate acceleration from Lorentz force: a = (q * (v × B)) / m
    double force_factor = (obj->charge * b->magnitude) / obj->mass;
    obj->acceleration[0] += force_factor * cross_product[0];
    obj->acceleration[1] += force_factor * cross_product[1];
    obj->acceleration[2] += force_factor * cross_product[2];
    
    return FIELD_SUCCESS;
}