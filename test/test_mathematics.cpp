//
// Created by efarhan on 11/16/19.
//
#ifdef WIN32
#define _USE_MATH_DEFINES
#endif
#include <cmath>
#include <random>
#include <gtest/gtest.h>
#include <mathematics/func_table.h>

#include <mathematics/quaternion.h>
#include <mathematics/matrix.h>
#include "mathematics/vector.h"


const float maxNmb = 100.0f;

void RandomFill(std::vector<float>& numbers, float start = -maxNmb, float end = maxNmb)
{
    std::random_device rd;
    std::mt19937 g(rd());
    std::uniform_real_distribution<float> dist{start, end};
    std::generate(numbers.begin(), numbers.end(), [&g, &dist](){return dist(g);});
}

TEST(Engine, TestMathematics)
{
    neko::FuncTable<float> sinFuncTable(0.0f, M_PI, [](float x){ return sinf(x);});
    sinFuncTable.GenerateTable();
    size_t sampleSize = 1024;
    std::vector<float> localNumbers(sampleSize);
    RandomFill(localNumbers, 0.0f, M_PI);

    float error = 0.0f;
    for(auto v : localNumbers)
    {
        error += sinFuncTable.GetValue(v) - sinf(v);
    }
    error /= float(sampleSize);
    std::cout << "Error margin for sinFuncTable with resolution 512: "<<error<<"\n";
}

TEST(Engine, TestQuaternion)
{
    neko::Quaternion quaternion = neko::Quaternion(0, 0, 0, 1);
    std::cout << "test Dot" << std::endl;
    std::cout << "test Normalize" << std::endl;
    std::cout << "test Magnitude" << std::endl;
    std::cout << "test AngleAxis" << std::endl;
    std::cout << "test Angle" << std::endl;
    std::cout << "test Conjugate" << std::endl;
    std::cout << "test Inverse" << std::endl;
    std::cout << "test FromEuler" << std::endl;
    std::cout << "test operator/Quaternion" << std::endl;
    std::cout << "test operator/float" << std::endl;
    std::cout << "test operator/=" << std::endl;
    std::cout << "test operator-Quaternion" << std::endl;
    std::cout << "test operator-=" << std::endl;
    std::cout << "test operator+" << std::endl;
    std::cout << "test operator+=" << std::endl;
    std::cout << "test operator*Quaternion" << std::endl;
    std::cout << "test operator*float" << std::endl;
    std::cout << "test operator*=" << std::endl;
    std::cout << "test operator==" << std::endl;
    std::cout << "test operator !=" << std::endl;
}
TEST(Engine, TestMatrix4)
{
    neko::Mat4f m1 (std::array<neko::Vec4f,4>
            {
                    neko::Vec4f{1,2,3,4},
                    neko::Vec4f{-1,-2,-3,-4},
                    neko::Vec4f{4,2,2,1},
                    neko::Vec4f{-4,-3,-2,-1}
            });

    std::cout << (m1.MultiplyNaive(m1))<<'\n';
    std::cout << (m1.MultiplyIntrinsincs(m1))<<'\n';
}