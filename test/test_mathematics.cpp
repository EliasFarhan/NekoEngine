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

#include <mathematics/quaternion.h>
#include <mathematics/matrix.h>
#include "mathematics/vector.h"


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

TEST(Engine, TestQuaternion)
{
    //Variables
    neko::Quaternion quaternionA = neko::Quaternion(0.71, 0, 0, 0.71);
    neko::Quaternion quaternionB = neko::Quaternion(0, 0, 0, 1);
    neko::Quaternion quaternionACopy;

    //Display start variables
    std::cout << "QuaternionA"<<quaternionA<<'\n';
    std::cout << std::endl;
    std::cout <<"QuaternionB"<< quaternionB<<'\n';
    std::cout << std::endl << std::endl;

    //Dot Product Test
    std::cout << "Dot product: " << neko::Quaternion::Dot(quaternionA, quaternionB) << std::endl << std::endl;

    //Normalize Test
    std::cout << "Normalize: ";
    std::cout << "NormalizedQuaternionA" << neko::Quaternion::Normalized(quaternionA) << '\n';
    std::cout << std::endl << std::endl;

    //Magnitude Test
    std::cout << "Magnitude: " << neko::Quaternion::Magnitude(quaternionA) <<'\n'<<'\n';

    //AngleAxis Test
    std::cout << "AngleAxis: " << "Cannot be tested right now"<<'\n'<<'\n';

    //Angle Test
    std::cout << "Angle: " << neko::Quaternion::Angle(quaternionA, quaternionB) <<'\n'<<'\n';

    //Conjugate Test
    quaternionACopy = quaternionA;
    std::cout << "Conjugate: ";
    std::cout <<"QuaternionAConjugate"<< quaternionACopy.Conjugate()<<'\n';
    std::cout <<'\n'<<'\n';

    //Inverse Test
    quaternionACopy = quaternionA;
    std::cout << "Inverse: ";
    std::cout <<"QuaternionAInverse"<< quaternionACopy.Inverse()<<'\n';
    std::cout << std::endl << std::endl;

    //FromEuler
    std::cout << "Euler: " << "Cannot be tested right now" <<'\n'<<'\n';
}
TEST(Engine, TestMatrix4)
{
    neko::Mat4f m1 (std::array<neko::Vec4f,4>
            {
                    neko::Vec4f{1,2,3,4},
                    neko::Vec4f{-1,-2,-3,-4},
                    neko::Vec4f{4,2,2,1},
                    neko::Vec4f{-4,-3,-2,-1}
            });

    neko::Mat4f result = neko::Mat4f(std::array<neko::Vec4f, 4>{
            neko::Vec4f(-5,5,6,-5 ),
            neko::Vec4f(-8,8,5,-3 ),
            neko::Vec4f(-5,5,8,-5 ),
            neko::Vec4f(-5,5,9,-5 )
    });
    result = result.Transpose();
    EXPECT_TRUE(neko::Mat4f::MatrixDifference(m1.MultiplyNaive(m1), result)< 0.01f);
    EXPECT_TRUE(neko::Mat4f::MatrixDifference(m1.MultiplyIntrinsincs(m1), result)<0.01f);
    EXPECT_TRUE(neko::Mat4f::MatrixDifference(m1.MultiplyNaive(m1), m1.MultiplyIntrinsincs(m1))<0.01f);
}


