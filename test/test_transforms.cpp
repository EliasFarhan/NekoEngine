#include <gtest/gtest.h>
#include <mathematics/transform.h>

namespace neko
{

TEST(Mathematics, Transforms_Translations)
{
    Transform3d t = Transform3d::Identity;

    Transform3d expectedTransform = Transform3d(std::array<Vec4f, 4>
                                                        {
                                                                Vec4f(1, 0, 0, 0),
                                                                Vec4f(0, 1, 0, 0),
                                                                Vec4f(0, 0, 1, 0),
                                                                Vec4f(1, 1, 1, 1)});
    t = Transform3d::Translate(t, Vec3f::One);

    EXPECT_TRUE(Transform3d::MatrixDifference(t, expectedTransform) < 0.001f);

    const Vec3f position = Transform3d::Position(t);
    EXPECT_EQ(position, Vec3f::One);
}

TEST(Mathematics, Transforms_Scaling)
{
    Transform3d t = Transform3d::Identity;

    const Vec3f expectedScale(2.0f, 2.0f, 2.0f);
    Transform3d expectedTransform = Transform3d(std::array<Vec4f, 4>
                                                        {
                                                                Vec4f(expectedScale[0], 0, 0, 0),
                                                                Vec4f(0, expectedScale[1], 0, 0),
                                                                Vec4f(0, 0, expectedScale[2], 0),
                                                                Vec4f(0, 0, 0, 1)});
    t = Transform3d::Scale(t, expectedScale);

    EXPECT_TRUE(Transform3d::MatrixDifference(t, expectedTransform) < 0.001f);

    const Vec3f scale = Transform3d::Scale(t);
    EXPECT_EQ(expectedScale, scale);
}

TEST(Mathematics, Transforms_Rotations_AngleAxis)
{
    // TODO
}

TEST(Mathematics, Transforms_Rotations_Cardinal)
{
    // TODO
}

TEST(Mathematics, Transforms_RotationRetrieval)
{
    const Transform3d rotationMat = Transform3d::RotationMatrixFrom(degree_t(90), Vec3f::One);

    Transform3d t = Transform3d::Identity;
    t = Transform3d::Rotate(t, degree_t(90), Vec3f::One);
    const Transform3d retrieved = Transform3d::RotationMatrix(t);

    EXPECT_TRUE(Transform3d::MatrixDifference(rotationMat, retrieved) < 0.001f);
}

TEST(Mathematics, Transforms_Rotations_Quaternion)
{
    // TODO
}

}// !neko
