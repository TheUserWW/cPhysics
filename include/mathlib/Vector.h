#ifndef CPHYSICS_VECTOR_H
#define CPHYSICS_VECTOR_H
#include <math.h>

typedef struct Vector {
    double x,y,z;
}Vector;

double dot_product(const Vector a, const Vector b);

Vector cross_product(const Vector a, const Vector b);

double normalize(const Vector a);

#endif //CPHYSICS_VECTOR_H