TEST(Engine, TestAabb)
{
    neko::Aabb2d aabb1;
    aabb1.FromCenterExtends(neko::Vec2f(0, 0), neko::Vec2f(0.5, 0.5));
    neko::Aabb2d aabb2;
    aabb2.FromCenterExtends(neko::Vec2f(1, 1), neko::Vec2f(1, 1));
    std::cout << "AABB1 (" << aabb1.lowerLeftBound << " , " << aabb1.upperRightBound << "); AABB2 (" << aabb2.lowerLeftBound << " , " << aabb2.upperRightBound << ")  Contains :" << aabb1.ContainsAabb(aabb2) << "  Intersect :" << aabb1.IntersectAabb(aabb2) << "\n";
    EXPECT_FALSE(aabb1.ContainsAabb(aabb2));
    EXPECT_TRUE(aabb1.IntersectAabb(aabb2));

    neko::Vec2f origin1 = neko::Vec2f(-1, -1);
    neko::Vec2f dir1 = neko::Vec2f(1, 1);
    std::cout << "AABB1 (" << aabb1.lowerLeftBound << " , " << aabb1.upperRightBound << "); Ray (" << origin1 << " , " << dir1 << ")  Intersect :" << aabb1.IntersectRay(dir1, origin1) << "\n";
    EXPECT_TRUE(aabb1.IntersectRay(dir1, origin1));

    neko::radian_t angle = static_cast<neko::radian_t>(neko::PI / 4);
    neko::Obb2d obb1;
    obb1.SetCenterExtendsRot(neko::Vec2f(0, 0), neko::Vec2f(0.5, 0.5), angle);
    neko::Obb2d obb2;
    obb2.SetCenterExtendsRot(neko::Vec2f(1, 1), neko::Vec2f(0.5, 0.5), angle);
    std::cout << "OBB1 (" << obb1.lowerLeftBound << " , " << obb1.upperRightBound << " , " << obb1.rotation << "); OBB2 (" << obb2.lowerLeftBound << " , " << obb2.upperRightBound << " , " << obb2.rotation << ")  Intersect :" << obb1.IntersectObb(obb2) << "\n";
    EXPECT_FALSE(obb1.IntersectObb(obb2));

    aabb1.FromObb(obb1);
    aabb2.FromObb(obb2);
    std::cout << "AABB1 (" << aabb1.lowerLeftBound << " , " << aabb1.upperRightBound << "); AABB2 (" << aabb2.lowerLeftBound << " , " << aabb2.upperRightBound << ")  Contains :" << aabb1.ContainsAabb(aabb2) << "  Intersect :" << aabb1.IntersectAabb(aabb2) << "\n";
    EXPECT_FALSE(aabb1.ContainsAabb(aabb2));
    EXPECT_TRUE(aabb1.IntersectAabb(aabb2));

    neko::Aabb3d aabb3;
    aabb3.FromCenterExtends(neko::Vec3f(0, 0, 0), neko::Vec3f(1, 1, 1));
    neko::Aabb3d aabb4;
    aabb4.FromCenterExtends(neko::Vec3f(1, 1, 1), neko::Vec3f(1, 1, 1));
    std::cout << "AABB1 (" << aabb3.lowerLeftBound << " , " << aabb3.upperRightBound << "); AABB2 (" << aabb4.lowerLeftBound << " , " << aabb4.upperRightBound << ")  Contains :" << aabb3.ContainsAabb(aabb4) << "  Intersect :" << aabb3.IntersectAabb(aabb4) << "\n";
    EXPECT_FALSE(aabb3.ContainsAabb(aabb4));
    EXPECT_TRUE(aabb3.IntersectAabb(aabb4));

    neko::Vec3f origin2 = neko::Vec3f(-2, -2, -2);
    neko::Vec3f dir2 = neko::Vec3f(1, 1, 1);
    std::cout << "AABB1 (" << aabb3.lowerLeftBound << " , " << aabb3.upperRightBound << "); Ray (" << origin2 << " , " << dir2 << ")  Intersect :" << aabb3.IntersectRay(dir2, origin2) << "\n";
    EXPECT_TRUE(aabb3.IntersectRay(dir2, origin2));

    neko::Vec3f origin3 = neko::Vec3f(-2, 0, -2);
    neko::Vec3f normal3 = neko::Vec3f(1, 1, 0);
    std::cout << "AABB1 (" << aabb3.lowerLeftBound << " , " << aabb3.upperRightBound << "); Plane (" << origin3 << " , " << normal3 << ")  Intersect :" << aabb3.IntersectPlane(normal3, origin3) << "\n";
    EXPECT_TRUE(aabb3.IntersectPlane(normal3, origin3));

    neko::Obb2d obb3;
    obb3.SetCenterExtendsRot(neko::Vec2f(0, 0), neko::Vec2f(0.5, 0.5), angle);
    neko::Obb2d obb4;
    obb4.SetCenterExtendsRot(neko::Vec2f(1, 1), neko::Vec2f(0.5, 0.5), angle);
    std::cout << "OBB1 (" << obb3.lowerLeftBound << " , " << obb3.upperRightBound << "); OBB2 (" << obb4.lowerLeftBound << " , " << obb4.upperRightBound << ")  Intersect :" << obb3.IntersectObb(obb4) << "\n";
    //EXPECT_FALSE(obb3.IntersectObb(obb4));

    //aabb3.FromObb(obb3);
    //aabb4.FromObb(obb4);
    //std::cout << "AABB1 (" << aabb3.lowerLeftBound << " , " << aabb3.upperRightBound << "); AABB2 (" << aabb4.lowerLeftBound << " , " << aabb4.upperRightBound << ")  Contains :" << aabb3.ContainsAabb(aabb4) << "  Intersect :" << aabb3.IntersectAabb(aabb4) << "\n";
    //EXPECT_FALSE(aabb1.ContainsAabb(aabb2));
    //EXPECT_TRUE(aabb1.IntersectAabb(aabb2));
}