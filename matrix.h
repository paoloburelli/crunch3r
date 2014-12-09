//
//  Matrix.h
//  Headless
//
//  Created by Paolo Burelli on 21/11/14.
//  Copyright (c) 2014 Paolo Burelli. All rights reserved.
//

#ifndef Crunch3r_Matrix_h
#define Crunch3r_Matrix_h

#include <exception>
#include <initializer_list>
#include <cassert>
#include "vector.h"
#include "algebra.h"

#ifdef SSE 
#include <pmmintrin.h>
#endif

#define MATRIX_SIZE 16
#define MATRIX_MEMORY sizeof(float)*MATRIX_SIZE

namespace crunch3r {
    
    class Matrix4x4 {
    private:
        Matrix4x4(float* a);
        float* values;
    public:
        Matrix4x4(const Matrix4x4&);
        Matrix4x4(std::initializer_list<float> seq);
        inline ~Matrix4x4() {delete values;};
        bool operator==(const Matrix4x4 other) const;
        bool operator!=(const Matrix4x4 other) const;
        inline Matrix4x4& operator*(const float a) { return *(new Matrix4x4(matrix_scale(values, a, new float[MATRIX_SIZE])));};
        inline Vector& operator*(const Vector v) { return *(new Vector(matrix_vector_product(values, v.values, new float[VECTOR_MAX_SIZE]),VECTOR_MAX_SIZE));};
        inline Matrix4x4& operator*(const Matrix4x4 other) {return *(new Matrix4x4(matrix_matrix_product(values, other.values, new float[MATRIX_SIZE])));};
        void set(unsigned int,unsigned int,float);
        float get(unsigned int, unsigned int) const;
        Matrix4x4& transpose();
        Matrix4x4& invert();
        
        enum class Type {Translation, Scaling, Identity, Other};
        Type getType();
        
        static const Matrix4x4 IDENTITY;
        inline static Matrix4x4& translate(float x,float y,float z){return *(new Matrix4x4({1,0,0,x,0,1,0,y,0,0,1,z,0,0,0,1}));}
        inline static Matrix4x4& scale(float x,float y,float z){return *(new Matrix4x4({x,0,0,0,0,y,0,0,0,0,z,0,0,0,0,1}));}
    };
    
}


#endif
