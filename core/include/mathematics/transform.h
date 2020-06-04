#pragma once
#include "mathematics/matrix.h"


namespace neko::Transform3d
{
Mat4f const TranslationMatrixFrom(const Vec3f translation);
Mat4f const ScalingMatrixFrom(const Vec3f& scale);

Mat4f const RotationMatrixFrom(const degree_t angle, const Vec3f axis);

Mat4f const RotationMatrixFrom(const radian_t angle, const Vec3f axis);


Mat4<float> const RotationMatrixFrom(const EulerAngles cardinalRotation);

Mat4f const RotationMatrixFrom(const Quaternion& quaternion);



Mat4f Translate(const Mat4f& transform, const Vec3f translation);

Mat4f Scale(const Mat4f& transform, const Vec3f scale);

Mat4f Rotate(const Mat4f& transform, const degree_t angle, const Vec3f axis);

Mat4f Rotate(const Mat4f& transform, const radian_t angle, const Vec3f axis);


Mat4f Rotate(const Mat4f& transform, const Quaternion& quaternion);

Mat4f Rotate(const Mat4f& transform, const EulerAngles eulerAngles);


Mat4f Perspective(radian_t fovy, float aspect, float near, float far);
}
