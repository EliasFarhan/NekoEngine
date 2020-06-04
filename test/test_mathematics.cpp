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

TEST(Aabb, Aabb2d_Aabb2d)
{
    //Same Aabb
    neko::Aabb2d aabb1;
    aabb1.FromCenterExtends(neko::Vec2f(0.0f, 0.0f), neko::Vec2f(0.5f, 0.5f));
    neko::Aabb2d aabb2;
    aabb2.FromCenterExtends(neko::Vec2f(0.0f, 0.0f), neko::Vec2f(0.5f, 0.5f));
    EXPECT_TRUE(aabb1.ContainsAabb(aabb2));
    EXPECT_TRUE(aabb1.IntersectAabb(aabb2));
    EXPECT_TRUE(aabb2.ContainsAabb(aabb1));
    EXPECT_TRUE(aabb2.IntersectAabb(aabb1));

    //Null Intersect
    aabb2.FromCenterExtends(neko::Vec2f(0.0f, 0.0f), neko::Vec2f(0.0f, 0.0f));
    EXPECT_TRUE(aabb1.ContainsAabb(aabb2));
    EXPECT_TRUE(aabb1.IntersectAabb(aabb2));
    EXPECT_FALSE(aabb2.ContainsAabb(aabb1));
    EXPECT_TRUE(aabb2.IntersectAabb(aabb1));

    //Null Limit Intersect
    aabb2.FromCenterExtends(neko::Vec2f(0.5f, 0.5f), neko::Vec2f(0.0f, 0.0f));
    EXPECT_TRUE(aabb1.ContainsAabb(aabb2));
    EXPECT_TRUE(aabb1.IntersectAabb(aabb2));
    EXPECT_FALSE(aabb2.ContainsAabb(aabb1));
    EXPECT_TRUE(aabb2.IntersectAabb(aabb1));

    //Null Not Intersect
    aabb2.FromCenterExtends(neko::Vec2f(5.0f, 0.0f), neko::Vec2f(0.0f, 0.0f));
    EXPECT_FALSE(aabb1.ContainsAabb(aabb2));
    EXPECT_FALSE(aabb1.IntersectAabb(aabb2));
    EXPECT_FALSE(aabb2.ContainsAabb(aabb1));
    EXPECT_FALSE(aabb2.IntersectAabb(aabb1));

    //Basic Intersect
    aabb2.FromCenterExtends(neko::Vec2f(0.5f, 0.0f), neko::Vec2f(0.5f, 0.5f));
    EXPECT_FALSE(aabb1.ContainsAabb(aabb2));
    EXPECT_TRUE(aabb1.IntersectAabb(aabb2));
    EXPECT_FALSE(aabb2.ContainsAabb(aabb1));
    EXPECT_TRUE(aabb2.IntersectAabb(aabb1));

    //Basic Contains
    aabb2.FromCenterExtends(neko::Vec2f(0.1f, 0.1f), neko::Vec2f(0.1f, 0.1f));
    EXPECT_TRUE(aabb1.ContainsAabb(aabb2));
    EXPECT_TRUE(aabb1.IntersectAabb(aabb2));
    EXPECT_FALSE(aabb2.ContainsAabb(aabb1));
    EXPECT_TRUE(aabb2.IntersectAabb(aabb1));

    //Limit Intersect
    aabb2.FromCenterExtends(neko::Vec2f(1.0f, 0.0f), neko::Vec2f(0.5f, 0.5f));
    EXPECT_FALSE(aabb1.ContainsAabb(aabb2));
    EXPECT_TRUE(aabb1.IntersectAabb(aabb2));
    EXPECT_FALSE(aabb2.ContainsAabb(aabb1));
    EXPECT_TRUE(aabb2.IntersectAabb(aabb1));

    //Not Intersect
    aabb2.FromCenterExtends(neko::Vec2f(1.5f, 0.0f), neko::Vec2f(0.5f, 0.5f));
    EXPECT_FALSE(aabb1.ContainsAabb(aabb2));
    EXPECT_FALSE(aabb1.IntersectAabb(aabb2));
    EXPECT_FALSE(aabb2.ContainsAabb(aabb1));
    EXPECT_FALSE(aabb2.IntersectAabb(aabb1));

    //Basic Intersect Positive
    aabb2.FromCenterExtends(neko::Vec2f(1.0f, 1.0f), neko::Vec2f(1.0f, 1.0f));
    EXPECT_FALSE(aabb1.ContainsAabb(aabb2));
    EXPECT_TRUE(aabb1.IntersectAabb(aabb2));
    EXPECT_FALSE(aabb2.ContainsAabb(aabb1));
    EXPECT_TRUE(aabb2.IntersectAabb(aabb1));

    //Basic Intersect Negative
    aabb2.FromCenterExtends(neko::Vec2f(-1.0f, -1.0f), neko::Vec2f(1.0f, 1.0f));
    EXPECT_FALSE(aabb1.ContainsAabb(aabb2));
    EXPECT_TRUE(aabb1.IntersectAabb(aabb2));
    EXPECT_FALSE(aabb2.ContainsAabb(aabb1));
    EXPECT_TRUE(aabb2.IntersectAabb(aabb1));

    //Limit Intersect Positive
    aabb2.FromCenterExtends(neko::Vec2f(1.5f, 1.5f), neko::Vec2f(1.0f, 1.0f));
    EXPECT_FALSE(aabb1.ContainsAabb(aabb2));
    EXPECT_TRUE(aabb1.IntersectAabb(aabb2));
    EXPECT_FALSE(aabb2.ContainsAabb(aabb1));
    EXPECT_TRUE(aabb2.IntersectAabb(aabb1));

    //Limit Intersect Negative
    aabb2.FromCenterExtends(neko::Vec2f(-1.5f, -1.5f), neko::Vec2f(1.0f, 1.0f));
    EXPECT_FALSE(aabb1.ContainsAabb(aabb2));
    EXPECT_TRUE(aabb1.IntersectAabb(aabb2));
    EXPECT_FALSE(aabb2.ContainsAabb(aabb1));
    EXPECT_TRUE(aabb2.IntersectAabb(aabb1));

    //Not Intersect Positive
    aabb2.FromCenterExtends(neko::Vec2f(2.0f, 2.0f), neko::Vec2f(1.0f, 1.0f));
    EXPECT_FALSE(aabb1.ContainsAabb(aabb2));
    EXPECT_FALSE(aabb1.IntersectAabb(aabb2));
    EXPECT_FALSE(aabb2.ContainsAabb(aabb1));
    EXPECT_FALSE(aabb2.IntersectAabb(aabb1));

    //Not Intersect Negative
    aabb2.FromCenterExtends(neko::Vec2f(-2.0f, -2.0f), neko::Vec2f(1.0f, 1.0f));
    EXPECT_FALSE(aabb1.ContainsAabb(aabb2));
    EXPECT_FALSE(aabb1.IntersectAabb(aabb2));
    EXPECT_FALSE(aabb2.ContainsAabb(aabb1));
    EXPECT_FALSE(aabb2.IntersectAabb(aabb1));

    //Transverse Intersect
    aabb2.FromCenterExtends(neko::Vec2f(-5.0, 0.1f), neko::Vec2f(10.0f, 0.1f));
    EXPECT_FALSE(aabb1.ContainsAabb(aabb2));
    EXPECT_TRUE(aabb1.IntersectAabb(aabb2));
    EXPECT_FALSE(aabb2.ContainsAabb(aabb1));
    EXPECT_TRUE(aabb2.IntersectAabb(aabb1));
}

