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

#include "mathematics/vec4.h"
#include "mathematics/matrix3.h"
#include "mathematics/matrix2.h"
#include "mathematics/basic.h"
#include <array>
#include <ostream>

namespace neko
{

template<typename T>
class alignas(4 * sizeof(T)) Mat4
{
public:
    constexpr Mat4() : columns_(identity().columns_)
    {
    }

    constexpr Mat4& operator=(Mat4 m)
    {
        columns_ = m.columns_;
        return *this;
    }

    constexpr Mat4(const Mat4 & m) noexcept : columns_(m.columns_)
    {
    }

    constexpr explicit Mat4(const std::array<Vec4 < T>, 4> & v) : columns_(v)
    {
    }
    constexpr explicit Mat4(const Mat3<T>& m) noexcept
    {
        for (int col = 0; col < 4; col++)
        {
            for (int row = 0; row < 4; row++)
            {
                if(col == 3 || row == 3)
                {
                    columns_[col][row] = 0;
                }
                else
                {
                    columns_[col][row] = m[col][row];
                }
            }
        }
    }

    constexpr explicit operator Mat3<T>()
    {
        std::array<Vec3<T>, 3> m;
        for (int col = 0; col < 3; col++)
        {
            for (int row = 0; row < 3; row++)
            {
                m[col][row] = columns_[col][row];
            }
        }
        return Mat3<T>(m);
    }

    constexpr explicit operator Mat2<T>()
    {
        std::array<Vec2<T>, 2> m;
        for (int col = 0; col < 2; col++)
        {
            for (int row = 0; row < 2; row++)
            {
                m[col][row] = columns_[col][row];
            }
        }
        return Mat2<T>(m);
    }

    constexpr const T& operator()(size_t row, size_t column) const
    {
        return columns_[column][row];
    }

    constexpr T& operator()(size_t row, size_t column)
    {
        return columns_[column][row];
    }

    constexpr const Vec4 <T>& operator[](size_t column) const
    {
        return columns_[column];
    }

    constexpr Vec4 <T>& operator[](size_t column)
    {
        return columns_[column];
    }

    [[nodiscard]] inline Mat4<T> Transpose() const
    {
        std::array<Vec4<T>, 4> v;
        for (int column = 0; column < 4; column++)
        {
            for (int row = 0; row < 4; row++)
            {
                v[row][column] = columns_[column][row];
            }
        }
        return Mat4<T>(v);
    }

    constexpr Mat4<T> operator+(const Mat4<T> & rhs) const
    {
        std::array<Vec4<T>, 4> v;
        for (int i = 0; i < 4; i++)
        {
            v[i] = columns_[i] + rhs.columns_[i];
        }
        return Mat4<T>(v);
    }

    constexpr Mat4<T> operator-(const Mat4<T> & rhs) const
    {
        std::array<Vec4<T>, 4> v;
        for (int i = 0; i < 4; i++)
        {
            v[i] = columns_[i] - rhs.columns_[i];
        }
        return Mat4<T>(v);
    }

    constexpr Vec4 <T> operator*(const Vec4 <T> & rhs) const
    {
        Vec4<T> v;
        for (int column = 0; column < 4; column++)
        {
            v[column] = 0;
            for (int row = 0; row < 4; row++)
            {
                v[column] += columns_[column][row] * rhs[row];
            }
        }
        return Vec4<T>(v);
    }
    constexpr Mat4<T> operator*(float rhs) const noexcept
    {
        Mat4<T> m;
        for(int col = 0; col < 4; col++)
        {
            for(int row = 0; row < 4 ; row++)
            {
                m[col][row] = columns_[col][row] * rhs;
            }
        }
        return m;
    }
    constexpr Mat4<T> operator*(const Mat4<T> & rhs) const noexcept
    {
        return MultiplyIntrinsincs(rhs);
    }

    constexpr Mat4<T> MultiplyNaive(const Mat4<T> & rhs) const noexcept
    {
        std::array<Vec4<T>, 4> v;
        for (int column = 0; column < 4; column++)
        {
            for (int row = 0; row < 4; row++)
            {
                v[column][row] = 0;
                for (int i = 0; i < 4; i++)
                {
                    v[column][row] += columns_[i][row] * rhs[column][i];
                }
            }
        }
        return Mat4<T>(v);
    }

    constexpr Mat4<T> MultiplyTranpose(const Mat4<T> & rhs) const noexcept
    {
        const auto lhsT = this->Transpose();
        std::array<Vec4<T>, 4> v;
        for (int column = 0; column < 4; column++)
        {
            for (int row = 0; row < 4; row++)
            {
                v[column][row] = Vec4<T>::Dot(lhsT[row], Vec4<T>(rhs.columns_[column]));
            }
        }
        return Mat4<T>(v);
    }

    constexpr Mat4<T> MultiplyAoSoA(const Mat4<T> & rhs) const noexcept;

    Mat4<T> MultiplyIntrinsincs(const Mat4<T> & rhs) const noexcept;

