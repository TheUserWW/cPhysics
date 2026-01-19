//
// Created by wcx16 on 2026/1/19.
//

#include "../include/plog.h"

char* show_entity_details(Entity* obj) {
    printf("=== Entity Details ===\n");
    printf("Name: %s\n", obj->name);
    printf("Mass: %f\n", obj->mass);
    printf("Charge: %f\n", obj->charge);
    printf("Position: x: %f, y: %f, z: %f\n", obj->position[0], obj->position[1], obj->position[2]);
    printf("Velocity: x: %f, y: %f, z: %f\n", obj->velocity[0], obj->velocity[1], obj->velocity[2]);
    printf("Acceleration: x: %f, y: %f, z: %f\n", obj->acceleration[0], obj->acceleration[1], obj->acceleration[2]);
    printf("Quaternion: w: %f, x: %f, y: %f, z: %f\n", obj->quaternion[0], obj->quaternion[1], obj->quaternion[2], obj->quaternion[3]);
    printf("Angular Velocity: x: %f, y: %f, z: %f\n", obj->angular_velocity[0], obj->angular_velocity[1], obj->angular_velocity[2]);
    printf("Angular Acceleration: x: %f, y: %f, z: %f\n", obj->angular_acceleration[0], obj->angular_acceleration[1], obj->angular_acceleration[2]);
    printf("Moment of Inertia: %f\n", obj->moment_of_inertia);
    printf("Coefficient of Restitution: %f\n", obj->coefficient_of_restitution);
    printf("Rigid Body: %s\n", obj->rigid_body ? "true" : "false");
    printf("Is Static: %s\n", obj->is_static ? "true" : "false");
    printf("Memory Address: %p\n", (void*)obj);
    printf("=====================\n");
    
    return (char*)obj;
}

char* LOG(const char* str) {
    printf("%s",str);
    return str;
}