TEST(Aabb, Aabb3d_Aabb3d)
{
    //Same Aabb
    neko::Aabb3d aabb1;
    aabb1.FromCenterExtends(neko::Vec3f(0.0f, 0.0f, 0.0f), neko::Vec3f(0.5f, 0.5f, 0.5f));
    neko::Aabb3d aabb2;
    aabb2.FromCenterExtends(neko::Vec3f(0.0f, 0.0f, 0.0f), neko::Vec3f(0.5f, 0.5f, 0.5f));
    EXPECT_TRUE(aabb1.ContainsAabb(aabb2));
    EXPECT_TRUE(aabb1.IntersectAabb(aabb2));
    EXPECT_TRUE(aabb2.ContainsAabb(aabb1));
    EXPECT_TRUE(aabb2.IntersectAabb(aabb1));

    //Null Intersect
    aabb2.FromCenterExtends(neko::Vec3f(0.0f, 0.0f, 0.0f), neko::Vec3f(0.0f, 0.0f, 0.0f));
    EXPECT_TRUE(aabb1.ContainsAabb(aabb2));
    EXPECT_TRUE(aabb1.IntersectAabb(aabb2));
    EXPECT_FALSE(aabb2.ContainsAabb(aabb1));
    EXPECT_TRUE(aabb2.IntersectAabb(aabb1));

    //Null Limit Intersect
    aabb2.FromCenterExtends(neko::Vec3f(0.5f, 0.5f, 0.0f), neko::Vec3f(0.0f, 0.0f, 0.0f));
    EXPECT_TRUE(aabb1.ContainsAabb(aabb2));
    EXPECT_TRUE(aabb1.IntersectAabb(aabb2));
    EXPECT_FALSE(aabb2.ContainsAabb(aabb1));
    EXPECT_TRUE(aabb2.IntersectAabb(aabb1));

    //Null Not Intersect
    aabb2.FromCenterExtends(neko::Vec3f(5.0f, 0.0f, 0.0f), neko::Vec3f(0.0f, 0.0f, 0.0f));
    EXPECT_FALSE(aabb1.ContainsAabb(aabb2));
    EXPECT_FALSE(aabb1.IntersectAabb(aabb2));
    EXPECT_FALSE(aabb2.ContainsAabb(aabb1));
    EXPECT_FALSE(aabb2.IntersectAabb(aabb1));

    //Basic Intersect
    aabb2.FromCenterExtends(neko::Vec3f(0.5f, 0.0f, 0.0f), neko::Vec3f(0.5f, 0.5f, 0.5f));
    EXPECT_FALSE(aabb1.ContainsAabb(aabb2));
    EXPECT_TRUE(aabb1.IntersectAabb(aabb2));
    EXPECT_FALSE(aabb2.ContainsAabb(aabb1));
    EXPECT_TRUE(aabb2.IntersectAabb(aabb1));

    //Basic Contains
    aabb2.FromCenterExtends(neko::Vec3f(0.1f, 0.1f, 0.1f), neko::Vec3f(0.1f, 0.1f, 0.1f));
    EXPECT_TRUE(aabb1.ContainsAabb(aabb2));
    EXPECT_TRUE(aabb1.IntersectAabb(aabb2));
    EXPECT_FALSE(aabb2.ContainsAabb(aabb1));
    EXPECT_TRUE(aabb2.IntersectAabb(aabb1));

    //Limit Intersect
    aabb2.FromCenterExtends(neko::Vec3f(1.0f, 0.0f, 0.0f), neko::Vec3f(0.5f, 0.5f, 0.5f));
    EXPECT_FALSE(aabb1.ContainsAabb(aabb2));
    EXPECT_TRUE(aabb1.IntersectAabb(aabb2));
    EXPECT_FALSE(aabb2.ContainsAabb(aabb1));
    EXPECT_TRUE(aabb2.IntersectAabb(aabb1));

    //Not Intersect
    aabb2.FromCenterExtends(neko::Vec3f(1.5f, 0.0f, 0.0f), neko::Vec3f(0.5f, 0.5f, 0.5f));
    EXPECT_FALSE(aabb1.ContainsAabb(aabb2));
    EXPECT_FALSE(aabb1.IntersectAabb(aabb2));
    EXPECT_FALSE(aabb2.ContainsAabb(aabb1));
    EXPECT_FALSE(aabb2.IntersectAabb(aabb1));

    //Basic Intersect Positive
    aabb2.FromCenterExtends(neko::Vec3f(1.0f, 1.0f, 1.0f), neko::Vec3f(1.0f, 1.0f, 1.0f));
    EXPECT_FALSE(aabb1.ContainsAabb(aabb2));
    EXPECT_TRUE(aabb1.IntersectAabb(aabb2));
    EXPECT_FALSE(aabb2.ContainsAabb(aabb1));
    EXPECT_TRUE(aabb2.IntersectAabb(aabb1));

    //Basic Intersect Negative
    aabb2.FromCenterExtends(neko::Vec3f(-1.0f, -1.0f, -1.0f), neko::Vec3f(1.0f, 1.0f, 1.0f));
    EXPECT_FALSE(aabb1.ContainsAabb(aabb2));
    EXPECT_TRUE(aabb1.IntersectAabb(aabb2));
    EXPECT_FALSE(aabb2.ContainsAabb(aabb1));
    EXPECT_TRUE(aabb2.IntersectAabb(aabb1));

    //Limit Intersect Positive
    aabb2.FromCenterExtends(neko::Vec3f(1.5f, 1.5f, 1.5f), neko::Vec3f(1.0f, 1.0f, 1.0f));
    EXPECT_FALSE(aabb1.ContainsAabb(aabb2));
    EXPECT_TRUE(aabb1.IntersectAabb(aabb2));
    EXPECT_FALSE(aabb2.ContainsAabb(aabb1));
    EXPECT_TRUE(aabb2.IntersectAabb(aabb1));

    //Limit Intersect Negative
    aabb2.FromCenterExtends(neko::Vec3f(-1.5f, -1.5f, -1.5f), neko::Vec3f(1.0f, 1.0f, 1.0f));
    EXPECT_FALSE(aabb1.ContainsAabb(aabb2));
    EXPECT_TRUE(aabb1.IntersectAabb(aabb2));
    EXPECT_FALSE(aabb2.ContainsAabb(aabb1));
    EXPECT_TRUE(aabb2.IntersectAabb(aabb1));

    //Not Intersect Positive
    aabb2.FromCenterExtends(neko::Vec3f(2.0f, 2.0f, 2.0f), neko::Vec3f(1.0f, 1.0f, 1.0f));
    EXPECT_FALSE(aabb1.ContainsAabb(aabb2));
    EXPECT_FALSE(aabb1.IntersectAabb(aabb2));
    EXPECT_FALSE(aabb2.ContainsAabb(aabb1));
    EXPECT_FALSE(aabb2.IntersectAabb(aabb1));

    //Not Intersect Negative
    aabb2.FromCenterExtends(neko::Vec3f(-2.0f, -2.0f, -2.0f), neko::Vec3f(1.0f, 1.0f, 1.0f));
    EXPECT_FALSE(aabb1.ContainsAabb(aabb2));
    EXPECT_FALSE(aabb1.IntersectAabb(aabb2));
    EXPECT_FALSE(aabb2.ContainsAabb(aabb1));
    EXPECT_FALSE(aabb2.IntersectAabb(aabb1));

    //Transverse Intersect
    aabb2.FromCenterExtends(neko::Vec3f(-5.0, 0.1f, 0.1f), neko::Vec3f(10.0f, 0.1f, 0.1f));
    EXPECT_FALSE(aabb1.ContainsAabb(aabb2));
    EXPECT_TRUE(aabb1.IntersectAabb(aabb2));
    EXPECT_FALSE(aabb2.ContainsAabb(aabb1));
    EXPECT_TRUE(aabb2.IntersectAabb(aabb1));
}

