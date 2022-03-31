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

#include "mathematics/vec2.h"
#include "mathematics/obb2.h"
#include "engine/assert.h"

namespace neko
{
/*
 * \brief Axis-Aligned Bounding Box in 2d with single-floating point number
 */
struct Aabb2f
{
    ///\brief Get the center of the AABB.
    [[nodiscard]] Vec2f CalculateCenter() const;

    ///\brief Get the extends of the AABB (half-widths).
    [[nodiscard]] Vec2f CalculateExtends() const;

    ///\brief Set the AABB from the center, extends.
    void FromCenterExtends(Vec2f center, Vec2f extends);

    ///\brief Enlarge AABB after a rotation
    void FromCenterExtendsRotation(
            Vec2f center,
            Vec2f extends,
            Degree rotation);

    ///\brief Set the AABB from OBB.
    void FromObb(Obb2f obb);

    [[nodiscard]] bool ContainsPoint(Vec2f point) const;
    [[nodiscard]] bool ContainsAabb(const Aabb2f& aabb) const;
    [[nodiscard]] bool IntersectAabb(const Aabb2f& aabb) const;
    [[nodiscard]] bool IntersectRay(const Vec2f& dirRay, const Vec2f& origin) const;

    Vec2f lowerLeftBound = Vec2f::zero(); // the lower vertex
    Vec2f upperRightBound = Vec2f::zero(); // the upper vertex
};
}