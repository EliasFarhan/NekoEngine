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

#include "mathematics/obb2.h"

namespace neko
{

Vec2f Obb2d::GetRight()
{
    return {CalculateDirection().y,-CalculateDirection().x};
}

Vec2f Obb2d::GetUp()
{
    return Vec2f(CalculateDirection());
}

void Obb2d::FromCenterExtendsRotation(Vec2f newCenter, Vec2f localExtends, Degree rot)
{
    center = newCenter;
    localLowerLeftBound = localExtends * -1.0f;
    localUpperRightBound = localExtends;
    rotation = rot;
}

float Obb2d::GetExtendOnAxis(Vec2f axis)
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

    float rotationToAxis = Vec2f::AngleBetween(axis, CalculateDirection()).GetValue();
    rotationToAxis = std::fmod(rotationToAxis, PI);

    if((rotationToAxis >= 0 && rotationToAxis <= PI / 2) || (rotationToAxis >= -PI && rotationToAxis <= -PI / 2))
    {
        Vec2f lowerLeftToTopRight = localLowerLeftBound - localUpperRightBound;

        extend = (lowerLeftToTopRight.Magnitude() * Vec2f::AngleBetween(lowerLeftToTopRight, axis)).GetValue();
    }
    else
    {
        Vec2f upperLeftBound = GetOppositeBound(localUpperRightBound, true);
        Vec2f lowerRightToUpperLeft = (upperLeftBound - center) * 2;

        extend = (lowerRightToUpperLeft.Magnitude() * Vec2f::AngleBetween(lowerRightToUpperLeft, axis)).GetValue();
    }

    return extend;
}

Vec2f Obb2d::GetOppositeBound(Vec2f bound, bool isUpper)
{
    Vec2f centerToProjectionOnDir;
    Vec2f oppositeBound;
    Vec2f boundToOppositeBound;
    Vec2f centerToBound = bound - center;

    if (isUpper)
    {
        centerToProjectionOnDir =
                CalculateDirection() * (centerToBound).Magnitude() *
                Cos(Vec2f::AngleBetween(centerToBound, CalculateDirection()));
        boundToOppositeBound =
                center + centerToProjectionOnDir - bound;
        oppositeBound = localUpperRightBound + boundToOppositeBound +
                        boundToOppositeBound;
    } else
    {
        centerToProjectionOnDir =
                CalculateDirection() * (centerToBound).Magnitude() *
                Cos(Vec2f::AngleBetween(centerToBound, CalculateDirection())) * -1.0f;
        boundToOppositeBound =
                center + centerToProjectionOnDir - bound;
        oppositeBound = localLowerLeftBound + boundToOppositeBound +
                        boundToOppositeBound;
    }

    return oppositeBound;
}

Vec2f Obb2d::ProjectOnAxis(const Vec2f& axis) const
{
    float min = std::numeric_limits<float>::infinity();
    float max = -std::numeric_limits<float>::infinity();
    std::array<Vec2f, 4> corners;
    corners[0] = localLowerLeftBound.Rotate(rotation) + center;
    corners[1] = Vec2f(localUpperRightBound.x, localLowerLeftBound.y).Rotate(rotation) + center;
    corners[2] = Vec2f(localLowerLeftBound.x, localUpperRightBound.y).Rotate(rotation) + center;
    corners[3] = localUpperRightBound.Rotate(rotation) + center;
    for (auto& corner : corners) {
        float projection = Vec2f::Dot(corner, axis);
        if (projection < min) { min = projection; }
        if (projection > max) { max = projection; }
    }
    return Vec2f(min, max);
}

bool Obb2d::IntersectObb(Obb2d& obb1)
{
    std::array<Vec2f, 4> perpendicularAxis = { GetUp(), GetRight(), obb1.GetUp(), obb1.GetRight()};
    // we need to find the minimal overlap and axis on which it happens
    for (auto& axis : perpendicularAxis) {
        Vec2f proj1 = ProjectOnAxis(axis);
        Vec2f proj2 = obb1.ProjectOnAxis(axis);

        if (!(proj1.x <= proj2.y && proj1.y >= proj2.x) || !(proj2.x <= proj1.y && proj2.y >= proj1.x))
        {
            return false;
        }
    }
    return true;
}

Vec2f Obb2d::CalculateDirection() const    ///< return the normal of the upper side
{
    Vec2f direction;
    direction.x = Cos(rotation);
    direction.y = Sin(rotation);
    return direction;
}
}