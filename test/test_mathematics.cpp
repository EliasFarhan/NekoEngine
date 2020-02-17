//
// Created by efarhan on 11/16/19.
//
#ifdef WIN32
#define _USE_MATH_DEFINES
#endif
#include <cmath>
#include <random>
#include <gtest/gtest.h>
#include <mathematics/func_table.h>
#include <mathematics/aabb.h>

const float maxNmb = 100.0f;

void RandomFill(std::vector<float>& numbers, float start = -maxNmb, float end = maxNmb)
{
    std::random_device rd;
    std::mt19937 g(rd());
    std::uniform_real_distribution<float> dist{start, end};
    std::generate(numbers.begin(), numbers.end(), [&g, &dist](){return dist(g);});
}

TEST(Engine, TestMathematics)
{
    neko::FuncTable<float> sinFuncTable(0.0f, M_PI, [](float x){ return sinf(x);});
    sinFuncTable.GenerateTable();
    size_t sampleSize = 1024;
    std::vector<float> localNumbers(sampleSize);
    RandomFill(localNumbers, 0.0f, M_PI);

    float error = 0.0f;
    for(auto v : localNumbers)
    {
        error += sinFuncTable.GetValue(v) - sinf(v);
    }
    error /= float(sampleSize);
    std::cout << "Error margin for sinFuncTable with resolution 512: "<<error<<"\n";
}

TEST(Engine, TestAabb)
{
    neko::Aabb2d aabb1;
    aabb1.FromCenterExtends(neko::Vec2f(0, 0), neko::Vec2f(0.5, 0.5));
    neko::Aabb2d aabb2;
    aabb2.FromCenterExtends(neko::Vec2f(1, 1), neko::Vec2f(0.4, 0.4));
    std::cout << "AABB1 (" << aabb1.lowerBound << " , " << aabb1.upperBound << "); AABB2 (" << aabb2.lowerBound << " , " << aabb2.upperBound << ")  Contains :" << aabb1.ContainsAabb(aabb2) << "  Intersect :" << aabb1.IntersectAabb(aabb2) << "\n";
    neko::Aabb3d aabb3;
    aabb3.FromCenterExtends(neko::Vec3f(0, 0, 0), neko::Vec3f(1, 1, 1));
    neko::Aabb3d aabb4;
    aabb4.FromCenterExtends(neko::Vec3f(1, 1, 1), neko::Vec3f(1, 1, 1));
    std::cout << "AABB1 (" << aabb3.lowerBound << " , " << aabb3.upperBound << "); AABB2 (" << aabb4.lowerBound << " , " << aabb4.upperBound << ")  Contains :" << aabb3.ContainsAabb(aabb4) << "  Intersect :" << aabb3.IntersectAabb(aabb4) << "\n";
    
}