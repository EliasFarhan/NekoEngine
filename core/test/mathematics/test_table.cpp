/*
MIT License

Copyright (c) 2022 SAE Institute Switzerland AG

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/


#include <gtest/gtest.h>
#include <random>
#include "mathematics/func_table.h"
#include "mathematics/angle.h"

constexpr float maxNmb = 100.0f;
void RandomFill(std::vector<float>& numbers, float start = -maxNmb, float end = maxNmb)
{
    std::random_device rd;
    std::mt19937 g(rd());
    std::uniform_real_distribution<float> dist{ start, end };
    std::generate(numbers.begin(), numbers.end(), [&g, &dist]() {return dist(g); });
}

TEST(Engine, TestSinTable)
{
neko::FuncTable<float> sinFuncTable(0.0f, neko::PI, [](float x) { return neko::Sin(neko::Radian(x)); });
sinFuncTable.GenerateTable();
const size_t sampleSize = 1024;
std::vector<float> localNumbers(sampleSize);
RandomFill(localNumbers, 0.0f, M_PI);

float error = 0.0f;
for (auto v : localNumbers)
{
error += std::abs(sinFuncTable.GetValue(v) - neko::Sin(neko::Radian(v)));
}
error /= float(sampleSize);
EXPECT_LT(error, 0.01f);
}