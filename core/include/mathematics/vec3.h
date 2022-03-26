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


//-----------------------------------------------------------------------------
// Vec3
//-----------------------------------------------------------------------------
template<typename T>
struct Vec3
{
#ifdef _MSC_VER
    #pragma warning( push )
#pragma warning(disable : 4201)
#endif
    union
    {
        struct
        {
            T x; ///< X coordinate of the vector
            T y; ///< Y coordinate of the vector
            T z;
        };
        //For color
        struct
        {
            T r; ///< X coordinate of the vector
            T g; ///< Y coordinate of the vector
            T b;
        };
        T coord[3];
    };
#ifdef _MSC_VER
#pragma warning( pop )
#endif
    constexpr static Vec3 zero() { return Vec3(); }
    constexpr static Vec3 one() { return Vec3(1, 1, 1); }
    constexpr static Vec3 up() { return Vec3(0, 1, 0); }
    constexpr static Vec3 down() { return Vec3(0, -1, 0); }
    constexpr static Vec3 left() { return Vec3(-1, 0, 0); }
    constexpr static Vec3 right() { return Vec3(1, 0, 0); }
    constexpr static Vec3 forward() { return Vec3(0, 0, 1); }
    constexpr static Vec3 back() { return Vec3(0, 0, -1); }

    //-----------------------------------------------------------------------------
    // Constructors
    //-----------------------------------------------------------------------------
    constexpr Vec3()  noexcept : x(0), y(0), z(0)
    {
    }

    constexpr explicit Vec3(T same)  noexcept
            : x(same), y(same), z(same)
    {

    }

    constexpr Vec3(T X, T Y, T Z)  noexcept
            : x(X), y(Y), z(Z)
    {

    }

    constexpr explicit Vec3(const Vec2<T>& v)  noexcept : x(v.x), y(v.y), z(0)
    {

    }
    constexpr explicit Vec3(const Vec2<T>& v, T z)  noexcept : x(v.x), y(v.y), z(z)
    {

    }

    constexpr explicit operator Vec2<T>()
    {
        return {x, y};
    }

    template<typename U>
    constexpr explicit operator Vec2<T>()
    {
        return {static_cast<U>(x), static_cast<U>(y)};
    }

    /**
     * \brief Adding explicit constructor for vector-like type
     */
    template<class U>
    constexpr explicit Vec3(U u) noexcept : x(u.x), y(u.y), z(u.z)
    {

    }

    constexpr explicit Vec3(const T* ptr) noexcept : x(ptr[0]), y(ptr[1]), z(ptr[2])
    {

    }

    template<typename U>
    constexpr explicit Vec3(const Vec2<U>& vec2)  noexcept
            : x(static_cast<T>(vec2.x)),
              y(static_cast<T>(vec2.y)),
              z(static_cast<T>(0))
    {

    }

    template<typename U>
    constexpr explicit Vec3(const Vec3<U>& vector)  noexcept
            : x(static_cast<T>(vector.x)),
              y(static_cast<T>(vector.y)),
              z(static_cast<T>(vector.z))
    {
    }

    constexpr const T& operator[](size_t p_axis) const
    {
        return coord[p_axis];
    }

    constexpr T& operator[](size_t p_axis)
    {

        return coord[p_axis];
    }

    //-----------------------------------------------------------------------------
    // Operators
    //-----------------------------------------------------------------------------
    constexpr Vec3<T> operator+(const Vec3<T>& rhs) const
    {
        return Vec3<T>(x + rhs.x, y + rhs.y, z + rhs.z);
    }

    constexpr Vec3<T>& operator+=(const Vec3<T>& rhs)
    {
        this->x += rhs.x;
        this->y += rhs.y;
        this->z += rhs.z;
        return *this;
    }

    constexpr Vec3<T> operator-(const Vec3<T>& rhs) const
    {
        return Vec3<T>(x - rhs.x, y - rhs.y, z - rhs.z);
    }

    constexpr Vec3<T> operator-() const
    {
        return Vec3<T>(-x, -y , -z );
    }

