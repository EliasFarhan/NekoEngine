//
// Created by efarhan on 25.01.20.
//
#include "benchmark/benchmark.h"
#include <random>
#include <vector>

const long fromRange = 8;
const long toRange = 1 << 20;

static void BM_VectorPushBack(benchmark::State& state)
{
    const size_t length = state.range(0);
    for (auto _ : state)
    {

        std::vector<int> numbers;
        for(size_t i = 0; i < length; i++)
        {
            numbers.push_back(rand());
        }
        benchmark::DoNotOptimize(numbers);

    }
}

BENCHMARK(BM_VectorPushBack)->RangeMultiplier(2)->Range(fromRange, toRange);

static void BM_VectorReserveAndPushBack(benchmark::State& state)
{
    const size_t length = state.range(0);
    for (auto _ : state)
    {

        std::vector<int> numbers;
        numbers.reserve(length);
        for(size_t i = 0; i < length; i++)
        {
            numbers.push_back(rand());
        }
        benchmark::DoNotOptimize(numbers);

    }
}

BENCHMARK(BM_VectorReserveAndPushBack)->RangeMultiplier(2)->Range(fromRange, toRange);

BENCHMARK_MAIN();