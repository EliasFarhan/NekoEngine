#include <iostream>
#include <random>
#include "random_fill.h"
#include <benchmark/benchmark.h>
#include <mathematics/transform.h>

const int range[2] = {1, 10};

void RandomFill(std::vector<float>& numbers, float start, float end)
{
    std::random_device rd;
    std::mt19937 g(rd());
    std::uniform_real_distribution<float> dist{start, end};
    std::generate(numbers.begin(), numbers.end(), [&g, &dist](){return dist(g);});
}

float Sum(float a, float b){
    return a+b;
}

static void BM_Sum(benchmark::State& state) {
    const int arg = 1;
    std::array<float, arg> a = std::array<float, arg>();
    std::array<float, arg> b = std::array<float, arg>();
    for (int i = 0; i < arg; ++i)
    {
        a[i] = rand();
        b[i] = rand();
    }

    for(auto _ : state){
        for (int i = 0; i < arg; ++i)
        {
            benchmark::DoNotOptimize(Sum(a[i], b[i]));
        }
    }
}

BENCHMARK(BM_Sum)->DenseRange(range[0], range[1]);

BENCHMARK_MAIN();