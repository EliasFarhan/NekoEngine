/*
 MIT License

 Copyright (c) 2022 SAE Institute Switzerland AG

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

#include "mathematics/obb3.h"

namespace neko
{

Vec3f Obb3f::GetRight() const
{
    return RotateAxis(Vec3f(1, 0, 0), rotation);
}

Vec3f Obb3f::GetUp() const
{
    return RotateAxis(Vec3f(0, 1, 0), rotation);
}

Vec3f Obb3f::GetForward() const
{
    return RotateAxis(Vec3f(0, 0, 1), rotation);
}

void Obb3f::FromCenterExtendsRotation(Vec3f newCenter, Vec3f localExtends, RadianAngles rot)
{
    center = newCenter;
    localLowerLeftBound = localExtends * -1.0f;
    localUpperRightBound = localExtends;
    rotation.x = rot.x;
    rotation.y = rot.y;
    rotation.z = rot.z;
}

float Obb3f::GetExtendOnAxis(Vec3f axis)
{
    float extend;

    if (axis == GetUp()) {
        extend = 0.5f * (localUpperRightBound.y - localLowerLeftBound.y);
        return extend;
    }

    if (axis == GetRight()) {
        extend = 0.5f * (localUpperRightBound.x - localLowerLeftBound.x);
        return extend;
    }

    if (axis == GetForward()) {
        extend = 0.5f * (localUpperRightBound.z - localLowerLeftBound.z);
        return extend;
    }

    float rotationToAxis = Vec3f::AngleBetween(axis, GetUp()).GetValue();
    rotationToAxis = std::fmod(rotationToAxis, PI);

    if ((rotationToAxis >= 0 && rotationToAxis <= PI / 2) || (rotationToAxis >= -PI && rotationToAxis <= -PI / 2))
    {
        const Vec3f lowerLeftToTopRight = localLowerLeftBound - localUpperRightBound;

        extend = (lowerLeftToTopRight.Magnitude() * Vec3f::AngleBetween(lowerLeftToTopRight, axis)).GetValue();
    }
    else
    {
        const Vec3f upperLeftBound = GetOppositeBound(localUpperRightBound, true);
        const Vec3f lowerRightToUpperLeft = (upperLeftBound - center) * 2;

        extend = (lowerRightToUpperLeft.Magnitude() * Vec3f::AngleBetween(lowerRightToUpperLeft, axis)).GetValue();
    }

    return extend;
}

Vec3f Obb3f::GetOppositeBound(Vec3f bound, bool isUpper) const
{
    Vec3f centerToProjectionOnDir;
    Vec3f oppositeBound;
    Vec3f boundToOppositeBound;
    const Vec3f centerToBound = bound - center;

    if (isUpper)
    {
        centerToProjectionOnDir =
                GetUp() * (centerToBound).Magnitude() *
                Cos(Vec3f::AngleBetween(centerToBound, GetUp()));
        boundToOppositeBound =
                center + centerToProjectionOnDir - bound;
        oppositeBound = localUpperRightBound + boundToOppositeBound +
                        boundToOppositeBound;
    }
    else
    {
        centerToProjectionOnDir =
                GetUp() * (centerToBound).Magnitude() *
                Cos(Vec3f::AngleBetween(centerToBound, GetUp())) * -1;
        boundToOppositeBound =
                center + centerToProjectionOnDir - bound;
        oppositeBound = localLowerLeftBound + boundToOppositeBound +
                        boundToOppositeBound;
    }

    return oppositeBound;
}

Vec2f Obb3f::ProjectOnAxis(const Vec3f& axis) const
{
    float min = std::numeric_limits<float>::infinity();
    float max = -std::numeric_limits<float>::infinity();
    std::array<Vec3f, 8> corners;
    corners[0] = RotateAxis(Vec3f(localLowerLeftBound.x, localLowerLeftBound.y, localLowerLeftBound.z), rotation) + center;
    corners[1] = RotateAxis(Vec3f(-localLowerLeftBound.x, localLowerLeftBound.y, localLowerLeftBound.z), rotation) + center;
    corners[2] = RotateAxis(Vec3f(localLowerLeftBound.x, localLowerLeftBound.y, -localLowerLeftBound.z), rotation) + center;
    corners[3] = RotateAxis(Vec3f(-localLowerLeftBound.x, localLowerLeftBound.y, -localLowerLeftBound.z), rotation) + center;
    corners[4] = RotateAxis(Vec3f(localLowerLeftBound.x, -localLowerLeftBound.y, localLowerLeftBound.z), rotation) + center;
    corners[5] = RotateAxis(Vec3f(-localLowerLeftBound.x, -localLowerLeftBound.y, localLowerLeftBound.z), rotation) + center;
    corners[6] = RotateAxis(Vec3f(localLowerLeftBound.x, -localLowerLeftBound.y, -localLowerLeftBound.z), rotation) + center;
    corners[7] = RotateAxis(Vec3f(-localLowerLeftBound.x, -localLowerLeftBound.y, -localLowerLeftBound.z), rotation) + center;
    for (const auto& corner : corners) {
        const float projection = Vec3f::Dot(corner, axis);
        if (projection < min) { min = projection; }
        if (projection > max) { max = projection; }
    }
    return {min, max};
}

bool Obb3f::IntersectObb(const Obb3f& obb1) const
{
    const std::array<Vec3f, 6> perpendicularAxis = { GetUp(), GetRight(), GetForward(), obb1.GetUp(), obb1.GetRight(), obb1.GetForward()};
    // we need to find the minimal overlap and axis on which it happens
    for (auto& axis : perpendicularAxis) {
        const Vec2f proj1 = ProjectOnAxis(axis);
        const Vec2f proj2 = obb1.ProjectOnAxis(axis);

        if (!(proj1.x <= proj2.y && proj1.y >= proj2.x) || !(proj2.x <= proj1.y && proj2.y >= proj1.x))
        {
            return false;
        }
    }

    return true;
}

Vec3f Obb3f::RotateAxis(Vec3f axis, RadianAngles rotation)    ///< return the normal of the upper side
{
    const EulerAngles euler(rotation.x, rotation.y, rotation.z);
    const Quaternion q = Quaternion::FromEuler(euler);
    Vec3f newAxis;
    newAxis.x = axis.x * (q.x * q.x + q.w * q.w - q.y * q.y - q.z * q.z) + axis.y * (2 * q.x * q.y - 2 * q.w * q.z) + axis.z * (2 * q.x * q.z + 2 * q.w * q.y);
    newAxis.y = axis.x * (2 * q.w * q.z + 2 * q.x * q.y) + axis.y * (q.w * q.w - q.x * q.x + q.y * q.y - q.z * q.z) + axis.z * (-2 * q.w * q.x + 2 * q.y * q.z);
    newAxis.z = axis.x * (-2 * q.w * q.y + 2 * q.x * q.z) + axis.y * (2 * q.w * q.x + 2 * q.y * q.z) + axis.z * (q.w * q.w - q.x * q.x - q.y * q.y + q.z * q.z);
    return newAxis;
}
}