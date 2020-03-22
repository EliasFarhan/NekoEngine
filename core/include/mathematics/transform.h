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

#include <mathematics/vector.h>
#include <mathematics/matrix.h>
#include <mathematics/const.h>
#include <mathematics/angle.h>
#include <mathematics/quaternion.h>
#include <engine/assert.h>

namespace neko
{

using Transform3d = neko::Mat4f;

template<>
inline Mat4f const Transform3d::TranslationMatrixFrom(const Vec3f translation)
{
    return Mat4f(
            std::array<Vec4f, 4>
                    {
                            Vec4f(1.0f, 0, 0, 0),
                            Vec4f(0, 1.0f, 0, 0),
                            Vec4f(0, 0, 1.0f, 0),
                            Vec4f(translation[0], translation[1], translation[2], 1)});
}

template<>
inline Mat4f const Transform3d::ScalingMatrixFrom(const Vec3f scale)
{
    return Mat4f(
            std::array<Vec4f, 4>
                    {
                            Vec4f(scale[0], 0, 0, 0),
                            Vec4f(0, scale[1], 0, 0),
                            Vec4f(0, 0, scale[2], 0),
                            Vec4f(0, 0, 0, 1)});
}

template<>
inline Mat4f const Transform3d::RotationMatrixFrom(const degree_t angle, const Vec3f axis)
{
    neko_assert(axis.Magnitude() - 1.0f < 0.0001f,
                "neko::Transform3d::RotationMatrixFrom(degree_t angle, Vec3f axis): Axis passed is not normalized.")

    const float x = axis[0];
    const float y = axis[1];
    const float z = axis[2];
    const float c = Cos(angle);
    const float s = Sin(angle);
    const float t = 1.0f - c;
    const float txx = t * x * x;
    const float tyy = t * y * y;
    const float tzz = t * z * z;
    const float txy = t * x * y;
    const float txz = t * x * z;
    const float tyz = t * y * z;
    const float sx = s * x;
    const float sy = s * y;
    const float sz = s * z;

    return Mat4f(
            std::array<Vec4f, 4>
                    {
                            Vec4f(txx + c, txy - sz, txz + sy, 0),
                            Vec4f(txy + sz, tyy + c, tyz - sx, 0),
                            Vec4f(txz - sy, tyz + sx, tzz + c, 0),
                            Vec4f(0, 0, 0, 1)});
}

template<>
inline Mat4<float> const Transform3d::RotationMatrixFrom(const EulerAngles cardinalRotation)
{
    const radian_t rotX = cardinalRotation[0];
    const radian_t rotY = cardinalRotation[1];
    const radian_t rotZ = cardinalRotation[2];
    const float cosX = Cos(rotX);
    const float sinX = Sin(rotX);
    const float cosY = Cos(rotY);
    const float sinY = Sin(rotY);
    const float cosZ = Cos(rotZ);
    const float sinZ = Sin(rotZ);

    const Mat4f matX = Mat4f(
            std::array<Vec4f, 4>
                    {
                            Vec4f(1, 0, 0, 0),
                            Vec4f(0, cosX, sinX, 0),
                            Vec4f(0, -sinX, cosX, 0),
                            Vec4f(0, 0, 0, 1)});
    const Mat4f matY = Mat4f(
            std::array<Vec4f, 4>
                    {
                            Vec4f(cosY, 0, -sinY, 0),
                            Vec4f(0, 1, 0, 0),
                            Vec4f(sinY, 0, cosY, 0),
                            Vec4f(0, 0, 0, 1)});
    const Mat4f matZ = Mat4f(
            std::array<Vec4f, 4>
                    {
                            Vec4f(cosZ, sinZ, 0, 0),
                            Vec4f(-sinZ, cosZ, 0, 0),
                            Vec4f(0, 0, 1, 0),
                            Vec4f(0, 0, 0, 1)});

    return matZ * matY * matX;
}

template<>
inline Mat4f const Transform3d::RotationMatrixFrom(const Quaternion& quaternion)
{
    const float x = quaternion.x;
    const float y = quaternion.y;
    const float z = quaternion.z;
    const float w = quaternion.w;
    const float xx = 2.0f * x * x;
    const float yy = 2.0f * y * y;
    const float zz = 2.0f * z * z;
    const float xy = 2.0f * z * y;
    const float xz = 2.0f * x * z;
    const float yz = 2.0f * y * z;
    const float zw = 2.0f * z * w;
    const float yw = 2.0f * y * w;
    const float xw = 2.0f * x * w;

    return Mat4f(
            std::array<Vec4f, 4>
                    {
                            Vec4f(1.0f - yy - zz, xy + zw, xz - yw, 0),
                            Vec4f(xy - zw, 1.0f - xx - zz, yz + xw, 0),
                            Vec4f(xz + yw, yz - xw, 1.0f - xx - yy, 0),
                            Vec4f(0, 0, 0, 1)
                    });
}


template<>
inline void Transform3d::Translate(const Vec3f translation)
{
    *this = TranslationMatrixFrom(translation) * (*this);
}

template<>
inline void Transform3d::Scale(const Vec3f scale)
{
    *this = ScalingMatrixFrom(scale) * (*this);
}

template<>
inline void Transform3d::Rotate(const degree_t angle, const Vec3f axis)
{
    *this = RotationMatrixFrom(angle, axis) * (*this);
}

template<>
inline void Transform3d::Rotate(const Quaternion& quaternion)
{
    *this = RotationMatrixFrom(quaternion) * (*this);
}

template<>
inline void Transform3d::Rotate(const EulerAngles eulerAngles)
{
    *this = RotationMatrixFrom(eulerAngles) * (*this);
}

// TODO: Works only with simple matrices! Not ones that combine multiple transformations!
template<>
inline Vec3f Transform3d::Position() const
{
    // Possible solution:: https://www.gamedev.net/forums/topic/467665-decomposing-rotationtranslationscale-from-matrix/#entry4076502
    const Vec4f column = columns_[3];
    return Vec3f(column[0], column[1], column[2]);
}

// TODO: Works only with simple matrices! Not ones that combine multiple transformations!
template<>
inline Vec3f Transform3d::Scale() const
{
    // Possible solution:: https://www.gamedev.net/forums/topic/467665-decomposing-rotationtranslationscale-from-matrix/#entry4076502
    const Transform3d transposed = Transpose();
    return Vec3f(
            transposed[0].Magnitude(),
            transposed[1].Magnitude(),
            transposed[2].Magnitude());
}

// TODO: Works only with simple matrices! Not ones that combine multiple transformations!
template<>
inline Mat4<float> Transform3d::RotationMatrix() const
{
    // Possible solution:: https://www.gamedev.net/forums/topic/467665-decomposing-rotationtranslationscale-from-matrix/#entry4076502
    const Vec3f scale = Scale();
    return Mat4f(
            std::array<Vec4f, 4>
                    {
                            Vec4f(columns_[0][0] / scale[0], columns_[0][1] / scale[0], columns_[0][2] / scale[0], 0),
                            Vec4f(columns_[1][0] / scale[1], columns_[1][1] / scale[1], columns_[1][2] / scale[1], 0),
                            Vec4f(columns_[2][0] / scale[2], columns_[2][1] / scale[2], columns_[2][2] / scale[2], 0),
                            Vec4f(0, 0, 0, 1)});
}

}
