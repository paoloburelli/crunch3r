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

#ifdef SSE 
#include <pmmintrin.h>
#endif

namespace crunch3r {
    
    class Matrix4x4 {
    protected:
        std::vector<float> values;
        void copy(const Matrix4x4 &from,Matrix4x4 &to);
    public:
        Matrix4x4();
        Matrix4x4(const Matrix4x4&);
        Matrix4x4(std::initializer_list<float> seq);
        bool operator==(const Matrix4x4 other) const;
        bool operator!=(const Matrix4x4 other) const;
        Matrix4x4& operator*(const float);
        Vector4& operator*(const Vector4);
        Matrix4x4& operator*(const Matrix4x4);
        void set(unsigned int,unsigned int,float);
        float get(unsigned int, unsigned int) const;
        void transpose();
        Matrix4x4& transposed() const;
        void invert();
        Matrix4x4& inverted() const;
        
        enum class Type {Translation, Scaling, Identity, Other};
        Type getType();
        
        static const Matrix4x4 IDENTITY;
        static Matrix4x4& translate(float,float,float);
        static Matrix4x4& scale(float,float,float);
    };
    
}


#endif
