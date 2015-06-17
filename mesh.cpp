//
//  mesh.cpp
//  crunch3r
//
//  Created by Paolo Burelli on 12/06/15.
//  Copyright (c) 2015 Paolo Burelli. All rights reserved.
//

#include <string.h>
#include <stdio.h>
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

void Mesh::addFace(unsigned long a, unsigned long b, unsigned long c){
    indices.push_back(a);
    indices.push_back(b);
    indices.push_back(c);
}

Mesh* Mesh::transform(crunch3r::Matrix4x4 transform){
    for (int i=0;i<this->vertices.size()/4;i++)
        matrix_vector_product(transform.values, &this->vertices[i*4], &this->vertices[i*4]);
    return this;
}

Mesh* Mesh::loadObj(const char *filepath){
    Mesh* rValue = new Mesh();
    FILE *obj = fopen(filepath, "r");
    char line[LINE_MAX];
    float x,y,z,w;
    unsigned long a,b,c;
    
    if (obj != NULL)
        while (fgets(line , sizeof line, obj) != NULL){
            w = 1;
            if (sscanf(line,"v %f %f %f %f", &x,&y,&z,&w) >=3)
                rValue->addVertex({x,y,z,w});
            
            if (sscanf(line,"f %lu %lu %lu", &a,&b,&c) >=3)
                rValue->addFace(a-1, b-1, c-1);
        }
    
    fclose(obj);
    return rValue;
}