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


#include <gtest/gtest.h>
#include "mathematics/obb3.h"


TEST(Aabb, Obb3d_Obb3d)
{
    neko::RadianAngles angles = neko::RadianAngles(static_cast<neko::Radian>(0), static_cast<neko::Radian>(0), static_cast<neko::Radian>(neko::PI / 4));
    //Same Intersect
    neko::Obb3d obb1;
    obb1.FromCenterExtendsRotation(neko::Vec3f(0, 0, 0), neko::Vec3f(0.5f, 0.5f, 0.5f), angles);
    neko::Obb3d obb2;
    obb2.FromCenterExtendsRotation(neko::Vec3f(0, 0, 0), neko::Vec3f(0.5f, 0.5f, 0.5f), angles);
    EXPECT_TRUE(obb1.IntersectObb(obb2));
    EXPECT_TRUE(obb2.IntersectObb(obb1));

    angles = neko::RadianAngles(static_cast<neko::Radian>(-neko::PI / 4), static_cast<neko::Radian>(0), static_cast<neko::Radian>(-neko::PI / 4));
    //Transverse Intersect
    obb1.FromCenterExtendsRotation(neko::Vec3f(0, 0, 0), neko::Vec3f(0.5f, 0.5f, 0.5f), angles);
    obb2.FromCenterExtendsRotation(neko::Vec3f(1, 1, 1), neko::Vec3f(0.2f, 5.0f, 0.2f), angles);
    EXPECT_TRUE(obb1.IntersectObb(obb2));
    EXPECT_TRUE(obb2.IntersectObb(obb1));

    obb1.FromCenterExtendsRotation(neko::Vec3f(0, 0, 0), neko::Vec3f(0.5f, 0.5f, 0.5f), angles);
    obb2.FromCenterExtendsRotation(neko::Vec3f(1, 1, 1), neko::Vec3f(0.5f, 0.5f, 0.5f), angles);
    EXPECT_FALSE(obb1.IntersectObb(obb2));
    EXPECT_FALSE(obb2.IntersectObb(obb1));

    angles = neko::RadianAngles(static_cast<neko::Radian>(neko::PI / 4), static_cast<neko::Radian>(0), static_cast<neko::Radian>(neko::PI / 4));
    obb1.FromCenterExtendsRotation(neko::Vec3f(0.0f, 0.0f, 0.0f), neko::Vec3f(0.5f, 0.5f, 0.5f), angles);
    obb2.FromCenterExtendsRotation(neko::Vec3f(1.0f, 1.0f, 1.0f), neko::Vec3f(1.0f, 1.0f, 1.0f), angles);
    EXPECT_TRUE(obb1.IntersectObb(obb2));
    EXPECT_TRUE(obb2.IntersectObb(obb1));

    //Same Intersect
    obb1.FromCenterExtendsRotation(neko::Vec3f(0.0f, 0.0f, 0.0f), neko::Vec3f(0.5f, 0.5f, 0.5f), angles);
    obb2.FromCenterExtendsRotation(neko::Vec3f(0.0f, 0.0f, 0.0f), neko::Vec3f(0.5f, 0.5f, 0.5f), angles);
    EXPECT_TRUE(obb1.IntersectObb(obb2));
    EXPECT_TRUE(obb2.IntersectObb(obb1));

    //Basic Intersect
    obb1.FromCenterExtendsRotation(neko::Vec3f(0, 0, 0), neko::Vec3f(0.5f, 0.5f, 0.5f), angles);
    obb2.FromCenterExtendsRotation(neko::Vec3f(0.5f, 0.5f, 0.5f), neko::Vec3f(0.5f, 0.5f, 0.5f), angles);
    EXPECT_TRUE(obb1.IntersectObb(obb2));
    EXPECT_TRUE(obb2.IntersectObb(obb1));

    //Limit Intersect
    obb1.FromCenterExtendsRotation(neko::Vec3f(0, 0, 0), neko::Vec3f(0.5f, 0.5f, 0.5f), angles);
    obb2.FromCenterExtendsRotation(neko::Vec3f(neko::Sin(angles.x), 0, neko::Sin(angles.z)), neko::Vec3f(0.5f, 0.5f, 0.5f), angles);

    //Not Intersect
    obb1.FromCenterExtendsRotation(neko::Vec3f(0, 0, 0), neko::Vec3f(0.5f, 0.5f, 0.5f), angles);
    obb2.FromCenterExtendsRotation(neko::Vec3f(1, 1, 1), neko::Vec3f(0.5f, 0.5f, 0.5f), angles);
    EXPECT_FALSE(obb1.IntersectObb(obb2));
    EXPECT_FALSE(obb2.IntersectObb(obb1));

    //From Obb
    neko::Aabb3d aabb1;
    neko::Aabb3d aabb2;
    aabb1.FromObb(obb1);
    aabb2.FromObb(obb2);
    EXPECT_FALSE(aabb1.ContainsAabb(aabb2));
    EXPECT_TRUE(aabb1.IntersectAabb(aabb2));
    EXPECT_FALSE(aabb2.ContainsAabb(aabb1));
    EXPECT_TRUE(aabb2.IntersectAabb(aabb1));
}