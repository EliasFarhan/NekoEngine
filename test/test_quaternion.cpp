//
// Created by sfeser on 23/03/20.
//
#ifdef WIN32
#define _USE_MATH_DEFINES
#endif
#include <cmath>
#include <random>
#include <gtest/gtest.h>

#include <mathematics/quaternion.h>


const float maxNmb = 100.0f;

TEST(Engine, Quaternion_Dot)
{
	neko::Quaternion q1 = neko::Quaternion(1, 0, 0, 0);
	neko::Quaternion q2 = neko::Quaternion(1, 0, 0, 0);
	float expectedDot = 1;
	float dotQ1Q2 = neko::Quaternion::Dot(q1, q2);
	EXPECT_TRUE(expectedDot - dotQ1Q2 < 0.001f);
}

TEST(Engine, Quaternion_Normalized)
{
	neko::Quaternion q = neko::Quaternion(3.33f, 0, 0, 0);
	neko::Quaternion expectedNormalized = neko::Quaternion(1, 0, 0, 0);
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
	//Todo: Calculate
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
	neko::EulerAngles angle = neko::Vec3<neko::degree_t>(neko::degree_t(90));
	neko::Quaternion q = neko::Quaternion(1, 0.5, 0.5, 0);
	//TODO Calculate expected Quaternion
	//EXPECT_TRUE(q.x - expectedQuaternion.x < 0.001f && q.y - expectedQuaternion.y < 0.001f && q.z - expectedQuaternion.z < 0.001f && q.w - expectedQuaternion.w < 0.001f);
}

TEST(Engine, Quaternion_Div_By_Float)
{
	neko::Quaternion q1 = neko::Quaternion(5, 15, 30, 45);
	float divisor = 5;
	neko::Quaternion result = q1 / divisor;
	neko::Quaternion expectedQuaternion = neko::Quaternion(1, 3, 6, 9);
	EXPECT_TRUE(result.x - expectedQuaternion.x < 0.001f && result.y - expectedQuaternion.y < 0.001f && result.z - expectedQuaternion.z < 0.001f && result.w - expectedQuaternion.w < 0.001f);
}

TEST(Engine, Quaternion_Div_By_Quat)
{
	neko::Quaternion q1 = neko::Quaternion(42, 80, 30, 4);
	neko::Quaternion q2 = neko::Quaternion(2, 20, 10, 4);
	neko::Quaternion result = q1 / q2;
	neko::Quaternion expectedQuaternion = neko::Quaternion(21, 4, 3, 1);
	//EXPECT_TRUE(result.x - expectedQuaternion.x < 0.001f && result.y - expectedQuaternion.y < 0.001f && result.z - expectedQuaternion.z < 0.001f && result.w - expectedQuaternion.w < 0.001f);
	//TODO Calculate
}

TEST(Engine, Quaternion_Plus_Eq_Float)
{
	neko::Quaternion q1 = neko::Quaternion(8, 3, 7, 1);
	float value = 2.0f;
	neko::Quaternion result = q1 += value;
	neko::Quaternion expectedQuaternion = neko::Quaternion(10, 5, 9, 3);
	EXPECT_TRUE(result.x - expectedQuaternion.x < 0.001f && result.y - expectedQuaternion.y < 0.001f && result.z - expectedQuaternion.z < 0.001f && result.w - expectedQuaternion.w < 0.001f);
}

TEST(Engine, Quaternion_Min_Quat)
{
	neko::Quaternion q1 = neko::Quaternion(42, 80, 30, 4);
	neko::Quaternion q2 = neko::Quaternion(2, 20, 10, 4);
	neko::Quaternion result = q1 - q2;
	neko::Quaternion expectedQuaternion = neko::Quaternion(40, 60, 20, 0);
	EXPECT_TRUE(result.x - expectedQuaternion.x < 0.001f && result.y - expectedQuaternion.y < 0.001f && result.z - expectedQuaternion.z < 0.001f && result.w - expectedQuaternion.w < 0.001f);
}

