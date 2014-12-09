//
//  main.cpp
//  Headless
//
//  Created by Paolo Burelli on 20/11/14.
//  Copyright (c) 2014 Paolo Burelli. All rights reserved.
//

#include <iostream>
#include "crunch3r.h"
#include "gtest/gtest.h"

using namespace crunch3r;

TEST(Vector,CrossProduct){
    EXPECT_EQ(Vector3::UP, Vector3::cross(Vector::FORWARD, Vector::RIGHT));
}

TEST(Vector,DotProduct){
    EXPECT_FLOAT_EQ(0, Vector::dot(Vector::FORWARD, Vector::RIGHT));
    EXPECT_FLOAT_EQ(1, Vector::dot(Vector::FORWARD, Vector::FORWARD));
    EXPECT_FLOAT_EQ(cosf(M_PI_4), Vector::dot(Vector2(1,0),Vector2(1,1))/sqrtf(2));
    EXPECT_FLOAT_EQ(cosf(M_PI_4), Vector::dot(Vector2(.5f,0),Vector2(.5f,.5f)) / (sqrtf(.5f*.5f + .5f*.5f)*sqrtf(.5f*.5f)));
}

TEST(Vector,Length){
    EXPECT_FLOAT_EQ(1, Vector::FORWARD.magnitude());
    EXPECT_FLOAT_EQ(0, ((Vector2)Vector::FORWARD).magnitude());
    EXPECT_FLOAT_EQ(sqrtf(3), ((Vector3)Vector::ONE).magnitude());
}

TEST(Vector,Normalisation){
    EXPECT_FLOAT_EQ(1, Vector2(1,1).normalise().magnitude());
}

TEST(Vector,Scaling){
    EXPECT_FLOAT_EQ(2, Vector::scale(Vector2(1,0),2).magnitude());
    EXPECT_FLOAT_EQ(.3f, Vector::scale(Vector2(1,0),.3f).magnitude());
}


TEST(Matrix, Allocation){
    EXPECT_FLOAT_EQ(0, Matrix4x4({}).get(3,2));
    EXPECT_FLOAT_EQ(2, Matrix4x4({2,0,0,0,0,2,0,0,0,0,2,0,0,0,0,2}).get(0,0));
    Matrix4x4 m1 = {1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};
    EXPECT_EQ(m1,Matrix4x4({1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1}));
}


TEST(Matrix, Multiplication){
    EXPECT_EQ(Vector3(0,0,2),Matrix4x4({1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1})*Vector3(0,0,2));
    EXPECT_EQ(Vector3(2,6,4),Matrix4x4({2,0,0,0,0,2,0,0,0,0,2,0,0,0,0,1})*Vector3(1,3,2));
    EXPECT_EQ(Vector4(2,6,4,1),Matrix4x4({1,0,0,1,0,1,0,3,0,0,1,2,0,0,0,1})*Vector4(1,3,2,1));
    EXPECT_EQ(Vector3(1,3,2),Matrix4x4({1,0,0,1,0,1,0,3,0,0,1,2,0,0,0,1})*Vector3(1,3,2));
    EXPECT_EQ(Matrix4x4({2,0,0,2,0,2,0,6,0,0,2,4,0,0,0,2}),Matrix4x4::translate(1, 3, 2)*2);
    EXPECT_EQ(Matrix4x4({2,0,0,1,0,2,0,3,0,0,2,2,0,0,0,1}),Matrix4x4::translate(1, 3, 2)*Matrix4x4::scale(2, 2, 2));
}

TEST(Matrix, Transpose){
    EXPECT_EQ(Matrix4x4({1,0,0,0,0,1,0,0,0,0,1,0,10,8,12,1}),Matrix4x4({1,0,0,10,0,1,0,8,0,0,1,12,0,0,0,1}).transpose());
}

TEST(Matrix, TypeCheck){
    EXPECT_EQ(Matrix4x4::Type::Identity,Matrix4x4({1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1}).getType());
    EXPECT_EQ(Matrix4x4::Type::Translation,Matrix4x4({1,0,0,10,0,1,0,8,0,0,1,12,0,0,0,1}).getType());
    EXPECT_EQ(Matrix4x4::Type::Scaling,Matrix4x4({5,0,0,0,0,10,0,0,0,0,2,0,0,0,0,1}).getType());
    EXPECT_EQ(Matrix4x4::Type::Other,Matrix4x4({5,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}).getType());
}

TEST(Matrix, Inversion){
    EXPECT_EQ(Matrix4x4::translate(-4, -5, -6),Matrix4x4::translate(4, 5, 6).invert());
    EXPECT_EQ(Matrix4x4::IDENTITY,Matrix4x4(Matrix4x4::IDENTITY).invert());
    EXPECT_EQ(Matrix4x4({.25f,0,0,0,0,.125f,0,0,0,0,.25f,0,0,0,0,1}),Matrix4x4::scale(4, 8, 4).invert());
}
