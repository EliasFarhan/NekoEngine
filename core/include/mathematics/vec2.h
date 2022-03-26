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

#include "mathematics/angle.h"
#include <fmt/core.h>
#include <fmt/compile.h>
#include <string>

namespace neko
{

template<typename T>
struct Vec3;

template<typename T>
struct Vec4;

template<typename T>
struct Vec2
{

    T x; ///< X coordinate of the vector
    T y; ///< Y coordinate of the vector




    constexpr static Vec2 zero()
    {
        return Vec2();
    }

    constexpr static Vec2 one()
    {
        return Vec2(1, 1);
    }

    constexpr static Vec2 up()
    {
        return Vec2(0, 1);
    }

    constexpr static Vec2 down()
    {
        return Vec2(0, -1);
    }

    constexpr static Vec2 left()
    {
        return Vec2(-1, 0);
    }

    constexpr static Vec2 right()
    {
        return Vec2(1, 0);
    }

    //-----------------------------------------------------------------------------
    // Constructors
    //-----------------------------------------------------------------------------
    constexpr Vec2() noexcept
            : x(0), y(0)
    {
    }

    constexpr explicit Vec2(T same) noexcept
            : x(same), y(same)
    {
    }

    constexpr Vec2(T X, T Y) noexcept
            : x(X), y(Y)
    {
    }

    template<typename U>
    constexpr explicit Vec2(const Vec2<U>& vector) noexcept
            : x(static_cast<T>(vector.x)),
              y(static_cast<T>(vector.y))
    {
    }

    //-----------------------------------------------------------------------------
    // Operators
    //-----------------------------------------------------------------------------
    constexpr Vec2<T> operator+(const Vec2<T>& rhs) const
    {
        return Vec2<T>(x + rhs.x, y + rhs.y);
    }

    constexpr Vec2<T>& operator+=(const Vec2<T>& rhs)
    {
        this->x += rhs.x;
        this->y += rhs.y;
        return *this;
    }

    constexpr Vec2<T> operator-(const Vec2<T>& rhs) const
    {
        return Vec2<T>(x - rhs.x, y - rhs.y);
    }

    constexpr Vec2<T> operator-() const
    {
        return Vec2<T>(-x, -y);
    }

    constexpr Vec2<T>& operator-=(const Vec2<T>& rhs)
    {
        this->x -= rhs.x;
        this->y -= rhs.y;
        return *this;
    }

    constexpr Vec2<T> operator*(const Vec2<T>& rhs) const
    {
        return Vec2<T>(x * rhs.x, y * rhs.y);
    }


    constexpr Vec2<T>& operator*=(T rhs)
    {
        this->x *= rhs;
        this->y *= rhs;
        return *this;
    }

    constexpr Vec2<T> operator/(const Vec2<T>& rhs) const
    {
        return Vec2<T>(x / rhs.x, y / rhs.y);
    }

    constexpr Vec2<T> operator/(T rhs) const
    {
        return (*this) * (1.0f / rhs);
    }

    constexpr Vec2<T>& operator/=(T rhs)
    {
        this->x /= rhs;
        this->y /= rhs;
        return *this;
    }

    constexpr bool operator==(const Vec2<T>& other) const
    {
        return x == other.x && y == other.y;
    }

    constexpr bool operator!=(const Vec2<T>& other) const
    {
        return !(*this == other);
    }

    constexpr const T& operator[](std::size_t p_axis) const
    {
        return p_axis == 0?x:y;
    }

    constexpr T& operator[](std::size_t p_axis)
    {

        return p_axis == 0?x:y;
    }

    [[nodiscard]] explicit operator std::string() const
    {
        return fmt::format(FMT_COMPILE("Vec2({},{})"), x, y);
    }

    //Used to specialize in case of other kind of vector
    template<typename U>
    constexpr explicit operator U() const;

    //-----------------------------------------------------------------------------
    // Formulas
    //-----------------------------------------------------------------------------
    /// \brief Calculates the dot product from two vectors.
    static constexpr T Dot(const Vec2<T>& v1, const Vec2<T>& v2)
    {
        return v1.x * v2.x + v1.y * v2.y;
    }

    /// \brief Calculates the square magnitude.
    [[nodiscard]] constexpr T SquareMagnitude() const
    {
        return Dot(*this, *this);
    }

    /// \brief Calculates the magnitude.
    [[nodiscard]] T Magnitude() const
    {
        return std::sqrt(SquareMagnitude());
    }

    /// \brief Calculates the normalized vector.
    [[nodiscard]] Vec2<T> Normalized() const
    {
        return (*this) / (*this).Magnitude();
    }

    /// \brief Interpolate between two vectors.
    /// \param t The interpolation amount.
    static constexpr Vec2<T> Lerp(const Vec2<T>& v1, const Vec2<T>& v2, T t)
    {
        return v1 + (v2 - v1) * t;
    }

