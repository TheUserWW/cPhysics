#ifndef CPHYSICS_CYLINDER_H
#define CPHYSICS_CYLINDER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../core/entity.h"
typedef struct Cylinder {
    Entity ent;
    double height;
    double radius;
}Cylinder;

Cylinder* new_cylinder(Entity e, double h, double r);

#ifdef __cplusplus
}
#endif

#endif //CPHYSICS_CYLINDER_H