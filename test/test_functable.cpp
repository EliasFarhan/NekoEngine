#pragma once
#include <random>
#include <gtest/gtest.h>

#include <mathematics/const.h>
#include <mathematics/func_table.h>
#include <mathematics/vector.h>

const float maxNmb = 100.0f;

static void RandomFill(std::vector<float>& numbers, float start = -maxNmb, float end = maxNmb)
{
    std::random_device rd;
    std::mt19937 g(rd());
    std::uniform_real_distribution<float> dist{ start, end };
    std::generate(numbers.begin(), numbers.end(), [&g, &dist]() {return dist(g); });
}

TEST(FuncTable, SinTable)
{
    const size_t sampleSize = 1024;

    neko::FuncTable<float> sinFuncTable(0.0f, neko::PI, [](float x) { return neko::Sin(neko::radian_t(x)); });
    sinFuncTable.GenerateTable();

    std::vector<float> localNumbers(sampleSize);
    RandomFill(localNumbers, 0.0f, neko::PI);

    float error = 0.0f;
    for (auto v : localNumbers)
    {
        error += std::abs(sinFuncTable.GetValue(v) - neko::Sin(neko::radian_t(v)));
    }
    error /= float(sampleSize);

    EXPECT_LT(error, 0.01f);
}
