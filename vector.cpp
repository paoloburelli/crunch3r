//
//  vector.cpp
//  Headless
//
//  Created by Paolo Burelli on 20/11/14.
//  Copyright (c) 2014 Paolo Burelli. All rights reserved.
//

#include "vector.h"

using namespace crunch3r;

Vector::Vector(std::initializer_list<float> seq){
    size = seq.size() > VECTOR_MAX_SIZE ? VECTOR_MAX_SIZE : (unsigned int)seq.size();
    
    values = new float[VECTOR_MAX_SIZE];
    memset(values, 0, VECTOR_MEMORY);
    
    int i =0;
    for (float f: seq)
        if (i<VECTOR_MAX_SIZE)
            values[i++] = f;
}

Vector::Vector(float*f,unsigned int dimensions){
    values = f;
    size = dimensions;
}

Vector::Vector(const Vector& other){
    values = new float[VECTOR_MAX_SIZE];
    memcpy(values, other.values, VECTOR_MEMORY);
    size = other.getDimensions();
}

Vector::Vector(const Vector& other, unsigned int size){
    values = new float[VECTOR_MAX_SIZE];
    memset(values, 0, VECTOR_MEMORY);
    memcpy(values, other.values, sizeof(float)*size);
    this->size = size;
}

bool Vector::operator==(const Vector other) const{
    ///TODO: float comparison
    return memcmp(values, other.values, VECTOR_MEMORY) == 0;
}

bool Vector::operator!=(const Vector other) const {
    ///TODO: float comparison
    return memcmp(values, other.values, VECTOR_MEMORY) != 0;
}


float& Vector::operator[](const unsigned int i){
    assert(i<VECTOR_MAX_SIZE);
    return values[i];
}

float Vector::operator[](const unsigned int i) const{
    assert(i<VECTOR_MAX_SIZE);
    return values[i];
}

#ifdef SSE
__m128 Vector::Load() const{
    return _mm_load_ps(values);
}
void Vector::Store(__m128 m){
    _mm_store_ps(values, m);
}
#endif

std::string& Vector::toString(){
    std::string *s = new std::string("(");
    
    for (int i=0;i<getDimensions();i++){
        s->append(std::to_string(values[i]));
        if (i == getDimensions() -1)
            s->append(")");
        else
            s->append(",");
    }
    
    return *s;
}

const Vector Vector::UP = {0,1,0,0};
const Vector Vector::RIGHT = {1,0,0,0};
const Vector Vector::FORWARD = {0,0,1,0};
const Vector Vector::ZERO = {0,0,0,0};
const Vector Vector::ONE = {1,1,1,1};
