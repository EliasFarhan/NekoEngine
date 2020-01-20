#include <benchmark/benchmark.h>
#include <cstdlib>
#include <iostream>
#include <cmath>
#include <vector>

const unsigned long fromRange = 8;
const unsigned long toRange = 1<<13;

static void BM_Count(benchmark::State& state) {
    const size_t n = state.range(0);
    std::vector<float> v(n);
    std::generate(v.begin(), v.end(),[]{ return rand()%2==0?-1:1;});
    for (auto _ : state) {
        benchmark::DoNotOptimize(std::count_if(v.cbegin(), v.cend(), [](float x){return x > 0.0f;}));
    }

}

BENCHMARK(BM_Count)->Range(fromRange, toRange);

static void BM_CountSort(benchmark::State& state) {
    const size_t n = state.range(0);
    std::vector<float> v(n);
    std::generate(v.begin(), v.end(),[]{ return rand()%2==0?-1:1;});

    std::partition(v.begin(), v.end(), [](float x){return x > 0.0f;});
    for (auto _ : state) {
        benchmark::DoNotOptimize(std::count_if(v.cbegin(), v.cend(), [](float x){return x > 0.0f;}));
    }

}

BENCHMARK(BM_CountSort)->Range(fromRange, toRange);
