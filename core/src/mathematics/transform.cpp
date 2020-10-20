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

#include "mathematics/vector.h"
#include "mathematics/matrix.h"
#include "mathematics/transform.h"
#include "mathematics/const.h"
#include "mathematics/angle.h"
#include "mathematics/quaternion.h"

namespace neko::Transform3d
{


Mat4f const TranslationMatrixFrom(const Vec3f translation)
{
    return Mat4f(
            std::array<Vec4f, 4>
                    {
                            Vec4f(1.0f, 0, 0, 0),
                            Vec4f(0, 1.0f, 0, 0),
                            Vec4f(0, 0, 1.0f, 0),
                            Vec4f(translation[0], translation[1], translation[2], 1)});
}


Mat4f const ScalingMatrixFrom(const Vec3f& scale)
{
    return Mat4f(
            std::array<Vec4f, 4>
                    {
                            Vec4f(scale[0], 0, 0, 0),
                            Vec4f(0, scale[1], 0, 0),
                            Vec4f(0, 0, scale[2], 0),
                            Vec4f(0, 0, 0, 1)});
}

Mat4f const RotationMatrixFrom(const degree_t angle, const Vec3f axis)
{
    const Vec3f normalizedAxis = axis.Normalized();

    const float x = normalizedAxis[0];
    const float y = normalizedAxis[1];
    const float z = normalizedAxis[2];
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


Mat4f const RotationMatrixFrom(const radian_t angle, const Vec3f axis)
{
    const Vec3f normalizedAxis = axis.Normalized();

    const float x = normalizedAxis[0];
    const float y = normalizedAxis[1];
    const float z = normalizedAxis[2];
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


Mat4<float> const RotationMatrixFrom(const EulerAngles cardinalRotation)
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
                            Vec4f(cosY, 0, 0, -sinY),
                            Vec4f(0, 1, 0, 0),
                            Vec4f(sinY, 0, 1, cosY),
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



Mat4f const RotationMatrixFrom(const Quaternion& q)
{
    const Mat4f left = Mat4f(std::array<Vec4f, 4>{
        Vec4f(q.w,-q.z,q.y,-q.x),
    	Vec4f(q.z,q.w,-q.x,-q.y),
    	Vec4f(-q.y,q.x,q.w,-q.z),
    	Vec4f(q.x,q.y,q.z,q.w)
    });
    const Mat4f right = Mat4f(std::array<Vec4f, 4>{
        Vec4f(q.w,-q.z,q.y,q.x),
    	Vec4f(q.z,q.w,-q.x,q.y),
    	Vec4f(-q.y,q.x,q.w,q.z),
    	Vec4f(-q.x,-q.y,-q.z,q.w)
    });

    return left*right;
}




Mat4f Translate(const Mat4f& transform, const Vec3f translation)
{
    return TranslationMatrixFrom(translation) * transform;
}


Mat4f Scale(const Mat4f& transform, const Vec3f scale)
{
    return ScalingMatrixFrom(scale) * transform;
}


Mat4f Rotate(const Mat4f& transform, const degree_t angle, const Vec3f axis)
{
    return RotationMatrixFrom(angle, axis) * transform;
}


Mat4f Rotate(const Mat4f& transform, const radian_t angle, const Vec3f axis)
{
    return RotationMatrixFrom(angle, axis) * transform;
}


Mat4f Rotate(const Mat4f& transform, const Quaternion& quaternion)
{
    return RotationMatrixFrom(quaternion) * transform;
}


Mat4f Rotate(const Mat4f& transform, const EulerAngles eulerAngles)
{
    const Quaternion quaternion = Quaternion::FromEuler(eulerAngles);
    return RotationMatrixFrom(quaternion) * transform;
}



Mat4f Perspective(radian_t fovy, float aspect, float nearPlane, float farPlane)
{
    neko_assert(fabsf(aspect - std::numeric_limits<float>::epsilon()) > 0.0f, "Aspect should not be zero");

    const float tanHalfFovy = Tan(fovy / 2.0f);
    Mat4f perspective{Mat4f::Zero};

    perspective[0][0] = 1.0f/ (aspect * tanHalfFovy);
    perspective[1][1] = 1.0f / (tanHalfFovy);
    perspective[2][2] = - (farPlane + nearPlane) / (farPlane - nearPlane);
    perspective[2][3] = - 1.0f;
    perspective[3][2] = - (2.0f * farPlane * nearPlane) / (farPlane - nearPlane);
    return perspective;
}

Mat4f Orthographic(float left, float right, float bottom, float top, float nearPlane, float farPlane)
{
    return Mat4f(std::array<Vec4f, 4>{
        Vec4f(2.0f / (right - left), 0, 0, 0),
            Vec4f(0, 2.0f / (top - bottom), 0, 0),
            Vec4f(0, 0, -2.0f / (farPlane - nearPlane), 0),
            Vec4f(-(right + left) / (right - left),
                -(top + bottom) / (top - bottom),
                -(farPlane + nearPlane) / (farPlane - nearPlane), 1.0f)
    });
}
}
