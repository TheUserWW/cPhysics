#include "../include/basic_obj/sphere.h"
#include <stdlib.h>
Sphere* new_sphere(const Entity e, const double r){
    Sphere* ball = (malloc(sizeof(Sphere)));
    ball -> ent = e;
    ball -> radius = r;
    return ball;
}