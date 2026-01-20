#include "../include/entity.h"
#include "../include/plog.h"
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

    if (!obj_1 || !obj_2 || obj_1->is_static && obj_2->is_static) {
        if (loss) *loss = 0.0;
        return;
    }
    
    // 计算碰撞法向量
    double normal[3];
    normal[0] = obj_2->position[0] - obj_1->position[0];
    normal[1] = obj_2->position[1] - obj_1->position[1];
    normal[2] = obj_2->position[2] - obj_1->position[2];
    
    double distance = sqrt(normal[0]*normal[0] + normal[1]*normal[1] + normal[2]*normal[2]);
    
    // 如果距离为0，无法计算法向量，直接返回
    if (distance == 0) {
        if (loss) *loss = 0.0;
        return;
    }
    
    // 归一化法向量
    normal[0] /= distance;
    normal[1] /= distance;
    normal[2] /= distance;
    
    // 处理静态物体碰撞
    if (obj_1->is_static || obj_2->is_static) {
        Entity* dynamic_obj = obj_1->is_static ? obj_2 : obj_1;
        Entity* static_obj = obj_1->is_static ? obj_1 : obj_2;
        
        // 位置分离 - 防止物体重叠
        double separation_distance = 0.1; // 分离距离
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
    
    // 计算相对速度
    double relative_velocity[3];
    relative_velocity[0] = obj_2->velocity[0] - obj_1->velocity[0];
    relative_velocity[1] = obj_2->velocity[1] - obj_1->velocity[1];
    relative_velocity[2] = obj_2->velocity[2] - obj_1->velocity[2];
    
    // 计算法向速度分量
    double v_rel = relative_velocity[0]*normal[0] + 
                  relative_velocity[1]*normal[1] + 
                  relative_velocity[2]*normal[2];
    
    // 如果物体正在分离，不处理碰撞
    if (v_rel > 0) {
        if (loss) *loss = 0.0;
        return;
    }
    
    // 位置分离 - 防止物体重叠（参考C++代码中的优秀实现）
    double separation_distance = 0.05; // 较小的分离距离
    double separation_factor_1 = obj_2->mass / (obj_1->mass + obj_2->mass);
    double separation_factor_2 = obj_1->mass / (obj_1->mass + obj_2->mass);
    
    obj_1->position[0] -= separation_distance * separation_factor_1 * normal[0];
    obj_1->position[1] -= separation_distance * separation_factor_1 * normal[1];
    obj_1->position[2] -= separation_distance * separation_factor_1 * normal[2];
    
    obj_2->position[0] += separation_distance * separation_factor_2 * normal[0];
    obj_2->position[1] += separation_distance * separation_factor_2 * normal[1];
    obj_2->position[2] += separation_distance * separation_factor_2 * normal[2];
    
    // 计算恢复系数（取较小值）
    double restitution = (obj_1->coefficient_of_restitution < obj_2->coefficient_of_restitution) ?
                        obj_1->coefficient_of_restitution : obj_2->coefficient_of_restitution;

    // 更精确的冲量计算（参考C++代码）
    double numerator = -(1.0 + restitution) * v_rel;
    double denominator = (1.0/obj_1->mass + 1.0/obj_2->mass);
    double impulse_magnitude = numerator / denominator;
    
    // 应用冲量
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


//set functions
int set_entity_position(Entity* obj, double x, double y, double z) {

    if (obj == NULL) {
        return OPERATION_SET_FAILED ;
    }

        obj -> position[0] = x;
        obj -> position[1] = y;
        obj -> position[2] = z;

    return OPERATION_SET_SUCCESS;
}

int set_entity_velocity(Entity* obj, double x, double y, double z) {
    if (obj == NULL) {
        return OPERATION_SET_FAILED;
    }
        obj -> velocity[0] = x;
        obj -> velocity[1] = y;
        obj -> velocity[2] = z;

    return OPERATION_SET_SUCCESS;
}

int set_entity_acceleration(Entity* obj, double x, double y, double z) {
    if (obj == NULL) {
        return OPERATION_SET_FAILED;
    }
        obj -> acceleration[0] = x;
        obj -> acceleration[1] = y;
        obj -> acceleration[2] = z;

    return OPERATION_SET_SUCCESS;
}

int set_entity_angular_velocity(Entity* obj, double x, double y, double z) {
    if (obj == NULL) {
        return OPERATION_SET_FAILED;
    }
        obj -> angular_velocity[0] = x;
        obj -> angular_velocity[1] = y;
        obj -> angular_velocity[2] = z;

    return OPERATION_SET_SUCCESS;
}
int set_entity_angular_acceleration(Entity* obj, double x, double y, double z) {
    if (obj == NULL) {
        return OPERATION_SET_FAILED;
    }
        obj -> angular_acceleration[0] = x;
        obj -> angular_acceleration[1] = y;
        obj -> angular_acceleration[2] = z;

    return OPERATION_SET_SUCCESS;
}




