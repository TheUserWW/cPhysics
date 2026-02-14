#include "../../include/core/collider.h"
#include <math.h>

void process_collision(Entity* obj_1, Entity* obj_2, double* loss) {

    if (!obj_1 || !obj_2 || obj_1->is_static && obj_2->is_static) {
        if (loss) *loss = 0.0;
        return;
    }
    
    Vector normal;
    normal.x = obj_2->position.x - obj_1->position.x;
    normal.y = obj_2->position.y - obj_1->position.y;
    normal.z = obj_2->position.z - obj_1->position.z;
    
    double distance = normalize(normal);
    
    if (distance == 0) {
        if (loss) *loss = 0.0;
        return;
    }
    
    normal.x /= distance;
    normal.y /= distance;
    normal.z /= distance;
    
    if (obj_1->is_static || obj_2->is_static) {
        Entity* dynamic_obj = obj_1->is_static ? obj_2 : obj_1;
        Entity* static_obj = obj_1->is_static ? obj_1 : obj_2;
        
        double separation_distance = 0.1;
        dynamic_obj->position.x -= separation_distance * normal.x;
        dynamic_obj->position.y -= separation_distance * normal.y;
        dynamic_obj->position.z -= separation_distance * normal.z;
        
        double vn = dynamic_obj->velocity.x * normal.x + 
                   dynamic_obj->velocity.y * normal.y + 
                   dynamic_obj->velocity.z * normal.z;
        
        double restitution = dynamic_obj->coefficient_of_restitution;
        double new_vn = -vn * restitution;
        
        if (loss) {
            *loss = 0.5 * dynamic_obj->mass * (vn*vn - new_vn*new_vn);
        }
        
        dynamic_obj->velocity.x += (new_vn - vn) * normal.x;
        dynamic_obj->velocity.y += (new_vn - vn) * normal.y;
        dynamic_obj->velocity.z += (new_vn - vn) * normal.z;
        
        return;
    }
    
    Vector relative_velocity;
    relative_velocity.x = obj_2->velocity.x - obj_1->velocity.x;
    relative_velocity.y = obj_2->velocity.y - obj_1->velocity.y;
    relative_velocity.z = obj_2->velocity.z - obj_1->velocity.z;
    
    double v_rel = relative_velocity.x * normal.x + 
                  relative_velocity.y * normal.y + 
                  relative_velocity.z * normal.z;
    
    if (v_rel > 0) {
        if (loss) *loss = 0.0;
        return;
    }
    
    double separation_distance = 0.05;
    double separation_factor_1 = obj_2->mass / (obj_1->mass + obj_2->mass);
    double separation_factor_2 = obj_1->mass / (obj_1->mass + obj_2->mass);
    
    obj_1->position.x -= separation_distance * separation_factor_1 * normal.x;
    obj_1->position.y -= separation_distance * separation_factor_1 * normal.y;
    obj_1->position.z -= separation_distance * separation_factor_1 * normal.z;
    
    obj_2->position.x += separation_distance * separation_factor_2 * normal.x;
    obj_2->position.y += separation_distance * separation_factor_2 * normal.y;
    obj_2->position.z += separation_distance * separation_factor_2 * normal.z;
    
    double restitution = (obj_1->coefficient_of_restitution < obj_2->coefficient_of_restitution) ?
                        obj_1->coefficient_of_restitution : obj_2->coefficient_of_restitution;

    double numerator = -(1.0 + restitution) * v_rel;
    double denominator = (1.0/obj_1->mass + 1.0/obj_2->mass);
    double impulse_magnitude = numerator / denominator;
    
    Vector impulse;
    impulse.x = impulse_magnitude * normal.x;
    impulse.y = impulse_magnitude * normal.y;
    impulse.z = impulse_magnitude * normal.z;

    if (loss) {
        Vector v1_before = obj_1->velocity;
        Vector v2_before = obj_2->velocity;

        double ke_before = 0.5 * obj_1->mass *
                          (v1_before.x*v1_before.x +
                           v1_before.y*v1_before.y +
                           v1_before.z*v1_before.z) +
                         0.5 * obj_2->mass *
                          (v2_before.x*v2_before.x +
                           v2_before.y*v2_before.y +
                           v2_before.z*v2_before.z);

        obj_1->velocity.x -= impulse.x / obj_1->mass;
        obj_1->velocity.y -= impulse.y / obj_1->mass;
        obj_1->velocity.z -= impulse.z / obj_1->mass;

        obj_2->velocity.x += impulse.x / obj_2->mass;
        obj_2->velocity.y += impulse.y / obj_2->mass;
        obj_2->velocity.z += impulse.z / obj_2->mass;

        double ke_after = 0.5 * obj_1->mass *
                         (obj_1->velocity.x*obj_1->velocity.x +
                          obj_1->velocity.y*obj_1->velocity.y +
                          obj_1->velocity.z*obj_1->velocity.z) +
                         0.5 * obj_2->mass *
                         (obj_2->velocity.x*obj_2->velocity.x +
                          obj_2->velocity.y*obj_2->velocity.y +
                          obj_2->velocity.z*obj_2->velocity.z);

        *loss = ke_before - ke_after;
    } else {
        obj_1->velocity.x -= impulse.x / obj_1->mass;
        obj_1->velocity.y -= impulse.y / obj_1->mass;
        obj_1->velocity.z -= impulse.z / obj_1->mass;

        obj_2->velocity.x += impulse.x / obj_2->mass;
        obj_2->velocity.y += impulse.y / obj_2->mass;
        obj_2->velocity.z += impulse.z / obj_2->mass;
    }
}
