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

Mesh::Mesh(std::vector<Vector> vertices, std::vector<unsigned long> indices){
    this->vertices_data = new float[vertices.size()*4];
    this->indices = indices;

    for (int i=0;i<vertices.size();i++){
        memcpy(&this->vertices_data[i*4], vertices[i].values, 4*sizeof(float));
        this->vertices.push_back(Vector(&this->vertices_data[i*4],4,true));
    }
}

Mesh::Mesh(std::initializer_list<Vector> vertices, std::initializer_list<unsigned long> indices){
    this->vertices_data = new float[vertices.size()*4];
    this->indices = indices;
    
    int i = 0;
    for (Vector4 v : vertices) {
        memcpy(&this->vertices_data[i*4], v.values, 4*sizeof(float));
        this->vertices.push_back(Vector(&this->vertices_data[i*4],4,true));
        i++;
    }
}

Mesh::Mesh(const Mesh& other) {
    this->vertices_data = new float[other.vertices.size()*4];
    this->indices = other.indices;
    
    for (int i=0;i<vertices.size();i++){
        memcpy(&this->vertices_data[i*4], other.vertices[i].values, 4*sizeof(float));
        this->vertices.push_back(Vector(&this->vertices_data[i*4],4,true));
    }
}

const Vector4 Mesh::getVertex(unsigned long index){
    return vertices[index];
}

void Mesh::setVertex(unsigned long index, const crunch3r::Vector4 v){
    memcpy(&this->vertices_data[index*4], v.values, 4*sizeof(float));
}

Mesh Mesh::transform(crunch3r::Matrix4x4 transform){
    int i = 0;
    for (Vector4 v : vertices) {
        setVertex(i++, transform*v);
    }
    return *this;
}