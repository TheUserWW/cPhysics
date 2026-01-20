#ifndef CPHYSICS_SPHERE_H
#define CPHYSICS_SPHERE_H
#include "../entity.h"
typedef struct Sphere {
    Entity ent;
    double radius;
}Sphere;

Sphere* new_sphere(const Entity e, const double r);

#endif //CPHYSICS_SPHERE_H