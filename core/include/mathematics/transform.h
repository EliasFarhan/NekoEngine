#pragma once

#include "cmath"
#include "mathematics/vector.h"
#include "mathematics/matrix.h"
#include "const.h"
#include "angle.h"

namespace neko
{

using Transform3d = neko::Mat4f;

template<>
inline Mat4f const Transform3d::TranslationMatrixFrom(const Vec3f translation)
{
    return Mat4f(
            std::array<Vec4f, 4>
                    {
                            Vec4f(1.0f, 0, 0, translation[0]),
                            Vec4f(0, 1.0f, 0, translation[1]),
                            Vec4f(0, 0, 1.0f, translation[2]),
                            Vec4f(0, 0, 0, 1)});
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
    const float angleAsFloat = static_cast<radian_t>(angle).to<float>();
    const Vec3f normalizedAxis = axis / axis.GetMagnitude();

    const float x = axis[0];
    const float y = axis[1];
    const float z = axis[2];
    const float c = std::cos(angleAsFloat);
    const float s = std::sin(angleAsFloat);
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
inline Mat4f const Transform3d::RotationMatrixFrom(const radian_t angle, const Vec3f axis)
{
    const float angleAsFloat = angle.to<float>();
    const Vec3f normalizedAxis = axis / axis.GetMagnitude();

    const float x = axis[0];
    const float y = axis[1];
    const float z = axis[2];
    const float c = std::cos(angleAsFloat);
    const float s = std::sin(angleAsFloat);
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
    const float cosX = std::cos(rotX.value());
    const float sinX = std::sin(rotX.value());
    const float cosY = std::cos(rotY.value());
    const float sinY = std::sin(rotY.value());
    const float cosZ = std::cos(rotZ.value());
    const float sinZ = std::sin(rotZ.value());

    const Mat4f matX = Mat4f(
            std::array<Vec4f, 4>
                    {
                            Vec4f(1, 0, 0, 0),
                            Vec4f(0, cosX, -sinX, 0),
                            Vec4f(0, sinX, cosX, 0),
                            Vec4f(0, 0, 0, 1)});
    const Mat4f matY = Mat4f(
            std::array<Vec4f, 4>
                    {
                            Vec4f(cosY, 0, sinY, 0),
                            Vec4f(0, 1, 0, 0),
                            Vec4f(0, 0, 1, 0),
                            Vec4f(-sinY, 0, cosY, 1)});
    const Mat4f matZ = Mat4f(
            std::array<Vec4f, 4>
                    {
                            Vec4f(cosZ, -sinZ, 0, 0),
                            Vec4f(sinZ, cosZ, 0, 0),
                            Vec4f(0, 0, 1, 0),
                            Vec4f(0, 0, 0, 1)});

    return matZ * matY * matX;
}

template<>
inline Mat4<float> const Transform3d::RotationMatrixFrom(const RadianAngles cardinalRotation)
{
    return RotationMatrixFrom(cardinalRotation);
}

template<>
inline Mat4f const Transform3d::RotationMatrixFrom(const Quaternion quaternion)
{
    const float x = quaternion[0];
    const float y = quaternion[1];
    const float z = quaternion[2];
    const float w = quaternion[3];
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
                            Vec4f(1.0f - yy - zz, xy - zw, xz + yw, 0),
                            Vec4f(xy + zw, 1.0f - xx - zz, yz - xw, 0),
                            Vec4f(xz - yw, yz + xw, 1.0f - xx - yy, 0),
                            Vec4f(0, 0, 0, 1)});
}

template<>
inline Transform3d Transform3d::Translate(const Transform3d& transform, const Vec3f translation)
{
    return transform * TranslationMatrixFrom(translation);
}

template<>
inline Transform3d Transform3d::Scale(const Transform3d& transform, const Vec3f scale)
{
    return transform * ScalingMatrixFrom(scale);
}

template<>
inline Transform3d Transform3d::Rotate(const Transform3d& transform, const degree_t angle, const Vec3f axis)
{
    return transform * RotationMatrixFrom(angle, axis);
}

template<>
inline Transform3d Transform3d::Rotate(const Transform3d& transform, const radian_t angle, const Vec3f axis)
{
    return transform * RotationMatrixFrom(angle, axis);
}

template<>
inline Transform3d Transform3d::Rotate(const Transform3d& transform, const Quaternion quaternion)
{
    return transform * RotationMatrixFrom(quaternion);
}

template<>
inline Transform3d Transform3d::Rotate(const Transform3d& transform, const EulerAngles eulerAngles)
{
    return transform * RotationMatrixFrom(eulerAngles);
}

template<>
inline EulerAngles const Transform3d::Rotation(const Mat4<float>& transform)
{
    return EulerAngles();
}

template<>
inline float Transform3d::Pitch(const Mat4<float>& transform)
{
    return 0.0f;
}

template<>
inline float Transform3d::RotationOnX(const Mat4<float>& transform)
{
    return Pitch(transform);
}

template<>
inline float Transform3d::Yaw(const Mat4<float>& transform)
{
    return 0.0f;
}

template<>
inline float Transform3d::RotationOnY(const Mat4<float>& transform)
{
    return Yaw(transform);
}

template<>
inline float Transform3d::Roll(const Mat4<float>& transform)
{
    return 0.0f;
}

template<>
inline float Transform3d::RotationOnZ(const Mat4<float>& transform)
{
    return Roll(transform);
}
}
