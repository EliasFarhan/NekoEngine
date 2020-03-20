#include <gtest/gtest.h>
#include <mathematics/transform.h>

namespace neko
{

static const float TOLERATED_MATRIX_DIFFERENCE = 0.001f;
static const float TOLERATED_VECTOR_DIFFERENCE = 0.0001f;

static float MatrixDifference(const Transform3d& mat0, const Transform3d& mat1){
    float returnValue = 0;
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            returnValue += std::abs(mat1[i][j] - mat0[i][j]);
        }
    }
    return returnValue;
}

static float VectorDifference(const Vec3f& vec0, const Vec3f& vec1){
    float returnValue = 0;
    for (int i = 0; i < 3; ++i)
    {
        returnValue += std::abs(vec1[i] - vec0[i]);
    }
    return returnValue;
}

TEST (Transforms, TranslationMatrixFromTranslation)
{
    const Vec3f translation(2,3,5);

    const Transform3d expected(
            std::array<Vec4f, 4>
                    {
                            Vec4f(1, 0, 0, 0),
                            Vec4f(0, 1, 0, 0),
                            Vec4f(0, 0, 1, 0),
                            Vec4f(translation[0], translation[1], translation[2], 1)});

    const auto mat = Transform3d::TranslationMatrixFrom(translation);

    EXPECT_LT(MatrixDifference(expected, mat), TOLERATED_MATRIX_DIFFERENCE);
}

TEST (Transforms, ScalingMatrixFromScale)
{
    const Vec3f scale(2,3,5);

    const Transform3d expected(
            std::array<Vec4f, 4>
                    {
                            Vec4f(scale[0], 0, 0, 0),
                            Vec4f(0, scale[1], 0, 0),
                            Vec4f(0, 0, scale[2], 0),
                            Vec4f(0, 0, 0, 1)});

    const auto mat = Transform3d::ScalingMatrixFrom(scale);

    EXPECT_LT(MatrixDifference(expected, mat), TOLERATED_MATRIX_DIFFERENCE);
}

TEST (Transforms, RotationMatrixFromAngleAxis)
{
    const degree_t degree(180);
    const radian_t radian(degree); // PI
    const Vec3f axis = Vec3f::one.Normalized(); // [1/3 ; 1/3; 1/3] WRONG!!!

    const float a = 2.0f / 9.0f;
    const float b = a - 1.0f;
    const Transform3d expected(
            std::array<Vec4f, 4>
                    {
                            Vec4f(b, a, a, 0),
                            Vec4f(a, b, a, 0),
                            Vec4f(a, a, b, 0),
                            Vec4f(0, 0, 0, 1)});

    const auto matDegree = Transform3d::RotationMatrixFrom(degree, axis);
    const auto matRadian = Transform3d::RotationMatrixFrom(radian, axis);

    EXPECT_LT(MatrixDifference(expected, matDegree), TOLERATED_MATRIX_DIFFERENCE);
    EXPECT_LT(MatrixDifference(expected, matRadian), TOLERATED_MATRIX_DIFFERENCE);
    EXPECT_EQ(MatrixDifference(expected, matDegree), MatrixDifference(expected, matRadian));
}

TEST (Transforms, RotationMatrixFromEulerAngles)
{
    const degree_t degree(180);
    const radian_t radian(degree); // PI

    const auto matDegree = Transform3d::RotationMatrixFrom(EulerAngles(degree, degree, degree));
    const auto matRadian = Transform3d::RotationMatrixFrom(EulerAngles (RadianAngles (radian, radian, radian)));

    // Starting mat == rotated mat for rotations of PI on all axis.
    EXPECT_LT(MatrixDifference(Transform3d::Identity, matDegree), TOLERATED_MATRIX_DIFFERENCE);
    EXPECT_LT(MatrixDifference(Transform3d::Identity, matRadian), TOLERATED_MATRIX_DIFFERENCE);
    EXPECT_EQ(MatrixDifference(Transform3d::Identity, matDegree), MatrixDifference(Transform3d::Identity, matRadian));
}

