#pragma once

#include "mathematics/vector.h"
#include "mathematics/matrix.h"
#include "mathematics/quaternion.h"


float RandomFloat()
{
    static std::random_device rd;
    static std::mt19937 g(rd());
    static std::uniform_real_distribution<float> dist{-100.0f, 100.0f};

    return dist(g);
}

void RandomFill(neko::Vec4f& v)
{
    for(int i = 0; i<4;i++)
    {
        v[i] = RandomFloat();
    }
}

void RandomFill(neko::Vec3f& v)
{
    for (int i = 0; i < 3; i++)
    {
        v[i] = RandomFloat();
    }
}

void RandomFill(neko::Mat4f& m)
{
    std::array<neko::Vec4f, 4> v;
    for(auto& vec : v)
    {
        RandomFill(vec);
    }
    m = neko::Mat4f(v);
}

void RandomFill(neko::Quaternion& q)
{
    neko::Vec4f v = neko::Vec4f::zero;
    RandomFill(v);
    q = neko::Quaternion(v);
}

void RandomFill(neko::IntrinsicsQuaternion q)
{
    neko::FourVec4f v;

    q = neko::IntrinsicsQuaternion(v);
}

/*void RandomFill(neko::Vec4fQuaternion& q)
{
    neko::Vec4f v = neko::Vec4f::zero;
    RandomFill(v);
    q = neko::Vec4fQuaternion(v);
}*/