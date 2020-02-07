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
#include <cmath>
#include "mathematics/const.h"
#include <mathematics/intrinsincs.h>

namespace neko
{
template<typename T>
struct Vec2
{
    union
    {
        struct
        {
            T x; ///< X coordinate of the vector
            T y; ///< Y coordinate of the vector

        };
        T coord[2];
    };

    Vec2()
    {
        x = y = 0;
    }

    explicit Vec2(T same)
            : x(same), y(same)
    {

    }

    Vec2(T X, T Y) noexcept
            : x(X), y(Y)
    {
    }

    template<typename U>
    explicit Vec2(const Vec2<U>& vector)
            : x(static_cast<T>(vector.x)), y(static_cast<T>(vector.y))
    {
    }

    const T& operator[](size_t p_axis) const
    {

        return coord[p_axis];
    }

    T& operator[](size_t p_axis)
    {

        return coord[p_axis];
    }

    T GetSquareMagnitude()
    {
        return x * x + y * y;
    }

    static T Dot(const Vec2<T>& v1, const Vec2<T>& v2)
    {
        return v1.x * v2.x + v1.y * v2.y;
    }

    Vec2<T> operator+(const Vec2<T>& rhs) const
    {
        return Vec2<T>(x + rhs.x, y + rhs.y);
    }

    Vec2<T>& operator+=(const Vec2<T>& rhs)
    {
        this->x += rhs.x;
        this->y += rhs.y;
        return *this;
    }

    Vec2<T> operator-(const Vec2<T>& rhs) const
    {
        return Vec2<T>(x - rhs.x, y - rhs.y);
    }

    Vec2<T>& operator-=(const Vec2<T>& rhs)
    {
        this->x -= rhs.x;
        this->y -= rhs.y;
        return *this;
    }

    Vec2<T> operator*(T rhs) const
    {
        return Vec2<T>(x * rhs, y * rhs);
    }

    Vec2<T> operator*(const Vec2<T>& rhs) const
    {
        return Vec2<T>(x * rhs.x, y * rhs.y);
    }

    Vec2<T>& operator*=(T rhs)
    {
        this->x *= rhs;
        this->y *= rhs;
        return *this;
    }

    Vec2<T> operator/(T rhs) const
    {
        return (*this) * (1.0f / rhs);
    }

    Vec2<T>& operator/=(T rhs)
    {
        this->x /= rhs;
        this->y /= rhs;
        return *this;
    }

    bool operator==(const Vec2<T>& right)
    {
        return x == right.x && y == right.y;
    }

    bool operator!=(const Vec2<T>& right)
    {
        return !(*this == right);
    }

    bool operator>=(const Vec2<T>& right) const
    {
        return x >= right.x && y >= right.y;
    }

    bool operator<=(const Vec2<T>& right) const
    {
        return x <= right.x && y <= right.y;
    }


    T GetMagnitude() const;

    Vec2<T> Normalized() const
    {
        return (*this) / (*this).GetMagnitude();
    }

    Vec2<T> Rotate(T angle) const;

    static Vec2<T> Lerp(const Vec2<T>& v1, const Vec2<T>& v2, T t)
    {
        return v1 + (v2 - v1) * t;
    }

    friend std::ostream& operator<<(std::ostream& os, const Vec2<T>& dt)
    {
        os << "Vec2(" << dt.x << "," << dt.y << ")";
        return os;
    }

    //Used to specialize in case of other kind of vector
    template<typename U>
    explicit Vec2(const U& v);

    //Used to specialize in case of other kind of vector
    template<typename U>
    explicit operator U() const;

    template<typename U = float>
    static U AngleBetween(const Vec2& v1, const Vec2& v2);

