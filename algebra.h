//
//  algebra.h
//  Headless
//
//  Created by Paolo Burelli on 06/12/14.
//  Copyright (c) 2014 Paolo Burelli. All rights reserved.
//

#ifndef Headless_algebra_h
#define Headless_algebra_h

extern "C" {

#include <math.h>
#include <string.h>
#ifdef SSE
#include <pmmintrin.h>
#endif

float vector_dot(const float a[4], const float b[4]);
float* vector_scale(const float a[4], const float b[4], float result[4]);
float* vector_float_scale(const float a[4], const float b, float result[4]);
float vector_magnitude(const float a[4]);
float* vector_normalise(float a[4]);
float* vector_negate (const float a[4], float result[4]);
float* vector_add(const float a[4], const float b[4], float result[4]);
float* vector_sub(const float a[4], const float b[4], float result[4]);
float* vector_cross(const float u[4],const float v[4], float result[4]);
    
float* matrix_scale(const float m[16],float a, float result[16]);
float* matrix_vector_product(const float m[16], const float v[4], float result[4]);
float* matrix_matrix_product(const float a[16], const float b[16], float result[16]);
float* matrix_transpose(const float m[16], float result[16]);

}
#endif
