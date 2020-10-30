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

#include "graphics/camera.h"
#include "engine/log.h"

namespace neko
{
neko::Mat4f NullCamera::GenerateProjectionMatrix() const
{
    neko_assert(false, "[Error] No camera defined in CameraLocator!");
    logDebug("[Error] No camera defined in CameraLocator!");
    return neko::Mat4f();
}

neko::Mat4f Camera::GenerateViewMatrix() const
{

    const Mat4f rotation(std::array<Vec4f, 4>{
            Vec4f(rightDir.x, upDir.x, reverseDir.x, 0.0f),
            Vec4f(rightDir.y, upDir.y, reverseDir.y, 0.0f),
            Vec4f(rightDir.z, upDir.z, reverseDir.z, 0.0f),
            Vec4f(0.0f, 0.0f, 0.0f, 1.0f)
    });
    const Mat4f translation(std::array<Vec4f, 4>{
            Vec4f(1, 0, 0, 0),
            Vec4f(0, 1, 0, 0),
            Vec4f(0, 0, 1, 0),
            Vec4f(-position.x, -position.y, -position.z, 1),
    });
    return rotation * translation;
}

void Camera::SetDirectionFromEuler(const EulerAngles& angles)
{
    const Quaternion q = Quaternion::FromEuler(angles);
    reverseDir = Vec3f(Transform3d::RotationMatrixFrom(q)*Vec4f(0,0,1,0));
}

void Camera::Rotate(const EulerAngles& angles)
{
    const auto pitch = Quaternion::AngleAxis(angles.x, rightDir);
    const auto yaw = Quaternion::AngleAxis(angles.y, upDir);
    const auto roll = Quaternion::AngleAxis(angles.z, reverseDir);
    reverseDir = Vec3f(Transform3d::RotationMatrixFrom(pitch*yaw*roll) * Vec4f(reverseDir)).Normalized();
    WorldLookAt(-reverseDir+position);
}

void Camera::WorldLookAt(Vec3f target, Vec3f lookUp)
{
    reverseDir = (position - target).Normalized();
    rightDir = Vec3f::Cross(reverseDir, lookUp).Normalized();
    upDir = Vec3f::Cross(reverseDir, rightDir).Normalized();

}

void Camera2D::SetExtends(Vec2f size)
{
    left = -size.x;
    right = size.x;
    top = size.y;
    bottom = -size.y;
}

Mat4f Camera2D::GenerateProjectionMatrix() const
{
    return Transform3d::Orthographic(left, right, bottom, top, nearPlane, farPlane);
}

Mat4f Camera3D::GenerateProjectionMatrix() const
{
    return Transform3d::Perspective(
            fovY,
            aspect,
            nearPlane,
            farPlane);
}

void Camera3D::SetAspect(int width, int height)
{
    aspect = static_cast<float>(width) / static_cast<float>(height);
}

radian_t Camera3D::GetFovX() const
{
    return 2.0f*Atan(Tan(fovY*0.5f) * aspect);
}
}