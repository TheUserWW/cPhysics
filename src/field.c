#include "../include/field.h"

void apply_gravitational_field(Entity* obj, gravitational_field g) {
    obj -> acceleration[0] += g.direction[0];
    obj -> acceleration[1] += g.direction[1];
    obj -> acceleration[2] += g.direction[2];
}