#ifndef CPHYSICS_MOVEMENT_H
#define CPHYSICS_MOVEMENT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "entity.h"

void apply_force(Entity* obj, const Vector* acceleration_vector);
void apply_electric_force(Entity* obj_1, Entity* obj_2);
void apply_universal_gravitation(Entity* obj_1, Entity* obj_2);


#include "../mathlib/Quaternion.h"

Quaternion quaternion_multiply(Quaternion q1, Quaternion q2);
Quaternion quaternion_conjugate(Quaternion q);
Quaternion quaternion_normalize(Quaternion q);
Quaternion axis_angle_to_quaternion(const Vector* axis, double angle);
Quaternion euler_to_quaternion(double pitch, double yaw, double roll);
void rotate_vector_by_quaternion(const Vector* v, Quaternion q, Vector* result);
void update_quaternion_with_angular_velocity(Quaternion* q, const Vector* omega, double dt);
void apply_torque(Entity* obj, const Vector* torque);
void update_rotation(Entity* obj, double dt);
void rotate_entity(Entity* obj, const Vector* axis, double angle);

// Collision detection and response functions
double get_entity_radius(const Entity* entity);
void set_entity_radius(Entity* entity, double radius);
int check_sphere_collision(const Entity* obj_1, const Entity* obj_2);
void resolve_sphere_collision(Entity* obj_1, Entity* obj_2);
void apply_collision_response(Entity* entities, int count);
#ifdef __cplusplus
}
#endif

#endif //CPHYSICS_MOVEMENT_H
