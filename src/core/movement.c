#include "core/movement.h"

void apply_force(Entity* obj, const Vector* acceleration_vector) {
    if (acceleration_vector && obj) {
        obj->acceleration.x += acceleration_vector->x;
        obj->acceleration.y += acceleration_vector->y;
        obj->acceleration.z += acceleration_vector->z;
    }

}

void apply_electric_force(Entity* obj_1, Entity* obj_2) {
    double dx = obj_1->position.x - obj_2->position.x,
       dy = obj_1->position.y - obj_2->position.y,
       dz = obj_1->position.z - obj_2->position.z;

    double euclidean_distance = get_euclidean_distance(obj_1,obj_2);
    
    if (euclidean_distance < 1e-10) {
        return;
    }
    
    double euclidean_distance_squared = pow(euclidean_distance,2);

    const double force_magnitude = (K*obj_1 -> charge * obj_2 -> charge) / euclidean_distance_squared;

    double force_direction_x = dx / euclidean_distance;
    double force_direction_y = dy / euclidean_distance;
    double force_direction_z = dz / euclidean_distance;


    double force_vector_x = force_magnitude * force_direction_x;
    double force_vector_y = force_magnitude * force_direction_y;
    double force_vector_z = force_magnitude * force_direction_z;

    Vector a_vector_1 = {force_vector_x / obj_1->mass, force_vector_y / obj_1->mass, force_vector_z / obj_1->mass};
    Vector a_vector_2 = {-force_vector_x / obj_2->mass, -force_vector_y / obj_2->mass, -force_vector_z / obj_2->mass};

    if (!obj_1 -> is_static) {
        apply_force((Entity*)obj_1, &a_vector_1);
    }
    if (!obj_2 -> is_static) {
        apply_force(obj_2, &a_vector_2);
    }

}

void apply_universal_gravitation(Entity* obj_1, Entity* obj_2) {

    double dx = obj_1->position.x - obj_2->position.x,
           dy = obj_1->position.y - obj_2->position.y,
           dz = obj_1->position.z - obj_2->position.z;

    double euclidean_distance = get_euclidean_distance(obj_1,obj_2);
    double euclidean_distance_squared = pow(euclidean_distance,2);


    const double force_magnitude = (G * obj_1->mass * obj_2->mass) / euclidean_distance_squared;

    double force_direction_x = dx / euclidean_distance;
    double force_direction_y = dy / euclidean_distance;
    double force_direction_z = dz / euclidean_distance;


    double force_vector_x = force_magnitude * force_direction_x;
    double force_vector_y = force_magnitude * force_direction_y;
    double force_vector_z = force_magnitude * force_direction_z;

    Vector a_vector_1 = {force_vector_x / obj_1->mass, force_vector_y / obj_1->mass, force_vector_z / obj_1->mass};
    Vector a_vector_2 = {-force_vector_x / obj_2->mass, -force_vector_y / obj_2->mass, -force_vector_z / obj_2->mass};

    if (!obj_1 -> is_static) {
        apply_force(obj_1, &a_vector_1);
    }
    if (!obj_2 -> is_static) {
        apply_force(obj_2, &a_vector_2);
    }

}

void apply_torque(Entity* obj, const Vector* torque) {
    if (torque && obj && !obj->is_static && obj->moment_of_inertia > 0) {
        obj->angular_acceleration.x += torque->x / obj->moment_of_inertia;
        obj->angular_acceleration.y += torque->y / obj->moment_of_inertia;
        obj->angular_acceleration.z += torque->z / obj->moment_of_inertia;
    }
}

void update_rotation(Entity* obj, double dt) {
    if (obj && !obj->is_static) {
        obj->angular_velocity.x += obj->angular_acceleration.x * dt;
        obj->angular_velocity.y += obj->angular_acceleration.y * dt;
        obj->angular_velocity.z += obj->angular_acceleration.z * dt;

        update_quaternion_with_angular_velocity(&obj->quaternion, &obj->angular_velocity, dt);

        obj->angular_acceleration.x = 0.0;
        obj->angular_acceleration.y = 0.0;
        obj->angular_acceleration.z = 0.0;
    }
}

