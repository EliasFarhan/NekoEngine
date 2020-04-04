#ifdef WIN32
#define _USE_MATH_DEFINES
#define  __SSE__
#endif
#include <cmath>
#include <random>
#include <gtest/gtest.h>
#include <mathematics/func_table.h>

#include <mathematics/quaternion.h>
#include <mathematics/matrix.h>
#include "mathematics/vector.h"

TEST(Engine, Quaternion_Dot)
{
    neko::Quaternion q1 = neko::Quaternion(1, 0, 0, 0);
    neko::Quaternion q2 = neko::Quaternion(1, 0, 0, 0);
    float expectedDot = 1;
    float dotQ1Q2 = neko::Quaternion::Dot(q1, q2);
    EXPECT_TRUE(expectedDot - dotQ1Q2 < 0.001f);
}

TEST(Engine,Quaternion_Intrinsics_Dot)
{
    std::array<float, 4> x1{ 5,6,42,29 };
    std::array<float, 4> y1{ 7,3,666,382 };
    std::array<float, 4> z1{ 30,2,69,4729 };
    std::array<float, 4> w1{ 8,1,13,773 };
    std::array<float, 4> x2{7,7,7,7};
    std::array<float, 4> y2{3,3,3,3};
    std::array<float, 4> z2{1,1,1,1};
    std::array<float, 4> w2{9,9,9,9};
    neko::Quaternion q1a = neko::Quaternion(x1[0], y1[0], z1[0], w1[0]);
    neko::Quaternion q1b = neko::Quaternion(x1[1], y1[1], z1[1], w1[1]);
    neko::Quaternion q1c = neko::Quaternion(x1[2], y1[2], z1[2], w1[2]);
    neko::Quaternion q1d = neko::Quaternion(x1[3], y1[3], z1[3], w1[3]);
    neko::Quaternion q2 = neko::Quaternion(x2[0], y2[0], z2[0], w2[0]);
    float quaternionDotQ1aQ2 = neko::Quaternion::Dot(q1a, q2);
    float quaternionDotQ1bQ2 = neko::Quaternion::Dot(q1b, q2);
    float quaternionDotQ1cQ2 = neko::Quaternion::Dot(q1c, q2);
    float quaternionDotQ1dQ2 = neko::Quaternion::Dot(q1d, q2);
	
    neko::FourQuaternion fqa = neko::FourQuaternion(x1, y1, z1, w1);
    neko::FourQuaternion fqb = neko::FourQuaternion(x2, y2, z2, w2);
    std::array<float, 4> quaternionDotIntrinsics = neko::FourQuaternion::DotIntrinsics(fqa, fqb);
    EXPECT_TRUE(quaternionDotQ1aQ2 - quaternionDotIntrinsics[0] < 0.001f &&
        quaternionDotQ1bQ2 - quaternionDotIntrinsics[1] < 0.001f &&
        quaternionDotQ1cQ2 - quaternionDotIntrinsics[2] < 0.001f &&
        quaternionDotQ1dQ2 - quaternionDotIntrinsics[3] < 0.001f);
	

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