    constexpr static T MatrixDifference(const Mat4<T> & rhs, const Mat4<T> & lhs)
    {
        T result = 0;
        for (int column = 0; column < 4; column++)
        {
            for (int row = 0; row < 4; row++)
            {
                result += std::abs(rhs[column][row] - lhs[column][row]);
            }
        }
        return result;
    };



    friend std::ostream& operator<<(std::ostream & os, const Mat4<T> & m)
    {
        for (int row = 0; row < 4; row++)
        {
            os << "(";
            for (int col = 0; col < 4; col++)
            {
                os << m[col][row] << ' ';
            }
            os << ")";
        }
        return os;
    }

    [[nodiscard]] constexpr float Determinant() const
    {
        float result = 0.0f;
        for (int i = 0; i < 4; i++)
        {
            const Mat3<T> m = Mat3<T>(std::array<Vec3<T>, 3>
                                                          {
                                                                  Vec3<T>(&columns_[(i + 1) % 4][1]),
                                                                  Vec3<T>(&columns_[(i + 2) % 4][1]),
                                                                  Vec3<T>(&columns_[(i + 3) % 4][1])
                                                          });
            result += (i % 2 == 1 ? -1.0f : 1.0f) * columns_[i][0] * m.Determinant();
        }
        return result;
    }
    [[nodiscard]] constexpr  Mat4 Inverse() const
    {
        Mat4 inverse = zero();
        const float determinant = Determinant();
        /*
        for (int i = 0; i < 4; i++)
        {
            const Mat3<T> sub = Mat3<T>(std::array<Vec3<T>, 3>
            {
                Vec3<T>(&columns_[(i + 1) % 4][1]),
                Vec3<T>(&columns_[(i + 2) % 4][1]),
                Vec3<T>(&columns_[(i + 3) % 4][1])
            });
            const float d = sub.Determinant();
            inverse[i][0] = (i % 2 == 1 ? -1.0f : 1.0f) * d;
            determinant += columns_[i][0] * d;
        }
        */
        if (Equal(determinant, 0.0f))
            return zero();
        //Calculate the cofactor matrix
        for (int col = 0; col < 4; col++)
        {
            for (int row = 0; row < 4; row++)
            {
                const Mat3 sub(std::array<Vec3<T>, 3>
                                                   {
                                                           Vec3f(columns_[(col + 1) % 4][(row + 1) % 4],
                                                                 columns_[(col + 1) % 4][(row + 2) % 4],
                                                                 columns_[(col + 1) % 4][(row + 3) % 4]),
                                                           Vec3f(columns_[(col + 2) % 4][(row + 1) % 4],
                                                                 columns_[(col + 2) % 4][(row + 2) % 4],
                                                                 columns_[(col + 2) % 4][(row + 3) % 4]),
                                                           Vec3f(columns_[(col + 3) % 4][(row + 1) % 4],
                                                                 columns_[(col + 3) % 4][(row + 2) % 4],
                                                                 columns_[(col + 3) % 4][(row + 3) % 4])
                                                   });
                inverse[col][row] = ((col + row) % 2 == 0 ? 1.0f : -1.0f) * sub.Determinant();
            }
        }
        inverse = inverse.Transpose();
        inverse = inverse * (1.0f / determinant);
        return inverse;
    }

    [[nodiscard]] constexpr  Mat3<T> ToMat3() const
    {
        std::array<Vec3<T>, 3> column;
        for(int col = 0; col < 3; col++)
        {
            for(int row = 0; row < 3; row++)
            {
                column[col][row] = columns_[col][row];
            }
        }
        return Mat3<T>(column);
    }
    constexpr  static Mat4<T> identity()
    {
        return Mat4(
                std::array<Vec4<T>, 4>
                                    {
                                            Vec4<T>(1, 0, 0, 0),
                                            Vec4<T>(0, 1, 0, 0),
                                            Vec4<T>(0, 0, 1, 0),
                                            Vec4<T>(0, 0, 0, 1)});
    }
    constexpr  static Mat4<T> zero()
    {
        return Mat4(
                std::array<Vec4f, 4>
                                  {
                                          Vec4<T>::zero(),
                                          Vec4<T>::zero(),
                                          Vec4<T>::zero(),
                                          Vec4<T>::zero()
                                  });
    }
private:
    std::array<Vec4 < T>, 4>
            columns_; //row vector
};

using Mat4f = Mat4<float>;

#ifdef __SSE__
template<>
Mat4f Mat4f::Transpose() const;
#endif

template<typename T>
constexpr Mat4<T> Mat4<T>::MultiplyAoSoA(const Mat4<T>& rhs) const noexcept
{
    const auto lhsT(Transpose());
    std::array<Vec4f, 4> v;
    for (int column = 0; column < 4; column++)
    {
        for (int row = 0; row < 4; row++)
        {
            const auto result = Vec4f::Dot(lhsT[row], rhs.columns_[column]);
            v[column][row] = result;
        }

    }
    return Mat4f(v);
}

template<>
Mat4f Mat4f::MultiplyIntrinsincs(const Mat4f& rhs) const noexcept;






}