TEST(Aabb, Aabb2d_Ray)
{
    neko::Aabb2d aabb1;
    aabb1.FromCenterExtends(neko::Vec2f(0.0f, 0.0f), neko::Vec2f(0.5f, 0.5f));
    //Inside Intersect
    neko::Vec2f origin1 = neko::Vec2f(-0.1f, -0.1f);
    neko::Vec2f dir1 = neko::Vec2f(1.0f, 1.0f);
    EXPECT_TRUE(aabb1.IntersectRay(dir1, origin1));

    //Basic Intersect
    origin1 = neko::Vec2f(-1.0f, -1.0f);
    dir1 = neko::Vec2f(1.0f, 1.0f);
    EXPECT_TRUE(aabb1.IntersectRay(dir1, origin1));

    //Limit Intersect
    origin1 = neko::Vec2f(-0.5f, -1.5f);
    EXPECT_TRUE(aabb1.IntersectRay(dir1, origin1));

    //Not Intersect
    origin1 = neko::Vec2f(-0.5f, -2.0f);
    EXPECT_FALSE(aabb1.IntersectRay(dir1, origin1));

    //Not 0,0
    aabb1.FromCenterExtends(neko::Vec2f(2.0f, 2.0f), neko::Vec2f(1.0f, 1.0f));

    //Basic Intersect
    origin1 = neko::Vec2f(-5.0f, 2.0f);
    dir1 = neko::Vec2f(1.0f, 0.0f);
    EXPECT_TRUE(aabb1.IntersectRay(dir1, origin1));

    //Limit Intersect
    origin1 = neko::Vec2f(-5.0f, 1.0f);
    EXPECT_TRUE(aabb1.IntersectRay(dir1, origin1));

    //Not Intersect
    origin1 = neko::Vec2f(-5.0f, 0.0f);
    EXPECT_FALSE(aabb1.IntersectRay(dir1, origin1));

    //Not Intersect
    origin1 = neko::Vec2f(-5.0f, 2.0f);
    dir1 = neko::Vec2f(-1.0f, 0.0f);
    EXPECT_FALSE(aabb1.IntersectRay(dir1, origin1));
}

