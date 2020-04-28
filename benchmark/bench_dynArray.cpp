#include <benchmark/benchmark.h>
#include <random>
#include <algorithm>
#include <numeric>
#include <vector>
#include "engine/array.h"
#include <mathematics/vector.h>

const long fromRange = 8;
const long toRange = 1 << 15;
const float start = -100.0f;
const float end = 100.0f;

static void BM_Push(benchmark::State& state){
	const size_t ALLOCATOR_HEADER_SIZE = 16;
	const size_t SIZE = 131072;
	void* start = malloc(SIZE + ALLOCATOR_HEADER_SIZE);

	neko::FreeListAllocator allocator(SIZE + ALLOCATOR_HEADER_SIZE, start);
	neko::DynArray<int> dyn(allocator);

	for (int i = 0; i < state.range(0); i++){
		dyn.Push(i);
	}
}
BENCHMARK(BM_Push)->Range(fromRange, toRange);