// Get sphere radius (from entity properties)
double get_entity_radius(const Entity* entity) {
    if (!entity) return 0.0;
    
    // 使用coefficient_of_restitution字段存储半径信息
    // 如果coefficient_of_restitution > 0.1，则认为是半径，否则使用默认值
    if (entity->coefficient_of_restitution > 0.1) {
        return entity->coefficient_of_restitution;
    }
    
    // 默认半径基于质量（简单估算）
    return pow(entity->mass / 1000.0, 1.0/3.0) * 0.1;
}

// Set sphere radius
void set_entity_radius(Entity* entity, double radius) {
    if (entity && radius > 0) {
        entity->coefficient_of_restitution = radius;
    }
}

// 检查两个球体是否发生碰撞
int check_sphere_collision(const Entity* obj_1, const Entity* obj_2) {
    if (!obj_1 || !obj_2) return 0;
    
    double radius_1 = get_entity_radius(obj_1);
    double radius_2 = get_entity_radius(obj_2);
    
    double distance = get_euclidean_distance(obj_1, obj_2);
    
    // 如果距离小于半径之和，则发生碰撞
    return distance < (radius_1 + radius_2);
}

// 解析球体碰撞（基于弹性碰撞理论）
void resolve_sphere_collision(Entity* obj_1, Entity* obj_2) {
    if (!obj_1 || !obj_2 || obj_1->is_static && obj_2->is_static) {
        return;
    }
    
    // 如果两个物体都是静态的，不处理
    if (obj_1->is_static && obj_2->is_static) {
        return;
    }
    
    double radius_1 = get_entity_radius(obj_1);
    double radius_2 = get_entity_radius(obj_2);
    
    // 计算碰撞法线（从obj_1指向obj_2）
    Vector collision_normal = {
        obj_2->position.x - obj_1->position.x,
        obj_2->position.y - obj_1->position.y,
        obj_2->position.z - obj_1->position.z
    };
    
    double distance = sqrt(collision_normal.x * collision_normal.x + 
                          collision_normal.y * collision_normal.y + 
                          collision_normal.z * collision_normal.z);
    
    if (distance < 1e-10) {
        // 防止除以零
        collision_normal.x = 1.0;
        collision_normal.y = 0.0;
        collision_normal.z = 0.0;
        distance = 1.0;
    }
    
    // 归一化碰撞法线
    collision_normal.x /= distance;
    collision_normal.y /= distance;
    collision_normal.z /= distance;
    
    // 计算相对速度
    Vector relative_velocity = {
        obj_2->velocity.x - obj_1->velocity.x,
        obj_2->velocity.y - obj_1->velocity.y,
        obj_2->velocity.z - obj_1->velocity.z
    };
    
    // 计算沿碰撞法线的相对速度
    double velocity_along_normal = relative_velocity.x * collision_normal.x +
                                  relative_velocity.y * collision_normal.y +
                                  relative_velocity.z * collision_normal.z;
    
    // 如果物体正在分离，不处理碰撞
    if (velocity_along_normal > 0) {
        return;
    }
    
    // 弹性系数（恢复系数）
    double restitution = 0.8; // 0.8表示80%的能量保留
    
    // 计算冲量标量
    double impulse_scalar = -(1.0 + restitution) * velocity_along_normal;
    
    // 考虑质量
    double inv_mass_1 = obj_1->is_static ? 0.0 : 1.0 / obj_1->mass;
    double inv_mass_2 = obj_2->is_static ? 0.0 : 1.0 / obj_2->mass;
    
    impulse_scalar /= (inv_mass_1 + inv_mass_2);
    
    // 计算冲量向量
    Vector impulse = {
        impulse_scalar * collision_normal.x,
        impulse_scalar * collision_normal.y,
        impulse_scalar * collision_normal.z
    };
    
    // 应用冲量
    if (!obj_1->is_static) {
        obj_1->velocity.x -= impulse.x * inv_mass_1;
        obj_1->velocity.y -= impulse.y * inv_mass_1;
        obj_1->velocity.z -= impulse.z * inv_mass_1;
    }
    
    if (!obj_2->is_static) {
        obj_2->velocity.x += impulse.x * inv_mass_2;
        obj_2->velocity.y += impulse.y * inv_mass_2;
        obj_2->velocity.z += impulse.z * inv_mass_2;
    }
    
    // 防止物体重叠（位置修正）
    double overlap = (radius_1 + radius_2) - distance;
    if (overlap > 0) {
        double total_inv_mass = inv_mass_1 + inv_mass_2;
        if (total_inv_mass > 0) {
            double correction = overlap / total_inv_mass * 0.8; // 80%的位置修正
            
            Vector correction_vector = {
                correction * collision_normal.x,
                correction * collision_normal.y,
                correction * collision_normal.z
            };
            
            if (!obj_1->is_static) {
                obj_1->position.x -= correction_vector.x * inv_mass_1;
                obj_1->position.y -= correction_vector.y * inv_mass_1;
                obj_1->position.z -= correction_vector.z * inv_mass_1;
            }
            
            if (!obj_2->is_static) {
                obj_2->position.x += correction_vector.x * inv_mass_2;
                obj_2->position.y += correction_vector.y * inv_mass_2;
                obj_2->position.z += correction_vector.z * inv_mass_2;
            }
        }
    }
}

