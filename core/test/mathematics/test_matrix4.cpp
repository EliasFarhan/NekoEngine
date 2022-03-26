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
#include "mathematics/matrix4.h"


TEST(Engine, TestMatrix4)
{
neko::Mat4f m1(std::array<neko::Vec4f, 4>
                                       {
                                               neko::Vec4f{ 1,2,3,4 },
                                               neko::Vec4f{ -1,-2,-3,-4 },
                                               neko::Vec4f{ 4,2,2,1 },
                                               neko::Vec4f{ -4,-3,-2,-1 }
                                       });

neko::Mat4f result = neko::Mat4f(std::array<neko::Vec4f, 4>{
        neko::Vec4f(-5, 5, 6, -5),
        neko::Vec4f(-8, 8, 5, -3),
        neko::Vec4f(-5, 5, 8, -5),
        neko::Vec4f(-5, 5, 9, -5)
});
result = result.Transpose();
EXPECT_LT(neko::Mat4f::MatrixDifference(m1.MultiplyNaive(m1), result), 0.01f);
EXPECT_LT(neko::Mat4f::MatrixDifference(m1.MultiplyIntrinsincs(m1), result), 0.01f);
EXPECT_LT(neko::Mat4f::MatrixDifference(m1.MultiplyNaive(m1), m1.MultiplyIntrinsincs(m1)), 0.01f);

EXPECT_LT(neko::Mat4f::MatrixDifference(neko::Mat4f::identity(), neko::Mat4f::identity().Inverse()), 0.01f);
EXPECT_LT(neko::Mat4f::MatrixDifference(neko::Mat4f::zero(), neko::Mat4f::zero().Inverse()), 0.01f);

const neko::Mat4f m = neko::Mat4f(std::array<neko::Vec4f, 4>
                                                          {
                                                                  neko::Vec4f(1.0f,5.0f,9.0f,13.0f),
                                                                  neko::Vec4f(2.0f,6.0f,-10.0f,14.0f),
                                                                  neko::Vec4f(3.0f,-7.0f,11.0f,15.0f),
                                                                  neko::Vec4f(4.0f,8.0f,12.0f,16.0f)
                                                          });

const auto mInvCalculus = m.Inverse();

const neko::Mat4f mInv = neko::Mat4f(std::array<neko::Vec4f, 4>
                                                             {
                                                                     neko::Vec4f(-198.0f,7.0f,20.0f,136.0f),
                                                                     neko::Vec4f(10.0f,0.0f,-30.0f,20.0f),
                                                                     neko::Vec4f(14.0f,-21.0f,0.0f,7.0f),
                                                                     neko::Vec4f(34.0f,14.0f,10.0f,-23.0f)
                                                             })*(1.0f/420.0f);


EXPECT_LT(neko::Mat4f::MatrixDifference(mInvCalculus, mInv), 0.01f);
EXPECT_GT(neko::Mat4f::MatrixDifference(mInvCalculus, neko::Mat4f::identity()), 0.01f);
}