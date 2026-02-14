#include "../../include/core/field.h"
#include <math.h>
#include <float.h>

FieldErrorCode apply_gravitational_field(Entity* obj, const gravitational_field* g) {
    if (obj == NULL || g == NULL) return FIELD_ERROR_NULL_POINTER;
    if (obj->is_static) return FIELD_ERROR_STATIC_OBJECT;
    
    obj->acceleration.x += g->magnitude * g->direction.x;
    obj->acceleration.y += g->magnitude * g->direction.y;
    obj->acceleration.z += g->magnitude * g->direction.z;
    
    return FIELD_SUCCESS;
}

FieldErrorCode apply_electric_field(Entity* obj, const electric_field* e) {
    if (obj == NULL || e == NULL) return FIELD_ERROR_NULL_POINTER;
    if (obj->is_static) return FIELD_ERROR_STATIC_OBJECT;
    if (obj->mass < DBL_EPSILON) return FIELD_ERROR_INVALID_MASS;
    
    double force_magnitude = obj->charge * e->magnitude;
    obj->acceleration.x += (force_magnitude / obj->mass) * e->direction.x;
    obj->acceleration.y += (force_magnitude / obj->mass) * e->direction.y;
    obj->acceleration.z += (force_magnitude / obj->mass) * e->direction.z;
    
    return FIELD_SUCCESS;
}

FieldErrorCode apply_magnetic_field(Entity* obj, const magnetic_field* b) {
    if (obj == NULL || b == NULL) return FIELD_ERROR_NULL_POINTER;
    if (obj->is_static) return FIELD_ERROR_STATIC_OBJECT;
    if (obj->mass < DBL_EPSILON) return FIELD_ERROR_INVALID_MASS;
    if (fabs(obj->charge) < DBL_EPSILON) return FIELD_ERROR_INVALID_CHARGE;
    
    Vector cross_result = cross_product(obj->velocity, b->direction);
    
    double force_factor = (obj->charge * b->magnitude) / obj->mass;
    obj->acceleration.x += force_factor * cross_result.x;
    obj->acceleration.y += force_factor * cross_result.y;
    obj->acceleration.z += force_factor * cross_result.z;
    
    return FIELD_SUCCESS;
}
