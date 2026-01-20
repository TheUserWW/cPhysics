#include "../include/collider.h"
#include <math.h>

void process_collision(Entity* obj_1, Entity* obj_2, double* loss) {

    if (!obj_1 || !obj_2 || obj_1->is_static && obj_2->is_static) {
        if (loss) *loss = 0.0;
        return;
    }
    
    // Calculate collision normal vector
    double normal[3];
    normal[0] = obj_2->position[0] - obj_1->position[0];
    normal[1] = obj_2->position[1] - obj_1->position[1];
    normal[2] = obj_2->position[2] - obj_1->position[2];
    
    double distance = sqrt(normal[0]*normal[0] + normal[1]*normal[1] + normal[2]*normal[2]);
    
    // If distance is zero, cannot calculate normal vector, return immediately
    if (distance == 0) {
        if (loss) *loss = 0.0;
        return;
    }
    
    // Normalize the normal vector
    normal[0] /= distance;
    normal[1] /= distance;
    normal[2] /= distance;
    
    // Handle static object collision
    if (obj_1->is_static || obj_2->is_static) {
        Entity* dynamic_obj = obj_1->is_static ? obj_2 : obj_1;
        Entity* static_obj = obj_1->is_static ? obj_1 : obj_2;
        
        // Position separation - prevent object overlap
        double separation_distance = 0.1; // Separation distance
        dynamic_obj->position[0] -= separation_distance * normal[0];
        dynamic_obj->position[1] -= separation_distance * normal[1];
        dynamic_obj->position[2] -= separation_distance * normal[2];
        
        double vn = dynamic_obj->velocity[0]*normal[0] + 
                   dynamic_obj->velocity[1]*normal[1] + 
                   dynamic_obj->velocity[2]*normal[2];
        
        double restitution = dynamic_obj->coefficient_of_restitution;
        double new_vn = -vn * restitution;
        
        if (loss) {
            *loss = 0.5 * dynamic_obj->mass * (vn*vn - new_vn*new_vn);
        }
        
        dynamic_obj->velocity[0] += (new_vn - vn) * normal[0];
        dynamic_obj->velocity[1] += (new_vn - vn) * normal[1];
        dynamic_obj->velocity[2] += (new_vn - vn) * normal[2];
        
        return;
    }
    
    // Calculate relative velocity
    double relative_velocity[3];
    relative_velocity[0] = obj_2->velocity[0] - obj_1->velocity[0];
    relative_velocity[1] = obj_2->velocity[1] - obj_1->velocity[1];
    relative_velocity[2] = obj_2->velocity[2] - obj_1->velocity[2];
    
    // Calculate normal velocity component
    double v_rel = relative_velocity[0]*normal[0] + 
                  relative_velocity[1]*normal[1] + 
                  relative_velocity[2]*normal[2];
    
    // If objects are separating, don't process collision
    if (v_rel > 0) {
        if (loss) *loss = 0.0;
        return;
    }
    
    // Position separation - prevent object overlap (reference to excellent C++ implementation)
    double separation_distance = 0.05; // Smaller separation distance
    double separation_factor_1 = obj_2->mass / (obj_1->mass + obj_2->mass);
    double separation_factor_2 = obj_1->mass / (obj_1->mass + obj_2->mass);
    
    obj_1->position[0] -= separation_distance * separation_factor_1 * normal[0];
    obj_1->position[1] -= separation_distance * separation_factor_1 * normal[1];
    obj_1->position[2] -= separation_distance * separation_factor_1 * normal[2];
    
    obj_2->position[0] += separation_distance * separation_factor_2 * normal[0];
    obj_2->position[1] += separation_distance * separation_factor_2 * normal[1];
    obj_2->position[2] += separation_distance * separation_factor_2 * normal[2];
    
    // Calculate coefficient of restitution (take the smaller value)
    double restitution = (obj_1->coefficient_of_restitution < obj_2->coefficient_of_restitution) ?
                        obj_1->coefficient_of_restitution : obj_2->coefficient_of_restitution;

    // More precise impulse calculation (reference to C++ code)
    double numerator = -(1.0 + restitution) * v_rel;
    double denominator = (1.0/obj_1->mass + 1.0/obj_2->mass);
    double impulse_magnitude = numerator / denominator;
    
    // Apply impulse
    double impulse[3];
    impulse[0] = impulse_magnitude * normal[0];
    impulse[1] = impulse_magnitude * normal[1];
    impulse[2] = impulse_magnitude * normal[2];

    if (loss) {
        double v1_before[3] = {obj_1->velocity[0], obj_1->velocity[1], obj_1->velocity[2]};
        double v2_before[3] = {obj_2->velocity[0], obj_2->velocity[1], obj_2->velocity[2]};

        double ke_before = 0.5 * obj_1->mass *
                          (v1_before[0]*v1_before[0] +
                           v1_before[1]*v1_before[1] +
                           v1_before[2]*v1_before[2]) +
                         0.5 * obj_2->mass *
                          (v2_before[0]*v2_before[0] +
                           v2_before[1]*v2_before[1] +
                           v2_before[2]*v2_before[2]);

        obj_1->velocity[0] -= impulse[0] / obj_1->mass;
        obj_1->velocity[1] -= impulse[1] / obj_1->mass;
        obj_1->velocity[2] -= impulse[2] / obj_1->mass;

        obj_2->velocity[0] += impulse[0] / obj_2->mass;
        obj_2->velocity[1] += impulse[1] / obj_2->mass;
        obj_2->velocity[2] += impulse[2] / obj_2->mass;

        double ke_after = 0.5 * obj_1->mass *
                         (obj_1->velocity[0]*obj_1->velocity[0] +
                          obj_1->velocity[1]*obj_1->velocity[1] +
                          obj_1->velocity[2]*obj_1->velocity[2]) +
                         0.5 * obj_2->mass *
                         (obj_2->velocity[0]*obj_2->velocity[0] +
                          obj_2->velocity[1]*obj_2->velocity[1] +
                          obj_2->velocity[2]*obj_2->velocity[2]);

        *loss = ke_before - ke_after;
    } else {
        obj_1->velocity[0] -= impulse[0] / obj_1->mass;
        obj_1->velocity[1] -= impulse[1] / obj_1->mass;
        obj_1->velocity[2] -= impulse[2] / obj_1->mass;

        obj_2->velocity[0] += impulse[0] / obj_2->mass;
        obj_2->velocity[1] += impulse[1] / obj_2->mass;
        obj_2->velocity[2] += impulse[2] / obj_2->mass;
    }
}
