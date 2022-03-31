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

#include "mathematics/vec3.h"
#include "mathematics/obb3.h"

namespace neko
{

struct Aabb3f {
    ///\brief Get the center of the AABB.
    [[nodiscard]] Vec3f CalculateCenter() const;

    ///\brief Get the extends of the AABB (half-widths).
    [[nodiscard]] Vec3f CalculateExtends() const;

    ///\brief Set the AABB from the center, extends.
    void FromCenterExtends(
            Vec3f center,
            Vec3f extends,
            Vec3f rotation = Vec3f::zero());

    ///\brief Enlarge AABB after a rotation
    void FromCenterExtendsRotation(
            Vec3f center,
            Vec3f extends,
            RadianAngles rotation);

    //\brief Set the AABB3d from OBB3d.
    void FromObb(Obb3f obb);

    [[nodiscard]] bool ContainsPoint(Vec3f point) const;

    [[nodiscard]] bool ContainsAabb(const Aabb3f& aabb) const;

    [[nodiscard]] bool IntersectAabb(const Aabb3f& aabb) const;

    [[nodiscard]] bool IntersectRay(const Vec3f& dirRay, const Vec3f& origin) const;


    [[nodiscard]] bool IntersectPlane(const Vec3f& normal, const Vec3f& point) const;


    static Vec3f RotateAxis(Vec3f axis, RadianAngles rotation);

    Vec3f lowerLeftBound = Vec3f::zero(); // the lower vertex
    Vec3f upperRightBound = Vec3f::zero(); // the upper vertex
};
}