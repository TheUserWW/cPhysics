#include "../../include/mathlib/Quaternion.h"
#include <math.h>

Quaternion quat_identity() {
    Quaternion q = {1.0f, 0.0f, 0.0f, 0.0f};
    return q;
}

Quaternion quat_create(float w, float x, float y, float z) {
    Quaternion q = {w, x, y, z};
    return q;
}

Quaternion quat_from_axis_angle(float angle, float axis_x, float axis_y, float axis_z) {
    float half_angle = angle * 0.5f;
    float sin_half = sinf(half_angle);
    
    // Normalize axis
    float length = sqrtf(axis_x * axis_x + axis_y * axis_y + axis_z * axis_z);
    if (length > 0.0f) {
        axis_x /= length;
        axis_y /= length;
        axis_z /= length;
    }
    
    Quaternion q;
    q.w = cosf(half_angle);
    q.x = axis_x * sin_half;
    q.y = axis_y * sin_half;
    q.z = axis_z * sin_half;
    
    return q;
}

Quaternion quat_from_euler(float yaw, float pitch, float roll) {
    float cy = cosf(yaw * 0.5f);
    float sy = sinf(yaw * 0.5f);
    float cp = cosf(pitch * 0.5f);
    float sp = sinf(pitch * 0.5f);
    float cr = cosf(roll * 0.5f);
    float sr = sinf(roll * 0.5f);
    
    Quaternion q;
    q.w = cr * cp * cy + sr * sp * sy;
    q.x = sr * cp * cy - cr * sp * sy;
    q.y = cr * sp * cy + sr * cp * sy;
    q.z = cr * cp * sy - sr * sp * cy;
    
    return q;
}

Quaternion quat_normalize(Quaternion q) {
    float mag = quat_magnitude(q);
    if (mag > 0.0f) {
        q.w /= mag;
        q.x /= mag;
        q.y /= mag;
        q.z /= mag;
    }
    return q;
}

Quaternion quat_multiply(Quaternion q1, Quaternion q2) {
    Quaternion result;
    result.w = q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z;
    result.x = q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y;
    result.y = q1.w * q2.y - q1.x * q2.z + q1.y * q2.w + q1.z * q2.x;
    result.z = q1.w * q2.z + q1.x * q2.y - q1.y * q2.x + q1.z * q2.w;
    return result;
}

Quaternion quat_conjugate(Quaternion q) {
    Quaternion result = {q.w, -q.x, -q.y, -q.z};
    return result;
}

Quaternion quat_inverse(Quaternion q) {
    float mag_sq = q.w * q.w + q.x * q.x + q.y * q.y + q.z * q.z;
    if (mag_sq > 0.0f) {
        Quaternion conj = quat_conjugate(q);
        conj.w /= mag_sq;
        conj.x /= mag_sq;
        conj.y /= mag_sq;
        conj.z /= mag_sq;
        return conj;
    }
    return quat_identity();
}

void quat_rotate_vector(Quaternion q, float vx, float vy, float vz, 
                       float* out_x, float* out_y, float* out_z) {
    // Convert vector to pure quaternion
    Quaternion v_quat = {0.0f, vx, vy, vz};
    
    // Rotate: v' = q * v * q^-1
    Quaternion q_inv = quat_inverse(q);
    Quaternion rotated = quat_multiply(quat_multiply(q, v_quat), q_inv);
    
    *out_x = rotated.x;
    *out_y = rotated.y;
    *out_z = rotated.z;
}

void quat_to_axis_angle(Quaternion q, float* angle, 
                       float* axis_x, float* axis_y, float* axis_z) {
    if (fabsf(q.w) > 1.0f) {
        q = quat_normalize(q);
    }
    
    *angle = 2.0f * acosf(q.w);
    float sin_half = sqrtf(1.0f - q.w * q.w);
    
    if (sin_half > 0.001f) {
        *axis_x = q.x / sin_half;
        *axis_y = q.y / sin_half;
        *axis_z = q.z / sin_half;
    } else {
        // If angle is very small, use x-axis as default
        *axis_x = 1.0f;
        *axis_y = 0.0f;
        *axis_z = 0.0f;
    }
}

void quat_to_euler(Quaternion q, float* yaw, float* pitch, float* roll) {
    // Roll (x-axis rotation)
    float sinr_cosp = 2.0f * (q.w * q.x + q.y * q.z);
    float cosr_cosp = 1.0f - 2.0f * (q.x * q.x + q.y * q.y);
    *roll = atan2f(sinr_cosp, cosr_cosp);
    
    // Pitch (y-axis rotation)
    float sinp = 2.0f * (q.w * q.y - q.z * q.x);
    if (fabsf(sinp) >= 1.0f) {
        *pitch = copysignf(M_PI / 2.0f, sinp); // Use 90 degrees if out of range
    } else {
        *pitch = asinf(sinp);
    }
    
    // Yaw (z-axis rotation)
    float siny_cosp = 2.0f * (q.w * q.z + q.x * q.y);
    float cosy_cosp = 1.0f - 2.0f * (q.y * q.y + q.z * q.z);
    *yaw = atan2f(siny_cosp, cosy_cosp);
}

Quaternion quat_lerp(Quaternion q1, Quaternion q2, float t) {
    Quaternion result;
    result.w = q1.w + t * (q2.w - q1.w);
    result.x = q1.x + t * (q2.x - q1.x);
    result.y = q1.y + t * (q2.y - q1.y);
    result.z = q1.z + t * (q2.z - q1.z);
    return quat_normalize(result);
}

Quaternion quat_slerp(Quaternion q1, Quaternion q2, float t) {
    float dot = quat_dot(q1, q2);
    
    // If dot is negative, the quaternions have opposite handedness
    if (dot < 0.0f) {
        q2.w = -q2.w;
        q2.x = -q2.x;
        q2.y = -q2.y;
        q2.z = -q2.z;
        dot = -dot;
    }
    
    const float DOT_THRESHOLD = 0.9995f;
    if (dot > DOT_THRESHOLD) {
        // If quaternions are very close, use linear interpolation
        return quat_lerp(q1, q2, t);
    }
    
    float theta_0 = acosf(dot); // Angle between quaternions
    float theta = theta_0 * t;  // Angle to interpolate
    
    Quaternion q3;
    q3.w = q2.w - q1.w * dot;
    q3.x = q2.x - q1.x * dot;
    q3.y = q2.y - q1.y * dot;
    q3.z = q2.z - q1.z * dot;
    q3 = quat_normalize(q3);
    
    Quaternion result;
    result.w = q1.w * cosf(theta) + q3.w * sinf(theta);
    result.x = q1.x * cosf(theta) + q3.x * sinf(theta);
    result.y = q1.y * cosf(theta) + q3.y * sinf(theta);
    result.z = q1.z * cosf(theta) + q3.z * sinf(theta);
    
    return result;
}

float quat_dot(Quaternion q1, Quaternion q2) {
    return q1.w * q2.w + q1.x * q2.x + q1.y * q2.y + q1.z * q2.z;
}

float quat_magnitude(Quaternion q) {
    return sqrtf(q.w * q.w + q.x * q.x + q.y * q.y + q.z * q.z);
}