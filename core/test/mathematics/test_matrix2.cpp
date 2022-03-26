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
#include "mathematics/matrix2.h"

TEST(Matrix2, DefaultConstructor)
{
    neko::Mat2<int> m;
    constexpr auto other = neko::Mat2<int>::identity();
    for(int i = 0; i < 2; i++)
    {
        for(int j = 0; j < 2; j++)
        {
            EXPECT_EQ(m[i][j], other[i][j]);
            EXPECT_EQ(m(i,j), other(i,j));
        }
    }
}

TEST(Matrix2, ArrayConstructor)
{
    constexpr std::array<neko::Vec2i, 2> array{{neko::Vec2i(1, 3), neko::Vec2i(2, 4)}};
    neko::Mat2<int> m{array};
    for(int i = 0; i < 2; i++)
    {
        for(int j = 0; j < 2; j++)
        {
            EXPECT_EQ(m[i][j], array[i][j]);
            EXPECT_EQ(m(i,j), array[i][j]);
        }
    }
}

TEST(Matrix2, Addition)
{
    constexpr std::array<neko::Vec2i, 2> array{{neko::Vec2i(1, 3), neko::Vec2i(2, 4)}};
    constexpr std::array<neko::Vec2i, 2> identity {{neko::Vec2i(1, 0), neko::Vec2i(0, 1)}};
    constexpr neko::Mat2<int> m1(array);
    constexpr neko::Mat2<int> m2(identity);
    constexpr auto result =m1+m2;
    for(int i = 0; i < 2; i++)
    {
        for(int j = 0; j < 2; j++)
        {
            EXPECT_EQ(result[i][j], array[i][j] + identity[i][j]);
        }
    }
}

TEST(Matrix2, Subtraction)
{
    constexpr std::array<neko::Vec2i, 2> array{{neko::Vec2i(1, 3), neko::Vec2i(2, 4)}};
    constexpr std::array<neko::Vec2i, 2> identity {{neko::Vec2i(1, 0), neko::Vec2i(0, 1)}};
    constexpr neko::Mat2<int> m1(array);
    constexpr neko::Mat2<int> m2(identity);
    constexpr auto result =m1-m2;
    for(int i = 0; i < 2; i++)
    {
        for(int j = 0; j < 2; j++)
        {
            EXPECT_EQ(result[i][j], array[i][j] - identity[i][j]);
        }
    }
}

TEST(Matrix2, Multiplication)
{
    constexpr std::array<neko::Vec2i, 2> array{{neko::Vec2i(1, 3), neko::Vec2i(2, 4)}};
    constexpr std::array<neko::Vec2i, 2> array2{{neko::Vec2i(-1, 3), neko::Vec2i(-2, 4)}};
    constexpr std::array<neko::Vec2i, 2> resultArray{{neko::Vec2i(5, 9), neko::Vec2i(6, 10)}};
    constexpr std::array<neko::Vec2i, 2> identity {{neko::Vec2i(1, 0), neko::Vec2i(0, 1)}};
    constexpr neko::Mat2<int> m1(array);
    constexpr neko::Mat2<int> m2(array2);
    constexpr neko::Mat2<int> m3(identity);
    constexpr auto identic =m1*m3;
    constexpr  auto result =m1*m2;
    for(int i = 0; i < 2; i++)
    {
        for(int j = 0; j < 2; j++)
        {
            EXPECT_EQ(identic[i][j], array[i][j]);
            EXPECT_EQ(result[i][j], resultArray[i][j]);
        }
    }
}


TEST(Matrix2, VectorTransform)
{
    constexpr std::array<neko::Vec2i, 2> array{{neko::Vec2i(1, 3), neko::Vec2i(2, 4)}};
    constexpr std::array<neko::Vec2i, 2> identity {{neko::Vec2i(1, 0), neko::Vec2i(0, 1)}};
    constexpr neko::Mat2<int> m1(array);
    constexpr neko::Mat2<int> m3(identity);
    constexpr neko::Vec2i v1(1, 3);
    constexpr auto identic =m3*v1;
    constexpr auto result =m1*v1;
    constexpr neko::Vec2i handResult(7, 15);
    EXPECT_EQ(identic, v1);
    EXPECT_EQ(result, handResult);
}

TEST(Matrix2, ScalarMultiplication)
{
    constexpr std::array<neko::Vec2i, 2> array{{neko::Vec2i(1, 3), neko::Vec2i(2, 4)}};
    constexpr int scalarFactor = 3;
    constexpr neko::Mat2<int> m1(array);

    const auto result =m1*scalarFactor;
    for(int i = 0; i < 2; i++)
    {
        for(int j = 0; j < 2; j++)
        {
            EXPECT_EQ(result[i][j], array[i][j]*scalarFactor);
        }
    }
}

TEST(Matrix2, Transpose)
{
    constexpr std::array<neko::Vec2i, 2> array{{neko::Vec2i(1, 3), neko::Vec2i(2, 4)}};
    constexpr std::array<neko::Vec2i, 2> array2{{neko::Vec2i(1, 2), neko::Vec2i(3, 4)}};

    constexpr neko::Mat2<int> m1(array);

    constexpr auto result =m1.Transpose();
    for(int i = 0; i < 2; i++)
    {
        for(int j = 0; j < 2; j++)
        {
            EXPECT_EQ(result[i][j], array2[i][j]);
        }
    }
}

TEST(Matrix2, Determinant)
{
    constexpr std::array<neko::Vec2i, 2> array{{neko::Vec2i(1, 3), neko::Vec2i(2, 4)}};

    constexpr neko::Mat2<int> m1(array);
    constexpr auto determinant = m1.Determinant();
    EXPECT_EQ(determinant, -2);
    EXPECT_EQ(neko::Mat2<int>::identity().Determinant(), 1);
    EXPECT_EQ(neko::Mat2<int>::zero().Determinant(), 0);
}

TEST(Matrix2, Inverse)
{
    constexpr std::array<neko::Vec2f, 2> array{{neko::Vec2f(1, 3), neko::Vec2f(2, 4)}};
    constexpr std::array<neko::Vec2f, 2> result{{neko::Vec2f(-2.0f, 3.0f/2.0f),
                                                 neko::Vec2f(1.0f, -1.0f/2.0f)}};

    constexpr neko::Mat2<float> m1(array);
    constexpr auto inverse = m1.Inverse();
    constexpr auto identity = m1*inverse;
    for(int i = 0; i < 2; i++)
    {
        for(int j = 0; j < 2; j++)
        {
            EXPECT_FLOAT_EQ(result[i][j], inverse[i][j]);
            EXPECT_FLOAT_EQ(neko::Mat2<float>::identity()[i][j], identity[i][j]);
        }
    }
}