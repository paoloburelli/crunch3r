//
//  vector.h
//  Headless
//
//  Created by Paolo Burelli on 20/11/14.
//  Copyright (c) 2014 Paolo Burelli. All rights reserved.
//

#ifndef Crunch3r_Vector_h
#define Crunch3r_Vector_h

#define VECTOR_MAX_SIZE 4
#define VECTOR_MEMORY sizeof(float)*VECTOR_MAX_SIZE

#include <math.h>
#include <exception>
#include <string>
#include <vector>
#include <functional>
#include <algorithm>
#include <initializer_list>
#include <numeric>
#include <cassert>
#include "algebra.h"

namespace  crunch3r {
    
    class Vector {
    friend class Matrix4x4;
        friend class Mesh;
    private:
        bool shallow_copy = false;
        Vector(float *, unsigned int=4, bool=false);
        float* values;
        unsigned int size;
    protected:
        Vector(const Vector& other, unsigned int);
    public:
        Vector() {};
        Vector(const Vector& other);
        Vector(std::initializer_list<float> seq);
        inline ~Vector() {if (!shallow_copy) delete values;};
        inline float magnitude() const { return vector_magnitude(values);};
        inline Vector& normalise(){ vector_normalise(values); return *this;};
        inline unsigned short getDimensions() const {return size;};
        inline Vector& operator-() const {return *(new Vector(vector_negate(values, new float[4]),getDimensions()));};
        inline Vector& operator+(const Vector other) {return *(new Vector(vector_add(values, other.values, new float[4]),getDimensions()));};
        inline Vector& operator-(const Vector other) {return *(new Vector(vector_sub(values, other.values, new float[4]),getDimensions()));};;
        bool operator==(const Vector) const;
        bool operator!=(const Vector) const;
        float& operator[](unsigned int);
        float operator[](unsigned int) const;
        std::string& toString();
        
#ifdef SSE
        __m128 Load() const ;
        void Store(__m128);
#endif
        
    public:
        inline static float dot(const Vector& a, const Vector& b) {return vector_dot(a.values, b.values);};
        inline static Vector& scale(const Vector& a, const Vector& b) {return *(new Vector(vector_scale(a.values, b.values,new float[4]),a.getDimensions()));};
        inline static Vector& scale(const Vector& a, const float b) {return *(new Vector(vector_float_scale(a.values, b,new float[4]),a.getDimensions()));};
        inline static Vector& cross(const Vector& a,const Vector& b) {return *(new Vector(vector_cross(a.values, b.values, new float[4]),a.getDimensions()));};
        
        static const Vector UP;
        static const Vector RIGHT;
        static const Vector FORWARD;
        static const Vector ONE;
        static const Vector ZERO;
        
        enum Coordinate {X,Y,Z,W};
    };
    
    class Vector2 : public Vector {
    public:
        inline Vector2(float x, float y) : Vector({x,y}){} ;
        inline Vector2() : Vector({0,0}){ };
        inline Vector2(const Vector& other) : Vector(other,2){};
    };
    
    class Vector3 : public Vector {
    public:
        inline Vector3(float x,float y, float z) : Vector({x,y,z}){};
        inline Vector3() : Vector({0,0,0}) { };
        inline Vector3(const Vector& other) : Vector(other,3) {};
    };
    
    class Vector4 : public Vector {
    public:
        inline Vector4(float x,float y, float z, float w) : Vector({x,y,z,w}){};
        inline Vector4() : Vector({0,0,0,0}){ };
        inline Vector4(const Vector& other) : Vector(other,4){};
    };
}

#endif
