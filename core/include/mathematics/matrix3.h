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


#include "mathematics/vec3.h"
#include <array>

namespace neko
{

template<typename T>
class Mat3
{
public:
    constexpr Mat3()
    {
        columns_ = identity().columns_;
    }
    constexpr Mat3& operator=(Mat3 m) noexcept
    {
        columns_ = m.columns_;
        return *this;
    }

    constexpr Mat3(const Mat3& m) noexcept
    {
        columns_ = m.columns_;
    }

    constexpr explicit Mat3(const std::array<Vec3 < T>, 3>& v) : columns_(v)
    {
    }

    constexpr const T& operator()(std::size_t row, std::size_t column) const
    {
        return columns_[column][row];
    }

    constexpr T& operator()(std::size_t row, std::size_t column)
    {
        return columns_[column][row];
    }

    constexpr const Vec3 <T>& operator[](std::size_t column) const
    {
        return columns_[column];
    }

    constexpr Vec3 <T>& operator[](std::size_t column)
    {
        return columns_[column];
    }


    constexpr Mat3 operator+(const Mat3& other) const
    {
        std::array<Vec3<T>, 3> m;
        for (int column = 0; column < 3; column++)
        {
            for (int row = 0; row < 3; row++)
            {
                m[row][column] = columns_[column][row]+other[column][row];
            }
        }
        return {m};
    }

    constexpr Mat3 operator-(const Mat3& other) const
    {
        std::array<Vec3<T>, 3> m;
        for (int column = 0; column < 3; column++)
        {
            for (int row = 0; row < 3; row++)
            {
                m[row][column] = columns_[column][row]-other[column][row];
            }
        }
        return {m};
    }

    constexpr Mat3 operator*(const Mat3& other) const
    {
        std::array<Vec3<T>, 3> m;
        for (int column = 0; column < 3; column++)
        {
            for (int row = 0; row < 3; row++)
            {
                T sum = 0;
                for(int i = 0; i < 3; ++i)
                {
                    sum += columns_[i][row]*other[column][i];
                }
                m[row][column] = sum;
            }
        }
        return {m};
    }

    constexpr Vec3<T> operator*(const Vec3<T>& other) const
    {
        Vec3<T> m;

        for (int row = 0; row < 3; row++)
        {
            T sum = 0;
            for(int i = 0; i < 3; ++i)
            {
                sum += columns_[i][row]*other[i];
            }
            m[row] = sum;
        }

        return {m};
    }

    constexpr Vec3<T> operator*(T other) const
    {
        std::array<Vec3<T>, 3> m;

        for (int column = 0; column < 3; column++)
        {
            for (int row = 0; row < 3; row++)
            {
                m[row][column] = columns_[column][row]*other;
            }
        }

        return {m};
    }

    constexpr Mat3<T> Transpose() const
    {
        std::array<Vec3<T>, 3> v;
        for (int column = 0; column < 3; column++)
        {
            for (int row = 0; row < 3; row++)
            {
                v[row][column] = columns_[column][row];
            }
        }
        return Mat3<T>(v);
    }

    [[nodiscard]] constexpr  float Determinant() const
    {
        return
                columns_[0][0] * columns_[1][1] * columns_[2][2] +
                columns_[1][0] * columns_[2][1] * columns_[0][2] +
                columns_[2][0] * columns_[0][1] * columns_[1][2] -
                columns_[2][0] * columns_[1][1] * columns_[0][2] -
                columns_[0][0] * columns_[2][1] * columns_[1][2] -
                columns_[1][0] * columns_[0][1] * columns_[2][2];
    }
    constexpr static Mat3<T> identity()
    {
        return {
                std::array<Vec3<T>, 3>
                                  {
                                          Vec3<T>(1, 0, 0),
                                          Vec3<T>(0, 1, 0),
                                          Vec3<T>(0, 0, 1)
                                  } };
    }
    constexpr static Mat3<T> zero()
    {
        return Mat3(
                std::array<Vec3<T>, 3>
                                    {
                                            Vec3<T>::zero(),
                                            Vec3<T>::zero(),
                                            Vec3<T>::zero()
                                    });
    }
private:
    std::array<Vec3<T>, 3> columns_; //row vector
};


using Mat3f = Mat3<float>;

}