    constexpr Vec3<T>& operator-=(const Vec3<T>& rhs)
    {
        this->x -= rhs.x;
        this->y -= rhs.y;
        this->z -= rhs.z;
        return *this;
    }

    constexpr Vec3<T> operator*(const Vec3<T>& rhs) const
    {
        return Vec3<T>(x * rhs.x, y * rhs.y, z * rhs.z);
    }


    constexpr Vec3<T>& operator*=(T rhs)
    {
        this->x *= rhs;
        this->y *= rhs;
        this->z *= rhs;
        return *this;
    }

    constexpr Vec3<T> operator/(T rhs) const
    {
        return (*this) * (1.0f / rhs);
    }

    constexpr Vec3<T>& operator/=(T rhs)
    {
        *this = *this / rhs;
        return *this;
    }

    constexpr bool operator==(const Vec3<T>& other) const
    {
        return x == other.x && y == other.y && z == other.z;
    }

    constexpr bool operator!=(const Vec3<T>& other) const
    {
        return !(*this == other);
    }

    explicit operator std::string() const
    {
        return fmt::format("Vec3({},{},{})", x, y, z);
    }

    //-----------------------------------------------------------------------------
    // Formulas
    //-----------------------------------------------------------------------------
    /// \brief Calculates the dot product from two vectors.
    constexpr  static T Dot(const Vec3<T> v1, const Vec3<T> v2)
    {
        return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
    }

    /// \brief Calculates the cross product from two Vec3.
    constexpr static Vec3<T> Cross(const Vec3<T> v1, const Vec3<T> v2)
    {
        return Vec3<T>(v1.y * v2.z - v1.z * v2.y,
                       v1.z * v2.x - v1.x * v2.z,
                       v1.x * v2.y - v1.y * v2.x);
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
    [[nodiscard]] Vec3<T> Normalized() const
    {
        return (*this) / (*this).Magnitude();
    }

    /// \brief Interpolate between two vectors.
    /// \param t the interpolation amount.
    constexpr static Vec3<T> Lerp(const Vec3<T>& v1, const Vec3<T>& v2, T t)
    {
        return v1 + (v2 - v1) * t;
    }

    /// \brief Reflect the inVec using the normal given (doesn't need to be normalized).
    /// \param inVec the vector to reflect.
    /// \param normal the normal vector of the line to reflect off.
    static Vec3<T> Reflect(const Vec3<T>& inVec, const Vec3<T>& normal)
    {
        Vec3<T> normalized = normal.Normalized();
        return inVec - 2 * Dot(inVec, normalized) * normalized;
    }

    static Vec3<T> Refract(const Vec3<T>& inVec, const Vec3<T>& normal, const T eta)
    {
        Vec3<T> N = normal.Normalized();
        const T k = 1 - eta * eta * (1.0 - Dot(N, inVec) * Dot(N, inVec));
        if(k < 0)
        {
            return Vec3<T>::zero();
        }
        else
        {
            return eta * inVec - (eta * Dot(N, inVec) + std::sqrt(k)) * N;
        }
    }

    /// \brief Project v1 on v2 (doesn't need to be normalized).
    /// \param v1 the vector to project.
    /// \param v2 the vector to project on.
    static Vec3<T> Project(const Vec3<T>& v1, const Vec3<T>& v2)
    {
        const auto dot = Dot(v1, v2);
        const auto mag = v2.Magnitude();
        return {(dot / mag) * v2.x,
                (dot / mag) * v2.y,
                (dot / mag) * v2.z};
    }

    static Radian AngleBetween(const Vec3& v1, const Vec3& v2);

};

//-----------------------------------------------------------------------------
// Vec3 Implementations
//-----------------------------------------------------------------------------
template<typename T>
Radian Vec3<T>::AngleBetween(const Vec3& v1, const Vec3& v2)
{
    const float dot = Vec3<T>::Dot(v1, v2) / v1.Magnitude() / v2.Magnitude();
    const float det = v1.x * v2.y - v1.y * v2.x;
    const auto angle = Atan2(det, dot);
    return angle;
}

template<typename T, typename U>
constexpr Vec3<T> operator*(U lhs, const Vec3<T>& rhs)
{
    return Vec3<T>(rhs.x * lhs, rhs.y * lhs, rhs.z * lhs);
}

template<typename T, typename U>
constexpr Vec3<T> operator*(const Vec3<T>& rhs, U lhs)
{
    return Vec3<T>(rhs.x * lhs, rhs.y * lhs, rhs.z * lhs);
}

//-----------------------------------------------------------------------------
// Vec3 Aliases
//-----------------------------------------------------------------------------
using Vec3f = Vec3<float>;
using Vec3df = Vec3<double>;
using Vec3i = Vec3<int>;
using Vec3u = Vec3<unsigned int>;
using Vec3b = Vec3<std::uint8_t>;
using EulerAngles = Vec3<Degree>;
using RadianAngles = Vec3<Radian>;


//-----------------------------------------------------------------------------
// NVec3
//-----------------------------------------------------------------------------
template<typename T, int N>
class alignas(N * sizeof(T)) NVec3
{
public:
    std::array<T, N> xs;
    std::array<T, N> ys;
    std::array<T, N> zs;

