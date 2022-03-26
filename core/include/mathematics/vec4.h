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
#include "mathematics/vec3.h"

#include "fmt/compile.h"


namespace neko
{

//-----------------------------------------------------------------------------
// Vec4
//-----------------------------------------------------------------------------
template<typename T>
struct alignas(4 * sizeof(T)) Vec4
{

    T x, y, z, w;

    constexpr static Vec4 zero() { return Vec4(); }
    constexpr static Vec4 one() { return Vec4(1, 1, 1, 1); }

    //-----------------------------------------------------------------------------
    // Constructors
    //-----------------------------------------------------------------------------
    constexpr Vec4() noexcept
            : x(0), y(0), z(0), w(0)
    {
    }

    constexpr explicit Vec4(T same)  noexcept
            : x(same), y(same), z(same), w(same)
    {

    }

    constexpr explicit Vec4(const std::array<T, 4>& v)  noexcept
    {
        x = v[0];
        y = v[1];
        z = v[2];
        w = v[3];
    }

    constexpr Vec4(T X, T Y, T Z, T W)  noexcept
            : x(X), y(Y), z(Z), w(W)
    {

    }

    constexpr Vec4(Vec2<T> v) noexcept
    : x(v.x), y(v.y), z(0), w(0)
    {

    }

    constexpr Vec4(Vec2<T> v, T z, T w) noexcept
            : x(v.x), y(v.y), z(z), w(w)
    {

    }

    constexpr Vec4(Vec3<T> v) noexcept
            : x(v.x), y(v.y), z(v.z), w(0)
    {

    }

    constexpr Vec4(Vec3<T> v, T w) noexcept
            : x(v.x), y(v.y), z(v.z), w(w)
    {

    }

    constexpr Vec4(Vec2<T> v1, Vec2<T> v2) noexcept
        : x(v1.x), y(v1.y), z(v2.x), w(v2.y)
    {

    }

    template<typename U>
    constexpr explicit Vec4(const Vec2<U>& vec2)  noexcept
            : x(static_cast<T>(vec2.x)),
              y(static_cast<T>(vec2.y)),
              z(static_cast<T>(0)),
              w(static_cast<T>(0))
    {
    }

    template<typename U>
    constexpr explicit Vec4(const Vec3<U>& vec3)  noexcept
            : x(static_cast<T>(vec3.x)),
              y(static_cast<T>(vec3.y)),
              z(static_cast<T>(vec3.z)),
              w(static_cast<T>(0))
    {
    }

    template<typename U>
    constexpr explicit Vec4(const Vec3<U>& vec3, U w)  noexcept
            : x(static_cast<T>(vec3.x)),
              y(static_cast<T>(vec3.y)),
              z(static_cast<T>(vec3.z)),
              w(static_cast<T>(w))
    {
    }

    template<typename U>
    constexpr explicit Vec4(const Vec4<U>& vector)  noexcept
            : x(static_cast<T>(vector.x)),
              y(static_cast<T>(vector.y)),
              z(static_cast<T>(vector.z)),
              w(static_cast<T>(vector.w))
    {
    }

    constexpr explicit operator Vec2<T>() noexcept
    {
        return {x, y};
    }

    template<typename U>
    constexpr explicit operator Vec2<U>() noexcept
    {
        return {static_cast<U>(x), static_cast<U>(y)};
    }

    constexpr explicit operator Vec3<T>() noexcept
    {
        return {x, y, z};
    }

    template<typename U>
    constexpr explicit operator Vec3<U>() noexcept
    {
        return {static_cast<U>(x), static_cast<U>(y), static_cast<U>(z)};
    }

    //-----------------------------------------------------------------------------
    // Operators
    //-----------------------------------------------------------------------------
    constexpr Vec4<T> operator+(const Vec4<T>& rhs) const
    {
        return Vec4<T>(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w);
    }

    constexpr Vec4<T>& operator+=(const Vec4<T>& rhs)
    {
        this->x += rhs.x;
        this->y += rhs.y;
        this->z += rhs.z;
        this->w += rhs.w;
        return *this;
    }

