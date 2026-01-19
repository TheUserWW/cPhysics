#ifndef CPHYSICS_ENTITY_H
#define CPHYSICS_ENTITY_H
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "constant.h"




typedef struct Entity {
    char name[256];
    double mass;
    double charge;
    double position[3];
    double velocity[3];
    double acceleration[3];
    double quaternion[4];
    double angular_velocity[3];
    double angular_acceleration[3];
    double moment_of_inertia;
    double coefficient_of_restitution;
    bool rigid_body;
    bool is_static;
} Entity;


struct Entity new_entity(const char* n, double m, double c,
                                   const double d[3], const double v[3],
                                   const double a[3], double cor, bool rigid, bool s);

//get functions
inline double* get_position(Entity* obj);
inline double* get_acceleration(Entity* obj);
inline double* get_velocity(Entity* obj);
double get_euclidean_distance(const Entity* obj_1, const Entity* obj_2);
void get_linear_momentum(const Entity* obj, double result[3]);

//set functions
int set_entity_position(Entity* obj, double x, double y, double z);
int set_entity_velocity(Entity* obj, double x, double y, double z);
int set_entity_acceleration(Entity* obj, double x, double y, double z);
int set_entity_angular_velocity(Entity* obj, double x, double y, double z);
int set_entity_angular_acceleration(Entity* obj, double x, double y, double z);



void process_collision(Entity* obj_1, Entity* obj_2, double* loss);


#endif //CPHYSICS_ENTITY_H