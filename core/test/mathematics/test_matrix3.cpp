/*
MIT License

Copyright (c) 2022 SAE Institute Switzerland AG

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
#include "mathematics/basic.h"
#include "mathematics/matrix3.h"

TEST(Matrix3, DefaultConstructor)
{
    constexpr neko::Mat3i m;
    constexpr auto identity = neko::Mat3i::identity();
    for(int i = 0; i < 3; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            EXPECT_EQ(m[i][j], identity[i][j]);
            EXPECT_EQ(m[i][j], identity(i,j));
            EXPECT_EQ(m(i,j), identity(i,j));
        }
    }
}

TEST(Matrix3, ArrayConstructor)
{
    constexpr std::array<neko::Vec3i, 3> mArray
    {
        {
            neko::Vec3i(1,4, 7),
            neko::Vec3i(2,5,8),
            neko::Vec3i(3,6,9)
        } };
    constexpr neko::Mat3i m(mArray);
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            EXPECT_EQ(m[i][j], mArray[i][j]);
            EXPECT_EQ(m(i,j), mArray[i][j]);
        }
    }
}


TEST(Matrix3, Addition)
{
    constexpr std::array<neko::Vec3i, 3> mArray
    {
        {
            neko::Vec3i(1,4, 7),
            neko::Vec3i(2,5,8),
            neko::Vec3i(3,6,9)
        } };
    constexpr std::array<neko::Vec3i, 3> m2Array
    {
        {
            neko::Vec3i(1,2, 3),
            neko::Vec3i(4,5,6),
            neko::Vec3i(7,8,9)
        } };
    constexpr neko::Mat3i m(mArray);
    constexpr neko::Mat3i m2(m2Array);
    constexpr auto result = m + m2;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            EXPECT_EQ(result[i][j], mArray[i][j]+m2Array[i][j]);
        }
    }
}

TEST(Matrix3, Subtraction)
{
    constexpr std::array<neko::Vec3i, 3> mArray
    {
        {
            neko::Vec3i(1,4, 7),
            neko::Vec3i(2,5,8),
            neko::Vec3i(3,6,9)
        } };
    constexpr std::array<neko::Vec3i, 3> m2Array
    {
        {
            neko::Vec3i(1,2, 3),
            neko::Vec3i(4,5,6),
            neko::Vec3i(7,8,9)
        } };
    constexpr neko::Mat3i m(mArray);
    constexpr neko::Mat3i m2(m2Array);
    constexpr auto result = m - m2;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            EXPECT_EQ(result[i][j], mArray[i][j] - m2Array[i][j]);
        }
    }
}


TEST(Matrix3, Multiplication)
{
    constexpr std::array<neko::Vec3i, 3> mArray
    {
        {
            neko::Vec3i(2,1, 0),
            neko::Vec3i(7,5,4),
            neko::Vec3i(3,8,1)
        } };
    constexpr std::array<neko::Vec3i, 3> m2Array
    {
        {
            neko::Vec3i(3,2, 1),
            neko::Vec3i(0,1,2),
            neko::Vec3i(1,0,4)
        } };
    constexpr std::array<neko::Vec3i, 3> handResultArray
    {
        {
            neko::Vec3i(23,21, 9),
            neko::Vec3i(13,21,6),
            neko::Vec3i(14,33,4)
        } };
    constexpr neko::Mat3i m(mArray);
    constexpr neko::Mat3i m2(m2Array);
    constexpr auto result = m * m2;
    constexpr auto identityResult = m * neko::Mat3i::identity();
    constexpr auto zeroResult = m * neko::Mat3i::zero();
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            EXPECT_EQ(result[i][j], handResultArray[i][j]);
            EXPECT_EQ(m[i][j], identityResult[i][j]);
            EXPECT_EQ(neko::Mat3i::zero()[i][j], zeroResult[i][j]);
        }
    }
}


TEST(Matrix3, VectorMultiplication)
{
    constexpr std::array<neko::Vec3i, 3> mArray
    {
        {
            neko::Vec3i(5,-2,7),
            neko::Vec3i(3,2,0),
            neko::Vec3i(1,4,-1)
        } };
    constexpr neko::Vec3i v(2, 9, -3);
    constexpr neko::Vec3i handResult(34,2,17);

    constexpr neko::Mat3i m(mArray);
    constexpr auto result = m * v;
    EXPECT_EQ(handResult, result);
}

TEST(Matrix3, ScalarMultiplication)
{
    constexpr std::array<neko::Vec3i, 3> mArray
    {
        {
            neko::Vec3i(5,-2,7),
            neko::Vec3i(3,2,0),
            neko::Vec3i(1,4,-1)
        } };
    constexpr int scalar = 2;
    constexpr std::array<neko::Vec3i, 3> handResult
    {
        {
            neko::Vec3i(10,-4,14),
            neko::Vec3i(6,4,0),
            neko::Vec3i(2,8,-2)
        } };

    constexpr neko::Mat3i m(mArray);
    constexpr auto result = m * scalar;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            EXPECT_EQ(result[i][j], handResult[i][j]);
        }
    }
}


TEST(Matrix3, Transpose)
{
    constexpr std::array<neko::Vec3i, 3> mArray
    {
        {
            neko::Vec3i(5,-2,7),
            neko::Vec3i(3,2,0),
            neko::Vec3i(1,4,-1)
        } };
    constexpr std::array<neko::Vec3i, 3> handResult
    {
        {
            neko::Vec3i(5,3,1),
            neko::Vec3i(-2,2,4),
            neko::Vec3i(7,0,-1)
        } };

    constexpr neko::Mat3i m(mArray);
    constexpr auto result = m.Transpose();
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            EXPECT_EQ(result[i][j], handResult[i][j]);
        }
    }
}

TEST(Matrix3, Determinant)
{
    constexpr auto m1 = neko::Mat3f(std::array<neko::Vec3f, 3>
    {
        neko::Vec3f(6, 10, 14),
            neko::Vec3f(7, 11, 15),
            neko::Vec3f(8, 12, 16)
    });
    constexpr float det1 = 0.0f;
    EXPECT_TRUE(neko::Equal(m1.Determinant(), det1));
    constexpr neko::Mat3f m2 = neko::Mat3f(std::array<neko::Vec3f, 3>
    {
        neko::Vec3f(7, 11, 15),
            neko::Vec3f(8, 12, 16),
            neko::Vec3f(5, 9, 13)
    });
    constexpr float det2 = 0.0f;
    EXPECT_TRUE(neko::Equal(m2.Determinant(), det2));
    constexpr neko::Mat3f m3 = neko::Mat3f(std::array<neko::Vec3f, 3>
    {
        neko::Vec3f(8, 12, 16),
            neko::Vec3f(5, 9, 13),
            neko::Vec3f(6, 10, 14)
    });
    constexpr float det3 = 0.0f;
    EXPECT_TRUE(neko::Equal(m3.Determinant(), det3));
    constexpr neko::Mat3f m4 = neko::Mat3f(std::array<neko::Vec3f, 3>
    {
        neko::Vec3f(5, 9, 13),
            neko::Vec3f(6, 10, 14),
            neko::Vec3f(7, 11, 15)
    });
    constexpr float det4 = 0.0f;
    EXPECT_TRUE(neko::Equal(m4.Determinant(), det4));
}

TEST(Matrix3, Inverse)
{
    constexpr std::array<neko::Vec3f, 3> mArray{
    {
        neko::Vec3f(3,2,0),
        neko::Vec3f(0,0,1),
        neko::Vec3f(2,-2,1)
    } };
    constexpr std::array<neko::Vec3f, 3> mInvArray{
    {
        neko::Vec3f(0.2f,-0.2f,0.2f),
        neko::Vec3f(0.2f,0.3f,-0.3f),
        neko::Vec3f(0,1,0)
    } };
    constexpr neko::Mat3f m(mArray);

    EXPECT_FLOAT_EQ(m.Determinant(), 10.0f);
    constexpr auto result = m.Inverse();
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            EXPECT_FLOAT_EQ(result[i][j], mInvArray[i][j]);
        }
    }
}