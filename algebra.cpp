//
//  algebra.c
//  Headless
//
//  Created by Paolo Burelli on 06/12/14.
//  Copyright (c) 2014 Paolo Burelli. All rights reserved.
//

#include "algebra.h"

float vector_dot(const float a[4], const float b[4]) {
#ifdef SSE
    __m128 r = _mm_mul_ps(_mm_load_ps(a),_mm_load_ps(b));
    r = _mm_hadd_ps(r,r);
    r = _mm_hadd_ps(r, r);
    return r[0];
#else
    float rval=0;
    for (int i=0;i<4;i++)
        rval += a[i]*b[i];
    return rval;
#endif
}

float* vector_scale(const float a[4], const float b[4], float result[4]) {
#ifdef SSE
    _mm_store_ps(result, _mm_mul_ps(_mm_load_ps(a),_mm_load_ps(b)));
#else
    for (int i=0;i<4;i++)
        result[i] += a[i]*b[i];
#endif
    return result;
}

float* vector_scale(const float a[4], const float b, float result[4]) {
#ifdef SSE
    _mm_store_ps(result, _mm_mul_ps(_mm_load_ps(a),_mm_load1_ps(&b)));
#else
    for (int i=0;i<4;i++)
        result[i] += a[i]*b;
#endif
    return result;
}

float vector_magnitude(const float a[4]) {
#ifdef SSE
    __m128 r = _mm_mul_ps(_mm_load_ps(a),_mm_load_ps(a));
    r = _mm_hadd_ps(r,r);
    r = _mm_hadd_ps(r, r);
    return _mm_sqrt_ss(r)[0];
#else
    float rval=0;
    for (int i=0;i<4;i++)
        rval += a[i]*a[i];
    return sqrtf(rval);
#endif
}

float* vector_normalise(float a[4]) {
    float l = 1/vector_magnitude(a);
#ifdef SSE
    _mm_store_ps(a,_mm_mul_ps(_mm_load_ps(a),_mm_load1_ps(&l)));
#else
    for (int i=0;i<4;i++)
        a[i] *= l;
#endif
    return a;
}


float* vector_negate (const float a[4], float result[4])  {
#ifdef SSE
    const __m128 SIGNMASK = _mm_castsi128_ps(_mm_set1_epi32(0x80000000));
    _mm_store_ps(result,_mm_xor_ps(_mm_load_ps(a), SIGNMASK));
#else
    for (int i=0;i<4;i++)
        result[i] = -a[i];
#endif
    return result;
}

float* vector_add(const float a[4], const float b[4], float result[4]) {
#ifdef SSE
        _mm_store_ps(result,_mm_add_ps(_mm_load_ps(a),_mm_load_ps(b)));
#else
    for (int i=0;i<4;i++)
        result[i] = a[i]+b[i];
#endif
        return result;
}

float* vector_sub(const float a[4], const float b[4], float result[4]) {
#ifdef SSE
    _mm_store_ps(result,_mm_sub_ps(_mm_load_ps(a),_mm_load_ps(b)));
#else
    for (int i=0;i<4;i++)
        result[i] = a[i]+b[i];
#endif
    return result;
}

float* vector_cross(const float u[4],const float v[4], float result[4]) {
    result[0] = u[1] * v[2] - u[2] * v[1];
    result[1] = u[2] * v[0] - u[0] * v[2];
    result[2] = u[0] * v[1] - u[1] * v[0];
    result[3] = u[3];
    return result;
}