    //-----------------------------------------------------------------------------
    // Constructors
    //-----------------------------------------------------------------------------
    NVec3() noexcept
            : xs{}, ys{}, zs{}
    {
    }

    explicit NVec3(const Vec2<T>* soaV)
    {
        for (int i = 0; i < N; i++)
        {
            xs[i] = soaV[i].x;
            ys[i] = soaV[i].y;
            zs[i] = 0.0f;
        }
    }

    explicit NVec3(const Vec3<T>* soaV)
    {
        for (int i = 0; i < N; i++)
        {
            xs[i] = soaV[i].x;
            ys[i] = soaV[i].y;
            zs[i] = soaV[i].z;
        }
    }

    explicit NVec3(const Vec4<T>* soaV)
    {
        for (int i = 0; i < N; i++)
        {
            xs[i] = soaV[i].x;
            ys[i] = soaV[i].y;
            zs[i] = soaV[i].z;
        }
    }

    explicit NVec3(const std::array<Vec3<T>, N> soaV)
    {
        for (int i = 0; i < N; i++)
        {
            xs[i] = soaV[i].x;
            ys[i] = soaV[i].y;
            zs[i] = soaV[i].z;
        }
    }

    //-----------------------------------------------------------------------------
    // Operators
    //-----------------------------------------------------------------------------
    NVec3<T, N> operator+(const Vec3<T>& rhs) const
    {
        NVec3<T, N> v = this;
        std::for_each(v.xs.begin(), v.xs.end(), [rhs](T x) { return x + rhs.x; });
        std::for_each(v.ys.begin(), v.ys.end(), [rhs](T y) { return y + rhs.y; });
        std::for_each(v.zs.begin(), v.zs.end(), [rhs](T z) { return z + rhs.z; });
        return v;
    }

    NVec3<T, N>& operator+=(const Vec3<T>& rhs)
    {
        std::for_each(xs.begin(), xs.end(), [rhs](T x) { return x + rhs.x; });
        std::for_each(ys.begin(), ys.end(), [rhs](T y) { return y + rhs.y; });
        std::for_each(zs.begin(), zs.end(), [rhs](T z) { return z + rhs.z; });
        return *this;
    }

    NVec3<T, N> operator-(const Vec3<T>& rhs) const
    {
        NVec3<T, N> v = this;
        std::for_each(v.xs.begin(), v.xs.end(), [rhs](T x) { return x - rhs.x; });
        std::for_each(v.ys.begin(), v.ys.end(), [rhs](T y) { return y - rhs.y; });
        std::for_each(v.zs.begin(), v.zs.end(), [rhs](T z) { return z - rhs.z; });
        return v;
    }

    NVec3<T, N>& operator-=(const Vec3<T>& rhs)
    {
        std::for_each(xs.begin(), xs.end(), [rhs](T x) { return x - rhs.x; });
        std::for_each(ys.begin(), ys.end(), [rhs](T y) { return y - rhs.y; });
        std::for_each(zs.begin(), zs.end(), [rhs](T z) { return z - rhs.z; });
        return *this;
    }

