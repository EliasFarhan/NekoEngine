//
// Created by efarhan on 20.01.20.
//

#include <benchmark/benchmark.h>
#include <cstdlib>
#include <iostream>
#include <cmath>

const unsigned long fromRange = 32;
const unsigned long toRange = 1024;

static int* v = nullptr;
static const size_t length = 1024*1024; //4MB



static void CustomArguments(benchmark::internal::Benchmark* b) {
    for (size_t i = fromRange; i <= toRange; i*=2) {
        const size_t denseStep = i / 4;

        for (size_t j = 0; 2 * i > i + denseStep * (j/3) && j < 12; j++)
        {

            b->Args({static_cast<long>(i + denseStep * (j/3)+(j%3==0?-1:j%3==1?0:1))});

        }
    }
}

static void BM_Step(benchmark::State& state) {
    const size_t step = state.range(0);
    size_t i = 0;
    for (auto _ : state) {
        for(size_t repeat = 0; repeat < 10000; ++repeat)
        {
            v[i]++;
            i = (i+step)%length;
        }
    }
}

BENCHMARK(BM_Step)->Apply(CustomArguments);

int main(int argc, char** argv) {
    std::vector<int> numbers(length);
    std::generate(numbers.begin(), numbers.end(),[](){ return rand();});
    v = numbers.data();
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
}