#include "../include/basic_obj/cylinder.h"
#include <stdlib.h>

Cylinder* new_cylinder(Entity e, double h, double r) {
    Cylinder* cy = malloc(sizeof(Cylinder));
    cy -> ent = e;
    cy -> height = h;
    cy -> radius = r;
    return cy;
}