TEST(Aabb, Aabb3d_Ray)
{
    neko::Aabb3d aabb1;
    aabb1.FromCenterExtends(neko::Vec3f(0.0f, 0.0f, 0.0f), neko::Vec3f(0.5f, 0.5f, 0.5f));
    //Inside Intersect
    neko::Vec3f origin1 = neko::Vec3f(-0.1f, -0.1f, -0.1f);
    neko::Vec3f dir1 = neko::Vec3f(1.0f, 1.0f, 1.0f);
    EXPECT_TRUE(aabb1.IntersectRay(dir1, origin1));

    //Basic Intersect
    origin1 = neko::Vec3f(-1.0f, -1.0f, -1.0f);
    dir1 = neko::Vec3f(1.0f, 1.0f, 1.0f);
    EXPECT_TRUE(aabb1.IntersectRay(dir1, origin1));

    //Limit Intersect
    origin1 = neko::Vec3f(-0.5f, -1.5f, -1.5f);
    EXPECT_TRUE(aabb1.IntersectRay(dir1, origin1));

    //Not Intersect
    origin1 = neko::Vec3f(-0.5f, -2.0f, -2.0f);
    EXPECT_FALSE(aabb1.IntersectRay(dir1, origin1));

    //Not 0,0
    aabb1.FromCenterExtends(neko::Vec3f(2.0f, 2.0f, 2.0f), neko::Vec3f(1.0f, 1.0f, 1.0f));

    //Basic Intersect
    origin1 = neko::Vec3f(-5.0f, 2.0f, 2.0f);
    dir1 = neko::Vec3f(1.0f, 0.0f, 0.0f);
    EXPECT_TRUE(aabb1.IntersectRay(dir1, origin1));

    //Limit Intersect
    origin1 = neko::Vec3f(-5.0f, 1.0f, 1.0f);
    EXPECT_TRUE(aabb1.IntersectRay(dir1, origin1));

    //Not Intersect
    origin1 = neko::Vec3f(-5.0f, 0.0f, 0.0f);
    EXPECT_FALSE(aabb1.IntersectRay(dir1, origin1));

    //Not Intersect
    origin1 = neko::Vec3f(-5.0f, 2.0f, 2.0f);
    dir1 = neko::Vec3f(-1.0f, 0.0f, 0.0f);
    EXPECT_FALSE(aabb1.IntersectRay(dir1, origin1));
}

TEST(Aabb, Aabb3d_Plane)
{
    neko::Aabb3d aabb1;
    aabb1.FromCenterExtends(neko::Vec3f(0, 0, 0), neko::Vec3f(1, 1, 1));
    //Inside Intersect
    neko::Vec3f origin = neko::Vec3f(0, 0, 0);
    neko::Vec3f normal = neko::Vec3f(1, 1, 0);
    EXPECT_TRUE(aabb1.IntersectPlane(normal, origin));

    //Basic Intersect
    origin = neko::Vec3f(-2, 0.5f, -2);
    EXPECT_TRUE(aabb1.IntersectPlane(normal, origin));

    //Limit Intersect
    origin = neko::Vec3f(-2, 0.0f, -2);
    EXPECT_TRUE(aabb1.IntersectPlane(normal, origin));

    //Not Intersect
    origin = neko::Vec3f(-2, -1.0f, -2);
    EXPECT_FALSE(aabb1.IntersectPlane(normal, origin));

    //Not 0,0
    aabb1.FromCenterExtends(neko::Vec3f(2.0f, 2.0f, 2.0f), neko::Vec3f(1.0f, 1.0f, 1.0f));

    //Basic Intersect
    origin = neko::Vec3f(0, 2, 0);
    normal = neko::Vec3f(0, 1, 0);
    EXPECT_TRUE(aabb1.IntersectPlane(normal, origin));

    //Limit Intersect
    origin = neko::Vec3f(0, 1, 0);
    EXPECT_TRUE(aabb1.IntersectPlane(normal, origin));

    //Not Intersect
    origin = neko::Vec3f(0, 0, 0);
    EXPECT_FALSE(aabb1.IntersectPlane(normal, origin));
}

