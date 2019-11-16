//
// Created by efarhan on 15/11/2019.
//
#include <cmath>
#include <random>
#include <algorithm>
#include <benchmark/benchmark.h>
#include "mathematics/func_table.h"

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
float fabs_cmath(float n)
{
    return fabs(n);
}

float sin_table(const neko::FuncTable<float>& funcTable, float x)
{
    return funcTable.GetValue(x);
}
float sin_cmath(float x)
{
    return sinf(x);
}

float log_table(const neko::FuncTable<float>& funcTable, float x)
{
    return funcTable.GetValue(x);
}
float log_cmath(float x)
{
    return logf(x);
}

void RandomFill(std::vector<float>& numbers, float start = -maxNmb, float end = maxNmb)
{
    std::random_device rd;
    std::mt19937 g(rd());
    std::uniform_real_distribution<float> dist{start, end};
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
            benchmark::DoNotOptimize(fabs_cmath(v));
    }
}
BENCHMARK(BM_FabsCmath)->RangeMultiplier(2)->Range(fromRange, toRange);

static void BM_SinTable(benchmark::State& state)
{
    neko::FuncTable<float> sinFuncTable(0.0f, M_PI, [](float x){return sinf(x);});
    sinFuncTable.GenerateTable();
    std::vector<float> local_numbers(state.range(0));
    RandomFill(local_numbers, 0.0f, M_PI);
    for (auto _ : state)
    {
        for(auto v : local_numbers)
            benchmark::DoNotOptimize(sin_table(sinFuncTable, v));
    }
}
BENCHMARK(BM_SinTable)->RangeMultiplier(2)->Range(fromRange, toRange);

static void BM_SinCmath(benchmark::State& state)
{
    std::vector<float> local_numbers(state.range(0));
    RandomFill(local_numbers, 0.0f, M_PI);
    for (auto _ : state)
    {
        for(auto v : local_numbers)
            benchmark::DoNotOptimize(sin_cmath(v));
    }

}
BENCHMARK(BM_SinCmath)->RangeMultiplier(2)->Range(fromRange, toRange);

static void BM_LogTable(benchmark::State& state)
{
    neko::FuncTable<float> sinFuncTable(0.0f, maxNmb, [](float x){return logf(x);});
    sinFuncTable.GenerateTable();
    std::vector<float> local_numbers(state.range(0));
    RandomFill(local_numbers, 0.0f, maxNmb);
    for (auto _ : state)
    {
        for(auto v : local_numbers)
            benchmark::DoNotOptimize(log_table(sinFuncTable, v));
    }
}
BENCHMARK(BM_LogTable)->RangeMultiplier(2)->Range(fromRange, toRange);

static void BM_LogCmath(benchmark::State& state)
{
    std::vector<float> local_numbers(state.range(0));
    RandomFill(local_numbers, 0.0f, maxNmb);
    for (auto _ : state)
    {
        for(auto v : local_numbers)
            benchmark::DoNotOptimize(log_cmath(v));
    }

}
BENCHMARK(BM_LogCmath)->RangeMultiplier(2)->Range(fromRange, toRange);

BENCHMARK_MAIN();