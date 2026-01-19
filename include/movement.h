#ifndef CPHYSICS_MOVEMENT_H
#define CPHYSICS_MOVEMENT_H
#include "../include/entity.h"

//Just impose a arbitrary for an object
void apply_force(Entity* obj, const double* acceleration_vector);

//According to the Coulomb's law F = (k*q1*q2)/r^2
void apply_electric_force(const Entity* obj_1, const Entity* obj_2);

//F = (G*m1*m2)r^2
void apply_universal_gravitation(Entity* obj_1, Entity* obj_2);


void quaternion_multiply(const double q1[4], const double q2[4], double result[4]);
void quaternion_conjugate(const double q[4], double result[4]);
void quaternion_normalize(double q[4]);
void axis_angle_to_quaternion(const double axis[3], double angle, double q[4]);
void euler_to_quaternion(double pitch, double yaw, double roll, double q[4]);
void rotate_vector_by_quaternion(const double v[3], const double q[4], double result[3]);
void update_quaternion_with_angular_velocity(double q[4], const double omega[3], double dt);
void apply_torque(Entity* obj, const double torque[3]);
void update_rotation(Entity* obj, double dt);
void rotate_entity(Entity* obj, const double axis[3], double angle);
#endif //CPHYSICS_MOVEMENT_H