    /// \brief Reflect the inVec using the normal given (doesn't need to be normalized).
    /// \param inVec The vector to reflect.
    /// \param normal The normal vector of the line to reflect off.
    static constexpr Vec2<T> Reflect(const Vec2<T>& inVec, const Vec2<T>& normal)
    {
        return inVec - normal * 2 * Dot(inVec, normal);
    }

    /// \brief Project v1 on v2 (doesn't need to be normalized).
    /// \param v1 The vector to project.
    /// \param v2 The vector to project on.
    static Vec2<T> Project(const Vec2<T>& v1, const Vec2<T>& v2)
    {
        const auto dot = Dot(v1, v2);
        const auto mag = v2.Magnitude();
        return {(dot / mag) * v2.x,
                (dot / mag) * v2.y};
    }

    /// \brief Calculates the angle between two vectors.
    static constexpr Radian AngleBetween(const Vec2& v1, const Vec2& v2);

    /// \brief Rotates the Vec2 from the given angle (in degrees).
    [[nodiscard]] Vec2<T> Rotate(Radian angle) const
    {
        return {x * Cos(angle) - y * Sin(angle),
                x * Sin(angle) + y * Cos(angle)};
    }

    //-----------------------------------------------------------------------------
    // Other
    //-----------------------------------------------------------------------------
    //Used to specialize in case of other kind of vector
    template<typename U>
    constexpr explicit Vec2(const U& v);

};


//-----------------------------------------------------------------------------
// Vec2 Implementations
//-----------------------------------------------------------------------------
template<typename T>
constexpr Radian Vec2<T>::AngleBetween(const Vec2& v1, const Vec2& v2)
{
    const float dot = Vec2<T>::Dot(v1, v2) / v1.Magnitude() / v2.Magnitude();
    const float det = v1.x * v2.y - v1.y * v2.x;
    const Radian angle = Atan2(det, dot);
    return angle;
}

template<typename T, typename U>
constexpr Vec2<T> operator*(U lhs, const Vec2<T>& rhs)
{
    return Vec2<T>(rhs.x * lhs, rhs.y * lhs);
}

template<typename T, typename U>
constexpr Vec2<T> operator*(const Vec2<T>& lhs, U rhs)
{
    return Vec2<T>(lhs.x * rhs, lhs.y * rhs);
}
//-----------------------------------------------------------------------------
// Vec2 Aliases
//-----------------------------------------------------------------------------
using Vec2f = Vec2<float>;
using Vec2df = Vec2<double>;
using Vec2i = Vec2<int>;
using Vec2u = Vec2<unsigned>;

//-----------------------------------------------------------------------------
// NVec2
//-----------------------------------------------------------------------------
template<typename T, int N>
class alignas(N * sizeof(T)) NVec2
{
public:
    std::array<T, N> xs;
    std::array<T, N> ys;

    //-----------------------------------------------------------------------------
    // Constructors
    //-----------------------------------------------------------------------------
    NVec2() noexcept
            : xs{}, ys{}
    {
    }

    explicit NVec2(const Vec2<T>* soaV)
    {
        for (int i = 0; i < N; i++)
        {
            xs[i] = soaV[i].x;
            ys[i] = soaV[i].y;
        }
    }

    explicit NVec2(const Vec3<T>* soaV)
    {
        for (int i = 0; i < N; i++)
        {
            xs[i] = soaV[i].x;
            ys[i] = soaV[i].y;
        }
    }

    explicit NVec2(const Vec4 <T>* soaV)
    {
        for (int i = 0; i < N; i++)
        {
            xs[i] = soaV[i].x;
            ys[i] = soaV[i].y;
        }
    }

    explicit NVec2(const std::array<Vec2<T>, N> soaV)
    {
        for (int i = 0; i < N; i++)
        {
            xs[i] = soaV[i].x;
            ys[i] = soaV[i].y;
        }
    }

    //-----------------------------------------------------------------------------
    // Operators
    //-----------------------------------------------------------------------------
    NVec2<T, N> operator+(const Vec2<T>& rhs) const
    {
        NVec2<T, N> v = this;
        std::for_each(v.xs.begin(), v.xs.end(), [rhs](T x) { return x + rhs.x; });
        std::for_each(v.ys.begin(), v.ys.end(), [rhs](T y) { return y + rhs.y; });
        return v;
    }

    NVec2<T, N>& operator+=(const Vec2<T>& rhs)
    {
        std::for_each(xs.begin(), xs.end(), [rhs](T x) { return x + rhs.x; });
        std::for_each(ys.begin(), ys.end(), [rhs](T y) { return y + rhs.y; });
        return *this;
    }

    NVec2<T, N> operator-(const Vec2<T>& rhs) const
    {
        NVec2<T, N> v = this;
        std::for_each(v.xs.begin(), v.xs.end(), [rhs](T x) { return x - rhs.x; });
        std::for_each(v.ys.begin(), v.ys.end(), [rhs](T y) { return y - rhs.y; });
        return v;
    }

