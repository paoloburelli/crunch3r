//
//  Matrix.cpp
//  Headless
//
//  Created by Paolo Burelli on 21/11/14.
//  Copyright (c) 2014 Paolo Burelli. All rights reserved.
//

#include "matrix.h"

using namespace crunch3r;

const Matrix4x4 Matrix4x4::IDENTITY = {1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};

Matrix4x4::Matrix4x4(std::initializer_list<float> seq){
    values = new float[MATRIX_SIZE];
    memset(values, 0, MATRIX_MEMORY);
    int i = 0;
    for (float f:seq)
        if (i<MATRIX_SIZE)
            values[i++] = f;
    
    transpose();
}

Matrix4x4::Matrix4x4(const Matrix4x4& other){
    values = new float[MATRIX_SIZE];
    memcpy(values, other.values, MATRIX_MEMORY);
}

Matrix4x4::Matrix4x4(float *other){
    values = other;
}

bool Matrix4x4::operator==(const Matrix4x4 other) const{
    return memcmp(values, other.values, MATRIX_MEMORY) == 0;
}

bool Matrix4x4::operator!=(const Matrix4x4 other) const{
    return memcmp(values, other.values, MATRIX_MEMORY) != 0;
}

void Matrix4x4::set(unsigned int row, unsigned int col, float v){
    assert(col*4+row < MATRIX_SIZE);
    values[col*4+row] = v;
}

float Matrix4x4::get(unsigned int row, unsigned int col) const{
    assert(col*4+row < MATRIX_SIZE);
    return values[col*4+row];
}

Matrix4x4& Matrix4x4::transpose(){
    float* tmp = matrix_transpose(values, new float[MATRIX_SIZE]);
    delete values;
    values = tmp;
    return *this;
}

Matrix4x4::Type Matrix4x4::getType(){
    if ((*this) == IDENTITY) {
        return Type::Identity;
    } else {
        Matrix4x4 tmp = Matrix4x4(*this);
        for (int i=0;i<4;i++)
            tmp.set(i,i,1);
        
        if (tmp == IDENTITY)
            return Type::Scaling;
        else {
            Matrix4x4 tmp = Matrix4x4(*this);
            tmp.set(0,3,0);
            tmp.set(1,3,0);
            tmp.set(2,3,0);
            if (tmp == IDENTITY)
                return Type::Translation;
        }
    }
    return Type::Other;
}