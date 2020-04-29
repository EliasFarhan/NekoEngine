#include <benchmark/benchmark.h>
#include <random>
#include "engine/array.h"
#include <vector>
#include <mathematics/vector.h>

const unsigned long fromRange = 8;
const unsigned long toRange = 1 << 16;

static void BM_DynPush(benchmark::State& state)
{
    const size_t ALLOCATOR_HEADER_SIZE = 16;
    const size_t SIZE = 4194304;
    void* start = malloc(SIZE + ALLOCATOR_HEADER_SIZE);

    neko::FreeListAllocator allocator(SIZE + ALLOCATOR_HEADER_SIZE, start);
	
    const auto length = state.range(0);
    neko::DynArray<int> dyn(allocator);
	
    for (int i = 0; i < length; i++){
        dyn.Push(rand() % 32768);
    }
    for (auto _ : state){
        for (size_t i = 0; i < dyn.GetSize(); i++) {
            benchmark::DoNotOptimize(dyn);
        }
    }
}
BENCHMARK(BM_DynPush)->Range(fromRange, toRange);

static void BM_VecPush(benchmark::State& state) {

    const auto length = state.range(0);
    std::vector<int> vec(state.range(0));

    for (int i = 0; i < length; i++) {
        vec.push_back(rand() % 32768);
    }
    for (auto _ : state) {
        for (size_t i = 0; i < vec.size(); i++) {
            benchmark::DoNotOptimize(vec);
        }
    }
}
BENCHMARK(BM_VecPush)->Range(fromRange, toRange);