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

void DisplayQuaternion(std::string quaternionName, neko::Quaternion q)
{
    std::cout << quaternionName << "(" << q.x << "," << q.y << "," << q.z << "," << q.w << ") ";
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
    //Variables
    neko::Quaternion quaternionA = neko::Quaternion(0.71, 0, 0, 0.71);
    neko::Quaternion quaternionB = neko::Quaternion(0, 0, 0, 1);
    neko::Quaternion quaternionACopy;

    //Display start variables
    DisplayQuaternion("QuaternionA", quaternionA);
    std::cout << std::endl;
    DisplayQuaternion("QuaternionB", quaternionB);
    std::cout << std::endl << std::endl;

    //Dot Product Test
    std::cout << "Dot product: " << neko::Quaternion::Dot(quaternionA, quaternionB) << std::endl << std::endl;

    //Normalize Test
    std::cout << "Normalize: ";
    DisplayQuaternion("NormalizedQuaternionA", neko::Quaternion::Normalize(quaternionA));
    std::cout << std::endl << std::endl;

    //Magnitude Test
    std::cout << "Magnitude: " << neko::Quaternion::Magnitude(quaternionA) << std::endl << std::endl;

    //AngleAxis Test
    std::cout << "AngleAxis: " << "Cannot be tested right now" << std::endl << std::endl;

    //Angle Test
    std::cout << "Angle: " << neko::Quaternion::Angle(quaternionA, quaternionB) << std::endl << std::endl;

    //Conjugate Test
    quaternionACopy = quaternionA;
    std::cout << "Conjugate: ";
    DisplayQuaternion("QuaternionAConjugate", quaternionACopy.Conjugate());
    std::cout << std::endl << std::endl;

    //Inverse Test
    quaternionACopy = quaternionA;
    std::cout << "Inverse: ";
    DisplayQuaternion("QuaternionAInverse", quaternionACopy.Inverse());
    std::cout << std::endl << std::endl;

    //FromEuler
    std::cout << "Euler: " << "Cannot be tested right now" << std::endl << std::endl;
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