//
//  algebra.h
//  Headless
//
//  Created by Paolo Burelli on 06/12/14.
//  Copyright (c) 2014 Paolo Burelli. All rights reserved.
//

#ifndef Headless_algebra_h
#define Headless_algebra_h

#include <math.h>
#ifdef SSE
#include <pmmintrin.h>
#endif

float vector_dot(const float a[4], const float b[4]);
float* vector_scale(const float a[4], const float b[4], float result[4]);
float* vector_scale(const float a[4], const float b, float result[4]);
float vector_magnitude(const float a[4]);
float* vector_normalise(float a[4]);
float* vector_negate (const float a[4], float result[4]);
float* vector_add(const float a[4], const float b[4], float result[4]);
float* vector_sub(const float a[4], const float b[4], float result[4]);
float* vector_cross(const float u[4],const float v[4], float result[4]);

#endif
