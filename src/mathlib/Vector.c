#include "../../include/mathlib/Vector.h"

double dot_product(const Vector a, const Vector b) {
    return a.x*b.x + a.y*b.y + a.z*b.z;
}

Vector cross_product(const Vector a, const Vector b) {
    Vector result;
    result.x = a.y * b.z - a.z * b.y;
    result.y = a.z * b.x - a.x * b.z;
    result.z = a.x * b.y - a.y * b.x;
    return result;
}

double normalize(const Vector a) {
    double norm = sqrt(dot_product(a, a));
    return norm;
}