    NVec2<T, N>& operator-=(const Vec2<T>& rhs)
    {
        std::for_each(xs.begin(), xs.end(), [rhs](T x) { return x - rhs.x; });
        std::for_each(ys.begin(), ys.end(), [rhs](T y) { return y - rhs.y; });
        return *this;
    }

    //-----------------------------------------------------------------------------
    // Formulas
    //-----------------------------------------------------------------------------
    static std::array<T, N> Dot(NVec2<T, N> v1, NVec2<T, N> v2)
    {
        std::array<T, N> result;
        for (int i = 0; i < N; i++)
        {
            result[i] = v1.xs[i] * v2.xs[i] +
                        v1.ys[i] * v2.ys[i];
        }
        return result;
    }

    std::array<T, N> SquareMagnitude() const
    {
        std::array<T, N> result;
        for (int i = 0; i < N; i++)
        {
            result[i] = xs[i] * xs[i] + ys[i] * ys[i];
        }
        return result;
    }

    std::array<T, N> Magnitude() const
    {
        std::array<T, N> result;
        std::array<T, N> sqrtMag = this->SquareMagnitude();
        for (int i = 0; i < N; i++)
        {
            result[i] = std::sqrt(sqrtMag[i]);
        }
        return result;
    }

    NVec2<T, N> Normalized() const
    {
        NVec2<T, N> result;
        std::array<T, N> mag = this->Magnitude();
        for (int i = 0; i < N; i++)
        {
            result.xs[i] = xs[i] / mag[i];
            result.ys[i] = ys[i] / mag[i];
        }
        return result;
    }

    static NVec2<T, N> Lerp(const NVec2<T, N>& v1, const NVec2<T, N>& v2, T t)
    {
        NVec2<T, N> result;
        for (int i = 0; i < N; i++)
        {
            result.xs[i] = v1.xs[i] + (v2.xs[i] - v1.xs[i]) * t;
            result.ys[i] = v1.ys[i] + (v2.ys[i] - v1.ys[i]) * t;
        }
        return result;
    }

    static NVec2<T, N> Reflect(const NVec2<T, N>& inVec, const NVec2<T, N>& normal)
    {
        NVec2<T, N> result;
        NVec2<T, N> normalized = normal.Normalized();
        std::array<T, N> dot = Dot(inVec, normal);
        for (int i = 0; i < N; i++)
        {
            result.xs[i] = inVec.xs[i] - normalized.xs[i] * 2 * dot[i];
            result.ys[i] = inVec.ys[i] - normalized.ys[i] * 2 * dot[i];
        }
        return result;
    }

    NVec2<T, N> Rotate(std::array<T, N> angles) const
    {
        NVec2<T, N> result;
        for (int i = 0; i < N; i++)
        {
            angles[i] *= PI / 180.0f;
            result.xs[i] = xs[i] * cos(angles[i]) - xs[i] * sin(angles[i]);
            result.ys[i] = ys[i] * sin(angles[i]) - ys[i] * cos(angles[i]);
        }
        return result;
    }

    static std::array<T, N> DotIntrinsics(NVec2<T, N> v1, NVec2<T, N> v2);

    static std::array<T, N> DotIntrinsics(NVec2<T, N> v1, Vec2<T> v2);

    std::array<T, N> SquareMagnitudeIntrinsics() const;

    std::array<T, N> MagnitudeIntrinsics() const;

    NVec2<T, N> NormalizedIntrinsics();
};

//-----------------------------------------------------------------------------
// NVec2 Aliases
//-----------------------------------------------------------------------------
using FourVec2f = NVec2<float, 4>;
using EightVec2f = NVec2<float, 8>;

//-----------------------------------------------------------------------------
// NVec2 Implementations
//-----------------------------------------------------------------------------
#ifdef __SSE__
template <>
std::array<float, 4> FourVec2f::DotIntrinsics(FourVec2f v1, FourVec2f v2);
template <>
std::array<float, 4> FourVec2f::DotIntrinsics(FourVec2f v1, Vec2f v2);
template<>
std::array<float, 4> FourVec2f::SquareMagnitudeIntrinsics() const;
template<>
std::array<float, 4> FourVec2f::MagnitudeIntrinsics() const;
template<>
FourVec2f FourVec2f::NormalizedIntrinsics();
#endif

#ifdef __AVX2__
template <>
std::array<float, 8> EightVec2f::DotIntrinsics(EightVec2f v1, EightVec2f v2);
template <>
std::array<float, 8> EightVec2f::DotIntrinsics(EightVec2f v1, Vec2f v2);
template<>
std::array<float, 8> EightVec2f::SquareMagnitudeIntrinsics() const;
template<>
std::array<float, 8> EightVec2f::MagnitudeIntrinsics() const;
template<>
EightVec2f EightVec2f::NormalizedIntrinsics();
#endif
}