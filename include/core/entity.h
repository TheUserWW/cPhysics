#ifndef CPHYSICS_ENTITY_H
#define CPHYSICS_ENTITY_H

#ifdef __cplusplus
extern "C" {
#endif

#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "../constant.h"
#include "../error_codes.h"
#include "../mathlib/Vector.h"




typedef struct Entity {
    char name[256];
    double mass;
    double charge;
    Vector position;
    Vector velocity;
    Vector acceleration;
    double quaternion[4];
    Vector angular_velocity;
    Vector angular_acceleration;
    double moment_of_inertia;
    double coefficient_of_restitution;
    bool rigid_body;
    bool is_static;
} Entity;


struct Entity new_entity(const char* n, double m, double c,
                                   const Vector* d, const Vector* v,
                                   const Vector* a, double cor, bool rigid, bool s);

Vector* get_position(Entity* obj);
Vector* get_acceleration(Entity* obj);
Vector* get_velocity(Entity* obj);
double get_euclidean_distance(const Entity* obj_1, const Entity* obj_2);
void get_linear_momentum(const Entity* obj, Vector* result);

ErrorCode set_entity_position(Entity* obj, double x, double y, double z);
ErrorCode set_entity_velocity(Entity* obj, double x, double y, double z);
ErrorCode set_entity_acceleration(Entity* obj, double x, double y, double z);
ErrorCode set_entity_angular_velocity(Entity* obj, double x, double y, double z);
ErrorCode set_entity_angular_acceleration(Entity* obj, double x, double y, double z);


#ifdef __cplusplus
}
#endif

#endif //CPHYSICS_ENTITY_H