TEST (Transforms, RotationMatrixFromQuaternion)
{
    const Quaternion quaternion0(0.0f, 0.0f, 0.0f, -1.0f); // 180 degrees rotation on all axis.
    const Quaternion quaternion1(0.0f, 0.0f, 0.0f, 1.0f); // 0 degrees rotation on all axis.

    const Transform3d mat0 = Transform3d ::RotationMatrixFrom(quaternion0);
    const Transform3d mat1 = Transform3d ::RotationMatrixFrom(quaternion1);

    EXPECT_LT(MatrixDifference(Transform3d::Identity, mat0), TOLERATED_MATRIX_DIFFERENCE);
    EXPECT_LT(MatrixDifference(Transform3d::Identity, mat1), TOLERATED_MATRIX_DIFFERENCE);
    EXPECT_EQ(MatrixDifference(mat0, mat1), TOLERATED_MATRIX_DIFFERENCE); // Should be the same exact matrix.
}

TEST (Transforms, TranslateTranslation)
{
    const Vec3f translation(2, 3, 5);

    const Transform3d expected(
            std::array<Vec4f, 4>
                    {
                            Vec4f(1, 0, 0, 0),
                            Vec4f(0, 1, 0, 0),
                            Vec4f(0, 0, 1, 0),
                            Vec4f(translation[0], translation[1], translation[2], 1)});

    Transform3d mat = Transform3d::Identity;
    mat.Translate(translation);

    EXPECT_LT(MatrixDifference(expected, mat), TOLERATED_MATRIX_DIFFERENCE);
}

TEST (Transforms, ScaleScale)
{
    const Vec3f scale(2,3,5);

    const Transform3d expected(
            std::array<Vec4f, 4>
                    {
                            Vec4f(scale[0], 0, 0, 0),
                            Vec4f(0, scale[1], 0, 0),
                            Vec4f(0, 0, scale[2], 0),
                            Vec4f(0, 0, 0, 1)});

    Transform3d mat = Transform3d::Identity;
    mat.Scale(scale);

    EXPECT_LT(MatrixDifference(expected, mat), TOLERATED_MATRIX_DIFFERENCE);
}

TEST (Transforms, RotateAngleAxis)
{
    const degree_t degree(180);
    const radian_t radian(degree); // PI
    const Vec3f axis = Vec3f::one.Normalized(); // [1/3 ; 1/3; 1/3]

    const float a = 2.0f / 9.0f;
    const float b = a - 1.0f;
    const Transform3d expected(
            std::array<Vec4f, 4>
                    {
                            Vec4f(b, a, a, 0),
                            Vec4f(a, b, a, 0),
                            Vec4f(a, a, b, 0),
                            Vec4f(0, 0, 0, 1)});

    Transform3d matDegree = Transform3d::Identity;
    Transform3d matRadian = Transform3d::Identity;
    matDegree.Rotate(degree, axis);
    matRadian.Rotate(radian, axis);

    EXPECT_LT(MatrixDifference(expected, matDegree), TOLERATED_MATRIX_DIFFERENCE);
    EXPECT_LT(MatrixDifference(expected, matRadian), TOLERATED_MATRIX_DIFFERENCE);
    EXPECT_EQ(MatrixDifference(expected, matDegree), MatrixDifference(expected, matRadian));
}

TEST (Transforms, RotateQuaternion)
{
    const Quaternion quaternion0(0.0f, 0.0f, 0.0f, -1.0f); // 180 degrees rotation on all axis.
    const Quaternion quaternion1(0.0f, 0.0f, 0.0f, 1.0f); // 0 degrees rotation on all axis.

    Transform3d mat0 = Transform3d::Identity;
    Transform3d mat1 = Transform3d::Identity;
    mat0.Rotate(quaternion0);
    mat1.Rotate(quaternion1);

    EXPECT_LT(MatrixDifference(Transform3d::Identity, mat0), TOLERATED_MATRIX_DIFFERENCE);
    EXPECT_LT(MatrixDifference(Transform3d::Identity, mat1), TOLERATED_MATRIX_DIFFERENCE);
    EXPECT_EQ(MatrixDifference(mat0, mat1), TOLERATED_MATRIX_DIFFERENCE); // Should be the same exact matrix.
}

