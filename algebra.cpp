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

float* vector_float_scale(const float a[4], const float b, float result[4]) {
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

float* matrix_scale(const float m[16],float a, float result[16]){
#ifdef SSE
    for (int i=0;i<4;i++)
        _mm_store_ps(&result[i*4], _mm_mul_ps(_mm_load_ps(&m[i*4]),_mm_load_ps1(&a)));
#else
    for (int i=0;i<16;i++)
        result[i] = m[i]*a;
#endif
    return result;
}

float* matrix_vector_product(const float m[16], const float v[4], float result[4]){
#ifdef SSE
    __m128 vec = _mm_load_ps(v);
    
    __m128 u1 = _mm_shuffle_ps(vec,vec, _MM_SHUFFLE(0,0,0,0));
    __m128 u2 = _mm_shuffle_ps(vec,vec, _MM_SHUFFLE(1,1,1,1));
    __m128 u3 = _mm_shuffle_ps(vec,vec, _MM_SHUFFLE(2,2,2,2));
    __m128 u4 = _mm_shuffle_ps(vec,vec, _MM_SHUFFLE(3,3,3,3));
    
    __m128 prod1 = _mm_mul_ps(u1, _mm_load_ps(&m[0]));
    __m128 prod2 = _mm_mul_ps(u2, _mm_load_ps(&m[4]));
    __m128 prod3 = _mm_mul_ps(u3, _mm_load_ps(&m[8]));
    __m128 prod4 = _mm_mul_ps(u4, _mm_load_ps(&m[12]));
    
    _mm_store_ps(result, _mm_add_ps(_mm_add_ps(prod1, prod2), _mm_add_ps(prod3, prod4)));
#else
    float tmp[4];
    memcpy(tmp,v,4*sizeof(float));
    
    for (int i=0;i<4;i++){
        result[i] = 0;
        for (int j=0;j<4;j++)
            result[i] += m[i+j*4]*tmp[j];
    }
#endif
    
    return result;
}

float* matrix_matrix_product(const float a[16], const float b[16], float result[16]){
#ifdef SSE
    __m128 row1 = _mm_load_ps(&a[0]);
    __m128 row2 = _mm_load_ps(&a[4]);
    __m128 row3 = _mm_load_ps(&a[8]);
    __m128 row4 = _mm_load_ps(&a[12]);
    for(int i=0; i<4; i++) {
        __m128 brod1 = _mm_set1_ps(b[4*i + 0]);
        __m128 brod2 = _mm_set1_ps(b[4*i + 1]);
        __m128 brod3 = _mm_set1_ps(b[4*i + 2]);
        __m128 brod4 = _mm_set1_ps(b[4*i + 3]);
        __m128 row = _mm_add_ps(
                                _mm_add_ps(
                                           _mm_mul_ps(brod1, row1),
                                           _mm_mul_ps(brod2, row2)),
                                _mm_add_ps(
                                           _mm_mul_ps(brod3, row3),
                                           _mm_mul_ps(brod4, row4)));
        _mm_store_ps(&result[4*i], row);
    }
#else
    for (int i=0;i<4;i++)
        for (int j=0;j<4;j++){
            result[i+j*4] = 0;
            for (int k=0;k<4;k++)
                result[i+j*4] += a[i+k*4]*b[k+j*4];
        }
#endif
    return result;
}

float* matrix_transpose(const float a[16], float result[16]){
#ifdef SSE
    __m128 row1 = _mm_load_ps(&a[0]);
    __m128 row2 = _mm_load_ps(&a[4]);
    __m128 row3 = _mm_load_ps(&a[8]);
    __m128 row4 = _mm_load_ps(&a[12]);
    _MM_TRANSPOSE4_PS(row1, row2, row3, row4);
    _mm_store_ps(&result[0], row1);
    _mm_store_ps(&result[4], row2);
    _mm_store_ps(&result[8], row3);
    _mm_store_ps(&result[12], row4);
#else
    for (int i=0;i<4;i++)
        for (int j=0;j<4;j++)
            result[i+j*4] = a[j+i*4];
#endif
    return result;
}