    constexpr Vec4<T> operator-(const Vec4<T>& rhs) const
    {
        return Vec4<T>(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w);
    }

    constexpr Vec4<T> operator -() const
    {
        return Vec4<T>(-x, -y, -z, -w);
    }

    constexpr Vec4<T>& operator-=(const Vec4<T>& rhs)
    {
        this->x -= rhs.x;
        this->y -= rhs.y;
        this->z -= rhs.z;
        this->w -= rhs.w;
        return *this;
    }

    constexpr Vec4<T> operator*(const Vec4<T>& rhs) const
    {
        return Vec4<T>(x * rhs.x, y * rhs.y, z * rhs.z, w * rhs.w);
    }

    constexpr Vec4<T>& operator*=(T rhs)
    {
        this->x *= rhs;
        this->y *= rhs;
        this->z *= rhs;
        this->w *= rhs;
        return *this;
    }

    constexpr Vec4<T> operator/(T rhs) const
    {
        return (*this) * (1.0f / rhs);
    }

    constexpr Vec4<T>& operator/=(T rhs)
    {
        *this = *this / rhs;
        return *this;
    }

    constexpr bool operator==(const Vec4<T>& other) const
    {
        return x == other.x && y == other.y && z == other.z && w == other.w;
    }

    constexpr bool operator!=(const Vec4<T>& other) const
    {
        return !(*this == other);
    }

    constexpr const T& operator[](size_t p_axis) const
    {
        return p_axis == 0?x:p_axis==1?y:p_axis==2?z:w;
    }

    constexpr T& operator[](size_t p_axis)
    {
        return p_axis == 0?x:p_axis==1?y:p_axis==2?z:w;
    }

    explicit operator auto() const
    {
        return fmt::format(FMT_COMPILE("Vec4({},{},{},{})"), x, y, z, w);
    }

    //-----------------------------------------------------------------------------
    // Formulas
    //-----------------------------------------------------------------------------
    /// \brief Calculates the dot product from two vectors.
    constexpr static T Dot(const Vec4<T> v1, const Vec4<T> v2)
    {
        return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
    }

    /// \brief Calculates the 3D dot product from two vectors.
    constexpr static T Dot3(const Vec4<T> v1, const Vec4<T> v2)
    {
        return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
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
    [[nodiscard]] Vec4<T> Normalized() const
    {
        return (*this) / (*this).Magnitude();
    }

    /// \brief Interpolate between two vectors.
    /// \param t the interpolation amount.
    constexpr static Vec4<T> Lerp(const Vec4<T>& v1, const Vec4<T>& v2, T t)
    {
        return v1 + (v2 - v1) * t;
    }

    /// \brief Project v1 on v2 (doesn't need to be normalized).
    /// \param v1 the vector to project.
    /// \param v2 the vector to project on.
    static Vec4<T> Project(const Vec4<T>& v1, const Vec4<T>& v2)
    {
        const auto dot = Dot(v1, v2);
        const auto mag = v2.Magnitude();
        return {(dot / mag) * v2.x,
                (dot / mag) * v2.y,
                (dot / mag) * v2.z,
                (dot / mag) * v2.w};
    }
};


template<typename T>
constexpr Vec4<T> operator*(T lhs, const Vec4<T>& rhs)
{
    return Vec4<T>(rhs.x * lhs, rhs.y * lhs, rhs.z * lhs, rhs.w * lhs);
}

template<typename T>
constexpr Vec4<T> operator*(const Vec4<T>& rhs, T lhs)
{
    return Vec4<T>(rhs.x * lhs, rhs.y * lhs, rhs.z * lhs, rhs.w * lhs);
}
//-----------------------------------------------------------------------------
// Vec4 Aliases
//-----------------------------------------------------------------------------
using Vec4f = Vec4<float>;
using Vec4df = Vec4<double>;
using Vec4i = Vec4<int>;


//-----------------------------------------------------------------------------
// NVec4
//-----------------------------------------------------------------------------
template<typename T, int N>
class alignas(N * sizeof(T)) NVec4
{
public:
    std::array<T, N> xs;
    std::array<T, N> ys;
    std::array<T, N> zs;
    std::array<T, N> ws;