TEST (Transforms, RotateEulerAngles)
{
    const degree_t degree(180);
    const radian_t radian(degree); // PI

    Transform3d matDegree = Transform3d::Identity;
    Transform3d matRadian = Transform3d::Identity;
    matDegree.Rotate(EulerAngles(degree, degree, degree));
    matRadian.Rotate(EulerAngles(RadianAngles(radian, radian, radian)));

    // Starting mat == rotated mat for rotations of PI on all axis.
    EXPECT_LT(MatrixDifference(Transform3d::Identity, matDegree), TOLERATED_MATRIX_DIFFERENCE);
    EXPECT_LT(MatrixDifference(Transform3d::Identity, matRadian), TOLERATED_MATRIX_DIFFERENCE);
    EXPECT_EQ(MatrixDifference(Transform3d::Identity, matDegree), MatrixDifference(Transform3d::Identity, matRadian));
}

TEST (Transforms, Position)
{
    const Vec3f expected(2,3,5);
    const Transform3d mat = Transform3d::TranslationMatrixFrom(expected) * Transform3d::Identity;

    const Vec3f position = mat.Position();

    EXPECT_LT(VectorDifference(expected, position), TOLERATED_VECTOR_DIFFERENCE);
}

TEST (Transforms, Scale)
{
    const Vec3f expected(2,3,5);
    const Transform3d mat = Transform3d::ScalingMatrixFrom(expected) * Transform3d::Identity;

    const Vec3f scale = mat.Scale();

    EXPECT_LT(VectorDifference(expected, scale), TOLERATED_VECTOR_DIFFERENCE);
}

TEST (Transforms, RotationMatrix)
{
    const degree_t degree(90); // PI/2
    const radian_t radian(degree);
    const Vec3f axis = Vec3f::one.Normalized();

    const Transform3d expectedDegree = Transform3d::RotationMatrixFrom(degree, axis);
    const Transform3d expectedRadian = Transform3d::RotationMatrixFrom(radian, axis);

    Transform3d matDegree = Transform3d::Identity;
    Transform3d matRadian = Transform3d::Identity;
    matDegree.Rotate(degree, axis);
    matRadian.Rotate(radian, axis);

    const Transform3d rotationMatDegree = matDegree.RotationMatrix();
    const Transform3d rotationMatRadian = matRadian.RotationMatrix();

    EXPECT_LT(MatrixDifference(expectedDegree, rotationMatDegree), TOLERATED_MATRIX_DIFFERENCE);
    EXPECT_LT(MatrixDifference(expectedRadian, rotationMatRadian), TOLERATED_MATRIX_DIFFERENCE);
    EXPECT_EQ(MatrixDifference(expectedDegree, rotationMatDegree), MatrixDifference(expectedRadian, rotationMatRadian));
}

