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
#include <mathematics/vector.h>


namespace neko
{
template <>
Vec3f const Vec3f::Zero = Vec3f(0.0f,0.0f, 0.0f);
template <>
Vec3f const Vec3f::One = Vec3f(1.0f,1.0f,1.0f);

template<typename T>
template<typename U>
U Vec3<T>::AngleBetween(const Vec3& v1, const Vec3& v2)
{
    const U dot = Vec3<T>::Dot(v1, v2) / v1.GetMagnitude() / v2.GetMagnitude();
    const U det = v1.x * v2.y - v1.y * v2.x;
    const U angle = atan2(det, dot) / float(M_PI) * 180.0f;
    return angle;
}

template <>
Vec2f const Vec2f::Zero = Vec2f(0.0f,0.0f);
template <>
Vec2f const Vec2f::One = Vec2f(1.0f,1.0f);

template<typename T>
template<typename U>
U Vec2<T>::AngleBetween(const Vec2& v1, const Vec2& v2)
{
    const U dot = Vec2<T>::Dot(v1, v2) / v1.GetMagnitude() / v2.GetMagnitude();
    const U det = v1.x * v2.y - v1.y * v2.x;
    const U angle = atan2(det, dot) / float(M_PI) * 180.0f;
    return angle;
}

template <>
Vec4f const Vec4f::Zero = Vec4f(0.0f,0.0f, 0.0f, 0.0f);
template <>
Vec4f const Vec4f::One = Vec4f(1.0f,1.0f,1.0f, 1.0f);

}
