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
#include "mathematics/aabb2.h"


TEST(Aabb, Aabb2d_Aabb2d)
{
//Same Aabb
neko::Aabb2f aabb1;
aabb1.FromCenterExtends(neko::Vec2f(0.0f, 0.0f), neko::Vec2f(0.5f, 0.5f));
neko::Aabb2f aabb2;
aabb2.FromCenterExtends(neko::Vec2f(0.0f, 0.0f), neko::Vec2f(0.5f, 0.5f));
EXPECT_TRUE(aabb1.ContainsAabb(aabb2));
EXPECT_TRUE(aabb1.IntersectAabb(aabb2));
EXPECT_TRUE(aabb2.ContainsAabb(aabb1));
EXPECT_TRUE(aabb2.IntersectAabb(aabb1));

//Null Intersect
aabb2.FromCenterExtends(neko::Vec2f(0.0f, 0.0f), neko::Vec2f(0.0f, 0.0f));
EXPECT_TRUE(aabb1.ContainsAabb(aabb2));
EXPECT_TRUE(aabb1.IntersectAabb(aabb2));
EXPECT_FALSE(aabb2.ContainsAabb(aabb1));
EXPECT_TRUE(aabb2.IntersectAabb(aabb1));

//Null Limit Intersect
aabb2.FromCenterExtends(neko::Vec2f(0.5f, 0.5f), neko::Vec2f(0.0f, 0.0f));
EXPECT_TRUE(aabb1.ContainsAabb(aabb2));
EXPECT_TRUE(aabb1.IntersectAabb(aabb2));
EXPECT_FALSE(aabb2.ContainsAabb(aabb1));
EXPECT_TRUE(aabb2.IntersectAabb(aabb1));

//Null Not Intersect
aabb2.FromCenterExtends(neko::Vec2f(5.0f, 0.0f), neko::Vec2f(0.0f, 0.0f));
EXPECT_FALSE(aabb1.ContainsAabb(aabb2));
EXPECT_FALSE(aabb1.IntersectAabb(aabb2));
EXPECT_FALSE(aabb2.ContainsAabb(aabb1));
EXPECT_FALSE(aabb2.IntersectAabb(aabb1));

//Basic Intersect
aabb2.FromCenterExtends(neko::Vec2f(0.5f, 0.0f), neko::Vec2f(0.5f, 0.5f));
EXPECT_FALSE(aabb1.ContainsAabb(aabb2));
EXPECT_TRUE(aabb1.IntersectAabb(aabb2));
EXPECT_FALSE(aabb2.ContainsAabb(aabb1));
EXPECT_TRUE(aabb2.IntersectAabb(aabb1));

//Basic Contains
aabb2.FromCenterExtends(neko::Vec2f(0.1f, 0.1f), neko::Vec2f(0.1f, 0.1f));
EXPECT_TRUE(aabb1.ContainsAabb(aabb2));
EXPECT_TRUE(aabb1.IntersectAabb(aabb2));
EXPECT_FALSE(aabb2.ContainsAabb(aabb1));
EXPECT_TRUE(aabb2.IntersectAabb(aabb1));

//Limit Intersect
aabb2.FromCenterExtends(neko::Vec2f(1.0f, 0.0f), neko::Vec2f(0.5f, 0.5f));
EXPECT_FALSE(aabb1.ContainsAabb(aabb2));
EXPECT_TRUE(aabb1.IntersectAabb(aabb2));
EXPECT_FALSE(aabb2.ContainsAabb(aabb1));
EXPECT_TRUE(aabb2.IntersectAabb(aabb1));

//Not Intersect
aabb2.FromCenterExtends(neko::Vec2f(1.5f, 0.0f), neko::Vec2f(0.5f, 0.5f));
EXPECT_FALSE(aabb1.ContainsAabb(aabb2));
EXPECT_FALSE(aabb1.IntersectAabb(aabb2));
EXPECT_FALSE(aabb2.ContainsAabb(aabb1));
EXPECT_FALSE(aabb2.IntersectAabb(aabb1));

//Basic Intersect Positive
aabb2.FromCenterExtends(neko::Vec2f(1.0f, 1.0f), neko::Vec2f(1.0f, 1.0f));
EXPECT_FALSE(aabb1.ContainsAabb(aabb2));
EXPECT_TRUE(aabb1.IntersectAabb(aabb2));
EXPECT_FALSE(aabb2.ContainsAabb(aabb1));
EXPECT_TRUE(aabb2.IntersectAabb(aabb1));

//Basic Intersect Negative
aabb2.FromCenterExtends(neko::Vec2f(-1.0f, -1.0f), neko::Vec2f(1.0f, 1.0f));
EXPECT_FALSE(aabb1.ContainsAabb(aabb2));
EXPECT_TRUE(aabb1.IntersectAabb(aabb2));
EXPECT_FALSE(aabb2.ContainsAabb(aabb1));
EXPECT_TRUE(aabb2.IntersectAabb(aabb1));

//Limit Intersect Positive
aabb2.FromCenterExtends(neko::Vec2f(1.5f, 1.5f), neko::Vec2f(1.0f, 1.0f));
EXPECT_FALSE(aabb1.ContainsAabb(aabb2));
EXPECT_TRUE(aabb1.IntersectAabb(aabb2));
EXPECT_FALSE(aabb2.ContainsAabb(aabb1));
EXPECT_TRUE(aabb2.IntersectAabb(aabb1));

//Limit Intersect Negative
aabb2.FromCenterExtends(neko::Vec2f(-1.5f, -1.5f), neko::Vec2f(1.0f, 1.0f));
EXPECT_FALSE(aabb1.ContainsAabb(aabb2));
EXPECT_TRUE(aabb1.IntersectAabb(aabb2));
EXPECT_FALSE(aabb2.ContainsAabb(aabb1));
EXPECT_TRUE(aabb2.IntersectAabb(aabb1));

//Not Intersect Positive
aabb2.FromCenterExtends(neko::Vec2f(2.0f, 2.0f), neko::Vec2f(1.0f, 1.0f));
EXPECT_FALSE(aabb1.ContainsAabb(aabb2));
EXPECT_FALSE(aabb1.IntersectAabb(aabb2));
EXPECT_FALSE(aabb2.ContainsAabb(aabb1));
EXPECT_FALSE(aabb2.IntersectAabb(aabb1));

//Not Intersect Negative
aabb2.FromCenterExtends(neko::Vec2f(-2.0f, -2.0f), neko::Vec2f(1.0f, 1.0f));
EXPECT_FALSE(aabb1.ContainsAabb(aabb2));
EXPECT_FALSE(aabb1.IntersectAabb(aabb2));
EXPECT_FALSE(aabb2.ContainsAabb(aabb1));
EXPECT_FALSE(aabb2.IntersectAabb(aabb1));

//Transverse Intersect
aabb2.FromCenterExtends(neko::Vec2f(-5.0, 0.1f), neko::Vec2f(10.0f, 0.1f));
EXPECT_FALSE(aabb1.ContainsAabb(aabb2));
EXPECT_TRUE(aabb1.IntersectAabb(aabb2));
EXPECT_FALSE(aabb2.ContainsAabb(aabb1));
EXPECT_TRUE(aabb2.IntersectAabb(aabb1));
}



