#ifndef CPHYSICS_FIELD_H
#define CPHYSICS_FIELD_H
#include "entity.h"
typedef struct electric_field {
    double magnitude;
    double direction[3];
}electric_field;


typedef struct gravitational_field {
    double magnitude;
    double direction[3];
}gravitational_field;

typedef struct magnetic_field {
    double magnitude;
    double direction[3];
    double position[3];
}magnetic_field;

void apply_gravitational_field(Entity* obj, gravitational_field g);

#endif //CPHYSICS_FIELD_H