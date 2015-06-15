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
        Mesh(std::vector<Vector> vertices, std::vector<unsigned long> indices);
        std::vector<Vector4> vertices;
        std::vector<unsigned long> indices;
        float* vertices_data;
    public:
        Mesh(const Mesh&);
        Mesh(std::initializer_list<Vector> vertices,std::initializer_list<unsigned long> indices);
        const Vector4 getVertex(unsigned long);
        void setVertex(unsigned long index,const Vector4 v);
        const unsigned long getIndex(unsigned long n) { return indices[n];}
        void setIndex(unsigned long n, unsigned long index) { indices[n] = index;}
        Mesh transform(Matrix4x4);
        inline ~Mesh() {delete vertices_data;};
    };
    
}

#endif