// 应用碰撞响应到所有实体
void apply_collision_response(Entity* entities, int count) {
    if (!entities || count <= 0) return;
    
    // 检测所有可能的碰撞对
    for (int i = 0; i < count; i++) {
        for (int j = i + 1; j < count; j++) {
            if (check_sphere_collision(&entities[i], &entities[j])) {
                resolve_sphere_collision(&entities[i], &entities[j]);
            }
        }
    }
}


void rotate_entity(Entity* obj, const Vector* axis, double angle) {
    if (obj && axis) {
        Quaternion rotation = axis_angle_to_quaternion(axis, angle);
        obj->quaternion = quaternion_multiply(rotation, obj->quaternion);
        obj->quaternion = quaternion_normalize(obj->quaternion);
    }
}

Quaternion euler_to_quaternion(double pitch, double yaw, double roll) {
    double cy = cos(yaw * 0.5);
    double sy = sin(yaw * 0.5);
    double cp = cos(pitch * 0.5);
    double sp = sin(pitch * 0.5);
    double cr = cos(roll * 0.5);
    double sr = sin(roll * 0.5);

    Quaternion q;
    q.w = cr * cp * cy + sr * sp * sy;
    q.x = sr * cp * cy - cr * sp * sy;
    q.y = cr * sp * cy + sr * cp * sy;
    q.z = cr * cp * sy - sr * sp * cy;

    return quaternion_normalize(q);
}

void rotate_vector_by_quaternion(const Vector* v, Quaternion q, Vector* result) {
    // Use the new Quaternion library functions
    float out_x, out_y, out_z;
    quat_rotate_vector(q, v->x, v->y, v->z, &out_x, &out_y, &out_z);
    
    result->x = out_x;
    result->y = out_y;
    result->z = out_z;
}

void update_quaternion_with_angular_velocity(Quaternion* q, const Vector* omega, double dt) {
    // Convert angular velocity to quaternion derivative
    Quaternion omega_q = {0.0f, (float)omega->x, (float)omega->y, (float)omega->z};
    Quaternion dq = quat_multiply(omega_q, *q);
    
    // Update quaternion: q += 0.5 * dq * dt
    q->w += 0.5f * dq.w * dt;
    q->x += 0.5f * dq.x * dt;
    q->y += 0.5f * dq.y * dt;
    q->z += 0.5f * dq.z * dt;
    
    // Normalize
    *q = quat_normalize(*q);
}

Quaternion quaternion_multiply(Quaternion q1, Quaternion q2) {
    return quat_multiply(q1, q2);
}

Quaternion quaternion_conjugate(Quaternion q) {
    return quat_conjugate(q);
}

Quaternion quaternion_normalize(Quaternion q) {
    return quat_normalize(q);
}

Quaternion axis_angle_to_quaternion(const Vector* axis, double angle) {
    return quat_from_axis_angle((float)angle, (float)axis->x, (float)axis->y, (float)axis->z);
}
