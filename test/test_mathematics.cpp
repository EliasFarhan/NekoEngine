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
    aabb2.FromCenterExtends(neko::Vec2f(1, 1), neko::Vec2f(0.5, 0.5));
    std::cout << "AABB1 (" << aabb1.lowerLeftBound << " , " << aabb1.upperRightBound << "); AABB2 (" << aabb2.lowerLeftBound << " , " << aabb2.upperRightBound << ")  Contains :" << aabb1.ContainsAabb(aabb2) << "  Intersect :" << aabb1.IntersectAabb(aabb2) << "\n";
    neko::Vec2f origin1 = neko::Vec2f(-1, -1);
    neko::Vec2f dir1 = neko::Vec2f(1, 1);
    std::cout << "AABB1 (" << aabb1.lowerLeftBound << " , " << aabb1.upperRightBound << "); Ray (" << origin1 << " , " << dir1 << ")  Intersect :" << aabb1.IntersectRay(dir1, origin1) << "\n";
    neko::Obb2d obb1;
    obb1.SetCenterExtentRot(neko::Vec2f(0, 0), neko::Vec2f(0.5, 0.5), neko::PI/4);
    neko::Obb2d obb2;
    obb2.SetCenterExtentRot(neko::Vec2f(1, 1), neko::Vec2f(0.5, 0.5), neko::PI/4);
    std::cout << "OBB1 (" << obb1.lowerLeftBound << " , " << obb1.upperRightBound << "); OBB2 (" << obb2.lowerLeftBound << " , " << obb2.upperRightBound << ")  Intersect :" << obb1.IntersectObb(obb2) << "\n";
    aabb1.FromObb(obb1);
    aabb2.FromObb(obb2);
    std::cout << "AABB1 (" << aabb1.lowerLeftBound << " , " << aabb1.upperRightBound << "); AABB2 (" << aabb2.lowerLeftBound << " , " << aabb2.upperRightBound << ")  Contains :" << aabb1.ContainsAabb(aabb2) << "  Intersect :" << aabb1.IntersectAabb(aabb2) << "\n";
    neko::Aabb3d aabb3;
    aabb3.FromCenterExtends(neko::Vec3f(0, 0, 0), neko::Vec3f(1, 1, 1));
    neko::Aabb3d aabb4;
    aabb4.FromCenterExtends(neko::Vec3f(1, 1, 1), neko::Vec3f(1, 1, 1));
    std::cout << "AABB1 (" << aabb3.lowerLeftBound << " , " << aabb3.upperRightBound << "); AABB2 (" << aabb4.lowerLeftBound << " , " << aabb4.upperRightBound << ")  Contains :" << aabb3.ContainsAabb(aabb4) << "  Intersect :" << aabb3.IntersectAabb(aabb4) << "\n";
    neko::Vec3f origin2 = neko::Vec3f(-2, -2, -2);
    neko::Vec3f dir2 = neko::Vec3f(1, 0, 1);
    std::cout << "AABB1 (" << aabb3.lowerLeftBound << " , " << aabb3.upperRightBound << "); Ray (" << origin2 << " , " << dir2 << ")  Intersect :" << aabb3.IntersectRay(dir2, origin2) << "\n";
    neko::Vec3f origin3 = neko::Vec3f(-10, 0, -2);
    neko::Vec3f normal3 = neko::Vec3f(1, 1, 0);
    std::cout << "AABB1 (" << aabb3.lowerLeftBound << " , " << aabb3.upperRightBound << "); Plane (" << origin3 << " , " << normal3 << ")  Intersect :" << aabb3.IntersectPlane(normal3, origin3) << "\n";
    neko::Obb2d obb3;
    obb3.SetCenterExtentRot(neko::Vec2f(0, 0), neko::Vec2f(0.5, 0.5), neko::PI / 4);
    neko::Obb2d obb4;
    obb4.SetCenterExtentRot(neko::Vec2f(1, 1), neko::Vec2f(0.5, 0.5), neko::PI / 4);
    std::cout << "OBB1 (" << obb3.lowerLeftBound << " , " << obb3.upperRightBound << "); OBB2 (" << obb4.lowerLeftBound << " , " << obb4.upperRightBound << ")  Intersect :" << obb3.IntersectObb(obb4) << "\n";
    //aabb3.FromObb(obb3);
    //aabb4.FromObb(obb4);
    //std::cout << "AABB1 (" << aabb3.lowerLeftBound << " , " << aabb3.upperRightBound << "); AABB2 (" << aabb4.lowerLeftBound << " , " << aabb4.upperRightBound << ")  Contains :" << aabb3.ContainsAabb(aabb4) << "  Intersect :" << aabb3.IntersectAabb(aabb4) << "\n";

}