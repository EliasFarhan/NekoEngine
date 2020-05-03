#pragma once
#include "mathematics/matrix.h"


namespace neko::Transform3d
{
Mat4f const TranslationMatrixFrom(const Vec3f translation);
Mat4f const ScalingMatrixFrom(const Vec3f& scale);

Mat4f const RotationMatrixFrom(const degree_t angle, const Vec3f axis);

Mat4f const RotationMatrixFrom(const radian_t angle, const Vec3f axis);


Mat4<float> const RotationMatrixFrom(const EulerAngles cardinalRotation);


Mat4<float> const RotationMatrixFrom(const RadianAngles cardinalRotation);

Mat4f const RotationMatrixFrom(const Quaternion& quaternion);



Mat4f Translate(const Mat4f& transform, const Vec3f translation);

Mat4f Scale(const Mat4f& transform, const Vec3f scale);

Mat4f Rotate(const Mat4f& transform, const degree_t angle, const Vec3f axis);

Mat4f Rotate(const Mat4f& transform, const radian_t angle, const Vec3f axis);


Mat4f Rotate(const Mat4f& transform, const Quaternion& quaternion);

Mat4f Rotate(const Mat4f& transform, const EulerAngles eulerAngles);


EulerAngles const Rotation(const Mat4<float>& transform);


float Pitch(const Mat4<float>& transform);

float RotationOnX(const Mat4<float>& transform);


float Yaw(const Mat4<float>& transform);

float RotationOnY(const Mat4<float>& transform);


float Roll(const Mat4<float>& transform);


float RotationOnZ(const Mat4<float>& transform);

Mat4f Perspective(radian_t fovy, float aspect, float near, float far);
}
