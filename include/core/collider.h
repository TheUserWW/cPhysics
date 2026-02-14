#ifndef CPHYSICS_COLLIDER_H
#define CPHYSICS_COLLIDER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "entity.h"

/**
 * @brief Process collision between two entities
 * 
 * Handles elastic collisions and static object collisions, including position separation and velocity updates
 * 
 * @param obj_1 First entity
 * @param obj_2 Second entity
 * @param loss Energy loss pointer (optional, used to store energy loss during collision)
 */
void process_collision(Entity* obj_1, Entity* obj_2, double* loss);

#ifdef __cplusplus
}
#endif

#endif //CPHYSICS_COLLIDER_H