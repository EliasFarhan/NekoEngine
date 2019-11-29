#pragma once
/*
 MIT License

 Copyright (c) 2019 SAE Institute Switzerland AG

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
#include <glm/glm.hpp>

namespace neko
{
template <typename T>
class Vec2
{
public:

	Vec2() : x(0), y(0)
	{
	}
    explicit Vec2(T same) : x(same), y(same)
    {

    }
	Vec2(T X, T Y) : x(X), y(Y)
	{
	}

	template <typename U>
	explicit Vec2(const Vec2<U>& vector) : x(static_cast<T>(vector.x)), y(static_cast<T>(vector.y))
	{
	}

	T GetSquareMagnitude() { return x * x + y * y; }

	static T Dot(const Vec2<T>& v1, const Vec2<T>& v2)
	{
		return v1.x*v2.x + v1.y*v2.y;
	}
	
	Vec2<T> operator +(const Vec2<T>& rhs) const
	{
		return Vec2<T>(x + rhs.x, y + rhs.y);
	}
	Vec2<T>& operator +=(const Vec2<T>& rhs)
	{
		this->x += rhs.x;
		this->y += rhs.y;
		return *this;
	}

	Vec2<T> operator -(const Vec2<T>& rhs) const
	{
		return Vec2<T>(x - rhs.x, y - rhs.y);
	}
	Vec2<T>& operator -=(const Vec2<T>& rhs)
	{
		this->x -= rhs.x;
		this->y -= rhs.y;
		return *this;
	}

	Vec2<T> operator *(T rhs) const
	{
		return Vec2<T>(x * rhs, y * rhs);
	}

	Vec2<T> operator *(const Vec2<T>& rhs) const
	{
		return Vec2<T>(x * rhs.x, y * rhs.y);
	}

	

	Vec2<T>& operator *=(T rhs)
	{
		this->x *= rhs;
		this->y *= rhs;
		return *this;
	}

	Vec2<T> operator /( T rhs) const
	{
		return (*this) * (1.0f / rhs);
	}

	Vec2<T>& operator /=(T rhs)
	{
		this->x /= rhs;
		this->y /= rhs;
		return *this;
	}

	bool operator ==( const Vec2<T>& right)
	{
		return x == right.x && y == right.y;
	}

	bool operator !=( const Vec2<T>& right)
	{
		return !(*this == right);
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

	template <typename U>
	explicit Vec2(const U& v);
	template <typename U>
	explicit operator U() const;

	T x; ///< X coordinate of the vector
	T y; ///< Y coordinate of the vector

	const static Vec2 Zero;
	const static Vec2 One;
};
template<typename T>
Vec2<T> operator *(T lhs, const Vec2<T>& rhs)
{
	return Vec2<T>(rhs.x*lhs, rhs.y*lhs);
}

using Vec2f = Vec2<float>;


template <>
inline float Vec2f::GetMagnitude() const
{
	return sqrtf(x * x + y * y);
}

template <>
inline Vec2<float> Vec2f::Rotate(float angle) const
{
	const float radianAngle = glm::radians(angle);
	return Vec2f(cosf(radianAngle) * x - sinf(radianAngle) * y,
		sinf(radianAngle) * x + cosf(radianAngle) * y);
}



float AngleBetween(const Vec2f& v1, const Vec2f& v2);


template <typename T>
class Vec3
{
public:
    T x, y, z;
    const static Vec3 Zero;
    const static Vec3 One;
    Vec3() : x(0), y(0), z(0)
    {
    }

    explicit Vec3(T same) : x (same), y(same)
    {

    }

    Vec3(T X, T Y, T Z) : x(X), y(Y), z(Z)
    {

    }

    template <typename U>
    explicit Vec3(const Vec3<U>& vector) : x(static_cast<T>(vector.x)), y(static_cast<T>(vector.y))
    {
    }

    T GetSquareMagnitude() const { return x * x + y * y; }
    T GetMagnitude() const;
    static T Dot(const Vec2<T>& v1, const Vec2<T>& v2)
    {
        return v1.x*v2.x + v1.y*v2.y;
    }

    Vec3<T> operator +(const Vec3<T>& rhs)
    {
        return Vec3<T>(x + rhs.x, y + rhs.y, z + rhs.z);
    }
    Vec3<T>& operator +=(const Vec3<T>& rhs)
    {
        this->x += rhs.x;
        this->y += rhs.y;
        this->z += rhs.z;
        return *this;
    }

    Vec3<T> operator -(const Vec3<T>& rhs)
    {
        return Vec3<T>(x - rhs.x, y - rhs.y, z - rhs.z);
    }
    Vec3<T>& operator -=(const Vec3<T>& rhs)
    {
        this->x -= rhs.x;
        this->y -= rhs.y;
        this->z -= rhs.z;
        return *this;
    }

    Vec3<T> operator *(T rhs) const
    {
        return Vec3<T>(x * rhs, y * rhs, z * rhs);
    }
    Vec3<T> operator *(const Vec3<T>& rhs) const
    {
        return Vec3<T>(x * rhs.x, y * rhs.y, z * rhs.z);
    }



    Vec3<T>& operator *=(T rhs)
    {
        this->x *= rhs;
        this->y *= rhs;
        this->z *= rhs;
        return *this;
    }

    Vec3<T> operator /( T rhs) const
    {
        return (*this) * (1.0f / rhs);
    }

    Vec3<T>& operator /=(T rhs)
    {
        *this = *this / rhs;
        return *this;
    }

    bool operator ==( const Vec3<T>& right)
    {
        return x == right.x && y == right.y && z == right.z;
    }

    bool operator !=( const Vec3<T>& right)
    {
        return !(*this == right);
    }


};

using Vec3f = Vec3<float>;


template <>
inline float Vec3f::GetMagnitude() const
{
    return sqrtf(GetSquareMagnitude());
}
}
