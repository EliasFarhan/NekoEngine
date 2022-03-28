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
#include "mathematics/vec2.h"

TEST(Vector2, DefaultConstructor)
{
    constexpr neko::Vec2i v;
    EXPECT_EQ(v.x, 0);
    EXPECT_EQ(v.y, 0);
}

TEST(Vector2, ScalarConstructor)
{
    constexpr int scalar = 3;
    constexpr neko::Vec2i v(scalar);
    EXPECT_EQ(v.x, scalar);
    EXPECT_EQ(v.y, scalar);
}

TEST(Vector2, Constructor)
{
    constexpr int x = 3;
    constexpr int y = 2;
    constexpr neko::Vec2i v(x,y);
    EXPECT_EQ(v.x, x);
    EXPECT_EQ(v.y, y);
}

TEST(Vector2, OtherTypeVectorConstructor)
{
    constexpr neko::Vec2u u(3u, 2u);
    constexpr neko::Vec2i v(u);
    EXPECT_EQ(v.x, static_cast<int>(u.x));
    EXPECT_EQ(v.y, static_cast<int>(u.y));
}

TEST(Vector2, ConstValue)
{
    constexpr auto zero = neko::Vec2i::zero();
    EXPECT_EQ(zero, neko::Vec2i());
    constexpr auto one = neko::Vec2i::one();
    EXPECT_EQ(one, neko::Vec2i(1));
    constexpr auto up = neko::Vec2i::up();
    EXPECT_EQ(up, neko::Vec2i(0, 1));
    constexpr auto down = neko::Vec2i::down();
    EXPECT_EQ(down, neko::Vec2i(0, -1));
    constexpr auto left = neko::Vec2i::left();
    EXPECT_EQ(left, neko::Vec2i(-1, 0));
    constexpr auto right = neko::Vec2i::right();
    EXPECT_EQ(right, neko::Vec2i(1, 0));
}

TEST(Vector2, Addition)
{
    constexpr auto v1 = neko::Vec2i(1, 2);
    constexpr auto v2 = neko::Vec2i(-1, 2);
    constexpr auto result = v1 + v2;
    EXPECT_EQ(v1.x + v2.x, result.x);
    EXPECT_EQ(v1.y + v2.y, result.y);
}

TEST(Vector2, AdditionAssignement)
{
    auto v1 = neko::Vec2i(1, 2);
    const auto origin = v1;
    constexpr auto v2 = neko::Vec2i(-1, 2);
    v1 += v2;
    EXPECT_EQ(origin.x + v2.x, v1.x);
    EXPECT_EQ(origin.y + v2.y, v1.y);
}

TEST(Vector2, Subtraction)
{
    constexpr auto v1 = neko::Vec2i(1, 2);
    constexpr auto v2 = neko::Vec2i(-1, 2);
    constexpr auto result = v1 - v2;
    EXPECT_EQ(v1.x - v2.x, result.x);
    EXPECT_EQ(v1.y - v2.y, result.y);
}

TEST(Vector2, SubtractionAssignement)
{
    auto v1 = neko::Vec2i(1, 2);
    const auto origin = v1;
    constexpr auto v2 = neko::Vec2i(-1, 2);
    v1 -= v2;
    EXPECT_EQ(origin.x - v2.x, v1.x);
    EXPECT_EQ(origin.y - v2.y, v1.y);
}

TEST(Vector2, Multiply)
{
    constexpr auto v1 = neko::Vec2i(1, 2);
    constexpr auto v2 = neko::Vec2i(-1, 2);
    constexpr auto result = v1 * v2;
    EXPECT_EQ(v1.x * v2.x, result.x);
    EXPECT_EQ(v1.y * v2.y, result.y);
}

TEST(Vector2, MultiplyScalar)
{
    constexpr auto v1 = neko::Vec2i(1, 2);
    constexpr auto scalar = 4;
    constexpr auto result = v1 * scalar;
    EXPECT_EQ(v1.x * scalar, result.x);
    EXPECT_EQ(v1.y * scalar, result.y);
    constexpr auto reverseResult = scalar * v1;
    EXPECT_EQ(result, reverseResult);
}

TEST(Vector2, MultiplyScalarAssignement)
{
    auto v1 = neko::Vec2i(1, 2);
    const auto origin = v1;
    constexpr auto scalar = 4;
    v1 *= scalar;
    EXPECT_EQ(origin.x * scalar, v1.x);
    EXPECT_EQ(origin.y * scalar, v1.y);
}

TEST(Vector2, Divide)
{
    constexpr auto v1 = neko::Vec2i(10, 2);
    constexpr auto v2 = neko::Vec2i(-5, 2);
    constexpr auto result = v1 / v2;
    EXPECT_EQ(v1.x / v2.x, result.x);
    EXPECT_EQ(v1.y / v2.y, result.y);
}

TEST(Vector2, DivideScalar)
{
    constexpr auto v1 = neko::Vec2i(16, -4);
    constexpr auto scalar = 4;
    constexpr auto result = v1 * scalar;
    EXPECT_EQ(v1.x * scalar, result.x);
    EXPECT_EQ(v1.y * scalar, result.y);
}

TEST(Vector2, DivideScalarAssignement)
{
    auto v1 = neko::Vec2i(16, -4);
    const auto origin = v1;
    constexpr auto scalar = 4;
    v1 *= scalar;
    EXPECT_EQ(origin.x * scalar, v1.x);
    EXPECT_EQ(origin.y * scalar, v1.y);
}

TEST(Vector2, GetOperator)
{
    auto v1 = neko::Vec2i(16, -4);
    EXPECT_EQ(v1[0], v1.x);
    EXPECT_EQ(v1[1], v1.y);

    constexpr auto v2 = neko::Vec2i(16, -4);
    EXPECT_EQ(v2[0], v2.x);
    EXPECT_EQ(v2[1], v2.y);
}

TEST(Vector2, String)
{
    constexpr auto v1 = neko::Vec2i(16, -4);
    const auto v1Text = std::string(v1);
    EXPECT_EQ(v1Text, "Vec2(16,-4)");

}

TEST(Vector2, Dot)
{
    constexpr auto v1 = neko::Vec2i(16, -4);
    constexpr auto v2 = neko::Vec2i(2, 3);
    constexpr auto result = neko::Vec2i::Dot(v1, v2);
    EXPECT_EQ(result, v1.x*v2.x+v1.y*v2.y);

}