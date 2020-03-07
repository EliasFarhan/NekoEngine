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
#include <mathematics/vector.h>
#include <mathematics/map.h>


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
    //Variables
    neko::Quaternion quaternionA = neko::Quaternion(0.71, 0, 0, 0.71);
    neko::Quaternion quaternionB = neko::Quaternion(0, 0, 0, 1);
    neko::Quaternion quaternionACopy;

    //Display start variables
    std::cout << "QuaternionA"<<quaternionA<<'\n';
    std::cout << std::endl;
    std::cout <<"QuaternionB"<< quaternionB<<'\n';
    std::cout << std::endl << std::endl;

    //Dot Product Test
    std::cout << "Dot product: " << neko::Quaternion::Dot(quaternionA, quaternionB) << std::endl << std::endl;

    //Normalize Test
    std::cout << "Normalize: ";
    std::cout << "NormalizedQuaternionA" << neko::Quaternion::Normalized(quaternionA) << '\n';
    std::cout << std::endl << std::endl;

    //Magnitude Test
    std::cout << "Magnitude: " << neko::Quaternion::Magnitude(quaternionA) <<'\n'<<'\n';

    //AngleAxis Test
    std::cout << "AngleAxis: " << "Cannot be tested right now"<<'\n'<<'\n';

    //Angle Test
    std::cout << "Angle: " << neko::Quaternion::Angle(quaternionA, quaternionB) <<'\n'<<'\n';

    //Conjugate Test
    quaternionACopy = quaternionA;
    std::cout << "Conjugate: ";
    std::cout <<"QuaternionAConjugate"<< quaternionACopy.Conjugate()<<'\n';
    std::cout <<'\n'<<'\n';

    //Inverse Test
    quaternionACopy = quaternionA;
    std::cout << "Inverse: ";
    std::cout <<"QuaternionAInverse"<< quaternionACopy.Inverse()<<'\n';
    std::cout << std::endl << std::endl;

    //FromEuler
    std::cout << "Euler: " << "Cannot be tested right now" <<'\n'<<'\n';
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

    neko::Mat4f result = neko::Mat4f(std::array<neko::Vec4f, 4>{
            neko::Vec4f(-5,5,6,-5 ),
            neko::Vec4f(-8,8,5,-3 ),
            neko::Vec4f(-5,5,8,-5 ),
            neko::Vec4f(-5,5,9,-5 )
    });
    result = result.Transpose();
    EXPECT_TRUE(neko::Mat4f::MatrixDifference(m1.MultiplyNaive(m1), result)< 0.01f);
    EXPECT_TRUE(neko::Mat4f::MatrixDifference(m1.MultiplyIntrinsincs(m1), result)<0.01f);
    EXPECT_TRUE(neko::Mat4f::MatrixDifference(m1.MultiplyNaive(m1), m1.MultiplyIntrinsincs(m1))<0.01f);
}

TEST(Engine, Map)
{
    const size_t sizeInBytes = 512;

    void* mem = malloc(sizeInBytes);
    const auto allocator = neko::PoolAllocator<std::pair<xxh::hash_t<64>, double>>(sizeInBytes, mem);

    neko::Map<std::uint8_t, double> map((neko::Allocator*) &allocator, sizeInBytes);

    {// Set / retrieve.
        const uint8_t key[] = {1u, 2u, 3u, 4u, 5u};
        const double value[] = {1.0, 2.0, 3.0, 4.0, 5.0};
        for (uint8_t i = 0; i < 5u ; i++)
        {
            const bool result = map.Add(key[i], value[i]);
            EXPECT_TRUE(result);
        }
        for (uint8_t i = 0; i < 5 ; i++)
        {
            EXPECT_EQ(value[i], map.FindValue(key[i]));
        }
    }

    {// Swap.
        const double value = map.FindValue(1u);
        const double expectedSum = 1.0 + 2.0 + 3.0 + 4.0 + 5.0;
        double sum = 0.0;
        for (uint8_t i = 1u; i < 5u; i++)
        {
            const bool result = map.Swap(i, i+1u);
            EXPECT_TRUE(result);
        }
        for (uint8_t i = 1u; i < 6u; i++)
        {
            sum += map.FindValue(i);
        }
        EXPECT_EQ(value, map.FindValue(5u));
        EXPECT_EQ(expectedSum, sum);
    }

    {// Clear, remove and add.
        map.Clear();
        const uint8_t key[] = {1u, 2u, 3u, 4u, 5u};
        const double value[] = {1.0, 2.0, 3.0, 4.0, 5.0};
        for (uint8_t i = 0; i < 5u ; i++)
        {
            map.Add(key[i], value[i]);
        }

        map.Remove(2u);
        map.Remove(4u);

        double expectedSum = 1.0 + 3.0 + 5.0;
        double sum = 0.0;
        for (uint8_t i = 1u; i < 6u; i++)
        {
            sum += map.FindValue(i);
        }
        EXPECT_EQ(expectedSum, sum);

        map.Add(2u, 2.0);
        map.Add(4u, 4.0);

        expectedSum = 1.0 + 2.0 + 3.0 + 4.0 + 5.0;
        sum = 0.0;
        for (uint8_t i = 1u; i < 6u; i++)
        {
            sum += map.FindValue(i);
        }
        EXPECT_EQ(expectedSum, sum);
    }

    free(mem);
}