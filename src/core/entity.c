#include "../../include/core/entity.h"
#include "../../include/plog.h"
#include <string.h>
#include <math.h>

struct Entity new_entity(const char* n, double m, double c,
                                   const Vector* d, const Vector* v,
                                   const Vector* a, double cor, bool rigid, bool s){
    struct Entity obj;

    strncpy(obj.name, n, 255);
    obj.name[255] = '\0';

    obj.mass = m;
    obj.charge = c;
    obj.is_static = s;
    obj.coefficient_of_restitution = cor;

    if (d) {
        obj.position = *d;
    } else {
        obj.position.x = 0.0;
        obj.position.y = 0.0;
        obj.position.z = 0.0;
    }

    if (v) {
        obj.velocity = *v;
    } else {
        obj.velocity.x = 0.0;
        obj.velocity.y = 0.0;
        obj.velocity.z = 0.0;
    }

    if (a) {
        obj.acceleration = *a;
    } else {
        obj.acceleration.x = 0.0;
        obj.acceleration.y = 0.0;
        obj.acceleration.z = 0.0;
    }

    obj.rigid_body = rigid;

    obj.quaternion[0] = 1.0;
    obj.quaternion[1] = 0.0;
    obj.quaternion[2] = 0.0;
    obj.quaternion[3] = 0.0;
    obj.angular_velocity.x = 0.0;
    obj.angular_velocity.y = 0.0;
    obj.angular_velocity.z = 0.0;
    obj.angular_acceleration.x = 0.0;
    obj.angular_acceleration.y = 0.0;
    obj.angular_acceleration.z = 0.0;
    obj.moment_of_inertia = 1.0;

    return obj;
}



Vector* get_position(Entity* obj) {return &obj->position;}
Vector* get_acceleration(Entity* obj) {return &obj->acceleration;}
Vector* get_velocity(Entity* obj) {return &obj->velocity;}


double get_euclidean_distance(const Entity* obj_1, const Entity* obj_2) {
    double dx = obj_1->position.x - obj_2->position.x,
           dy = obj_1->position.y - obj_2->position.y,
           dz = obj_1->position.z - obj_2->position.z;

    double euclidean_distance_squared = dx*dx + dy*dy + dz*dz;

    return sqrt(euclidean_distance_squared);
}

void get_linear_momentum(const Entity* obj, Vector* result) {
    if (obj && result) {
        result->x = obj->mass * obj->velocity.x;
        result->y = obj->mass * obj->velocity.y;
        result->z = obj->mass * obj->velocity.z;
    }
}


ErrorCode set_entity_position(Entity* obj, double x, double y, double z) {

    if (obj == NULL) {
        return OPERATION_SET_FAILED ;
    }

        obj->position.x = x;
        obj->position.y = y;
        obj->position.z = z;

    return OPERATION_SET_SUCCESS;
}

ErrorCode set_entity_velocity(Entity* obj, double x, double y, double z) {
    if (obj == NULL) {
        return OPERATION_SET_FAILED;
    }
        obj->velocity.x = x;
        obj->velocity.y = y;
        obj->velocity.z = z;

    return OPERATION_SET_SUCCESS;
}

ErrorCode set_entity_acceleration(Entity* obj, double x, double y, double z) {
    if (obj == NULL) {
        return OPERATION_SET_FAILED;
    }
        obj->acceleration.x = x;
        obj->acceleration.y = y;
        obj->acceleration.z = z;

    return OPERATION_SET_SUCCESS;
}

ErrorCode set_entity_angular_velocity(Entity* obj, double x, double y, double z) {
    if (obj == NULL) {
        return OPERATION_SET_FAILED;
    }
        obj->angular_velocity.x = x;
        obj->angular_velocity.y = y;
        obj->angular_velocity.z = z;

    return OPERATION_SET_SUCCESS;
}
ErrorCode set_entity_angular_acceleration(Entity* obj, double x, double y, double z) {
    if (obj == NULL) {
        return OPERATION_SET_FAILED;
    }
        obj->angular_acceleration.x = x;
        obj->angular_acceleration.y = y;
        obj->angular_acceleration.z = z;

    return OPERATION_SET_SUCCESS;
}



