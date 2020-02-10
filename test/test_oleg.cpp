#include <gtest/gtest.h>
#include <iostream>
#include <mathematics/matrix.h>

TEST(Engine, Transform3d)
{
    // Translation test.
    {
        auto input = neko::Mat4f::Identity;
        auto translation = neko::Vec3f::One;

        std::cout << "Rotate identity 4x4 mat by vec3f one:" << std::endl;

        for (int i = 0; i < 4; ++i)
        {
            std::cout << input[0][i] << " " << input[1][i] << " " << input[2][i] << " " << input[3][i] << std::endl;
        }

        std::cout << std::endl;

        auto output = input.Translate(input, translation);

        for (int i = 0; i < 4; ++i)
        {
            std::cout << output[0][i] << " " << output[1][i] << " " << output[2][i] << " " << output[3][i] << std::endl;
        }

        std::cout << "--------------------------------------------" << std::endl;
    }

    // Scaling test.
    {
        auto input = neko::Mat4f::Identity;
        auto scaling = neko::Vec3f::One * 2;

        std::cout << "Scale identity 4x4 mat by (2,2,2)" << std::endl;

        for (int i = 0; i < 4; ++i)
        {
            std::cout << input[0][i] << " " << input[1][i] << " " << input[2][i] << " " << input[3][i] << std::endl;
        }

        std::cout << std::endl;

        auto output = input.Scale(input, scaling);

        for (int i = 0; i < 4; ++i)
        {
            std::cout << output[0][i] << " " << output[1][i] << " " << output[2][i] << " " << output[3][i] << std::endl;
        }

        std::cout << "--------------------------------------------" << std::endl;
    }

    // Euler rotation test.
    {
        auto input = neko::Mat4f::Identity;
        auto axis = neko::Vec3f::One;
        float angle = 180.0f;

        std::cout << "Rotate identity 4x4 mat by 180 around vec3f one" << std::endl;

        for (int i = 0; i < 4; ++i)
        {
            std::cout << input[0][i] << " " << input[1][i] << " " << input[2][i] << " " << input[3][i] << std::endl;
        }

        std::cout << std::endl;

        auto output = input.Rotate(input, angle, axis);

        for (int i = 0; i < 4; ++i)
        {
            std::cout << output[0][i] << " " << output[1][i] << " " << output[2][i] << " " << output[3][i] << std::endl;
        }

        std::cout << "--------------------------------------------" << std::endl;
    }

    // Euler rotation cardinal rotation.
    {
        auto input = neko::Mat4f::Identity;
        auto cardinalRotation = neko::Vec3f::One * 180.0f;

        std::cout << "Rotate identity 4x4 mat by 180 around all cardinal axes" << std::endl;

        for (int i = 0; i < 4; ++i)
        {
            std::cout << input[0][i] << " " << input[1][i] << " " << input[2][i] << " " << input[3][i] << std::endl;
        }

        std::cout << std::endl;

        auto output = input.Rotate(input, cardinalRotation);

        for (int i = 0; i < 4; ++i)
        {
            std::cout << output[0][i] << " " << output[1][i] << " " << output[2][i] << " " << output[3][i] << std::endl;
        }

        std::cout << "--------------------------------------------" << std::endl;
    }
}
