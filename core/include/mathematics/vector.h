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

#include <ostream>
#include <array>
#include "mathematics/const.h"
#include <engine/intrinsincs.h>
#include "mathematics/angle.h"
#include "mathematics/trigo.h"

#include <fmt/core.h>

namespace neko
{
//-----------------------------------------------------------------------------
// Forward Declarations
//-----------------------------------------------------------------------------
template<typename T>
struct Vec2;

template<typename T>
class Vec3;

template<typename T>
class alignas(4 * sizeof(T)) Vec4;

//-----------------------------------------------------------------------------
// Vec2
//-----------------------------------------------------------------------------
template<typename T>
struct Vec2
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

        };
        struct
        {
            T u; ///< X coordinate of the vector
            T v; ///< Y coordinate of the vector

        };
        T coord[2];
    };
#ifdef _MSC_VER
#pragma warning( pop )
#endif
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
        return Vec2(0,-1);
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
    constexpr explicit Vec2(const Vec2<U>& vector)  noexcept
            : x(static_cast<T>(vector.x)),
              y(static_cast<T>(vector.y))
    {
    }

    template<typename U>
    constexpr explicit Vec2(const Vec3<U>& vec3)  noexcept
            : x(static_cast<T>(vec3.x)),
              y(static_cast<T>(vec3.y))
    {
    }

    template<typename U>
    constexpr explicit Vec2(const Vec4<U>& vec4)  noexcept
            : x(static_cast<T>(vec4.x)),
              y(static_cast<T>(vec4.y))
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

    constexpr Vec2<T> operator -() const
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

    constexpr const T& operator[](size_t p_axis) const
    {

        return coord[p_axis];
    }

    constexpr T& operator[](size_t p_axis)
    {

        return coord[p_axis];
    }

    friend std::ostream& operator<<(std::ostream& os, const Vec2<T>& dt)
    {
        os << "Vec2(" << dt.x << "," << dt.y << ")";
        return os;
    }

    [[nodiscard]] auto ToString() const
    {
        return fmt::format("Vec2({},{})", x, y);
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
        constexpr auto dot = Dot(v1, v2);
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
// Vec3
//-----------------------------------------------------------------------------
template<typename T>
class Vec3
{
public:
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

    constexpr explicit Vec3(Vec2<T> v)  noexcept : x(v.x), y(v.y),z(0)
    {
	    
    }
    constexpr explicit Vec3(Vec2<T> v, T z)  noexcept : x(v.x), y(v.y), z(z)
    {

    }
    constexpr explicit Vec3(Vec4<T> v)  noexcept : x(v.x), y(v.y), z(v.z)
    {
	    
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


    template<typename U>
    constexpr explicit Vec3(const Vec4<U>& vec4)
            : x(static_cast<T>(vec4.x)),
              y(static_cast<T>(vec4.y)),
              z(static_cast<T>(vec4.z))
    {
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

    friend std::ostream& operator<<(std::ostream& os, const Vec3<T>& dt)
    {
        os << "Vec3(" << dt.x << "," << dt.y << "," << dt.z << ")";
        return os;
    }
    auto ToString()
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
            return Vec3<T>::zero;
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
// Vec4
//-----------------------------------------------------------------------------
template<typename T>
class alignas(4 * sizeof(T)) Vec4
{
public:
#ifdef _MSC_VER
#pragma warning( push )
#pragma warning(disable : 4201)
#endif
    union
    {
        struct
        {
            T x, y, z, w;
        };
        struct
        {
            T r, g, b, a;
        };
        T coord[4];
    };
#ifdef _MSC_VER
#pragma warning( pop )
#endif
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

    constexpr explicit Vec4(std::array<T, 4> v)  noexcept
    {
        for (int i = 0; i < 4; i++)
        {
            coord[i] = v[i];
        }
    }

    constexpr Vec4(T X, T Y, T Z, T W)  noexcept
            : x(X), y(Y), z(Z), w(W)
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

        return coord[p_axis];
    }

    constexpr T& operator[](size_t p_axis)
    {
        return coord[p_axis];
    }

    friend std::ostream& operator<<(std::ostream& os, const Vec4<T>& dt)
    {
        os << "Vec4(" << dt.x << "," << dt.y << "," << dt.z << "," << dt.w << ")";
        return os;
    }

    std::string ToString()
    {
        return fmt::format("Vec4({},{},{},{})", x, y, z, w);
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
using Vec4df = Vec4<float>;

//-----------------------------------------------------------------------------
// Vec4 Implementations
//-----------------------------------------------------------------------------

}
