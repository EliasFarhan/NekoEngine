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

#include "mathematics/aabb3.h"
#include "engine/assert.h"

namespace neko
{

Vec3f Aabb3f::CalculateCenter() const
{ return (lowerLeftBound + upperRightBound) * 0.5f; }

Vec3f Aabb3f::CalculateExtends() const
{ return (upperRightBound - lowerLeftBound) * 0.5f; }

void Aabb3f::FromCenterExtends(Vec3f center, Vec3f extends, Vec3f rotation)
{
    neko_assert(extends.x >= 0.0f, "Extends should be positive");
    neko_assert(extends.y >= 0.0f, "Extends should be positive");
    neko_assert(extends.z >= 0.0f, "Extends should be positive");
    
    lowerLeftBound = center - extends;
    upperRightBound = center + extends;
}

void Aabb3f::FromCenterExtendsRotation(Vec3f center, Vec3f extends, RadianAngles rotation)
{
    neko_assert(extends.x >= 0.0f, "Extends should be positive");
    neko_assert(extends.y >= 0.0f, "Extends should be positive");
    neko_assert(extends.z >= 0.0f, "Extends should be positive");
    Vec3f relativeLowerLeftBound = extends * -1.0f;
    Vec3f relativeUpperRightBound = extends;
    std::array<Vec3f, 8> corners;
    corners[0] = RotateAxis(Vec3f(relativeLowerLeftBound.x, relativeLowerLeftBound.y, relativeLowerLeftBound.z), rotation);
    corners[1] = RotateAxis(Vec3f(-relativeLowerLeftBound.x, relativeLowerLeftBound.y, relativeLowerLeftBound.z), rotation);
    corners[2] = RotateAxis(Vec3f(relativeLowerLeftBound.x, relativeLowerLeftBound.y, -relativeLowerLeftBound.z), rotation);
    corners[3] = RotateAxis(Vec3f(-relativeLowerLeftBound.x, relativeLowerLeftBound.y, -relativeLowerLeftBound.z), rotation);
    corners[4] = RotateAxis(Vec3f(relativeLowerLeftBound.x, -relativeLowerLeftBound.y, relativeLowerLeftBound.z), rotation);
    corners[5] = RotateAxis(Vec3f(-relativeLowerLeftBound.x, -relativeLowerLeftBound.y, relativeLowerLeftBound.z), rotation);
    corners[6] = RotateAxis(Vec3f(relativeLowerLeftBound.x, -relativeLowerLeftBound.y, -relativeLowerLeftBound.z), rotation);
    corners[7] = RotateAxis(Vec3f(-relativeLowerLeftBound.x, -relativeLowerLeftBound.y, -relativeLowerLeftBound.z), rotation);
    for (Vec3f corner : corners) {
        if (corner.x > relativeUpperRightBound.x) { relativeUpperRightBound.x = corner.x; }
        if (corner.x < relativeLowerLeftBound.x) { relativeLowerLeftBound.x = corner.x; }
        if (corner.y > relativeUpperRightBound.y) { relativeUpperRightBound.y = corner.y; }
        if (corner.y < relativeLowerLeftBound.y) { relativeLowerLeftBound.y = corner.y; }
        if (corner.z > relativeUpperRightBound.z) { relativeUpperRightBound.z = corner.z; }
        if (corner.z < relativeLowerLeftBound.z) { relativeLowerLeftBound.z = corner.z; }
    }
    lowerLeftBound = relativeLowerLeftBound + center;
    upperRightBound = relativeUpperRightBound + center;
}

void Aabb3f::FromObb(Obb3f obb)
{
    FromCenterExtendsRotation(obb.center, Vec3f(obb.GetExtendOnAxis(obb.GetRight()), obb.GetExtendOnAxis(obb.GetUp()), obb.GetExtendOnAxis(obb.GetForward())), obb.rotation);
}

bool Aabb3f::ContainsPoint(Vec3f point) const
{
    bool contains = point.x <= upperRightBound.x && point.x >= lowerLeftBound.x;
    contains = point.y <= upperRightBound.y && point.y >= lowerLeftBound.y &&
               contains;
    contains = point.z <= upperRightBound.z && point.z >= lowerLeftBound.z &&
               contains;
    return contains;
}

bool Aabb3f::ContainsAabb(const Aabb3f& aabb) const
{
    return (ContainsPoint(aabb.upperRightBound) &&
            ContainsPoint(aabb.lowerLeftBound));
}

bool Aabb3f::IntersectAabb(const Aabb3f& aabb) const
{
    const bool x = std::abs(aabb.CalculateCenter().x - CalculateCenter().x) <= (aabb.CalculateExtends().x + CalculateExtends().x);
    const bool y = std::abs(aabb.CalculateCenter().y - CalculateCenter().y) <= (aabb.CalculateExtends().y + CalculateExtends().y);
    const bool z = std::abs(aabb.CalculateCenter().z - CalculateCenter().z) <= (aabb.CalculateExtends().z + CalculateExtends().z);

    return x && y && z;
}

bool Aabb3f::IntersectRay(const Vec3f& dirRay, const Vec3f& origin) const
{

    neko_assert(Vec3f(0, 0, 0) != dirRay, "Null Ray Direction");
    if (ContainsPoint(origin))
    {
        return true;
    }
    std::array<float, 6> touch{};
    touch[0] = (lowerLeftBound.x - origin.x) / dirRay.x;
    touch[1] = (upperRightBound.x - origin.x) / dirRay.x;
    touch[2] = (lowerLeftBound.y - origin.y) / dirRay.y;
    touch[3] = (upperRightBound.y - origin.y) / dirRay.y;
    touch[4] = (lowerLeftBound.z - origin.z) / dirRay.z;
    touch[5] = (upperRightBound.z - origin.z) / dirRay.z;
    const float touchMin = std::max(std::max(std::min(touch[0], touch[1]), std::min(touch[2], touch[3])), std::min(touch[4], touch[5]));
    const float touchMax = std::min(std::min(std::max(touch[0], touch[1]), std::max(touch[2], touch[3])), std::max(touch[4], touch[5]));

    // if tmax < 0, ray (line) is intersecting AABB, but the whole AABB is behind us
    if (touchMax < 0)
    {
        return false;
    }

    // if tmin > tmax, ray doesn't intersect AABB
    if (touchMin > touchMax)
    {
        return false;
    }
    return true;

}

bool Aabb3f::IntersectPlane(const Vec3f& normal, const Vec3f& point) const
{
    if (ContainsPoint(point))
    {
        return true;
    }
    const Vec3f extends = CalculateExtends();
    const Vec3f center = CalculateCenter();
    const float r = extends.x * std::abs(normal.x) + extends.y * std::abs(normal.y) + extends.z * std::abs(normal.z);
    const float d = Vec3f::Dot(normal, point);
    const float s = Vec3f::Dot(normal, center)-d;
    return std::abs(s) <= r;
}

Vec3f Aabb3f::RotateAxis(Vec3f axis, RadianAngles rotation)    ///< return the normal of the upper side
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