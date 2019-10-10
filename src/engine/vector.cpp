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

#ifdef WIN32
#define _USE_MATH_DEFINES
#include <corecrt_math_defines.h>
#endif

#include <cmath>
#include <engine/vector.h>
#include <SFML/Audio.hpp>
#include <glm/glm.hpp>

namespace neko
{

Vec2f::Vec2f(float x, float y) : x(x), y(y)
{

}

Vec2f::Vec2f()
{

}

float Vec2f::GetMagnitude() const
{
    return sqrtf(x * x + y * y);
}

Vec2f Vec2f::Normalized() const
{
    return (*this) / (*this).GetMagnitude();
}

bool Vec2f::operator==(const Vec2f& rhs) const
{
    return x == rhs.x && y == rhs.y;
}

bool Vec2f::operator!=(const Vec2f& rhs) const
{
    return !(rhs == *this);
}

Vec2f Vec2f::operator+(const Vec2f& rhs) const
{
    return Vec2f(x + rhs.x, y + rhs.y);
}

Vec2f Vec2f::operator-(const Vec2f& rhs) const
{
    return Vec2f(x - rhs.x, y - rhs.y);
}

Vec2f Vec2f::operator*(float rhs) const
{
    return Vec2f(x * rhs, y * rhs);
}

Vec2f Vec2f::operator/(float rhs) const
{
    return (*this) * (1.0f / rhs);
}

Vec2f::operator sf::Vector2f() const
{
    return sf::Vector2f(x, y);
}

Vec2f::Vec2f(const sf::Vector2f& v)
{
    x = v.x;
    y = v.y;
}

Vec2f& Vec2f::operator+=(const Vec2f& rhs)
{
    this->x += rhs.x;
    this->y += rhs.y;
    return *this;
}

Vec2f Vec2f::Lerp(const Vec2f& v1, const Vec2f& v2, float t)
{
    return v1 + (v2 - v1) * t;
}

float Vec2f::AngleBetween(const Vec2f& v1, const Vec2f& v2)
{
	const float dot = Dot(v1, v2) / v1.GetMagnitude() / v2.GetMagnitude();
	
	const float det = v1.x * v2.y - v1.y * v2.x;
	const float angle = atan2(det, dot) / float(M_PI) * 180.0f;
    return angle;
}

float Vec2f::Dot(const Vec2f& v1, const Vec2f& v2)
{
    return v1.x * v2.x + v1.y * v2.y;
}

Vec2f& Vec2f::operator-=(const Vec2f& rhs)
{
    this->x -= rhs.x;
    this->y -= rhs.y;
    return *this;
}

Vec2f Vec2f::Rotate(float angle) const
{
    float radianAngle = glm::radians(angle);
    return Vec2f(cosf(radianAngle) * x - sinf(radianAngle) * y,
                 sinf(radianAngle) * x + cosf(radianAngle) * y);
}

Vec2f& Vec2f::operator/=(float rhs)
{
    this->x /= rhs;
    this->y /= rhs;
    return *this;
}
}
