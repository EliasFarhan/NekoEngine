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
#include "mathematics/matrix3.h"

TEST(Engine, Matrix3Det)
{
const neko::Mat3f m1 = neko::Mat3f(std::array<neko::Vec3f, 3>
                                                           {
                                                                   neko::Vec3f(6,10,14),
                                                                   neko::Vec3f(7,11,15),
                                                                   neko::Vec3f(8,12,16)
                                                           });
const float det1 = 0.0f;
EXPECT_TRUE(neko::Equal(m1.Determinant(), det1));
const neko::Mat3f m2 = neko::Mat3f(std::array<neko::Vec3f, 3>
                                                           {
                                                                   neko::Vec3f(7,11,15),
                                                                   neko::Vec3f(8,12,16),
                                                                   neko::Vec3f(5,9,13)
                                                           });
const float det2 = 0.0f;
EXPECT_TRUE(neko::Equal(m2.Determinant(), det2));
const neko::Mat3f m3 = neko::Mat3f(std::array<neko::Vec3f, 3>
                                                           {
                                                                   neko::Vec3f(8,12,16),
                                                                   neko::Vec3f(5,9,13),
                                                                   neko::Vec3f(6,10,14)
                                                           });
const float det3 = 0.0f;
EXPECT_TRUE(neko::Equal(m3.Determinant(), det3));
const neko::Mat3f m4 = neko::Mat3f(std::array<neko::Vec3f, 3>
                                                           {
                                                                   neko::Vec3f(5, 9, 13),
                                                                   neko::Vec3f(6, 10, 14),
                                                                   neko::Vec3f(7, 11, 15)
                                                           });
const float det4 = 0.0f;
EXPECT_TRUE(neko::Equal(m4.Determinant(), det4));
}