    //-----------------------------------------------------------------------------
    // Constructors
    //-----------------------------------------------------------------------------
    NVec4() noexcept
            : xs{}, ys{}, zs{}, ws{}
    {
    }

    explicit NVec4(const Vec2<T>* soaV)
    {
        for (int i = 0; i < N; i++)
        {
            xs[i] = soaV[i].x;
            ys[i] = soaV[i].y;
            zs[i] = 0;
            ws[i] = 0;
        }
    }

    explicit NVec4(const Vec3<T>* soaV)
    {
        for (int i = 0; i < N; i++)
        {
            xs[i] = soaV[i].x;
            ys[i] = soaV[i].y;
            zs[i] = soaV[i].z;
            ws[i] = 0;
        }
    }

    explicit NVec4(const Vec4<T>* soaV)
    {
        for (int i = 0; i < N; i++)
        {
            xs[i] = soaV[i].x;
            ys[i] = soaV[i].y;
            zs[i] = soaV[i].z;
            ws[i] = soaV[i].w;
        }
    }

    explicit NVec4(const std::array<Vec4<T>, N> soaV)
    {
        for (int i = 0; i < N; i++)
        {
            xs[i] = soaV[i].x;
            ys[i] = soaV[i].y;
            zs[i] = soaV[i].z;
            ws[i] = soaV[i].w;
        }
    }

    //-----------------------------------------------------------------------------
    // Operators
    //-----------------------------------------------------------------------------
    NVec4<T, N> operator+(const Vec4<T>& rhs) const
    {
        NVec4<T, N> v = this;
        std::for_each(v.xs.begin(), v.xs.end(), [rhs](T x) { return x + rhs.x; });
        std::for_each(v.ys.begin(), v.ys.end(), [rhs](T y) { return y + rhs.y; });
        std::for_each(v.zs.begin(), v.zs.end(), [rhs](T z) { return z + rhs.z; });
        std::for_each(v.ws.begin(), v.ws.end(), [rhs](T w) { return w + rhs.w; });
        return v;
    }

    NVec4<T, N>& operator+=(const Vec4<T>& rhs)
    {
        std::for_each(xs.begin(), xs.end(), [rhs](T x) { return x + rhs.x; });
        std::for_each(ys.begin(), ys.end(), [rhs](T y) { return y + rhs.y; });
        std::for_each(zs.begin(), zs.end(), [rhs](T z) { return z + rhs.z; });
        std::for_each(ws.begin(), ws.end(), [rhs](T w) { return w + rhs.w; });
        return *this;
    }

    NVec4<T, N> operator-(const Vec4<T>& rhs) const
    {
        NVec4<T, N> v = this;
        std::for_each(v.xs.begin(), v.xs.end(), [rhs](T x) { return x - rhs.x; });
        std::for_each(v.ys.begin(), v.ys.end(), [rhs](T y) { return y - rhs.y; });
        std::for_each(v.zs.begin(), v.zs.end(), [rhs](T z) { return z - rhs.z; });
        std::for_each(v.ws.begin(), v.ws.end(), [rhs](T w) { return w - rhs.w; });
        return v;
    }

    NVec4<T, N>& operator-=(const Vec4<T>& rhs)
    {
        std::for_each(xs.begin(), xs.end(), [rhs](T x) { return x - rhs.x; });
        std::for_each(ys.begin(), ys.end(), [rhs](T y) { return y - rhs.y; });
        std::for_each(zs.begin(), zs.end(), [rhs](T z) { return z - rhs.z; });
        std::for_each(ws.begin(), ws.end(), [rhs](T w) { return w - rhs.w; });
        return *this;
    }

