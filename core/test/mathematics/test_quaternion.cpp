/*
 MIT License

 Copyright (c) 2020 SAE Institute Switzerland AG

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */
#include <gtest/gtest.h>
#include "mathematics/quaternion.h"

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
neko::Radian rad(30);
neko::Vec3f axis(1, 1, 1);
neko::Quaternion expectedAngleAxisQuaternion = neko::Quaternion(0, 0, 0, 1);    //TODO: Calculate the expected value
q = q.AngleAxis(rad, axis);
//EXPECT_EQ(q, expectedAngleAxisQuaternion);
}

TEST(Engine, Quaternion_Angle)
{
neko::Quaternion q1 = neko::Quaternion::Identity();
neko::Quaternion q2 = neko::Quaternion::Identity();
neko::Degree expectedAngle(0);
neko::Degree angle(neko::Quaternion::Angle(q1, q2));
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