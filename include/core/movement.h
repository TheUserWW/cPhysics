#ifndef CPHYSICS_MOVEMENT_H
#define CPHYSICS_MOVEMENT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "entity.h"

void apply_force(Entity* obj, const Vector* acceleration_vector);

void apply_electric_force(const Entity* obj_1, const Entity* obj_2);

void apply_universal_gravitation(Entity* obj_1, Entity* obj_2);


void quaternion_multiply(const double q1[4], const double q2[4], double result[4]);
void quaternion_conjugate(const double q[4], double result[4]);
void quaternion_normalize(double q[4]);
void axis_angle_to_quaternion(const Vector* axis, double angle, double q[4]);
void euler_to_quaternion(double pitch, double yaw, double roll, double q[4]);
void rotate_vector_by_quaternion(const Vector* v, const double q[4], Vector* result);
void update_quaternion_with_angular_velocity(double q[4], const Vector* omega, double dt);
void apply_torque(Entity* obj, const Vector* torque);
void update_rotation(Entity* obj, double dt);
void rotate_entity(Entity* obj, const Vector* axis, double angle);
#ifdef __cplusplus
}
#endif

#endif //CPHYSICS_MOVEMENT_H
