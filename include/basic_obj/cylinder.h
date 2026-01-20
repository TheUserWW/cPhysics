#ifndef CPHYSICS_CYLINDER_H
#define CPHYSICS_CYLINDER_H
#include "../entity.h"
typedef struct Cylinder {
    Entity ent;
    double height;
    double radius;
}Cylinder;

Cylinder* new_cylinder(Entity e, double h, double r);

#endif //CPHYSICS_CYLINDER_H