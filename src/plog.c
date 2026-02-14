//
// Created by wcx16 on 2026/1/19.
//

#include "../include/plog.h"

char* show_entity_details(Entity* obj) {
    printf("=== Entity Details ===\n");
    printf("Name: %s\n", obj->name);
    printf("Mass: %f\n", obj->mass);
    printf("Charge: %f\n", obj->charge);
    printf("Position: x: %f, y: %f, z: %f\n", obj->position.x, obj->position.y, obj->position.z);
    printf("Velocity: x: %f, y: %f, z: %f\n", obj->velocity.x, obj->velocity.y, obj->velocity.z);
    printf("Acceleration: x: %f, y: %f, z: %f\n", obj->acceleration.x, obj->acceleration.y, obj->acceleration.z);
    printf("Quaternion: w: %f, x: %f, y: %f, z: %f\n", obj->quaternion[0], obj->quaternion[1], obj->quaternion[2], obj->quaternion[3]);
    printf("Angular Velocity: x: %f, y: %f, z: %f\n", obj->angular_velocity.x, obj->angular_velocity.y, obj->angular_velocity.z);
    printf("Angular Acceleration: x: %f, y: %f, z: %f\n", obj->angular_acceleration.x, obj->angular_acceleration.y, obj->angular_acceleration.z);
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

const char* get_error_description(ErrorCode error) {
    switch (error) {
        case OPERATION_SET_SUCCESS:
            return "Operation set successfully";
        case OPERATION_SET_FAILED:
            return "Operation set failed";
        case OPERATION_GET_SUCCESS:
            return "Operation get successfully";
        case OPERATION_GET_FAILED:
            return "Operation get failed";
        case ENTITY_VOID_ERROR:
            return "Entity is null pointer error";
        case ENTITY_UNFULLY_UNDEFINED:
            return "Entity not fully defined error";
        case DIVISION_BY_ZERO:
            return "Division by zero error";
        default:
            return "Unknown error";
    }
}
