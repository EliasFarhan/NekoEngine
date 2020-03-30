//
// Created by frousties on 11/02/2020.
//

#include <gtest/gtest.h>
#include <iostream>

#include <mathematics/vector_nvec.h>

namespace neko
{
TEST(Engine, TestVector)
{
    std::array<Vec4f, 4> array;
    array.fill(Vec4f(5, 1, 3, 9));
    FourVec4f test(array);

    auto result = test.MagnitudeIntrinsics();

    for (auto& tet : result) {
        std::cout << tet << "\n";
        EXPECT_TRUE(tet-array[0].Magnitude() < 0.01f);
    }
}
}
