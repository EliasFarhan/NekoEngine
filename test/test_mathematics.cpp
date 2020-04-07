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
	std::uniform_real_distribution<float> dist{ start, end };
	std::generate(numbers.begin(), numbers.end(), [&g, &dist]() {return dist(g); });
}

TEST(Engine, TestSinTable)
{
	neko::FuncTable<float> sinFuncTable(0.0f, neko::PI, [](float x) { return neko::Sin(neko::radian_t(x)); });
	sinFuncTable.GenerateTable();
	const size_t sampleSize = 1024;
	std::vector<float> localNumbers(sampleSize);
	RandomFill(localNumbers, 0.0f, M_PI);

	float error = 0.0f;
	for (auto v : localNumbers)
	{
		error += std::abs(sinFuncTable.GetValue(v) - neko::Sin(neko::radian_t(v)));
	}
	error /= float(sampleSize);
	EXPECT_LT(error, 0.01f);
}

TEST(Engine, Quaternion_Dot)
{
    neko::Quaternion q1 = neko::Quaternion(1,0,0,0);
    neko::Quaternion q2 = neko::Quaternion(1,0,0,0);
    float expectedDot = 1;
    float dotQ1Q2 = neko::Quaternion::Dot(q1, q2);
    EXPECT_TRUE(expectedDot - dotQ1Q2 < 0.001f);
}

TEST(Engine, Quaternion_Normalized)
{
    neko::Quaternion q = neko::Quaternion(3.33f, 0, 0, 0);
    neko::Quaternion expectedNormalized = neko::Quaternion(1,0,0,0);
    neko::Quaternion normalized = neko::Quaternion::Normalized(q);
    EXPECT_EQ(expectedNormalized, normalized);
}

TEST(Engine, Quaternion_Magnitude)
{
    neko::Quaternion q = neko::Quaternion(1, 0, 0, 0);
    float expectedMagnitude = 1;
    float magnitude = neko::Quaternion::Magnitude(q);
    EXPECT_TRUE(expectedMagnitude - magnitude < 0.001f);
}

TEST(Engine, Quaternion_AngleAxis)
{
    neko::Quaternion q = neko::Quaternion::Identity();
    neko::radian_t rad(30);
    neko::Vec3f axis(1, 1, 1);
    neko::Quaternion expectedAngleAxisQuaternion = neko::Quaternion(0, 0, 0, 1);    //TODO: Calculate the expected value
    q = q.AngleAxis(rad, axis);
    //EXPECT_EQ(q, expectedAngleAxisQuaternion);
}

TEST(Engine, Quaternion_Angle)
{
    neko::Quaternion q1 = neko::Quaternion::Identity();
    neko::Quaternion q2 = neko::Quaternion::Identity();
    neko::degree_t expectedAngle(0);
    neko::degree_t angle(neko::Quaternion::Angle(q1, q2));
    EXPECT_EQ(expectedAngle, angle);
}

TEST(Engine, Quaternion_Conjugate)
{
    neko::Quaternion q = neko::Quaternion(8, 10, 888, 2);
    neko::Quaternion expectedQuaternion = neko::Quaternion(-8, -10, -888, 2);
    q = q.Conjugate();
    EXPECT_EQ(q, expectedQuaternion);
}

TEST(Engine, Quaternion_Inverse)
{
    neko::Quaternion q = neko::Quaternion(1, 0.5, 0.5, 0);
    neko::Quaternion expectedInverse = neko::Quaternion(-0.6666666f, -0.3333333f, -0.3333333f, 0);
    q = q.Inverse();
    EXPECT_TRUE(q.x - expectedInverse.x < 0.001f && q.y - expectedInverse.y < 0.001f && q.z - expectedInverse.z < 0.001f && q.w - expectedInverse.w < 0.001f);
}

TEST(Engine, Quaternion_FromEuler)
{
    //TODO
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
    obb1.FromCenterExtendsRotation(neko::Vec2f(0, 0), neko::Vec2f(0.5, 0.5), angle);
    neko::Obb2d obb2;
    obb2.FromCenterExtendsRotation(neko::Vec2f(1, 1), neko::Vec2f(0.5, 0.5), angle);
    std::cout << "OBB1 (" << obb1.localLowerLeftBound << " , " << obb1.localUpperRightBound << " , " << obb1.rotation << "); OBB2 (" << obb2.localLowerLeftBound << " , " << obb2.localUpperRightBound << " , " << obb2.rotation << ")  Intersect :" << obb1.IsOverlapping(obb2) << "\n";
    EXPECT_FALSE(obb1.IsOverlapping(obb2));

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

    neko::RadianAngles angles = neko::RadianAngles(static_cast<neko::radian_t>(0), static_cast<neko::radian_t>(0), static_cast<neko::radian_t>(neko::PI / 4));
    neko::Obb3d obb3;
    obb3.FromCenterExtendsRotation(neko::Vec3f(0, 0, 0), neko::Vec3f(0.5, 0.5, 0.5), angles);
    neko::Obb3d obb4;
    obb4.FromCenterExtendsRotation(neko::Vec3f(1, 1, 1), neko::Vec3f(0.5, 0.5, 0.5), angles);
    std::cout << "OBB1 (" << obb3.localLowerLeftBound << " , " << obb3.localUpperRightBound << "); OBB2 (" << obb4.localLowerLeftBound << " , " << obb4.localUpperRightBound << ")  Intersect :" << obb3.IsOverlapping(obb4) << "\n";
    EXPECT_FALSE(obb3.IsOverlapping(obb4));

    aabb3.FromObb(obb3);
    aabb4.FromObb(obb4);
    std::cout << "AABB1 (" << aabb3.lowerLeftBound << " , " << aabb3.upperRightBound << "); AABB2 (" << aabb4.lowerLeftBound << " , " << aabb4.upperRightBound << ")  Contains :" << aabb3.ContainsAabb(aabb4) << "  Intersect :" << aabb3.IntersectAabb(aabb4) << "\n";
    EXPECT_FALSE(aabb1.ContainsAabb(aabb2));
    EXPECT_TRUE(aabb1.IntersectAabb(aabb2));
}

    TEST(Engine, TestMatrix4)
    {
        neko::Mat4f m1(std::array<neko::Vec4f, 4>
        {
            neko::Vec4f{ 1,2,3,4 },
                neko::Vec4f{ -1,-2,-3,-4 },
                neko::Vec4f{ 4,2,2,1 },
                neko::Vec4f{ -4,-3,-2,-1 }
        });

        neko::Mat4f result = neko::Mat4f(std::array<neko::Vec4f, 4>{
            neko::Vec4f(-5, 5, 6, -5),
                neko::Vec4f(-8, 8, 5, -3),
                neko::Vec4f(-5, 5, 8, -5),
                neko::Vec4f(-5, 5, 9, -5)
        });
        result = result.Transpose();
        EXPECT_LT(neko::Mat4f::MatrixDifference(m1.MultiplyNaive(m1), result), 0.01f);
        EXPECT_LT(neko::Mat4f::MatrixDifference(m1.MultiplyIntrinsincs(m1), result), 0.01f);
        EXPECT_LT(neko::Mat4f::MatrixDifference(m1.MultiplyNaive(m1), m1.MultiplyIntrinsincs(m1)), 0.01f);
    }