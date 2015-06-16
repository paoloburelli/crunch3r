//
//  mesh.cpp
//  crunch3r
//
//  Created by Paolo Burelli on 12/06/15.
//  Copyright (c) 2015 Paolo Burelli. All rights reserved.
//

#include <string.h>
#include "mesh.h"
#include "algebra.h"

using namespace crunch3r;

Mesh::Mesh(std::initializer_list<Vector> vertices, std::initializer_list<unsigned long> indices){
    this->indices = indices;
    for (Vector4 v : vertices)
        for (int i = 0;i<4;i++)
            this->vertices.push_back(v.values[i]);
}

Mesh::Mesh(const Mesh& other) {
    this->vertices = other.vertices;
    this->indices = other.indices;
}

Mesh* Mesh::transform(crunch3r::Matrix4x4 transform){
    for (int i=0;i<this->vertices.size()/4;i++)
        matrix_vector_product(transform.values, &this->vertices[i*4], &this->vertices[i*4]);
    return this;
}