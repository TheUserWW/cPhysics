#include "../../include/core/movement.h"

void apply_force(Entity* obj, const Vector* acceleration_vector) {
    if (acceleration_vector && obj) {
        obj->acceleration.x += acceleration_vector->x;
        obj->acceleration.y += acceleration_vector->y;
        obj->acceleration.z += acceleration_vector->z;
    }

}

void apply_electric_force(const Entity* obj_1, const Entity* obj_2) {
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

        update_quaternion_with_angular_velocity(obj->quaternion, &obj->angular_velocity, dt);

        obj->angular_acceleration.x = 0.0;
        obj->angular_acceleration.y = 0.0;
        obj->angular_acceleration.z = 0.0;
    }
}

void rotate_entity(Entity* obj, const Vector* axis, double angle) {
    if (obj && axis) {
        double rotation[4];
        axis_angle_to_quaternion(axis, angle, rotation);
        quaternion_multiply(rotation, obj->quaternion, obj->quaternion);
        quaternion_normalize(obj->quaternion);
    }
}

void euler_to_quaternion(double pitch, double yaw, double roll, double q[4]) {
    double cy = cos(yaw * 0.5);
    double sy = sin(yaw * 0.5);
    double cp = cos(pitch * 0.5);
    double sp = sin(pitch * 0.5);
    double cr = cos(roll * 0.5);
    double sr = sin(roll * 0.5);

    q[0] = cr * cp * cy + sr * sp * sy;
    q[1] = sr * cp * cy - cr * sp * sy;
    q[2] = cr * sp * cy + sr * cp * sy;
    q[3] = cr * cp * sy - sr * sp * cy;

    quaternion_normalize(q);
}

void rotate_vector_by_quaternion(const Vector* v, const double q[4], Vector* result) {
    double vq[4] = {0, v->x, v->y, v->z};
    double q_conj[4];
    quaternion_conjugate(q, q_conj);

    double temp[4];
    quaternion_multiply(q, vq, temp);
    quaternion_multiply(temp, q_conj, vq);

    result->x = vq[1];
    result->y = vq[2];
    result->z = vq[3];
}

void update_quaternion_with_angular_velocity(double q[4], const Vector* omega, double dt) {
    double omega_q[4] = {0, omega->x, omega->y, omega->z};
    double dq[4];
    quaternion_multiply(omega_q, q, dq);

    q[0] += 0.5 * dq[0] * dt;
    q[1] += 0.5 * dq[1] * dt;
    q[2] += 0.5 * dq[2] * dt;
    q[3] += 0.5 * dq[3] * dt;

    quaternion_normalize(q);
}

void quaternion_multiply(const double q1[4], const double q2[4], double result[4]) {
    result[0] = q1[0]*q2[0] - q1[1]*q2[1] - q1[2]*q2[2] - q1[3]*q2[3];
    result[1] = q1[0]*q2[1] + q1[1]*q2[0] + q1[2]*q2[3] - q1[3]*q2[2];
    result[2] = q1[0]*q2[2] - q1[1]*q2[3] + q1[2]*q2[0] + q1[3]*q2[1];
    result[3] = q1[0]*q2[3] + q1[1]*q2[2] - q1[2]*q2[1] + q1[3]*q2[0];
}

void quaternion_conjugate(const double q[4], double result[4]) {
    result[0] = q[0];
    result[1] = -q[1];
    result[2] = -q[2];
    result[3] = -q[3];
}

void quaternion_normalize(double q[4]) {
    double length = sqrt(q[0]*q[0] + q[1]*q[1] + q[2]*q[2] + q[3]*q[3]);
    if (length > 1e-10) {
        q[0] /= length;
        q[1] /= length;
        q[2] /= length;
        q[3] /= length;
    }
}

void axis_angle_to_quaternion(const Vector* axis, double angle, double q[4]) {
    double half_angle = angle / 2.0;
    double sin_half = sin(half_angle);

    q[0] = cos(half_angle);
    q[1] = axis->x * sin_half;
    q[2] = axis->y * sin_half;
    q[3] = axis->z * sin_half;

    quaternion_normalize(q);
}
