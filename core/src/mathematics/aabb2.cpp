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

#include "mathematics/aabb2.h"

namespace neko
{

Vec2f Aabb2f::CalculateCenter() const
{ return 0.5f * (lowerLeftBound + upperRightBound); }

Vec2f Aabb2f::CalculateExtends() const
{ return 0.5f * (upperRightBound - lowerLeftBound); }

void Aabb2f::FromCenterExtends(Vec2f center, Vec2f extends)
{
    neko_assert(extends.x >= 0.0f, "Extends width should be positive");
    neko_assert(extends.y >= 0.0f, "Extends height should be positive");
    lowerLeftBound = center - extends;
    upperRightBound = center + extends;
}

void Aabb2f::FromCenterExtendsRotation(Vec2f center, Vec2f extends, Degree rotation)
{
    neko_assert(extends.x >= 0.0f, "Extends width should be positive");
    neko_assert(extends.y >= 0.0f, "Extends height should be positive");
    Vec2f relativeLowerLeftBound = -extends;
    Vec2f relativeUpperRightBound = extends;
    const Radian newAngle = rotation;
    std::array<Vec2f, 4> corners;
    corners[0] = relativeLowerLeftBound.Rotate(newAngle);
    corners[1] = Vec2f(relativeLowerLeftBound.x, relativeUpperRightBound.y).Rotate(newAngle);
    corners[2] = Vec2f(relativeUpperRightBound.x, relativeLowerLeftBound.y).Rotate(newAngle);
    corners[3] = relativeUpperRightBound.Rotate(newAngle);

    for (const Vec2f corner : corners) {
        if (corner.x > relativeUpperRightBound.x) { relativeUpperRightBound.x = corner.x; }
        if (corner.x < relativeLowerLeftBound.x) { relativeLowerLeftBound.x = corner.x; }
        if (corner.y > relativeUpperRightBound.y) { relativeUpperRightBound.y = corner.y; }
        if (corner.y < relativeLowerLeftBound.y) { relativeLowerLeftBound.y = corner.y; }
    }
    upperRightBound = relativeUpperRightBound + center;
    lowerLeftBound = relativeLowerLeftBound + center;
}

void Aabb2f::FromObb(Obb2f obb)
{
    FromCenterExtendsRotation(obb.center, Vec2f(obb.GetExtendOnAxis(obb.GetRight()), obb.GetExtendOnAxis(obb.GetUp())),obb.rotation);
}

bool Aabb2f::ContainsPoint(Vec2f point) const
{
    bool contains = point.x <= upperRightBound.x && point.x >= lowerLeftBound.x;
    contains = point.y <= upperRightBound.y && point.y >= lowerLeftBound.y &&
               contains;
    return contains;
}

bool Aabb2f::ContainsAabb(const Aabb2f& aabb) const
{
    return (ContainsPoint(aabb.upperRightBound) && ContainsPoint(aabb.lowerLeftBound));
}

bool Aabb2f::IntersectAabb(const Aabb2f& aabb) const
{
    const bool x = std::abs(aabb.CalculateCenter().x - CalculateCenter().x) <= (aabb.CalculateExtends().x + CalculateExtends().x);
    const bool y = std::abs(aabb.CalculateCenter().y - CalculateCenter().y) <= (aabb.CalculateExtends().y + CalculateExtends().y);

    return x && y;
}

bool Aabb2f::IntersectRay(const Vec2f& dirRay, const Vec2f& origin) const
{
    neko_assert(Vec2f(0, 0) != dirRay, "Null Ray Direction");
    if (ContainsPoint(origin))
    {
        return true;
    }
    std::array<float, 4> touch;
    touch[0] = (lowerLeftBound.x - origin.x) / dirRay.x;
    touch[1] = (upperRightBound.x - origin.x) / dirRay.x;
    touch[2] = (lowerLeftBound.y - origin.y) / dirRay.y;
    touch[3] = (upperRightBound.y - origin.y) / dirRay.y;
    const float touchMin = std::max(std::min(touch[0], touch[1]), std::min(touch[2], touch[3]));
    const float touchMax = std::min(std::max(touch[0], touch[1]), std::max(touch[2], touch[3]));

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
}