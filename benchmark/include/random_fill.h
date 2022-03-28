#pragma once

#include "mathematics/vec4.h"
#include "mathematics/matrix4.h"


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

void RandomFill(neko::Mat4f& m)
{
    std::array<neko::Vec4f, 4> v;
    for(auto& vec : v)
    {
        RandomFill(vec);
    }
    m = neko::Mat4f(v);
}