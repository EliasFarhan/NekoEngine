//
// Created by frousties on 11/02/2020.
//

#include <gtest/gtest.h>
#include <mathematics/vector.h>

TEST(Engine, TestVector) {
    neko::Vec4f vec;
    neko::Vec3f vec1 = neko::Vec2f::right;

    neko::Vec2f vec2 = vec + vec1;

    std::cout << vec2;
}
