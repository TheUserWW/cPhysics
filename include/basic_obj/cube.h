
#ifndef CPHYSICS_CUBE_H
#define CPHYSICS_CUBE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "../core/entity.h"
typedef struct Cube {
    Entity ent;
    double height;
    double width;
}Cube;

#ifdef __cplusplus
}
#endif

#endif //CPHYSICS_CUBE_H