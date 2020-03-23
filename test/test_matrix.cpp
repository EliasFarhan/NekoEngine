#pragma once
#include <gtest/gtest.h>

#include <mathematics/matrix.h>

TEST(Matrix, Mat4f_Multiply)
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
}
