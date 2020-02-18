// TODO @Oleg: Check correctness of translate, scale, rotations, matrices generation

#include <random>
#include <gtest/gtest.h>
#include <mathematics/transform.h>

using namespace neko;

TEST(Engine, TestTransforms)
{
    // Pitch = X, Yaw = Y, Roll = Z

    // Translation

    // Scaling

    // Rotation around axis using degrees

    // Rotation around axis using radians

    // Rotation using cardinals
    Transform3d t = Transform3d::Identity;
    t = Transform3d::Rotate(t, EulerAngles(degree_t(45.0f), degree_t(45.0f), degree_t(45.0f)));
    std::cout << "Rotation using cardinals by 45° Euler on all axes: " << std::endl;
    std::cout << "X: " << t.Pitch(t) << std::endl;
    std::cout << "Y: " << t.Yaw(t) << std::endl;
    std::cout << "Z: " << t.Roll(t) << std::endl;
    std::cout << "--------------------" << std::endl;

    // Rotation using quaternions

}