    //-----------------------------------------------------------------------------
    // Formulas
    //-----------------------------------------------------------------------------
    static std::array<T, N> Dot(NVec3<T, N> v1, NVec3<T, N> v2)
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
            result[i] = xs[i] * xs[i] + ys[i] * ys[i] + zs[i] * zs[i];
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

    NVec3<T, N> Normalized() const
    {
        NVec3<T, N> result;
        std::array<T, N> mag = this->Magnitude();
        for (int i = 0; i < N; i++)
        {
            result.xs[i] = xs[i] / mag[i];
            result.ys[i] = ys[i] / mag[i];
            result.zs[i] = zs[i] / mag[i];
        }
        return result;
    }

    static NVec3<T, N> Lerp(const NVec3<T, N>& v1, const NVec3<T, N>& v2, T t)
    {
        NVec3<T, N> result;
        for (int i = 0; i < N; i++)
        {
            result.xs[i] = v1.xs[i] + (v2.xs[i] - v1.xs[i]) * t;
            result.ys[i] = v1.ys[i] + (v2.ys[i] - v1.ys[i]) * t;
            result.zs[i] = v1.zs[i] + (v2.zs[i] - v1.zs[i]) * t;
        }
        return result;
    }

    static NVec3<T, N> Reflect(const NVec3<T, N>& inVec, const NVec3<T, N>& normal)
    {
        NVec3<T, N> result;
        NVec3<T, N> normalized = normal.Normalized();
        std::array<T, N> dot = Dot(inVec, normal);
        for (int i = 0; i < N; i++)
        {
            result.xs[i] = inVec.xs[i] - normalized.xs[i] * 2 * dot[i];
            result.ys[i] = inVec.ys[i] - normalized.ys[i] * 2 * dot[i];
            result.zs[i] = inVec.zs[i] - normalized.zs[i] * 2 * dot[i];
        }
        return result;
    }

    static std::array<T, N> DotIntrinsics(NVec3<T, N> v1, NVec3<T, N> v2);
    static std::array<T, N> DotIntrinsics(NVec3<T, N> v1, Vec3<T> v2);
    std::array<T, N> SquareMagnitudeIntrinsics() const;
    std::array<T, N> MagnitudeIntrinsics() const;
    NVec3<T, N> NormalizedIntrinsics();
    static NVec3<T, N> ReflectIntrinsics(NVec3<T, N> inVec, NVec3<T, N> normal);
};

//-----------------------------------------------------------------------------
// NVec3 Aliases
//-----------------------------------------------------------------------------
using FourVec3f = NVec3<float, 4>;
using EightVec3f = NVec3<float, 8>;

//-----------------------------------------------------------------------------
// NVec3 Implementations
//-----------------------------------------------------------------------------
#ifdef __SSE__
template <>
std::array<float, 4> FourVec3f::DotIntrinsics(FourVec3f v1, FourVec3f v2);
template <>
std::array<float, 4> FourVec3f::DotIntrinsics(FourVec3f v1, Vec3f v2);
template<>
std::array<float, 4> FourVec3f::SquareMagnitudeIntrinsics() const;
template<>
std::array<float, 4> FourVec3f::MagnitudeIntrinsics() const;
template<>
FourVec3f FourVec3f::NormalizedIntrinsics();
template<>
FourVec3f FourVec3f::ReflectIntrinsics(FourVec3f inVec, FourVec3f normal);
#endif

#ifdef __AVX2__
template <>
std::array<float, 8> EightVec3f::DotIntrinsics(EightVec3f v1, EightVec3f v2);
template <>
std::array<float, 8> EightVec3f::DotIntrinsics(EightVec3f v1, Vec3f v2);
template<>
std::array<float, 8> EightVec3f::SquareMagnitudeIntrinsics() const;
template<>
 std::array<float, 8> EightVec3f::MagnitudeIntrinsics() const;
template<>
EightVec3f EightVec3f::NormalizedIntrinsics();
template<>
EightVec3f EightVec3f::ReflectIntrinsics(EightVec3f inVec, EightVec3f normal);
#endif

}