TEST(Aabb, Obb2d_Obb2d)
{
    neko::radian_t angle = static_cast<neko::radian_t>(neko::PI / 4);

    //Same Intersect
    neko::Obb2d obb1;
    obb1.FromCenterExtendsRotation(neko::Vec2f(0.0f, 0.0f), neko::Vec2f(0.5f, 0.5f), angle);
    neko::Obb2d obb2;
    obb2.FromCenterExtendsRotation(neko::Vec2f(0.0f, 0.0f), neko::Vec2f(0.5f, 0.5f), angle);
    EXPECT_TRUE(obb1.IntersectObb(obb2));
    EXPECT_TRUE(obb2.IntersectObb(obb1));

    //Basic Intersect
    obb1.FromCenterExtendsRotation(neko::Vec2f(0, 0), neko::Vec2f(0.5f, 0.5f), angle);
    obb2.FromCenterExtendsRotation(neko::Vec2f(0.5f, 0.5f), neko::Vec2f(0.5f, 0.5f), angle);
    EXPECT_TRUE(obb1.IntersectObb(obb2));
    EXPECT_TRUE(obb2.IntersectObb(obb1));


    //Transverse Intersect
    obb1.FromCenterExtendsRotation(neko::Vec2f(0, 0), neko::Vec2f(0.5f, 0.5f), angle);
    obb2.FromCenterExtendsRotation(neko::Vec2f(1.0f, 1.0f), neko::Vec2f(5.0f, 0.2f), angle);
    EXPECT_TRUE(obb1.IntersectObb(obb2));
    EXPECT_TRUE(obb2.IntersectObb(obb1));

    //Limit Intersect
    obb1.FromCenterExtendsRotation(neko::Vec2f(0, 0), neko::Vec2f(0.5f, 0.5f), angle);
    obb2.FromCenterExtendsRotation(neko::Vec2f(neko::Sin(angle), neko::Sin(angle)), neko::Vec2f(0.5f, 0.5f), angle);
    EXPECT_TRUE(obb1.IntersectObb(obb2));
    EXPECT_TRUE(obb2.IntersectObb(obb1));

    //Limit Intersect
    obb1.FromCenterExtendsRotation(neko::Vec2f(0, 0), neko::Vec2f(0.5f, 0.5f), angle);
    obb2.FromCenterExtendsRotation(neko::Vec2f(neko::Sin(angle), neko::Sin(angle)), neko::Vec2f(0.4f, 0.4f), angle);
    EXPECT_FALSE(obb1.IntersectObb(obb2));
    EXPECT_FALSE(obb2.IntersectObb(obb1));

    //Not Intersect
    obb1.FromCenterExtendsRotation(neko::Vec2f(0, 0), neko::Vec2f(0.5f, 0.5f), angle);
    obb2.FromCenterExtendsRotation(neko::Vec2f(1, 1), neko::Vec2f(0.5f, 0.5f), angle);
    EXPECT_FALSE(obb1.IntersectObb(obb2));
    EXPECT_FALSE(obb2.IntersectObb(obb1));

    //From Obb
    neko::Aabb2d aabb1;
    neko::Aabb2d aabb2;
    aabb1.FromObb(obb1);
    aabb2.FromObb(obb2);
    EXPECT_FALSE(aabb1.ContainsAabb(aabb2));
    EXPECT_TRUE(aabb1.IntersectAabb(aabb2));
    EXPECT_FALSE(aabb2.ContainsAabb(aabb1));
    EXPECT_TRUE(aabb2.IntersectAabb(aabb1));
}

