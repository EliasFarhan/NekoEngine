//
// Created by efarhan on 15/11/2019.
//
#include <cmath>
#include <random>
#include <algorithm>
#include <benchmark/benchmark.h>

const long fromRange = 2;
const long toRange = 512;
const float maxNmb = 100.0f;
const size_t arraySize = 1024;

float powf_range(float base, int exponent)
{
    float result = 1.0f;
    if(exponent > 0)
    {
        for(int i = 0; i < exponent; i++)
        {
            result *= base;
        }
    }
    else
    {
        for(int i = 0; i < -exponent; i++)
        {
            result /= base;
        }
    }
    return result;
}

float powf_cmath(float base, int exponent)
{
    return powf(base, exponent);
}

float fabs_raw(float n)
{
    return n < 0 ? -n : n;
}

static void RandomFill(std::vector<float>& numbers)
{
    std::random_device rd;
    std::mt19937 g(rd());
    std::uniform_real_distribution<float> dist{-maxNmb, maxNmb};
    std::generate(numbers.begin(), numbers.end(), [&g, &dist](){return dist(g);});
}
static void BM_PowRange(benchmark::State& state)
{
    std::vector<float> local_numbers(arraySize);
    int exponent = state.range(0);
    RandomFill(local_numbers);
    for (auto _ : state)
    {
        for(auto v : local_numbers)
            benchmark::DoNotOptimize(powf_range(v, exponent));
    }
}
BENCHMARK(BM_PowRange)->RangeMultiplier(2)->Range(fromRange, toRange);

static void BM_PowCmath(benchmark::State& state)
{
    std::vector<float> local_numbers(arraySize);
    int exponent = state.range(0);
    RandomFill(local_numbers);
    for (auto _ : state)
    {
        for(auto v : local_numbers)
            benchmark::DoNotOptimize(powf_cmath(v, exponent));
    }
}
BENCHMARK(BM_PowCmath)->RangeMultiplier(2)->Range(fromRange, toRange);

static void BM_FabsRaw(benchmark::State& state)
{
    std::vector<float> local_numbers(state.range(0));
    RandomFill(local_numbers);
    for (auto _ : state)
    {
        for(auto v : local_numbers)
            benchmark::DoNotOptimize(fabs_raw(v));
    }
}
BENCHMARK(BM_FabsRaw)->RangeMultiplier(2)->Range(fromRange, toRange);

static void BM_FabsCmath(benchmark::State& state)
{
    std::vector<float> local_numbers(state.range(0));
    RandomFill(local_numbers);
    for (auto _ : state)
    {
        for(auto v : local_numbers)
            benchmark::DoNotOptimize(fabs(v));
    }
}
BENCHMARK(BM_FabsCmath)->RangeMultiplier(2)->Range(fromRange, toRange);

BENCHMARK_MAIN();