TEST (Transforms, ScaleThenRotate)
{
    const Vec3f expectedScale(2,3,5);
    const degree_t degree(90); // PI
    const radian_t radian(degree);
    const Vec3f axis = Vec3f::zero.Normalized();

    const Transform3d expected(
            std::array<Vec4f, 4>
                    {
                            Vec4f(-14.0f / 9.0f, 6.0f / 7.0f, 16.0f / 7.0f, 0),
                            Vec4f(4.0f / 7.0f, -7.0f / 3.0f, 16.0f / 7.0f, 0),
                            Vec4f(4.0f / 7.0f, 6.0f / 7.0f, -56.0f / 9.0f, 0),
                            Vec4f(0, 0, 0, 1)});

    const float a = 2.0f / 9.0f;
    const float b = a - 1.0f;
    const Transform3d expectedRotationMat(
            std::array<Vec4f, 4>
                    {
                            Vec4f(b, a, a, 0),
                            Vec4f(a, b, a, 0),
                            Vec4f(a, a, b, 0),
                            Vec4f(0, 0, 0, 1)});

    Transform3d matDegree = Transform3d::Identity;
    Transform3d matRadian = Transform3d::Identity;
    matDegree.Scale(expectedScale);
    matRadian.Scale(expectedScale);
    matDegree.Rotate(degree, axis);
    matRadian.Rotate(radian, axis);

    const Vec3f scaleDegree = matDegree.Scale();
    const Vec3f scaleRadian = matRadian.Scale();

    EXPECT_LT(VectorDifference(expectedScale, scaleDegree), TOLERATED_VECTOR_DIFFERENCE);
    EXPECT_LT(VectorDifference(expectedScale, scaleRadian), TOLERATED_VECTOR_DIFFERENCE);
    EXPECT_EQ(VectorDifference(expectedScale, scaleDegree), VectorDifference(expectedScale, scaleRadian));

    const Transform3d rotationMatDegree = matDegree.RotationMatrix();
    const Transform3d rotationMatRadian = matRadian.RotationMatrix();

    EXPECT_LT(MatrixDifference(expectedRotationMat, rotationMatDegree), TOLERATED_MATRIX_DIFFERENCE);
    EXPECT_LT(MatrixDifference(expectedRotationMat, rotationMatRadian), TOLERATED_MATRIX_DIFFERENCE);
    EXPECT_EQ(MatrixDifference(expectedRotationMat, rotationMatDegree), MatrixDifference(expectedRotationMat, rotationMatRadian));

    EXPECT_LT(MatrixDifference(expected, matDegree), TOLERATED_MATRIX_DIFFERENCE);
    EXPECT_LT(MatrixDifference(expected, matRadian), TOLERATED_MATRIX_DIFFERENCE);
    EXPECT_EQ(MatrixDifference(expected, matDegree), MatrixDifference(expected, matRadian));
}

TEST (Transforms, RotateThenTranslate)
{
    const degree_t degree(90);
    const radian_t radian(degree);
    const Vec3f axis = Vec3f::one.Normalized();
    const Vec3f expectedTranslation(2,3,5);

    const float a = 2.0f / 9.0f;
    const float b = a - 1.0f;
    const Transform3d expected(
            std::array<Vec4f, 4>
                    {
                            Vec4f(b, a, a, 0),
                            Vec4f(a, b, a, 0),
                            Vec4f(a, a, b, 0),
                            Vec4f(a, b, -25.0f / 9.0f, 1)});

    const Transform3d expectedRotationMat(
            std::array<Vec4f, 4>
                    {
                            Vec4f(b, a, a, 0),
                            Vec4f(a, b, a, 0),
                            Vec4f(a, a, b, 0),
                            Vec4f(0, 0, 0, 1)});

    Transform3d matDegree = Transform3d::Identity;
    Transform3d matRadian = Transform3d::Identity;
    matDegree.Rotate(degree, axis);
    matRadian.Rotate(radian, axis);

    const Vec3f positionDegree = matDegree.Position();
    const Vec3f positionRadian = matRadian.Position();

    EXPECT_LT(VectorDifference(expectedTranslation, positionDegree), TOLERATED_VECTOR_DIFFERENCE);
    EXPECT_LT(VectorDifference(expectedTranslation, positionRadian), TOLERATED_VECTOR_DIFFERENCE);
    EXPECT_EQ(VectorDifference(expectedTranslation, positionDegree), VectorDifference(expectedTranslation, positionRadian));

    const Transform3d rotationMatDegree = matDegree.RotationMatrix();
    const Transform3d rotationMatRadian = matRadian.RotationMatrix();

    EXPECT_LT(MatrixDifference(expectedRotationMat, rotationMatDegree), TOLERATED_MATRIX_DIFFERENCE);
    EXPECT_LT(MatrixDifference(expectedRotationMat, rotationMatRadian), TOLERATED_MATRIX_DIFFERENCE);
    EXPECT_EQ(MatrixDifference(expectedRotationMat, rotationMatDegree), MatrixDifference(expectedRotationMat, rotationMatRadian));

    EXPECT_LT(MatrixDifference(expected, matDegree), TOLERATED_MATRIX_DIFFERENCE);
    EXPECT_LT(MatrixDifference(expected, matRadian), TOLERATED_MATRIX_DIFFERENCE);
    EXPECT_EQ(MatrixDifference(expected, matDegree), MatrixDifference(expected, matRadian));
}

TEST (Transforms, ScaleThenRotateThenTranslate)
{
    // TODO
}

}// !neko