TEST(Aabb, Obb3d_Obb3d)
{
    neko::RadianAngles angles = neko::RadianAngles(static_cast<neko::radian_t>(0), static_cast<neko::radian_t>(0), static_cast<neko::radian_t>(neko::PI / 4));
    //Same Intersect
    neko::Obb3d obb1;
    obb1.FromCenterExtendsRotation(neko::Vec3f(0, 0, 0), neko::Vec3f(0.5f, 0.5f, 0.5f), angles);
    neko::Obb3d obb2;
    obb2.FromCenterExtendsRotation(neko::Vec3f(0, 0, 0), neko::Vec3f(0.5f, 0.5f, 0.5f), angles);
    EXPECT_TRUE(obb1.IntersectObb(obb2));
    EXPECT_TRUE(obb2.IntersectObb(obb1));

    angles = neko::RadianAngles(static_cast<neko::radian_t>(-neko::PI / 4), static_cast<neko::radian_t>(0), static_cast<neko::radian_t>(-neko::PI / 4));
    //Transverse Intersect
    obb1.FromCenterExtendsRotation(neko::Vec3f(0, 0, 0), neko::Vec3f(0.5f, 0.5f, 0.5f), angles);
    obb2.FromCenterExtendsRotation(neko::Vec3f(1, 1, 1), neko::Vec3f(0.2f, 5.0f, 0.2f), angles);
    EXPECT_TRUE(obb1.IntersectObb(obb2));
    EXPECT_TRUE(obb2.IntersectObb(obb1));

    obb1.FromCenterExtendsRotation(neko::Vec3f(0, 0, 0), neko::Vec3f(0.5f, 0.5f, 0.5f), angles);
    obb2.FromCenterExtendsRotation(neko::Vec3f(1, 1, 1), neko::Vec3f(0.5f, 0.5f, 0.5f), angles);
    EXPECT_FALSE(obb1.IntersectObb(obb2));
    EXPECT_FALSE(obb2.IntersectObb(obb1));

    angles = neko::RadianAngles(static_cast<neko::radian_t>(neko::PI / 4), static_cast<neko::radian_t>(0), static_cast<neko::radian_t>(neko::PI / 4));
    obb1.FromCenterExtendsRotation(neko::Vec3f(0.0f, 0.0f, 0.0f), neko::Vec3f(0.5f, 0.5f, 0.5f), angles);
    obb2.FromCenterExtendsRotation(neko::Vec3f(1.0f, 1.0f, 1.0f), neko::Vec3f(1.0f, 1.0f, 1.0f), angles);
    EXPECT_TRUE(obb1.IntersectObb(obb2));
    EXPECT_TRUE(obb2.IntersectObb(obb1));

    //Same Intersect
    obb1.FromCenterExtendsRotation(neko::Vec3f(0.0f, 0.0f, 0.0f), neko::Vec3f(0.5f, 0.5f, 0.5f), angles);
    obb2.FromCenterExtendsRotation(neko::Vec3f(0.0f, 0.0f, 0.0f), neko::Vec3f(0.5f, 0.5f, 0.5f), angles);
    EXPECT_TRUE(obb1.IntersectObb(obb2));
    EXPECT_TRUE(obb2.IntersectObb(obb1));

    //Basic Intersect
    obb1.FromCenterExtendsRotation(neko::Vec3f(0, 0, 0), neko::Vec3f(0.5f, 0.5f, 0.5f), angles);
    obb2.FromCenterExtendsRotation(neko::Vec3f(0.5f, 0.5f, 0.5f), neko::Vec3f(0.5f, 0.5f, 0.5f), angles);
    EXPECT_TRUE(obb1.IntersectObb(obb2));
    EXPECT_TRUE(obb2.IntersectObb(obb1));

    //Limit Intersect
    obb1.FromCenterExtendsRotation(neko::Vec3f(0, 0, 0), neko::Vec3f(0.5f, 0.5f, 0.5f), angles);
    obb2.FromCenterExtendsRotation(neko::Vec3f(neko::Sin(angles.x), 0, neko::Sin(angles.z)), neko::Vec3f(0.5f, 0.5f, 0.5f), angles);

    //Not Intersect
    obb1.FromCenterExtendsRotation(neko::Vec3f(0, 0, 0), neko::Vec3f(0.5f, 0.5f, 0.5f), angles);
    obb2.FromCenterExtendsRotation(neko::Vec3f(1, 1, 1), neko::Vec3f(0.5f, 0.5f, 0.5f), angles);
    EXPECT_FALSE(obb1.IntersectObb(obb2));
    EXPECT_FALSE(obb2.IntersectObb(obb1));

    //From Obb
    neko::Aabb3d aabb1;
    neko::Aabb3d aabb2;
    aabb1.FromObb(obb1);
    aabb2.FromObb(obb2);
    EXPECT_FALSE(aabb1.ContainsAabb(aabb2));
    EXPECT_TRUE(aabb1.IntersectAabb(aabb2));
    EXPECT_FALSE(aabb2.ContainsAabb(aabb1));
    EXPECT_TRUE(aabb2.IntersectAabb(aabb1));
}