    //-----------------------------------------------------------------------------
    // Formulas
    //-----------------------------------------------------------------------------
    static std::array<T, N> Dot(NVec4<T, N> v1, NVec4<T, N> v2)
    {
        std::array<T, N> result;
        for (int i = 0; i < N; i++)
        {
            result[i] = v1.xs[i] * v2.xs[i] +
                        v1.ys[i] * v2.ys[i] +
                        v1.zs[i] * v2.zs[i] +
                        v1.ws[i] * v2.ws[i];
        }
        return result;
    }

    static std::array<T, N> Dot3(NVec4<T, N> v1, NVec4<T, N> v2)
    {
        std::array<T, N> result;
        for (int i = 0; i < N; i++)
        {
            result[i] = v1.xs[i] * v2.xs[i] +
                        v1.ys[i] * v2.ys[i] +
                        v1.zs[i] * v2.zs[i];
        }
        return result;
    }

    std::array<T, N> SquareMagnitude() const
    {
        std::array<T, N> result;
        for (int i = 0; i < N; i++)
        {
            result[i] = xs[i] * xs[i] + ys[i] * ys[i] + zs[i] * zs[i] + ws[i] * ws[i];
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

    NVec4<T, N> Normalized() const
    {
        NVec4<T, N> result;
        std::array<T, N> mag = this->Magnitude();
        for (int i = 0; i < N; i++)
        {
            result.xs[i] = xs[i] / mag[i];
            result.ys[i] = ys[i] / mag[i];
            result.zs[i] = zs[i] / mag[i];
            result.ws[i] = ws[i] / mag[i];
        }
        return result;
    }

    static NVec4<T, N> Lerp(NVec4<T, N> v1, NVec4<T, N> v2, T t)
    {
        NVec4<T, N> result;
        for (int i = 0; i < N; i++)
        {
            result.xs[i] = v1.xs[i] + (v2.xs[i] - v1.xs[i]) * t;
            result.ys[i] = v1.ys[i] + (v2.ys[i] - v1.ys[i]) * t;
            result.zs[i] = v1.zs[i] + (v2.zs[i] - v1.zs[i]) * t;
            result.ws[i] = v1.ws[i] + (v2.ws[i] - v1.ws[i]) * t;
        }
        return result;
    }

    static std::array<T, N> DotIntrinsics(NVec4<T, N> v1, NVec4<T, N> v2);
    static std::array<T, N> DotIntrinsics(NVec4<T, N> v1, Vec4<T> v2);
    std::array<T, N> SquareMagnitudeIntrinsics() const;
    std::array<T, N> MagnitudeIntrinsics() const;
    NVec4<T, N> NormalizedIntrinsics();
};

//-----------------------------------------------------------------------------
// NVec4 Aliases
//-----------------------------------------------------------------------------
using FourVec4f = NVec4<float, 4>;
using EightVec4f = NVec4<float, 8>;

//-----------------------------------------------------------------------------
// NVec4 Implementations
//-----------------------------------------------------------------------------
#if defined(__arm__) || defined(__aarch64__) || defined(EMSCRIPTEN)
template<>
std::array<float, 4> FourVec4f::MagnitudeIntrinsics() const;
#endif
#ifdef __SSE__
template <>
std::array<float, 4> FourVec4f::DotIntrinsics(FourVec4f v1, FourVec4f v2);
template <>
std::array<float, 4> FourVec4f::DotIntrinsics(FourVec4f v1, Vec4f v2);
template<>
std::array<float, 4> FourVec4f::SquareMagnitudeIntrinsics() const;
template<>
std::array<float, 4> FourVec4f::MagnitudeIntrinsics() const;
template<>
FourVec4f FourVec4f::NormalizedIntrinsics();
#endif

#ifdef __AVX2__
template <>
std::array<float, 8> EightVec4f::DotIntrinsics(EightVec4f v1, EightVec4f v2);
template <>
std::array<float, 8> EightVec4f::DotIntrinsics(EightVec4f v1, Vec4f v2);
template<>
std::array<float, 8> EightVec4f::SquareMagnitudeIntrinsics() const;
template<>
std::array<float, 8> EightVec4f::MagnitudeIntrinsics() const;

template<>
EightVec4f EightVec4f::NormalizedIntrinsics();
#endif
}