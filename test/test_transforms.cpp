#include <gtest/gtest.h>
#include <mathematics/transform.h>

namespace neko{

TEST(Mathematics, Transforms_Translations)
{
    Transform3d t = Transform3d::Identity;

    Transform3d expected = Transform3d(std::array<Vec4f, 4>
                                               {
                                                       Vec4f(1, 0, 0, 0),
                                                       Vec4f(0, 1, 0, 0),
                                                       Vec4f(0, 0, 1, 0),
                                                       Vec4f(1, 1, 1, 1)});
    t = Transform3d::Translate(t, Vec3f::One);

    float cumulatedDifference = 0.0f;
    const float toleratedDifference = 0.0001f;
    for (int column = 0; column < 4; ++column)
    {
        for (int row = 0; row < 4; ++row)
        {
            cumulatedDifference += std::abs(t[column][row] - expected[column][row]);
        }
    }

    EXPECT_TRUE(cumulatedDifference < toleratedDifference);
}

TEST(Mathematics, Transforms_Scaling)
{
    Transform3d t = Transform3d::Identity;

    Transform3d expected = Transform3d(std::array<Vec4f, 4>
                                               {
                                                       Vec4f(2.0f, 0, 0, 0),
                                                       Vec4f(0, 2.0f, 0, 0),
                                                       Vec4f(0, 0, 2.0f, 0),
                                                       Vec4f(1, 1, 1, 1)});
    t = Transform3d::Scale(t, Vec3f(2.0f, 2.0f, 2.0f));

    float cumulatedDifference = 0.0f;
    const float toleratedDifference = 0.0001f;
    for (int column = 0; column < 4; ++column)
    {
        for (int row = 0; row < 4; ++row)
        {
            cumulatedDifference += std::abs(t[column][row] - expected[column][row]);
        }
    }

    EXPECT_TRUE(cumulatedDifference < toleratedDifference);
}

TEST(Mathematics, Transforms_Rotations_AngleAxis)
{

}

TEST(Mathematics, Transforms_Rotations_Cardinal)
{

}

TEST(Mathematics, Transforms_Rotations_Quaternion)
{

}

}// !neko
