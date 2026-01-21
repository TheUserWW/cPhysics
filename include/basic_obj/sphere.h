#ifndef CPHYSICS_SPHERE_H
#define CPHYSICS_SPHERE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../entity.h"
typedef struct Sphere {
    Entity ent;
    double radius;
}Sphere;

Sphere* new_sphere(const Entity e, const double r);

ErrorCode sphere_draw_basic(Sphere s);

#ifdef __cplusplus
}
#endif

#endif //CPHYSICS_SPHERE_H