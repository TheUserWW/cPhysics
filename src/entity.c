#include "../include/entity.h"
#include <string.h>
#include <math.h>

struct Entity new_entity(const char* n, double m, double c,
                                   const double d[3], const double v[3],
                                   const double a[3], double cor, bool rigid, bool s){
    struct Entity obj;

    strncpy(obj.name, n, 255);
    obj.name[255] = '\0';

    obj.mass = m;
    obj.charge = c;
    obj.is_static = s;
    obj.coefficient_of_restitution = cor;
    // 复制数组内容
    if (d) {
        memcpy(obj.position, d, 3 * sizeof(double));
    } else {
        memset(obj.position, 0, 3 * sizeof(double));
    }

    if (v) {
        memcpy(obj.velocity, v, 3 * sizeof(double));
    } else {
        memset(obj.velocity, 0, 3 * sizeof(double));
    }

    if (a) {
        memcpy(obj.acceleration, a, 3 * sizeof(double));
    } else {
        memset(obj.acceleration, 0, 3 * sizeof(double));
    }

    obj.rigid_body = rigid;

    obj.quaternion[0] = 1.0;
    obj.quaternion[1] = 0.0;
    obj.quaternion[2] = 0.0;
    obj.quaternion[3] = 0.0;
    memset(obj.angular_velocity, 0, 3 * sizeof(double));
    memset(obj.angular_acceleration, 0, 3 * sizeof(double));
    obj.moment_of_inertia = 1.0;

    return obj;
}

inline double* get_position(Entity* obj) {return obj -> position;}
inline double* get_acceleration(Entity* obj) {return obj -> acceleration;}
inline double* get_velocity(Entity* obj) {return obj -> velocity;}  


double get_euclidean_distance(const Entity* obj_1, const Entity* obj_2) {
    double dx = obj_1->position[0] - obj_2->position[0],
           dy = obj_1->position[1] - obj_2->position[1],
           dz = obj_1->position[2] - obj_2->position[2];

    double euclidean_distance_squared = dx*dx + dy*dy + dz*dz;

    return sqrt(euclidean_distance_squared);
}

void get_linear_momentum(const Entity* obj, double result[3]) {
    if (obj && result) {
        result[0] = obj->mass * obj->velocity[0];
        result[1] = obj->mass * obj->velocity[1];
        result[2] = obj->mass * obj->velocity[2];
    }
}


void process_collision(Entity* obj_1, Entity* obj_2, double* loss) {
    // P = Σmv
    double p1[3], p2[3];
    get_linear_momentum(obj_1, p1);
    get_linear_momentum(obj_2, p2);

    //TODO Sol the momentum
}