    const static Vec2 Zero;
    const static Vec2 One;
};

template<typename T>
template<typename U>
U Vec2<T>::AngleBetween(const Vec2& v1, const Vec2& v2)
{
    const U dot = Vec2<T>::Dot(v1, v2) / v1.GetMagnitude() / v2.GetMagnitude();
    const U det = v1.x * v2.y - v1.y * v2.x;
    const U angle = atan2(det, dot) / float(M_PI) * 180.0f;
    return angle;
}

template<typename T>
Vec2<T> operator*(T lhs, const Vec2<T>& rhs)
{
    return Vec2<T>(rhs.x * lhs, rhs.y * lhs);
}

using Vec2f = Vec2<float>;
using Vec2i = Vec2<int>;
using Vec2u = Vec2<unsigned>;

template <>
inline Vec2f const Vec2f::Zero = Vec2f(0.0f,0.0f);
template <>
inline Vec2f const Vec2f::One = Vec2f(1.0f,1.0f);

template<>
inline float Vec2f::GetMagnitude() const
{
    return sqrtf(x * x + y * y);
}

template<typename T>
class Vec3
{
public:
    union
    {
        struct
        {
            T x; ///< X coordinate of the vector
            T y; ///< Y coordinate of the vector
            T z;
        };
        T coord[3];
    };
    const static Vec3 Zero;
    const static Vec3 One;

    Vec3()
            : x(0), y(0), z(0)
    {
    }

    explicit Vec3(T same)
            : x(same), y(same), z(same)
    {

    }

    Vec3(T X, T Y, T Z)
            : x(X), y(Y), z(Z)
    {

    }

    template<typename U>
    explicit
    Vec3(const Vec3<U>& vector)
            : x(static_cast<T>(vector.x)),
              y(static_cast<T>(vector.y)),
              z(static_cast<T>(vector.z))
    {
    }

    const T& operator[](size_t p_axis) const
    {

        return coord[p_axis];
    }

    T& operator[](size_t p_axis)
    {

        return coord[p_axis];
    }

    T GetSquareMagnitude() const
    {
        return Dot(*this, *this);
    }

    template<typename ReturnT = float>
    ReturnT GetMagnitude() const
    {
        return std::sqrt(GetSquareMagnitude());
    }

    static T Dot(const Vec3<T> v1, const Vec3<T> v2)
    {
        return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
    }

    Vec3<T> operator+(const Vec3<T>& rhs) const
    {
        return Vec3<T>(x + rhs.x, y + rhs.y, z + rhs.z);
    }

    Vec3<T>& operator+=(const Vec3<T>& rhs)
    {
        this->x += rhs.x;
        this->y += rhs.y;
        this->z += rhs.z;
        return *this;
    }

    Vec3<T> operator-(const Vec3<T>& rhs) const
    {
        return Vec3<T>(x - rhs.x, y - rhs.y, z - rhs.z);
    }

    Vec3<T>& operator-=(const Vec3<T>& rhs)
    {
        this->x -= rhs.x;
        this->y -= rhs.y;
        this->z -= rhs.z;
        return *this;
    }

    Vec3<T> operator*(T rhs) const
    {
        return Vec3<T>(x * rhs, y * rhs, z * rhs);
    }

    Vec3<T> operator*(const Vec3<T>& rhs) const
    {
        return Vec3<T>(x * rhs.x, y * rhs.y, z * rhs.z);
    }

    Vec3<T>& operator*=(T rhs)
    {
        this->x *= rhs;
        this->y *= rhs;
        this->z *= rhs;
        return *this;
    }

    Vec3<T> operator/(T rhs) const
    {
        return (*this) * (1.0f / rhs);
    }

    Vec3<T>& operator/=(T rhs)
    {
        *this = *this / rhs;
        return *this;
    }

    bool operator==(const Vec3<T>& right) const
    {
        return x == right.x && y == right.y && z == right.z;
    }

    bool operator!=(const Vec3<T>& right) const
    {
        return !(*this == right);
    }

    bool operator>=(const Vec3<T>& right) const
    {
        return x >= right.x && y >= right.y && z >= right.z;
    }

    bool operator<=(const Vec3<T>& right) const
    {
        return x <= right.x && y <= right.y && z <= right.z;
    }

    friend std::ostream& operator<<(std::ostream& os, const Vec3<T>& dt)
    {
        os << "Vec3(" << dt.x << "," << dt.y << "," << dt.z << ")";
        return os;
    }

    template<typename U = float>
    static U AngleBetween(const Vec3& v1, const Vec3& v2);

};

template<typename T>
template<typename U>
U Vec3<T>::AngleBetween(const Vec3& v1, const Vec3& v2)
{
    const U dot = Vec3<T>::Dot(v1, v2) / v1.GetMagnitude() / v2.GetMagnitude();
    const U det = v1.x * v2.y - v1.y * v2.x;
    const U angle = atan2(det, dot) / float(M_PI) * 180.0f;
    return angle;
}




using Vec3f = Vec3<float>;
using Vec3i = Vec3<int>;
using Vec3u = Vec3<unsigned int>;

template <>
Vec3f inline const Vec3f::Zero = Vec3f(0.0f,0.0f, 0.0f);
template <>
Vec3f inline const Vec3f::One = Vec3f(1.0f,1.0f,1.0f);

template<typename T>
class alignas(4 * sizeof(T)) Vec4
{
public:
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
    const static Vec4 Zero;
    const static Vec4 One;

