//
// Created by frousties on 11/02/2020.
//

#include <gtest/gtest.h>
#include <mathematics/vector.h>

TEST(Engine, TestVector) {
    neko::Vec3f vec;
    neko::Vec2f vec1 = neko::Vec2f::right;

    neko::Vec4f vec2 = vec;

    std::cout << vec2;
}