TEST(Engine, Quaternion_Min_Eq_Quat)
{
	neko::Quaternion q1 = neko::Quaternion(42, 80, 30, 4);
	neko::Quaternion q2 = neko::Quaternion(2, 20, 10, 4);
	neko::Quaternion result = q1 -= q2;
	neko::Quaternion expectedQuaternion = neko::Quaternion(40, 60, 20, 0);
	EXPECT_TRUE(result.x - expectedQuaternion.x < 0.001f && result.y - expectedQuaternion.y < 0.001f && result.z - expectedQuaternion.z < 0.001f && result.w - expectedQuaternion.w < 0.001f);
}

TEST(Engine, Quaternion_Plus_Eq_Quat)
{
	neko::Quaternion q1 = neko::Quaternion(42, 80, 30, 4);
	neko::Quaternion q2 = neko::Quaternion(2, 20, 10, 4);
	neko::Quaternion result = q1 += q2;
	neko::Quaternion expectedQuaternion = neko::Quaternion(44, 100, 40, 8);
	EXPECT_TRUE(result.x - expectedQuaternion.x < 0.001f && result.y - expectedQuaternion.y < 0.001f && result.z - expectedQuaternion.z < 0.001f && result.w - expectedQuaternion.w < 0.001f);
}

TEST(Engine, Quaternion_Times_Quat)
{
	neko::Quaternion q1 = neko::Quaternion(8, 9, 7, 4);
	neko::Quaternion q2 = neko::Quaternion(2, 3, 5, 4);
	neko::Quaternion result = q1 * q2;
	neko::Quaternion expectedQuaternion = neko::Quaternion(40, 60, 20, 0);
	//EXPECT_TRUE(result.x - expectedQuaternion.x < 0.001f && result.y - expectedQuaternion.y < 0.001f && result.z - expectedQuaternion.z < 0.001f && result.w - expectedQuaternion.w < 0.001f);
	//TODO Calculate
}

TEST(Engine, Quaternion_Times_Float)
{
	neko::Quaternion q1 = neko::Quaternion(8, 3, 7, 1);
	float value = 2.0f;
	neko::Quaternion result = q1 * value;
	neko::Quaternion expectedQuaternion = neko::Quaternion(16, 6, 14, 2);
	EXPECT_TRUE(result.x - expectedQuaternion.x < 0.001f && result.y - expectedQuaternion.y < 0.001f && result.z - expectedQuaternion.z < 0.001f && result.w - expectedQuaternion.w < 0.001f);
}

TEST(Engine, Quaternion_Times_Eq_Quat)
{
	neko::Quaternion q1 = neko::Quaternion(8, 9, 7, 4);
	neko::Quaternion q2 = neko::Quaternion(2, 3, 5, 4);
	neko::Quaternion result = q1 *= q2;
	neko::Quaternion expectedQuaternion = neko::Quaternion(40, 60, 20, 0);
	//EXPECT_TRUE(result.x - expectedQuaternion.x < 0.001f && result.y - expectedQuaternion.y < 0.001f && result.z - expectedQuaternion.z < 0.001f && result.w - expectedQuaternion.w < 0.001f);
	//TODO Calculate
}

TEST(Engine, Quaternion_Eq_Eq)
{
	neko::Quaternion q1 = neko::Quaternion(13, 42, 69, 666);
	neko::Quaternion q2 = neko::Quaternion(13, 42, 69, 666);
	EXPECT_TRUE(q1 == q2);
}

TEST(Engine, Quaternion_Not_Eq)
{
	neko::Quaternion q1 = neko::Quaternion(13, 42, 69, 666);
	neko::Quaternion q2 = neko::Quaternion(13, 42, 69, 666.5f);
	EXPECT_TRUE(q1 != q2);
}