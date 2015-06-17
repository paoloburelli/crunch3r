//
//  mesh.h
//  crunch3r
//
//  Created by Paolo Burelli on 12/06/15.
//  Copyright (c) 2015 Paolo Burelli. All rights reserved.
//

#ifndef crunch3r_mesh_h
#define crunch3r_mesh_h

#include <exception>
#include <initializer_list>
#include "vector.h"
#include "matrix.h"

namespace crunch3r {

    class Mesh {
    private:
        std::vector<unsigned long> indices;
        std::vector<float> vertices;
    public:
        Mesh(){};
        Mesh(const Mesh&);
        Mesh(std::initializer_list<Vector> vertices,std::initializer_list<unsigned long> indices);
        inline const Vector4 getVertex(unsigned long index) {return Vector4(this->vertices[index*4], this->vertices[index*4+1], this->vertices[index*4+2], this->vertices[index*4+3]);}
        inline void setVertex(unsigned long index,const Vector4 v) {memcpy(&this->vertices[index*4], v.values, 4*sizeof(float));}
        inline void addVertex(Vector4 v) {for (int i = 0;i<4;i++) this->vertices.push_back(v.values[i]);}
        inline const unsigned long getIndex(unsigned long n) {return indices[n];}
        inline void setIndex(unsigned long n, unsigned long index) {indices[n] = index;}
        inline void addIndex(unsigned long index){indices.push_back(index);}
        void addFace(unsigned long, unsigned long, unsigned long);
        Mesh* transform(Matrix4x4);
        static Mesh* loadObj(const char*);
    };
    
}

#endif
