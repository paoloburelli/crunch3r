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

void Matrix4x4::copy(const Matrix4x4 &from,Matrix4x4 &to){
    std::copy(from.values.begin(), from.values.end(), to.values.begin());
}

Matrix4x4::Matrix4x4(){
    values = std::vector<float>(16,0);
}

Matrix4x4::Matrix4x4(std::initializer_list<float> seq){
    values = std::vector<float>(16,0);
    std::copy(seq.begin(), seq.end(), values.begin());
    transpose();
}

Matrix4x4::Matrix4x4(const Matrix4x4& other){
    values = std::vector<float>(other.values);
}

bool Matrix4x4::operator==(const Matrix4x4 other) const{
    return std::equal(values.begin(), values.end(), other.values.begin());
}

bool Matrix4x4::operator!=(const Matrix4x4 other) const{
    return !std::equal(values.begin(), values.end(), other.values.begin());
}

void Matrix4x4::set(unsigned int row, unsigned int col, float v){
    assert(col*4+row < 16);
    values[col*4+row] = v;
}

float Matrix4x4::get(unsigned int row, unsigned int col) const{
    assert(col*4+row < 16);
    return values[col*4+row];
}

Matrix4x4& Matrix4x4::operator*(const float a){
    Matrix4x4* m = new Matrix4x4(*this);
#ifdef SSE
    for (int i=0;i<4;i++)
         _mm_store_ps(&m->values.data()[i*4], _mm_mul_ps(_mm_load_ps(&values.data()[i*4]),_mm_load_ps1(&a)));
#else
    std::for_each(m->values.begin(), m->values.end(), [a] (float &f) {
        f *=a;
    });
#endif
    return *m;
}

Vector4& Matrix4x4::operator*(const Vector4 v){
    Vector4* rVal = new Vector4();
    
    
#ifdef SSE
    __m128 vec = v.Load();
    
    __m128 u1 = _mm_shuffle_ps(vec,vec, _MM_SHUFFLE(0,0,0,0));
    __m128 u2 = _mm_shuffle_ps(vec,vec, _MM_SHUFFLE(1,1,1,1));
    __m128 u3 = _mm_shuffle_ps(vec,vec, _MM_SHUFFLE(2,2,2,2));
    __m128 u4 = _mm_shuffle_ps(vec,vec, _MM_SHUFFLE(3,3,3,3));
    
    __m128 prod1 = _mm_mul_ps(u1, _mm_load_ps(&values.data()[0]));
    __m128 prod2 = _mm_mul_ps(u2, _mm_load_ps(&values.data()[4]));
    __m128 prod3 = _mm_mul_ps(u3, _mm_load_ps(&values.data()[8]));
    __m128 prod4 = _mm_mul_ps(u4, _mm_load_ps(&values.data()[12]));
    
    rVal->Store(_mm_add_ps(_mm_add_ps(prod1, prod2), _mm_add_ps(prod3, prod4)));
#else
    for (int i=0;i<4;i++)
        for (int j=0;j<4;j++)
            (*rVal)[i] += get(i,j)*v[j];
#endif
    
    return *rVal;
}

Matrix4x4& Matrix4x4::operator*(const Matrix4x4 m){
    Matrix4x4* rVal = new Matrix4x4();
    
#ifdef SSE
    __m128 row1 = _mm_load_ps(&values.data()[0]);
    __m128 row2 = _mm_load_ps(&values.data()[4]);
    __m128 row3 = _mm_load_ps(&values.data()[8]);
    __m128 row4 = _mm_load_ps(&values.data()[12]);
    for(int i=0; i<4; i++) {
        __m128 brod1 = _mm_set1_ps(m.values.data()[4*i + 0]);
        __m128 brod2 = _mm_set1_ps(m.values.data()[4*i + 1]);
        __m128 brod3 = _mm_set1_ps(m.values.data()[4*i + 2]);
        __m128 brod4 = _mm_set1_ps(m.values.data()[4*i + 3]);
        __m128 row = _mm_add_ps(
        _mm_add_ps(
                   _mm_mul_ps(brod1, row1),
                   _mm_mul_ps(brod2, row2)),
        _mm_add_ps(
                   _mm_mul_ps(brod3, row3),
                   _mm_mul_ps(brod4, row4)));
        _mm_store_ps(&rVal->values[4*i], row);
    }
#else
    for (int i=0;i<4;i++)
        for (int j=0;j<4;j++)
            for (int k=0;k<4;k++)
                rVal->set(i,j,rVal->get(i,j) + get(i,k)*m.get(k,j));
#endif
    return *rVal;
}

void Matrix4x4::transpose(){
#ifdef SSE
    __m128 row1 = _mm_load_ps(&values.data()[0]);
    __m128 row2 = _mm_load_ps(&values.data()[4]);
    __m128 row3 = _mm_load_ps(&values.data()[8]);
    __m128 row4 = _mm_load_ps(&values.data()[12]);
    _MM_TRANSPOSE4_PS(row1, row2, row3, row4);
    _mm_store_ps(&values.data()[0], row1);
    _mm_store_ps(&values.data()[4], row2);
    _mm_store_ps(&values.data()[8], row3);
    _mm_store_ps(&values.data()[12], row4);
#else
    Matrix4x4 m = Matrix4x4(*this);
    for (int i=0;i<4;i++)
        for (int j=0;j<4;j++)
            set(i,j,m.get(j,i));
#endif
}

Matrix4x4& Matrix4x4::transposed() const{
    Matrix4x4* tmp = new Matrix4x4(*this);
    tmp->transpose();
    return *tmp;
}

void Matrix4x4::invert(){
    switch (getType()) {
        case Type::Identity:
            break;
        case Type::Scaling:
            for (int i =0;i<4;i++)
                set(i,i,1/this->get(i,i));
            break;
        case Type::Translation:
            for (int i =0;i<3;i++)
                set(i,3,-this->get(i,3));
            break;
        default:
            throw new std::exception();
            break;
    }
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

Matrix4x4& Matrix4x4::inverted() const{
    Matrix4x4* m = new Matrix4x4(*this);
    m->invert();
    return *m;
}

Matrix4x4& Matrix4x4::translate(float x,float y,float z){
    return *(new Matrix4x4({1,0,0,x,0,1,0,y,0,0,1,z,0,0,0,1}));
}
Matrix4x4& Matrix4x4::scale(float x,float y,float z){
    return *(new Matrix4x4({x,0,0,0,0,y,0,0,0,0,z,0,0,0,0,1}));
}