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
#include "mathematics/quaternion.h"

namespace neko
{

struct Obb3d
{

    Vec3f GetRight();

    Vec3f GetUp();

    Vec3f GetForward();

    /// Set the center, the extends and the rotation of the OBB.
    void FromCenterExtendsRotation(Vec3f newCenter, Vec3f localExtends, RadianAngles rot);
    float GetExtendOnAxis(Vec3f axis);

    Vec3f GetOppositeBound(Vec3f bound, bool isUpper);

    Vec2f ProjectOnAxis(const Vec3f& axis);

    bool IntersectObb(Obb3d& obb1);

    static Vec3f RotateAxis(Vec3f axis, RadianAngles rotation);

    Vec3f localLowerLeftBound;	///< the lower vertex
    Vec3f localUpperRightBound;	///< the upper vertex
    Vec3f center;	///< the upper vertex
    RadianAngles rotation;
};
}