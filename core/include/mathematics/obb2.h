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


#include "mathematics/angle.h"
#include "mathematics/vec2.h"

namespace neko
{
struct Obb2f
{
    /// Get the direction of the OBB rotated 90 degrees clockwise.
    Vec2f GetRight() const;

    /// Get the direction of the OBB rotated 90 degrees clockwise.
    Vec2f GetUp() const;

    /// Set the center, the extends and the rotation of the OBB.
    void FromCenterExtendsRotation(Vec2f newCenter, Vec2f localExtends, Degree rot);

    float GetExtendOnAxis(Vec2f axis);

    Vec2f GetOppositeBound(Vec2f bound, bool isUpper) const;

    [[nodiscard]] Vec2f ProjectOnAxis(const Vec2f& axis) const;

    bool IntersectObb(const Obb2f& obb1) const;

    Vec2f CalculateDirection() const;

    Vec2f localLowerLeftBound;	///< the lower vertex
    Vec2f  localUpperRightBound;	///< the upper vertex
    Vec2f center;
    Radian rotation;       ///< the angle of rotation in rd
};
}