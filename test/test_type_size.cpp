//
// Created by efarhan on 26.01.20.
//

#include <gtest/gtest.h>
#include <mathematics/matrix.h>
#include "mathematics/vector.h"
#include "mathematics/vector_nvec.h"


TEST(Engine, TestTypeSize)
{
    EXPECT_EQ(sizeof(neko::Vec4f), sizeof(float)*4);
    EXPECT_EQ(sizeof(neko::Vec2f), sizeof(float)*2);
    EXPECT_EQ(sizeof(neko::FourVec4f), sizeof(neko::Vec4f)*4);
    EXPECT_EQ(sizeof(neko::Mat4f), sizeof(neko::Vec4f)*4);
}
