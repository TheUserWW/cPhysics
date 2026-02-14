#ifndef CPHYSICS_FIELD_H
#define CPHYSICS_FIELD_H

#ifdef __cplusplus
extern "C" {
#endif

#include "entity.h"
typedef struct electric_field {
    double magnitude;
    Vector direction;
}electric_field;


typedef struct gravitational_field {
    double magnitude;
    Vector direction;
}gravitational_field;

typedef struct magnetic_field {
    double magnitude;
    Vector direction;
    Vector position;
} magnetic_field;

typedef enum {
    FIELD_SUCCESS = 0,
    FIELD_ERROR_NULL_POINTER,
    FIELD_ERROR_INVALID_MASS,
    FIELD_ERROR_INVALID_CHARGE,
    FIELD_ERROR_STATIC_OBJECT
} FieldErrorCode;

FieldErrorCode apply_gravitational_field(Entity* obj, const gravitational_field* g);
FieldErrorCode apply_electric_field(Entity* obj, const electric_field* e);
FieldErrorCode apply_magnetic_field(Entity* obj, const magnetic_field* b);

#ifdef __cplusplus
}
#endif

#endif //CPHYSICS_FIELD_H
