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