TEST(Aabb, TestAabb)
{
    neko::Aabb2d aabb1;
    aabb1.FromCenterExtends(neko::Vec2f(0, 0), neko::Vec2f(0.5, 0.5));
    neko::Aabb2d aabb2;
    aabb2.FromCenterExtends(neko::Vec2f(0, 0), neko::Vec2f(0.5, 0.5));
    EXPECT_TRUE(aabb1.ContainsAabb(aabb2));
    EXPECT_TRUE(aabb1.IntersectAabb(aabb2));
    EXPECT_TRUE(aabb2.ContainsAabb(aabb1));
    EXPECT_TRUE(aabb2.IntersectAabb(aabb1));

    aabb2.FromCenterExtends(neko::Vec2f(-1, 1), neko::Vec2f(1, 1));
    //std::cout << "AABB1 (" << aabb1.lowerLeftBound << " , " << aabb1.upperRightBound << "); AABB2 (" << aabb2.lowerLeftBound << " , " << aabb2.upperRightBound << ")  Contains :" << aabb1.ContainsAabb(aabb2) << "  Intersect :" << aabb1.IntersectAabb(aabb2) << "\n";
    EXPECT_TRUE(aabb1.IntersectAabb(aabb2));

    neko::Vec2f origin1 = neko::Vec2f(-1, -1);
    neko::Vec2f dir1 = neko::Vec2f(1, 1);
    //std::cout << "AABB1 (" << aabb1.lowerLeftBound << " , " << aabb1.upperRightBound << "); Ray (" << origin1 << " , " << dir1 << ")  Intersect :" << aabb1.IntersectRay(dir1, origin1) << "\n";
    EXPECT_TRUE(aabb1.IntersectRay(dir1, origin1));
    origin1 = neko::Vec2f(-0.5f, -1.5f);
    //std::cout << "AABB1 (" << aabb1.lowerLeftBound << " , " << aabb1.upperRightBound << "); Ray (" << origin1 << " , " << dir1 << ")  Intersect :" << aabb1.IntersectRay(dir1, origin1) << "\n";
    EXPECT_TRUE(aabb1.IntersectRay(dir1, origin1));
    origin1 = neko::Vec2f(-0.5f, -2.0f);
    //std::cout << "AABB1 (" << aabb1.lowerLeftBound << " , " << aabb1.upperRightBound << "); Ray (" << origin1 << " , " << dir1 << ")  Intersect :" << aabb1.IntersectRay(dir1, origin1) << "\n";
    EXPECT_FALSE(aabb1.IntersectRay(dir1, origin1));

    aabb1.FromCenterExtends(neko::Vec2f(0, 0), neko::Vec2f(10.0f, 10.0f));
    aabb2.FromCenterExtends(neko::Vec2f(1, 1), neko::Vec2f(1.0f, 1.0f));
    EXPECT_TRUE(aabb1.ContainsAabb(aabb2));

    neko::radian_t angle = static_cast<neko::radian_t>(neko::PI / 4);
    neko::Obb2d obb1;
    obb1.FromCenterExtendsRotation(neko::Vec2f(0, 0), neko::Vec2f(0.5, 0.5), angle);
    neko::Obb2d obb2;
    obb2.FromCenterExtendsRotation(neko::Vec2f(1, 1), neko::Vec2f(0.5, 0.5), angle);
    //std::cout << "OBB1 (" << obb1.localLowerLeftBound << " , " << obb1.localUpperRightBound << " , " << obb1.rotation << "); OBB2 (" << obb2.localLowerLeftBound << " , " << obb2.localUpperRightBound << " , " << obb2.rotation << ")  Intersect :" << obb1.IntersectObb(obb2) << "\n";
    EXPECT_FALSE(obb1.IntersectObb(obb2));

    aabb1.FromObb(obb1);
    aabb2.FromObb(obb2);
    //std::cout << "AABB1 (" << aabb1.lowerLeftBound << " , " << aabb1.upperRightBound << "); AABB2 (" << aabb2.lowerLeftBound << " , " << aabb2.upperRightBound << ")  Contains :" << aabb1.ContainsAabb(aabb2) << "  Intersect :" << aabb1.IntersectAabb(aabb2) << "\n";
    EXPECT_FALSE(aabb1.ContainsAabb(aabb2));
    EXPECT_TRUE(aabb1.IntersectAabb(aabb2));

    neko::Aabb3d aabb3;
    aabb3.FromCenterExtends(neko::Vec3f(0, 0, 0), neko::Vec3f(1, 1, 1));
    neko::Aabb3d aabb4;
    aabb4.FromCenterExtends(neko::Vec3f(1, 1, 1), neko::Vec3f(1, 1, 1));
    //std::cout << "AABB1 (" << aabb3.lowerLeftBound << " , " << aabb3.upperRightBound << "); AABB2 (" << aabb4.lowerLeftBound << " , " << aabb4.upperRightBound << ")  Contains :" << aabb3.ContainsAabb(aabb4) << "  Intersect :" << aabb3.IntersectAabb(aabb4) << "\n";
    EXPECT_FALSE(aabb3.ContainsAabb(aabb4));
    EXPECT_TRUE(aabb3.IntersectAabb(aabb4));
    aabb4.FromCenterExtends(neko::Vec3f(-1, 1, 1), neko::Vec3f(1, 1, 1));
    //std::cout << "AABB1 (" << aabb3.lowerLeftBound << " , " << aabb3.upperRightBound << "); AABB2 (" << aabb4.lowerLeftBound << " , " << aabb4.upperRightBound << ")  Contains :" << aabb3.ContainsAabb(aabb4) << "  Intersect :" << aabb3.IntersectAabb(aabb4) << "\n";
    EXPECT_TRUE(aabb3.IntersectAabb(aabb4));

    neko::Vec3f origin2 = neko::Vec3f(-2, -2, -2);
    neko::Vec3f dir2 = neko::Vec3f(1, 1, 1);
    //std::cout << "AABB1 (" << aabb3.lowerLeftBound << " , " << aabb3.upperRightBound << "); Ray (" << origin2 << " , " << dir2 << ")  Intersect :" << aabb3.IntersectRay(dir2, origin2) << "\n";
    EXPECT_TRUE(aabb3.IntersectRay(dir2, origin2));

    neko::Vec3f origin3 = neko::Vec3f(-2, 0, -2);
    neko::Vec3f normal3 = neko::Vec3f(1, 1, 0);
    //std::cout << "AABB1 (" << aabb3.lowerLeftBound << " , " << aabb3.upperRightBound << "); Plane (" << origin3 << " , " << normal3 << ")  Intersect :" << aabb3.IntersectPlane(normal3, origin3) << "\n";
    EXPECT_TRUE(aabb3.IntersectPlane(normal3, origin3));

    neko::RadianAngles angles = neko::RadianAngles(static_cast<neko::radian_t>(0), static_cast<neko::radian_t>(0), static_cast<neko::radian_t>(neko::PI / 4));
    neko::Obb3d obb3;
    obb3.FromCenterExtendsRotation(neko::Vec3f(0, 0, 0), neko::Vec3f(0.5, 0.5, 0.5), angles);
    neko::Obb3d obb4;
    obb4.FromCenterExtendsRotation(neko::Vec3f(1, 1, 1), neko::Vec3f(0.5, 0.5, 0.5), angles);
    //std::cout << "OBB1 (" << obb3.localLowerLeftBound << " , " << obb3.localUpperRightBound << "); OBB2 (" << obb4.localLowerLeftBound << " , " << obb4.localUpperRightBound << ")  Intersect :" << obb3.IntersectObb(obb4) << "\n";
    EXPECT_FALSE(obb3.IntersectObb(obb4));

    angles = neko::RadianAngles(static_cast<neko::radian_t>(neko::PI / 4), static_cast<neko::radian_t>(neko::PI / 4), static_cast<neko::radian_t>(0));
    obb3.FromCenterExtendsRotation(neko::Vec3f(0.0f, 0.0f, 0.0f), neko::Vec3f(0.5f, 0.5f, 0.5f), angles);
    obb4.FromCenterExtendsRotation(neko::Vec3f(1.0f, 1.0f, 1.0f), neko::Vec3f(1.0f, 1.0f, 1.0f), angles);
    //std::cout << "OBB1 (" << obb3.localLowerLeftBound << " , " << obb3.localUpperRightBound << "); OBB2 (" << obb4.localLowerLeftBound << " , " << obb4.localUpperRightBound << ")  Intersect :" << obb3.IntersectObb(obb4) << "\n";
    EXPECT_TRUE(obb3.IntersectObb(obb4));

    aabb3.FromObb(obb3);
    aabb4.FromObb(obb4);
    EXPECT_FALSE(aabb3.ContainsAabb(aabb4));
    EXPECT_TRUE(aabb3.IntersectAabb(aabb4));
}

