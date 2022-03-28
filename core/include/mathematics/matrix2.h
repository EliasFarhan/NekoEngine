#pragma once
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
#include "mathematics/vec2.h"

namespace neko
{
template<typename T>
class Mat2
{
public:
    constexpr Mat2()
    {
        columns_ = identity().columns_;
    }
    constexpr explicit Mat2(const std::array<Vec2 < T>, 2>& v) : columns_(v)
    {
    }

    constexpr const T& operator()(std::size_t col, std::size_t row) const
    {
        return columns_[col][row];
    }

    constexpr T& operator()(std::size_t col, std::size_t row)
    {
        return columns_[col][row];
    }

    constexpr const Vec2<T>& operator[](std::size_t column) const
    {
        return columns_[column];
    }

    constexpr Vec2<T>& operator[](std::size_t column)
    {
        return columns_[column];
    }

    constexpr Mat2 operator+(const Mat2& other) const
    {
        std::array<Vec2<T>, 2> m;
        for (int column = 0; column < 2; column++)
        {
            for (int row = 0; row < 2; row++)
            {
                m[column][row] = columns_[column][row]+other[column][row];
            }
        }
        return Mat2{m};
    }

    constexpr Mat2 operator-(const Mat2& other) const
    {
        std::array<Vec2<T>, 2> m;
        for (int column = 0; column < 2; column++)
        {
            for (int row = 0; row < 2; row++)
            {
                m[column][row] = columns_[column][row]-other[column][row];
            }
        }
        return Mat2{m};
    }

    constexpr Mat2 operator*(const Mat2& other) const
    {
        std::array<Vec2<T>, 2> m;
        for (int column = 0; column < 2; column++)
        {
            for (int row = 0; row < 2; row++)
            {
                T sum = 0;
                for(int i = 0; i < 2; ++i)
                {
                    sum += columns_[i][row]*other[column][i];
                }
                m[column][row] = sum;
            }
        }
        return Mat2{m};
    }

    constexpr Vec2<T> operator*(const Vec2<T>& other) const
    {
        Vec2<T> v;

        for (int row = 0; row < 2; row++)
        {
            T sum = 0;
            for(int i = 0; i < 2; ++i)
            {
                sum += columns_[i][row]*other[i];
            }
            v[row] = sum;
        }

        return v;
    }

    constexpr Mat2 operator*(T other) const
    {
        std::array<Vec2<T>, 2> m;

        for (int column = 0; column < 2; column++)
        {
            for (int row = 0; row < 2; row++)
            {
                m[column][row] = columns_[column][row] * other;
            }
        }

        return Mat2{m};
    }

    constexpr Mat2<T> Transpose() const
    {
        std::array<Vec2<T>, 2> v;
        for (int column = 0; column < 2; column++)
        {
            for (int row = 0; row < 2; row++)
            {
                v[row][column] = columns_[column][row];
            }
        }
        return Mat2<T>(v);
    }

    [[nodiscard]] constexpr float Determinant() const
    {
        return columns_[0][0] * columns_[1][1] - columns_[0][1] * columns_[1][0];
    }

    constexpr Mat2 Inverse() const
    {
        std::array<Vec2<T>, 2> v;
        const auto determinant = Determinant();
        v[0][0] = columns_[1][1];
        v[1][1] = columns_[0][0];
        v[0][1] = -columns_[0][1];
        v[1][0] = -columns_[1][0];
        Mat2 result(v);
        result = result * (1.0f/determinant);
        return result;
    }


    constexpr static Mat2<T> identity()
    {
        return Mat2{
                std::array<Vec2<T>, 2>
                                    {
                                            Vec2<T>(1, 0),
                                            Vec2<T>(0, 1)
                                    } };
    }
    constexpr static Mat2<T> zero()
    {
        return Mat2(
                std::array<Vec2<T>, 2>
                                    {
                                            Vec2<T>::zero(),
                                            Vec2<T>::zero()
                                    });
    }
private:
    std::array<Vec2<T>, 2> columns_; //row vector
};
}