TEST(Aabb, Aabb2d_Ray)
{
neko::Aabb2f aabb1;
aabb1.FromCenterExtends(neko::Vec2f(0.0f, 0.0f), neko::Vec2f(0.5f, 0.5f));
//Inside Intersect
neko::Vec2f origin1 = neko::Vec2f(-0.1f, -0.1f);
neko::Vec2f dir1 = neko::Vec2f(1.0f, 1.0f);
EXPECT_TRUE(aabb1.IntersectRay(dir1, origin1));

//Basic Intersect
origin1 = neko::Vec2f(-1.0f, -1.0f);
dir1 = neko::Vec2f(1.0f, 1.0f);
EXPECT_TRUE(aabb1.IntersectRay(dir1, origin1));

//Limit Intersect
origin1 = neko::Vec2f(-0.5f, -1.5f);
EXPECT_TRUE(aabb1.IntersectRay(dir1, origin1));

//Not Intersect
origin1 = neko::Vec2f(-0.5f, -2.0f);
EXPECT_FALSE(aabb1.IntersectRay(dir1, origin1));

//Not 0,0
aabb1.FromCenterExtends(neko::Vec2f(2.0f, 2.0f), neko::Vec2f(1.0f, 1.0f));

//Basic Intersect
origin1 = neko::Vec2f(-5.0f, 2.0f);
dir1 = neko::Vec2f(1.0f, 0.0f);
EXPECT_TRUE(aabb1.IntersectRay(dir1, origin1));

//Limit Intersect
origin1 = neko::Vec2f(-5.0f, 1.0f);
EXPECT_TRUE(aabb1.IntersectRay(dir1, origin1));

//Not Intersect
origin1 = neko::Vec2f(-5.0f, 0.0f);
EXPECT_FALSE(aabb1.IntersectRay(dir1, origin1));

//Not Intersect
origin1 = neko::Vec2f(-5.0f, 2.0f);
dir1 = neko::Vec2f(-1.0f, 0.0f);
EXPECT_FALSE(aabb1.IntersectRay(dir1, origin1));
}