TEST(Engine, Matrix3Det)
{
	const neko::Mat3f m1 = neko::Mat3f(std::array<neko::Vec3f, 3>
	{
		neko::Vec3f(6,10,14),
		neko::Vec3f(7,11,15),
		neko::Vec3f(8,12,16)
	});
	const float det1 = 0.0f;
	EXPECT_TRUE(neko::Equal(m1.Determinant(), det1));
	const neko::Mat3f m2 = neko::Mat3f(std::array<neko::Vec3f, 3>
	{
		neko::Vec3f(7,11,15),
		neko::Vec3f(8,12,16),
		neko::Vec3f(5,9,13)
	});
	const float det2 = 0.0f;
	EXPECT_TRUE(neko::Equal(m2.Determinant(), det1));
	const neko::Mat3f m3 = neko::Mat3f(std::array<neko::Vec3f, 3>
	{
		neko::Vec3f(8,12,16),
			neko::Vec3f(5,9,13),
			neko::Vec3f(6,10,14)
	});
	const float det3 = 0.0f;
	EXPECT_TRUE(neko::Equal(m3.Determinant(), det1));
	const neko::Mat3f m4 = neko::Mat3f(std::array<neko::Vec3f, 3>
	{
		neko::Vec3f(5, 9, 13),
		neko::Vec3f(6, 10, 14),
		neko::Vec3f(7, 11, 15)
	});
	const float det4 = 0.0f;
	EXPECT_TRUE(neko::Equal(m4.Determinant(), det1));
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

	EXPECT_LT(neko::Mat4f::MatrixDifference(neko::Mat4f::Identity, neko::Mat4f::Identity.Inverse()), 0.01f);
	EXPECT_LT(neko::Mat4f::MatrixDifference(neko::Mat4f::Zero, neko::Mat4f::Zero.Inverse()), 0.01f);
	
	const neko::Mat4f m = neko::Mat4f(std::array<neko::Vec4f, 4>
	{
		neko::Vec4f(1.0f,5.0f,9.0f,13.0f),
		neko::Vec4f(2.0f,6.0f,-10.0f,14.0f),
		neko::Vec4f(3.0f,-7.0f,11.0f,15.0f),
		neko::Vec4f(4.0f,8.0f,12.0f,16.0f)
	});

	const auto mInvCalculus = m.Inverse();

	const neko::Mat4f mInv = neko::Mat4f(std::array<neko::Vec4f, 4>
	{
		neko::Vec4f(-198.0f,7.0f,20.0f,136.0f),
		neko::Vec4f(10.0f,0.0f,-30.0f,20.0f),
		neko::Vec4f(14.0f,-21.0f,0.0f,7.0f),
		neko::Vec4f(34.0f,14.0f,10.0f,-23.0f)
	})*(1.0f/420.0f);
	
	
	EXPECT_LT(neko::Mat4f::MatrixDifference(mInvCalculus, mInv), 0.01f);
	EXPECT_GT(neko::Mat4f::MatrixDifference(mInvCalculus, neko::Mat4f::Identity), 0.01f);
}