    Vec4() noexcept
            : x(0), y(0), z(0), w(0)
    {
    }

    explicit Vec4(T same)
            : x(same), y(same), z(same), w(same)
    {

    }

    explicit Vec4(std::array<T, 4> v)
    {
        for (int i = 0; i < 4; i++)
        {
            coord[i] = v[i];
        }
    }


    Vec4(T X, T Y, T Z, T W)
            : x(X), y(Y), z(Z), w(W)
    {

    }

    template<typename U>
    explicit Vec4(const Vec4<U>& vector)
            : x(static_cast<T>(vector.x)),
              y(static_cast<T>(vector.y)),
              z(static_cast<T>(vector.z)),
              w(static_cast<T>(vector.w))
    {
    }

    const T& operator[](size_t p_axis) const
    {

        return coord[p_axis];
    }

    T& operator[](size_t p_axis)
    {

        return coord[p_axis];
    }

    T GetSquareMagnitude() const
    {
        return Dot(*this, *this);
    }

    template<typename ReturnT = float>
    ReturnT GetMagnitude() const
    {
        return std::sqrt(GetSquareMagnitude());
    }

    static inline T Dot(const Vec4<T> v1, const Vec4<T> v2)
    {
        return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
    }

    static T Dot3(const Vec4<T> v1, const Vec4<T> v2)
    {
        return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
    }

    Vec4<T> operator+(const Vec4<T>& rhs) const
    {
        return Vec4<T>(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w);
    }

    Vec4<T>& operator+=(const Vec4<T>& rhs)
    {
        this->x += rhs.x;
        this->y += rhs.y;
        this->z += rhs.z;
        this->w += rhs.w;
        return *this;
    }

    Vec4<T> operator-(const Vec4<T>& rhs) const
    {
        return Vec4<T>(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w);
    }

    Vec4<T>& operator-=(const Vec4<T>& rhs)
    {
        this->x -= rhs.x;
        this->y -= rhs.y;
        this->z -= rhs.z;
        this->w -= rhs.w;
        return *this;
    }

    Vec4<T> operator*(T rhs) const
    {
        return Vec4<T>(x * rhs, y * rhs, z * rhs, w * rhs);
    }

    Vec4<T> operator*(const Vec4<T>& rhs) const
    {
        return Vec4<T>(x * rhs.x, y * rhs.y, z * rhs.z, w * rhs.w);
    }

    Vec4<T>& operator*=(T rhs)
    {
        this->x *= rhs;
        this->y *= rhs;
        this->z *= rhs;
        this->w *= rhs;
        return *this;
    }

    Vec4<T> operator/(T rhs) const
    {
        return (*this) * (1.0f / rhs);
    }

    Vec4<T>& operator/=(T rhs)
    {
        *this = *this / rhs;
        return *this;
    }

    bool operator==(const Vec4<T>& right) const
    {
        return x == right.x && y == right.y && z == right.z && w == right.w;
    }

    bool operator!=(const Vec4<T>& right) const
    {
        return !(*this == right);
    }

    static Vec4<T> AngleAxis(T angle, Vec3<T> axis);
    static Vec4<T> FromEulerAngles(Vec3<T> axisX);

};

using Vec4f = Vec4<float>;
using Quaternion = Vec4f;




template <>
Vec4f inline const Vec4f::Zero = Vec4f(0.0f,0.0f, 0.0f, 0.0f);
template <>
Vec4f inline const Vec4f::One = Vec4f(1.0f,1.0f,1.0f, 1.0f);

//TODO Implement quaternion specific method
template<>
inline Quaternion Quaternion::AngleAxis(float angle, Vec3f axis)
{
    (void)angle;
    (void)axis;
    return Zero;
}

template<>
inline Quaternion Quaternion::FromEulerAngles(Vec3f eulerAngles)
{
    (void) eulerAngles;
    return Zero;
}




}
