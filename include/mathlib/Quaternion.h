#ifndef QUATERNION_H
#define QUATERNION_H

/**
 * @file Quaternion.h
 * @brief Quaternion structure and operations for 3D rotations and orientations
 */

/**
 * @brief Quaternion structure representing a 4D complex number
 * 
 * A quaternion is defined as q = w + xi + yj + zk
 * where w is the real/scalar part and (x, y, z) is the vector/imaginary part
 */
typedef struct {
    float w; /**< Scalar (real) component */
    float x; /**< i-component (imaginary) */
    float y; /**< j-component (imaginary) */
    float z; /**< k-component (imaginary) */
} Quaternion;

/**
 * @brief Create an identity quaternion (no rotation)
 * @return Identity quaternion [1, 0, 0, 0]
 */
Quaternion quat_identity();

/**
 * @brief Create a quaternion from components
 * @param w Scalar component
 * @param x i-component
 * @param y j-component
 * @param z k-component
 * @return Constructed quaternion
 */
Quaternion quat_create(float w, float x, float y, float z);

/**
 * @brief Create a quaternion from axis-angle representation
 * @param angle Rotation angle in radians
 * @param axis_x X component of rotation axis
 * @param axis_y Y component of rotation axis
 * @param axis_z Z component of rotation axis
 * @return Quaternion representing the rotation
 */
Quaternion quat_from_axis_angle(float angle, float axis_x, float axis_y, float axis_z);

/**
 * @brief Create a quaternion from Euler angles (ZYX order)
 * @param yaw Yaw angle (Z-axis) in radians
 * @param pitch Pitch angle (Y-axis) in radians
 * @param roll Roll angle (X-axis) in radians
 * @return Quaternion representing the rotation
 */
Quaternion quat_from_euler(float yaw, float pitch, float roll);

/**
 * @brief Normalize a quaternion to unit length
 * @param q Input quaternion
 * @return Normalized quaternion
 */
Quaternion quat_normalize(Quaternion q);

/**
 * @brief Multiply two quaternions (composition of rotations)
 * @param q1 First quaternion
 * @param q2 Second quaternion
 * @return Product quaternion q1 * q2
 */
Quaternion quat_multiply(Quaternion q1, Quaternion q2);

/**
 * @brief Compute the conjugate of a quaternion
 * @param q Input quaternion
 * @return Conjugate quaternion [w, -x, -y, -z]
 */
Quaternion quat_conjugate(Quaternion q);

/**
 * @brief Compute the inverse of a quaternion
 * @param q Input quaternion (must be normalized)
 * @return Inverse quaternion
 */
Quaternion quat_inverse(Quaternion q);

/**
 * @brief Rotate a vector by a quaternion
 * @param q Rotation quaternion (should be normalized)
 * @param vx Vector x component
 * @param vy Vector y component
 * @param vz Vector z component
 * @param[out] out_x Rotated vector x component
 * @param[out] out_y Rotated vector y component
 * @param[out] out_z Rotated vector z component
 */
void quat_rotate_vector(Quaternion q, float vx, float vy, float vz, 
                       float* out_x, float* out_y, float* out_z);

/**
 * @brief Convert quaternion to axis-angle representation
 * @param q Input quaternion
 * @param[out] angle Rotation angle in radians
 * @param[out] axis_x X component of rotation axis
 * @param[out] axis_y Y component of rotation axis
 * @param[out] axis_z Z component of rotation axis
 */
void quat_to_axis_angle(Quaternion q, float* angle, 
                       float* axis_x, float* axis_y, float* axis_z);

/**
 * @brief Convert quaternion to Euler angles (ZYX order)
 * @param q Input quaternion
 * @param[out] yaw Yaw angle (Z-axis) in radians
 * @param[out] pitch Pitch angle (Y-axis) in radians
 * @param[out] roll Roll angle (X-axis) in radians
 */
void quat_to_euler(Quaternion q, float* yaw, float* pitch, float* roll);

/**
 * @brief Linear interpolation between two quaternions
 * @param q1 Start quaternion
 * @param q2 End quaternion
 * @param t Interpolation parameter [0, 1]
 * @return Interpolated quaternion
 */
Quaternion quat_lerp(Quaternion q1, Quaternion q2, float t);

/**
 * @brief Spherical linear interpolation between two quaternions
 * @param q1 Start quaternion (should be normalized)
 * @param q2 End quaternion (should be normalized)
 * @param t Interpolation parameter [0, 1]
 * @return Interpolated quaternion
 */
Quaternion quat_slerp(Quaternion q1, Quaternion q2, float t);

/**
 * @brief Compute the dot product of two quaternions
 * @param q1 First quaternion
 * @param q2 Second quaternion
 * @return Dot product (scalar)
 */
float quat_dot(Quaternion q1, Quaternion q2);

/**
 * @brief Compute the magnitude (length) of a quaternion
 * @param q Input quaternion
 * @return Magnitude
 */
float quat_magnitude(Quaternion q);

#endif /* QUATERNION_H */